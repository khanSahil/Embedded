// Project IoT-Server
/*
In this project I have used Tiva Microcontroller having TM4c123GH6PM microcontroller
which is acting as a web server. I have desined and saved three web pages on this 
web server. When we connect to this server, this will send a webpage back to client
and depending upon the response from the client another web page is sent to client.
TCP/IP protocol server state machine is implemented to interact between server and 
client.
The first web page basically asks user to enter username and password and if it is 
correct the status of the sensor is sent back to client otherwise another web page 4
showing wrong password or username is sent back to the client.
The moment sensor data is sent to client the connection is closed, and need to 
establish new connection for new request or querry.
Hardware - Tiva-C having TM4C123GH6PM Mircocontroller is used for server.
		 - ENC28J60 is used as ethernet jack connected to Tiva using SPI protocol.
*/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "enc28j60.h"
#include "graphics_lcd.h"
#include "wait.h"
#include <time.h>

#define RED_LED      (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4)))
#define GREEN_LED    (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 3*4)))
#define BLUE_LED     (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 2*4)))
#define PUSH_BUTTON  (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 4*4)))


uint8_t data[1518];
// size of this web page is 573 bytes
char webPage1[]="HTTP/1.1 200 OK\r\n\r\n <html>\n\r<head><title> IoT Test Web Page </title>\n\r<link rel=icon type=image/ico href='http://192.168.1.71:7388/favicon.ico' /></head>\n\r"
"<title> IoT Test Web Page </title>"
"</head>"
"<body bgcolor=#A9E2F3>"
"<h1> IoT Test </h1>"
"<p> Enter Details </p>"
"<form action=MyPage.html  method=POST>"
  "Username: <input type=text name=username/><p></p>"
  "Password: <input type=password name = password/><p></p>"
  "Sensor:"
  "<br/>	<input type = radio name=sensor/> Sensor1 <br/>"
			"<input type = radio name=sensor/> Sensor2<br/>"
			"<input type=submit value=Login name=submit/>"
"</form>"
"</body>"
"</html>'\0'";

// size of this web page is 169 bytes
char webPage2[]="HTTP/1.1 200 OK\r\n\r\n <HTML><HEAD><TITLE>Push Button Status Page</TITLE></HEAD><font color=black>The status of the push button is = </font></form><BR>&nbsp;</BODY></HTML>";

// size of this web page is 630 bytes
char webPage3[]="HTTP/1.1 200 OK\r\n\r\n <html>\n\r<head><title> IoT Test Web Page </title>\n\r<link rel=icon type=image/ico href='http://192.168.1.71:7388/favicon.ico' /></head>\n\r"
"<title> IoT Test Web Page </title>"
"</head>"
"<body bgcolor=#A9E2F3>"
"<h1> IoT Test </h1>"
"<p> Enter Details </p>"
"<font color=red>Username or Password is Incorrect</font>"
"<form action=MyPage.html  method=POST>"
  "Username: <input type=text name=username/><p></p>"
  "Password: <input type=password name = password/><p></p>"
  "Sensor:"
  "<br/>	<input type = radio name=sensor/> Sensor1 <br/>"
			"<input type = radio name=sensor/> Sensor2<br/>"
			"<input type=submit value=Login name=submit/>"
