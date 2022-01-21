#ifndef UART_LIB_H
#define	UART_LIB_H
#include <xc.h>

#define BUFFERSIZE 100
#define BUFFEROVERWRITE
#define NUMOFINTOSCSPEEDS 7
#define NUMOFBAUDRATES 8
#define NUMOFMULTIPLIERS 4

typedef enum {False, True} Bool;
typedef enum {FIVEK, ONEM, TWOM, FOURM, EIGHTM, SIXTEENM, THIRTYTWOM} clockspeed;
typedef enum {THREEHUNDRED, TWELVEH, TWENTYFOURH, NINETYSIXH, TENFOURSEVENTEEN, ONENINETYTWOHUNDERED, FIFTYSEVENK, ONEFIFTEENK} baudrate;
typedef enum {FOUR = 4, SIXTEENCHAR = 16, SIXTEENSHORT = 16, SIXTYFOUR = 64} multiplier;

const float CLOCKSPEED_VALUES[] = {5000.0f, 1000000.0f, 2000000.0f, 4000000.0f, 8000000.0f, 16000000.0f, 32000000.0f};
const float BAUD_VALUES[] = {300.0f, 1200.0f, 2400.0f, 9600.0f, 10417.0f, 19200.0f, 57600.0f, 115200.0f};
const float MULTIPLIER_VALUES[] = {4.0f, 16.0f, 16.0f, 64.0f};

const unsigned int ROM_GLOB_SPBRGLOOKUP[NUMOFINTOSCSPEEDS][NUMOFBAUDRATES][NUMOFMULTIPLIERS] = {
	{
		{416, 103, 103, 25},
		{103, 25, 25, 0},
		{51, 12, 12, 0},
		{12, 0, 0, 0},
		{11, 2, 2, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{832, 207, 207, 51},
		{207, 51, 51, 12},
		{103, 25, 25, 0},
		{25, 0, 0, 0},
		{23, 5, 5, 0},
		{12, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{1666, 0, 416, 103},
		{416, 103, 103, 25},
		{207, 51, 51, 12},
		{51, 12, 12, 0},
		{47, 11, 11, 2},
		{25, 0, 0, 0},
		{8, 0, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{3332, 0, 832, 207},
		{832, 207, 207, 51},
		{416, 103, 103, 25},
		{103, 25, 25, 0},
		{95, 23, 23, 5},
		{51, 12, 12, 0},
		{16, 0, 0, 0},
		{8, 0, 0, 0}
	},
	{
		{6666, 0, 1666, 416},
		{1666, 0, 416, 103},
		{832, 207, 207, 51},
		{207, 51, 51, 12},
		{191, 47, 47, 11},
		{103, 25, 25, 0},
		{34, 8, 8, 0},
		{16, 0, 0, 0}
	},
	{
		{13332, 0, 3332, 832},
		{3332, 0, 832, 207},
		{1666, 0, 416, 103},
		{416, 103, 103, 25},
		{383, 95, 95, 23},
		{207, 51, 51, 12},
		{68, 16, 16, 0},
		{34, 8, 8, 0}
	},
	{
		{26666, 0, 6666, 1666},
		{6666, 0, 1666, 416},
		{3332, 0, 832, 207},
		{832, 207, 207, 51},
		{767, 191, 191, 47},
		{416, 103, 103, 25},
		{138, 34, 34, 8},
		{68, 16, 16, 0}
	}
};

unsigned char FIFOcount;
unsigned char FIFO[BUFFERSIZE];
unsigned char *FIFOhead = &FIFO[0];
unsigned char *FIFOtail = &FIFO[1];

Bool uart_init(baudrate b, clockspeed c);
void uart_write_char(unsigned char byte);
void uart_write_string(const unsigned char *str);
void uart_write_newline(void);
void uart_write_line(const unsigned char *str);
void uart_handle_rcv_int(void);
unsigned char uart_read_char(void);
unsigned char uart_numof_bytes_in_buffer(void);
void uart_flush_buffer();

float absolute(float v);

#endif	/* UART_LIB_H */
