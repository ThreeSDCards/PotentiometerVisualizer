#include "uartlib.h"

Bool uart_init(baudrate b, clockspeed c){
    char i;
    //Calculate errors of possible UART configs
    float errors[NUMOFMULTIPLIERS];
    unsigned short baudvalues[NUMOFMULTIPLIERS];
    for(i=0;i<NUMOFMULTIPLIERS;i++){ //Iterative to check all possible multipliers
    unsigned short _SPBRG = ROM_GLOB_SPBRGLOOKUP[c][b][i];
    baudvalues[i] = _SPBRG;
    if(_SPBRG != 0){
	errors[i] = (float)CLOCKSPEED_VALUES[c] / ((float)MULTIPLIER_VALUES[i] * (float)(_SPBRG + 1));
	errors[i] = absolute(100.0f - (errors[i] / (float)BAUD_VALUES[b] * 100.0f));
    } else{
	errors[i] = 100.0f;
    }
    }

    //find smallest error
    float *first = errors;
    float *last = &errors[NUMOFMULTIPLIERS - 1];
    unsigned short *firstSPBRG = baudvalues;
    unsigned short *lastSPBRG = &baudvalues[NUMOFMULTIPLIERS - 1];
    while(first != last){
	    if(*first <= *last){
		    last--;
	lastSPBRG--;
	    }else{
		    first++;
	firstSPBRG++;
    }
    }

    //First is a pointer to the lowest error in the errors array.
    SPBRG = (*firstSPBRG);
    SYNC = 0;
    
    if(SPBRG == baudvalues[0]){
	BRGH = 1;
	BRG16 = 1;
    }else if(SPBRG == baudvalues[1]){
	BRGH = 1;
	BRG16 = 0;
    }else if(SPBRG == baudvalues[2]){
	BRGH = 0;
	BRG16 = 1;
    }else if(SPBRG == baudvalues[3]){
	BRGH = 0;
	BRG16 = 0;
    }

    // ** IO **
    TRISBbits.TRISB5 = 1;       // RX is input
    TRISBbits.TRISB7 = 0;       // TX is output
    ANSELB = ~(~ANSELB | 0xA0);
    
    //Enable receive interrupt
    SPEN = 1;
    TXEN = 1;
    CREN = 1;
    RCIE = 1;
    PEIE = 1;
    TXIE = 0;
    GIE = 1;

    FIFOcount = 0;
    FIFOhead = &FIFO[0];
    FIFOtail = &FIFO[1];
    return True;
}

void uart_write_char(unsigned char byte){
    while(!TRMT);
	TXREG = byte;
}
void uart_write_string(const unsigned char *str){
	char i;
    char length = 0;
    while(*(str + length) != '\0'){
        length++;
    }
	for(i=0;i<=length;i++){
		uart_write_char(*(str +i));
	}
	uart_write_char((unsigned char)'\0');
}

void uart_write_newline(void){
    uart_write_char((unsigned char)'\r');
    uart_write_char((unsigned char)'\n');
}

void uart_write_line(const unsigned char *str){
	char i;
	for(i=0;str[i]!='\0';i++){
		uart_write_char(str[i]);
	}
	uart_write_newline();
}

void uart_handle_rcv_int(void){
	*FIFOtail = RCREG;
	if(FIFOtail == &FIFO[BUFFERSIZE - 1])	//Move tail forwards in FIFO
	    FIFOtail = FIFO;
	else
	    FIFOtail++;

	if(FIFOtail == FIFOhead){ 		//Check if head and tail are next to one another
	    if(FIFOhead == &FIFO[BUFFERSIZE - 1])	//Move head forwards in FIFO
		FIFOhead = FIFO;
	    else
		FIFOhead++;
	}
    FIFOcount++;
}
unsigned char uart_read_char(void){
	if(FIFOhead == &FIFO[BUFFERSIZE-1])	//Move head forward one position.
		FIFOhead = FIFO;
	else
		FIFOhead++;
	if(FIFOhead == FIFOtail){	//Adjust tail in case head and tail are equal
		if(FIFOtail == &FIFO[BUFFERSIZE-1])
			FIFOtail = FIFO;
		else
			FIFOtail++;
	}
    char out = *FIFOhead;
    *FIFOhead = 0;
    FIFOcount--;
	return out;	//Return value of head.
}
unsigned char uart_numof_bytes_in_buffer(void){
	return FIFOcount;
}

void uart_flush_buffer(){
	char i;
	for(i=0;i<BUFFERSIZE;i++){
		FIFO[i] = 0;
	}
	FIFOhead = FIFO;
	FIFOtail = &FIFO[1];
}

float absolute(float v){
    return (v > 0) ? v : v * -1.0f;
}