"</form>"
"</body>"
"</html>'";

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Initialize Hardware
void initHw()
{
	// Configure HW to work with 16 MHz XTAL, PLL enabled, system clock of 40 MHz
    SYSCTL_RCC_R = SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_OSCSRC_MAIN | SYSCTL_RCC_USESYSDIV | (4 << SYSCTL_RCC_SYSDIV_S);

    // Set GPIO ports to use APB (not needed since default configuration -- for clarity)
    // Note UART on port A must use APB
    SYSCTL_GPIOHBCTL_R = 0;

    // Enable GPIO port B and E peripherals
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOB | SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOF | SYSCTL_RCGC2_GPIOA;

    // Configure LED and pushbutton pins
    GPIO_PORTF_DIR_R = 0x0E;  // bits 1-3 are outputs, other pins are inputs
    GPIO_PORTF_DR2R_R = 0x0E; // set drive strength to 2mA (not needed since default configuration -- for clarity)
    GPIO_PORTF_DEN_R = 0x1E;  // enable LEDs and pushbuttons
    GPIO_PORTF_PUR_R = 0x1E;  // enable internal pull-up for push button

    GPIO_PORTD_DIR_R = 0x0;  // bits 1-3 are outputs, other pins are inputs
    GPIO_PORTD_DEN_R = 0x0;  // enable LEDs and pushbuttons

    // Configure A0 and ~CS for graphics LCD
	GPIO_PORTB_DIR_R |= 0x44;  // make bits 2 outputs
	GPIO_PORTB_DR2R_R |= 0x44; // set drive strength to 2mA
	GPIO_PORTB_DEN_R |= 0x44;  // enable bits 1 and 6 for digital

	GPIO_PORTA_DIR_R |= 0x10;  // make bits 4 outputs
	GPIO_PORTA_DR2R_R |= 0x10; // set drive strength to 2mA
	GPIO_PORTA_DEN_R |= 0x10;  // enable bits 1 and 6 for digital

	// Configure SSI2 pins for SPI configuration
	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;           // turn-on SSI2 clocking
	GPIO_PORTA_DIR_R |= 0x24;                        // make bits 4 and 7 outputs
	GPIO_PORTA_DR2R_R |= 0x24;                       // set drive strength to 2mA
	GPIO_PORTA_AFSEL_R |= 0x24;                      // select alternative functions for MOSI, SCLK pins
	GPIO_PORTA_PCTL_R = GPIO_PCTL_PA5_SSI0TX | GPIO_PCTL_PA2_SSI0CLK; // map alt fns to SSI2
	GPIO_PORTA_DEN_R |= 0x24;                        // enable digital operation on TX, CLK pins

	// Configure the SSI2 as a SPI master, mode 3, 8bit operation, 1 MHz bit rate
	SSI0_CR1_R &= ~SSI_CR1_SSE;                      // turn off SSI2 to allow re-configuration
	SSI0_CR1_R = 0;                                  // select master mode
	SSI0_CC_R = 0;                                   // select system clock as the clock source
	SSI0_CPSR_R = 40;                                // set bit rate to 1 MHz (if SR=0 in CR0)
	SSI0_CR0_R = SSI_CR0_SPH | SSI_CR0_SPO | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_8; // set SR=0, mode 3 (SPH=1, SPO=1), 8-bit
	SSI0_CR1_R |= SSI_CR1_SSE;                       // turn on SSI2

    // Configure ~CS for ENC28J60
    GPIO_PORTB_DIR_R = 0x02;  // make bit 1 an output
    GPIO_PORTB_DR2R_R = 0x02; // set drive strength to 2mA
    GPIO_PORTB_DEN_R = 0x02;  // enable bits 1 for digital

    // Configure SSI2 pins for SPI configuration
    SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R2;           // turn-on SSI2 clocking
    GPIO_PORTB_DIR_R |= 0x90;                        // make bits 4 and 7 outputs
    GPIO_PORTB_DR2R_R |= 0x90;                       // set drive strength to 2mA
	GPIO_PORTB_AFSEL_R |= 0xD0;                      // select alternative functions for MOSI, MISO, SCLK pins
    GPIO_PORTB_PCTL_R = GPIO_PCTL_PB7_SSI2TX | GPIO_PCTL_PB6_SSI2RX | GPIO_PCTL_PB4_SSI2CLK; // map alt fns to SSI2
    GPIO_PORTB_DEN_R |= 0xD0;                        // enable digital operation on TX, RX, CLK pins

    // Configure the SSI2 as a SPI master, mode 3, 8bit operation, 1 MHz bit rate
    SSI2_CR1_R &= ~SSI_CR1_SSE;                      // turn off SSI2 to allow re-configuration
    SSI2_CR1_R = 0;                                  // select master mode
    SSI2_CC_R = 0;                                   // select system clock as the clock source
    SSI2_CPSR_R = 40;                                // set bit rate to 1 MHz (if SR=0 in CR0)
    SSI2_CR0_R = SSI_CR0_FRF_MOTO | SSI_CR0_DSS_8;   // set SR=0, mode 0 (SPH=0, SPO=0), 8-bit
    SSI2_CR1_R |= SSI_CR1_SSE;                       // turn on SSI2

    // Configure pushbutton pins
    GPIO_PORTF_DIR_R |= 0x08;  // bits 1 and 3 are outputs, other pins are inputs
    GPIO_PORTF_DR2R_R |= 0x08; // set drive strength to 2mA (not needed since default configuration -- for clarity)
    GPIO_PORTF_DEN_R |= 0x18;  // enable LEDs and pushbuttons
    GPIO_PORTF_PUR_R |= 0x10;  // enable internal pull-up for push button

}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(void)
{
	uint8_t* udpData,tcpFlags;
	uint8_t* tcpData;
	uint8_t establishedFlag = 0,terminateFlag = 0 , loginFlag = 0;
	extern int getFlag,postFlag,favFlag,fynFlag;
	// init controller
	initHw();
	initGraphicsLcd();
	setGraphicsLcdTextPosition(0, 0);
	putsGraphicsLcd(" Welcome IoT-Project");
	// init ethernet interface
	etherInit(ETHER_UNICAST | ETHER_BROADCAST | ETHER_HALFDUPLEX);
	etherSetIpAddress(192,168,1,2);

	// flash phy leds
	etherWritePhy(PHLCON, 0x0880);
	RED_LED = 1;
	waitMicrosecond(500000);
	etherWritePhy(PHLCON, 0x0990);
	RED_LED = 0;
	waitMicrosecond(500000);

    // message loop
	while (1)
	{
		if (etherKbhit())
		{
			if (etherIsOverflow())
			{
				RED_LED = 1;
				waitMicrosecond(100000);
				RED_LED = 0;
			}
			// get packet
			memset(data,0,1518);
			etherGetPacket(data, 1518);
			// handle arp request
			if (etherIsArp(data))
			{
				etherSendArpResp(data);
				RED_LED = 1;
				GREEN_LED = 1;
				waitMicrosecond(50000);
				RED_LED = 0;
				GREEN_LED = 0;
			}
		// handle ip datagram
			if (etherIsIp(data))
			{
				if (etherIsIpUnicast(data))
				{
					// handle icmp ping request
					if (etherIsPingReq(data))
					{
						etherSendPingResp(data);
						RED_LED = 1;
						BLUE_LED = 1;
						waitMicrosecond(50000);
						RED_LED = 0;
						BLUE_LED = 0;
					}
					// handle udp datagram
					if (etherIsUdp(data))
					{
						udpData = etherGetUdpData(data);
						if (udpData[0] == '1')
						GREEN_LED = 1;
						if (udpData[0] == '0')
						GREEN_LED = 0;
					etherSendUdpData(data, (uint8_t*)"Received", 9);
					}
					if(etherIsTCP(data))
					{
						processTCPData(data);
						if(!establishedFlag)
						{
							tcpFlags = etherGetTCPFlags(data);
							switch(tcpFlags)
							{
							case synFlag:			// received SYN
								sendSynAck(data, (uint8_t*)"", 0);
							break;
							case ackFlag:			// received ACK
								BLUE_LED = 1;
								establishedFlag = 1;
							break;
							}
						}
						else if(establishedFlag && !terminateFlag)
						{

							if(getFlag)
							{
								sendAck(data, (uint8_t*)"", 0);
								sendWebpage(data,(uint8_t*)webPage1, 573);
								tcpFlags = ackFlag | pushFlag;
								getFlag = 0;
							}
							else if(postFlag)
							{
								sendAck(data, (uint8_t*)"", 0);
								uint8_t userOK = 0,passwordOK = 0,sensorOK = 0,pushButton;
								postFlag = 0;
								tcpData = etherGetTCPData(data);
								userOK = getPOSTData((char*)tcpData, "username");
								passwordOK = getPOSTData((char*)tcpData, "password");
								sensorOK = getPOSTData((char*)tcpData, "sensor");
								if(sensorOK && passwordOK && userOK)
								{
									loginFlag = 1;
									pushButton = GPIO_PORTF_DATA_R & 0x10;
									if(pushButton)
										sendWebpage(data,(uint8_t*)"HTTP/1.1 200 OK\r\n\r\n <HTML><HEAD><TITLE>Push Button Status Page</TITLE></HEAD><font color=black>The status of the push button is = NOT PRESSED</font></form><BR>&nbsp;</BODY></HTML>", 180);
									else
										sendWebpage(data,(uint8_t*)"HTTP/1.1 200 OK\r\n\r\n <HTML><HEAD><TITLE>Push Button Status Page</TITLE></HEAD><font color=black>The status of the push button is = PRESSED</font></form><BR>&nbsp;</BODY></HTML>", 176);
								}
								else
								{
									loginFlag = 0;
									sendWebpage(data,(uint8_t*)webPage3, 630);
								}
							}
							tcpFlags = etherGetTCPFlags(data);
							switch(tcpFlags)
							{
								case ackFlag:			// received SYN
									sendFinAck(data, (uint8_t*)"", 0);
									terminateFlag = 1;
									BLUE_LED = 0;
								break;
							}
						}
						else
						{
							tcpFlags = etherGetTCPFlags(data);
							switch(tcpFlags)
							{
								case finFlag | ackFlag:			// received SYN
									fynFlag = 1;
									sendAck(data, (uint8_t*)"", 0);
									terminateFlag = 0;
									BLUE_LED = 0;
									establishedFlag = 0;
									fynFlag = 0;
								break;
							}
						}
					}
				}
			}
		}
	}
    return 0;
}
