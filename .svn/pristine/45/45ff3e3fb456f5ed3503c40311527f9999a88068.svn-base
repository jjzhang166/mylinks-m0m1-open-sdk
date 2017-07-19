/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*-----------------------------------------------------------------------------+
| Common Macros                                                                |
+-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------+
| Function Prototypes                                                          |
+-----------------------------------------------------------------------------*/
void serial_conf(int br_id, int parity, int stopbits, int chan, int threshold);
int serial_init(int chan);

int uart_no_wait_putc(int chan, int c);
int uart_timeout_getc(int chan);
void uart_set_timeout(unsigned int set_timeout);

struct serial_buffer* serial_buf_init(unsigned int size);
int serial_buffer_enqueue(struct serial_buffer *ptxbuf, char *pdata , int datalen);
int serial_write(int chan, char* pdata, int datalen);
int serial_read_byte(int mode, int chan, char *buf, int len, char end_c);
char *serial_read_string(int mode, int chan, char end_c);

#ifdef CONFIG_UR_FLOW_CONTROL
void uart_flow_cts_stat(void);
#endif

