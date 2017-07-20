/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*-----------------------------------------------------------------------------+
| Common Macros                                                                |
+-----------------------------------------------------------------------------*/
#if 0
/* only for STA mode, beacuse GPIO 10 is mode selection pin.
   NIC mode(LOW) would make SDA data wrong */
#define SDA 10
#define SCL 19
#else//only for testing
#define SDA 18
#define SCL 17
#endif

enum {
//	GPIO_IN = 0,
//	GPIO_OUT = 1,
	delay_us = 1,
	delay_mid = 2,
	delay_last = 5,
};

/*-----------------------------------------------------------------------------+
| Function Prototypes                                                          |
+-----------------------------------------------------------------------------*/
int i2c_send_byte(unsigned char slave_addr, unsigned char byte);
int i2c_send_str(unsigned char slave_addr, char *str);
int i2c_send_data(unsigned char slave_addr, char *str, int len);

int i2c_read_byte(unsigned char slave_addr);
void i2c_read_data(unsigned char slave_addr, char *str, int len);



