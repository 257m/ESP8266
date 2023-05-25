#ifndef UART_IO_H
#define UART_IO_H
#include "settings.h"

#define UART_BASE	0x60000000
#define UART1_BASE	0x60000F00

#define IOMUX_BASE	0x60000800

#define UART_CLOCK	(80 * 1000 * 1000)

// Much better than uart_register.h and using defines for each
typedef struct {
	volatile unsigned long fifo;		/* 00 */
	volatile unsigned long int_raw;		/* 04 */
	volatile unsigned long int_status;	/* 08 */
	volatile unsigned long int_ena;		/* 0C */
	volatile unsigned long int_clear;	/* 10 */
	volatile unsigned long clkdiv;		/* 14 */
	volatile unsigned long autobaud;	/* 18 */
	volatile unsigned long status;		/* 1c */
	volatile unsigned long conf0;		/* 20 */
	volatile unsigned long conf1;		/* 24 */
	volatile unsigned long low_pulse;	/* 28 */
	volatile unsigned long high_pulse;	/* 2c */
	volatile unsigned long rxd_count;	/* 30 */
} Uart;

// Defines stolen from trebisky

/* bits in status register */
#define ST_TXD		0x80000000
#define ST_RTS		0x40000000
#define ST_DTR		0x20000000

#define ST_RXD		0x00008000
#define ST_CTS		0x00004000
#define ST_DSR		0x00002000

#define ST_TX_MASK	0x00FF0000
#define ST_RX_MASK	0x000000FF

/* bits in conF0 register */
#define C_DTR_INV	0x01000000
#define C_RTS_INV	0x00800000
#define C_TXD_INV	0x00400000
#define C_DSR_INV	0x00200000
#define C_CTS_INV	0x00100000
#define C_RXD_INV	0x00080000

#define C_TX_RESET	0x00040000
#define C_RX_RESET	0x00020000

#define C_TX_FLOW_ENA	0x00008000
#define C_LOOPBACK	0x00004000
#define C_TXD_BRK	0x00000100	/* reserved - do not change this bit */
#define C_SW_DTR	0x00000080
#define C_SW_RTS	0x00000040
#define C_STOP		0x00000030	/* stop - 2 bit field */
#define C_SIZE		0x0000000C	/* size - 2 bit field */
#define C_PAR_ENA	0x00000002
#define C_PAR_ODD	0x00000001

#define C_STOP1		0x00000010	/* 1 stop bit */
#define C_STOP15	0x00000020	/* 1.5 stop bit */
#define C_STOP2		0x00000030	/* 2 stop bit */

#define C_5BIT		0x00000000
#define C_6BIT		0x00000004
#define C_7BIT		0x00000008
#define C_8BIT		0x0000000C

/* bits in conf1 register */
#define C1_RX_TOUT	0x80000000	/* Rx timeout enable */
#define C1_RX_THR	0x7F000000	/* Rx timeout threshold */
#define C1_RX_FLOW	0x00800000	/* Rx flow control enable */
#define C1_RX_FTHR	0x007F0000	/* Rx flow control threshold */

#define C1_TX_EMPTY	0x00007F00	/* Tx empty threshold */
#define C1_RX_FULL	0x0000007F	/* Rx full threshold */

/* ------------------------------------------------ */
/* ------------------------------------------------ */

/* These 32 bit registers seem to have 9 active bits.
 * There are 3 function control bits (with a 2 bit gap)
 * And there are 2 pullup enable bits
 *
 *		F 00FF Pp00
 */

typedef struct {
	volatile unsigned long conf;		/* 00 */
	volatile unsigned long mtdi;		/* 04 */
	volatile unsigned long mtck;		/* 08 */
	volatile unsigned long mtms;		/* 0C */

	volatile unsigned long mtdo;		/* 10 */
	volatile unsigned long u0_rxd;		/* 14 */
	volatile unsigned long u0_txd;		/* 18 */
	volatile unsigned long sd_clk;		/* 1c */

	volatile unsigned long sd_data0;	/* 20 */
	volatile unsigned long sd_data1;	/* 24 */
	volatile unsigned long sd_data2;	/* 28 */
	volatile unsigned long sd_data3;	/* 2c */

	volatile unsigned long sd_cmd;		/* 30 */
	volatile unsigned long gpio0;		/* 34 */
	volatile unsigned long gpio2;		/* 38 */
	volatile unsigned long gpio4;		/* 3c */

	volatile unsigned long gpio5;		/* 40 */
} Iomux;

#define F_U0TXD		0

#define PULLUP_ENA		0x80
#define PULLUP2_ENA		0x40	/* XXX */

extern void uart_init(int baud);
extern void uart_fifo_reset(void);
extern void uart_putchar(unsigned char c);
extern unsigned char uart_getchar(void);

#endif /* UART_IO_H */