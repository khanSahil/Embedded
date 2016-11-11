#define SUCCESS 			0
#define FAILURE 			-1

#define USB_VENDOR_ID  		0x0930
#define USB_PRODUCT_ID 		0x6544

static int __init usb_init(void);
static void __exit usb_exit(void);
static int usb_probe(struct usb_interface *intf, const struct usb_device_id *id);
static void usb_disconnect(struct usb_interface *intf);
