#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>                 
#include <linux/interrupt.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Khan");
MODULE_DESCRIPTION("This lab shows how to export kernel data structures or attributes to user space using kObjects");


static unsigned int	LED = 48;       // LED is connected to GPIO 48
static unsigned int Button = 49;    // Button is connected to GPIO 49
static unsigned int interruptNum;   // Dynamically assigned Intrrupt Number
static bool	    	ledStatus = 0;          // Default Status Off // exported to User space using kObjects


static irq_handler_t  my_gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs);

// function called when we try to read ledStatus from user space in /sys/ebb/ directory
static ssize_t ledStatus_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	return sprintf(buf, "%d\n", ledStatus);
}

// this attribute is RO-readOnly , so write to this will fail
static struct kobj_attribute ledStatus_attr = __ATTR_RO(ledStatus);    

static struct attribute *ebb_attrs[] = {
	&ledStatus_attr.attr,           
	NULL,
};

// declarinf pointer to kObject Structure
static struct kobject *ebb_kobj;

// specifying the attribute that is exported to User Space
static struct attribute_group attr_group = {
	.attrs = ebb_attrs,                
};

static int __init my_gpio_init(void){
   
	int result = 0;
	printk(KERN_INFO "I am in GPIO init Function\n");

	// create the kobject sysfs entry at /sys/ebb 
	ebb_kobj = kobject_create_and_add("ebb", kernel_kobj->parent); 
	if(!ebb_kobj){
		printk(KERN_ALERT "Failed to create Kobject Mapping\n");
		return -ENOMEM;
	}
	// add the attributes "ledStatus" to /sys/ebb/ directory
	result = sysfs_create_group(ebb_kobj, &attr_group);
	if(result) {
		printk(KERN_ALERT "Failed to create sysfs group\n");
		kobject_put(ebb_kobj); 
		return result;
	}

	if (!gpio_is_valid(LED)){
		printk(KERN_INFO "You requested invalid GPIO\n");
		return -ENODEV;
	}

	// requesting LED gpio , setting its direction and status
	ledStatus = true;
	gpio_request(LED, "sysfs");          
	gpio_direction_output(LED, ledStatus);   
	gpio_export(LED, false);             
	
	// requesting BUTTON gpio , setting its direction	                    
	gpio_request(Button, "sysfs");       
	gpio_direction_input(Button);        
	gpio_set_debounce(Button, 200);      
	gpio_export(Button, false);          

	// requesting interrupt number for GPIO_49
	interruptNum = gpio_to_irq(Button);
	printk(KERN_INFO "GPIO_TEST: The button is mapped to IRQ: %d\n", interruptNum);

	//requesting the interrupt line
	result = request_irq(interruptNum,             
			        (irq_handler_t) my_gpio_irq_handler, 
			        IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING,   
			        "my_gpio_handler",    
			        NULL);               
			        
	printk(KERN_INFO "Interrupt requested %s :", (result == 0) ? "GPIO_Granted" : "GPIO_Not_Granted");
	return result;
}

static void __exit my_gpio_exit(void){

	printk(KERN_INFO "I am in GPIO exit function\n");
	printk(KERN_INFO "The last state of the Button was : %d\n", gpio_get_value(Button));
	kobject_put(ebb_kobj);
	gpio_set_value(LED, 0);              
	gpio_unexport(LED);                  
	free_irq(interruptNum, NULL);        
	gpio_unexport(Button);               
	gpio_free(LED);                      
	gpio_free(Button);      
   
}

static irq_handler_t my_gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){
	ledStatus = !ledStatus;                          
	gpio_set_value(LED, ledStatus);          
	printk(KERN_INFO "Button status is %d \n", gpio_get_value(Button));
	return (irq_handler_t) IRQ_HANDLED;      
}

module_init(my_gpio_init);
module_exit(my_gpio_exit);
