#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<linux/mod_devicetable.h>
#include<linux/slab.h>
#include"lab_usb.h"
MODULE_LICENSE("GPL");

char *buff = NULL;

static struct usb_device_id usb_table [] = {
	{USB_DEVICE(USB_VENDOR_ID,USB_PRODUCT_ID)},
	{}
};

MODULE_DEVICE_TABLE(usb,usb_table);

static struct usb_driver usb_driver = {
	.name = "usb_skeleton",
	.id_table = usb_table,
	.probe = usb_probe,
	.disconnect = usb_disconnect,
};

static void usb_disconnect(struct usb_interface *interface)
{
	printk("MY_USB_DISCONNECT : Device Removed - LAB3\n");
} 

static int __init usb_init(void)
{	
	int result = usb_register(&usb_driver);
	if(result != SUCCESS){
		printk("ERROR : Couldn't register the USB Drive\n");
		return FAILURE;
	}
	printk("MY_USB_INIT : I am in USB_INIT function - LAB3\n");
}
static void __exit usb_exit(void)
{
	kfree(buff);
	usb_deregister(&usb_driver);

	printk("MY_USB_EXIT : I am in USB_EXIT function - LAB3\n");
	return SUCCESS;
}


// Printing the usb_device_id structure and all desciptors
static int usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	uint8_t i;
	struct usb_device *usbDevice = NULL;
	struct usb_host_config   *phost_conf = NULL;
	struct usb_config_descriptor *pconf_desc = NULL;
	struct usb_host_interface       *phost_iface = NULL;
	struct usb_interface_descriptor *piface_desc = NULL;
	struct usb_endpoint_descriptor  *pendp_desc = NULL;
	
	usbDevice = interface_to_usbdev(intf);
	phost_conf = usbDevice->actconfig;
	pconf_desc = &phost_conf->desc;
	phost_iface = &intf->altsetting[0];
	piface_desc = &phost_iface->desc;
	
	
	printk("\n*************USB_DEVICE_ID_STRUCTURE_BEGIN*************\n");
	printk("idVendor = 0x%x\n",id->idVendor);
	printk("idProduct = 0x%x\n",id->idProduct);
	printk("bcdDevice_lo = 0x%x\n",id->bcdDevice_lo);
	printk("bcdDevice_hi = 0x%x\n",id->bcdDevice_hi);
	printk("bDeviceSubClass = 0x%x\n",id->bDeviceClass);
	printk("bDeviceSubClass = 0x%x\n",id->bDeviceSubClass);
	printk("bDeviceProtocol = 0x%x\n",id->bDeviceProtocol);
	printk("bInterfaceClass = 0x%x\n",id->bInterfaceClass);
	printk("bInterfaceSubClass = 0x%x\n",id->bInterfaceSubClass);
	printk("bInterfaceProtocol = 0x%x\n",id->bInterfaceProtocol);
	printk("bInterfaceNumber = 0x%x\n",id->bInterfaceNumber);
	printk("*************USB_DEVICE_ID_STRUCTURE_END***************\n\n");

	printk("********USB_DEVICE_DESCRIPTOR_STRUCTURE_BEGIN************\n");
	printk("bLength = %d\n",usbDevice->descriptor.bLength);
	printk("bDescriptorType = 0x%x\n",usbDevice->descriptor.bDescriptorType);
	printk("bcdUSB = 0x%x\n",usbDevice->descriptor.bcdUSB);
	printk("bDeviceClass = 0x%x\n",usbDevice->descriptor.bDeviceClass);
	printk("bDeviceSubClass = %d\n",usbDevice->descriptor.bDeviceSubClass);
	printk("bDeviceProtocol = %d\n",usbDevice->descriptor.bDeviceProtocol);
	printk("bMaxPacketSize0 = %d\n",usbDevice->descriptor.bMaxPacketSize0);
	printk("idVendor = 0x%x\n",usbDevice->descriptor.idVendor);
	printk("idProduct = 0x%x\n",usbDevice->descriptor.idProduct);
	printk("bcdDevice = 0x%x\n",usbDevice->descriptor.bcdDevice);
	printk("iManufacturer = %d\n",usbDevice->descriptor.iManufacturer);
	printk("iProduct =%d\n",usbDevice->descriptor.iProduct);
	printk("iSerialNumber = %d\n",usbDevice->descriptor.iSerialNumber);
	printk("bNumConfigurations = %d\n",usbDevice->descriptor.bNumConfigurations);
	printk("**********USB_DEVICE_DESCRIPTOR_STRUCTURE_END**********\n\n");
	
	printk("********USB_CONFIGURATION_DESCRIPTOR_STRUCTURE_BEGIN************\n");
	printk("bLength = %d\n",pconf_desc->bLength);
	printk("bDescriptorType = %d\n",pconf_desc->bDescriptorType);
	printk("wTotalLength = %d\n",pconf_desc->wTotalLength);
	printk("bNumInterfaces = %d\n",pconf_desc->bNumInterfaces);
	printk("bConfigurationValue = %d\n",pconf_desc->bConfigurationValue);
	printk("iConfiguration = %d\n",pconf_desc->iConfiguration);
	printk("bmAttributes = 0x%x\n",pconf_desc->bmAttributes);
	printk("bMaxPower = %dmA\n",pconf_desc->bMaxPower);
	printk("**********USB_CONFIGURATION_DESCRIPTOR_STRUCTURE_END**********\n\n");
	
	printk("********USB_INTERFACE_DESCRIPTOR_STRUCTURE_BEGIN************\n");
	printk("bLength = %d\n",piface_desc->bLength);
	printk("bDescriptorType = %d\n",piface_desc->bDescriptorType);
	printk("bInterfaceNumber = %d\n",piface_desc->bInterfaceNumber);
	printk("bAlternateSetting = %d\n",piface_desc->bAlternateSetting);
	printk("bNumEndpoints = %d\n",piface_desc->bNumEndpoints);
	printk("bInterfaceClass = %d\n",piface_desc->bInterfaceClass);
	printk("bInterfaceSubClass = %d\n",piface_desc->bInterfaceSubClass);
	printk("bInterfaceProtocol = %d\n",piface_desc->bInterfaceProtocol);
	printk("iInterface = %d\n",piface_desc->iInterface);
	printk("**********USB_INTERFACE_DESCRIPTOR_STRUCTURE_END**********\n\n");
	
	for(i=0;i<piface_desc->bNumEndpoints;i++)
	{
		pendp_desc = &phost_iface->endpoint[i].desc;
		printk("********USB_ENDPOINT_DESCRIPTOR%d_STRUCTURE_BEGIN************\n",i);
		printk("bLength = %d\n",pendp_desc->bLength);
		printk("bDescriptorType = %d\n",pendp_desc->bDescriptorType);
		printk("bEndpointAddress = 0x%x\n",pendp_desc->bEndpointAddress);
		printk("bmAttributes = %d\n",pendp_desc->bmAttributes);
		printk("wMaxPacketSize = 0x%x\n",pendp_desc->wMaxPacketSize);
		printk("bInterval = %d\n",pendp_desc->bInterval);
		printk("bRefresh = %d\n",pendp_desc->bRefresh);
		printk("bSynchAddress = %d\n",pendp_desc->bSynchAddress);
		printk("**********USB_ENDPOINT_DESCRIPTOR%d_STRUCTURE_END**********\n\n",i);
	}
	
	return SUCCESS;
}

module_init(usb_init);
module_exit(usb_exit);
