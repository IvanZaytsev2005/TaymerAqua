/*
 * TAYM.c
 *
 * Created: 24.01.2021 20:41:39
 *  Author: HP
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
void DELAY_TWI (void);
void TWI_OUT(volatile char data);
void START(void);
void STOP (void);
void TWI_IN(void);
void NACK (void);
void ACK (void);
void OBNOV(void);
void INICIAL(void);
unsigned char EEPROM_read(unsigned int uiAddress);
void EEPROM_write(unsigned int Address, unsigned char Data);
void CLOC_OUT(volatile unsigned char SEC_OUT,volatile unsigned char MIN_OUT,volatile unsigned char HOUR_OUT,volatile unsigned char WEEK_OUT,volatile unsigned char DATA_OUT,volatile unsigned char MONTH_OUT);

#define CS PB5
#define OE PB2
#define MR PB0
#define mosi PB3
#define sck PB1
#define sda PD0
#define scl PD1
#define MAX_LIGHT 78
#define MIN_LIGHT 74
#define SVET 0// ¬€¬Œƒ “≈Ãœ≈–¿“”–€
#define CLOC 1//¬€¬Œƒ ¬–≈Ã≈Õ»
#define CLOC_EN 2//Œ—¬≈Ÿ≈Õ»≈ ¬ Àﬁ◊≈ÕŒ
#define LUNA_EN 3//À”ÕÕ€… —¬≈“ ¿ “»¬≈Õ
#define PLUS 4//
#define MINUS 5


volatile char FLAG;
volatile unsigned int ICR1_BUF;

void spi_out(volatile unsigned char a);
void DELAY(void);
volatile int cloc;


volatile char TEMPO[110]={67,  69,  71,  74,  76,  78,  80,  83,  85,  87,  90,  90,  91,  93,  95,  97,  100,  103,  107,  110,  113,  116,  120,  123,  126,  129,  131,  134,  137,  139,  142,  144,  147,  149,  151,  153,  156,  158,  160,  162,  164,  166,  168,  170,  172,  174,  176,  178,  180,  182,  184,  186,  188,  190,  191,  193,  195,  197,  199,  200,  202,  203,  204,  205,  206,  208,  209,  210,  211,  213,  214,  215,  216,  218,  219,  220,  221,  222,  223,  224,  224,  225,  226,  227,  227,  228,  229,  230,  230,  231,  232,  232,  233,  234,  234,  235,  236,  236,  237,  238};
volatile char NOMER[110]={ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99};

volatile int x;
//volatile long THEW;
volatile char NUMBER[13]={0b11010111, 0b00010100,0b11001101,0b01011101,0b00011110,0b01011011,0b11011011,0b00010111,0b11011111,0b01011111,0b11000011,0b00001111};
unsigned char GO_OUT[4]={7,5,0,1};
unsigned char GO_IN[2]={0,0};
unsigned char SEC[2];
unsigned char MIN[2];
unsigned char HOUR[2];
unsigned char DAY[2];
unsigned char MONTH[2];
unsigned char WEEK[2];
 //char TERM[5];
unsigned char LIGHT[2];
unsigned char ADC_TEMP;
unsigned char ADC_LIGHT;

int main(void)
{
	
	DDRB=(1<<PB3)|(1<<PB1)|(1<<OE)|(1<<CS)|(1<<MR);
	PORTB=(0<<OE)|(1<<CS)|(0<<MR);
	DELAY();
	DDRD=1<<PD4|1<<PD5|1<<PD6|1<<PD7;
	PORTD=0;
	PORTB|=(1<<MR);	
// 	
 	DDRB|=1<<PB7;
//	//Ì‡ÒÚÓÈÍË ¿÷œ 
//	ADMUX=1<<ADLAR|1<<MUX1|1<<MUX0|1<<REFS0;
//	ADCSRA=1<<ADEN|1<<ADPS2|1<<ADPS1|1<<ADPS0|1<<ADSC|1<<ADIE;
// 	

// Ú‡ÈÏÂ ‰Îˇ ÓÒÌÓ‚ÌÓ„Ó ˆËÍÎ‡
 		TCCR0=(1<<CS02)|(1<<CS00);
 		TCNT0=0x00;
 		TIMSK|=1<<TOIE0;
 	
//  	 	EEPROM_read(0);
	//Õ¿—“–Œ… » “¿…Ã≈–¿
	TCCR1A=1<<WGM11;
	TCCR1B=1<<WGM13|1<<WGM12|1<<CS11|1<<CS10;
	ICR1=0xFFFF;//LIGHT[1]*0x2-LIGHT[0]*0x2;
	/*OCR1A=0xFFF;*/
	OCR1A=0x888;
	ICR1=0x4FB;
	TIMSK|=1<<OCIE1A|1<<TOIE1|1<<OCIE1B;
// 	
//  	DDRC=0;
//  	PORTC=0;
// 	
	//Ì‡ÒÚÓÈÍË ÔÂ˚‚‡ÌËˇ ÓÚ ˜‡ÒÓ‚
	MCUCR=1<<ISC01|1<<ISC11;
/*	*/GICR=1<<INT0|1<<INT1;
	
	//Õ¿—“–Œ… » œ—≈¬ƒŒÿ»Ã
	TCCR2=1<<CS22|1<<WGM21|1<<WGM20;
	OCR2=39;
	TIMSK|=1<<TOIE2|1<<OCIE2;
// 	
// 
// 
// 	//«¿œ»—‹ MAX » MIN «Õ¿◊≈Õ»… Œ—¬≈Ÿ≈Õ»ﬂ »« EEPROM
// 	//TCNT2=0x45;//LIGHT[0];
// 	LIGHT[0]=EEDR;
// 	EEPROM_read(1);
// 	LIGHT[1]=EEDR;
// 
 	DDRC|=1<<PC0|1<<PC1;
 	PORTC|=1<<PC0|1<<PC1;
// 	
 	INICIAL();
	 START();
	TWI_OUT(0xD0);
	TWI_OUT(0);
	TWI_OUT(0);
	STOP();
//	CLOC_OUT(0x0,0x20,0x20,0x07,0x23,0x2);
//  	ADMUX=1<<ADLAR|1<<MUX1|0<<MUX0|1<<REFS0;
//  //	ADCSRA=1<<ADEN|1<<ADPS2|1<<ADPS1|1<<ADPS0|1<<ADSC|1<<ADIE;
// 	
 FLAG=0;
// x=0xFFF;
// 			ICR1_BUF=(LIGHT[1]*0xFF-LIGHT[0]*0xFF);
// 			ICR1=ICR1_BUF;
// 
// 
 ICR1_BUF=0x4FB;
OBNOV();
	 while(1)
    {		
		sei();
    }
}

#include "DS1307.c"
#include "SPI_OUT.c"
#include "TWI.c"
#include "DELAY.c"
#include "INTER_EEPROM.c"

ISR (INT0_vect)
{
//	OCR1A=0xF0;
	
	SEC[0]=SEC[0]+1;
	if(SEC[0]>=10)
	{
		SEC[0]=0;
		if((FLAG & (1<<CLOC))==(1<<CLOC))
		{
			FLAG&=~(1<<CLOC);
		}
		else
		{
			FLAG|=(1<<CLOC);
		}
		SEC[1]=SEC[1]+1;
		if(SEC[1]>=6)
		{
			SEC[1]=0;
			MIN[0]=MIN[0]+1;
			if(MIN[0]>=10)
			{
				MIN[0]=0;
				MIN[1]=MIN[1]+1;
				if(MIN[1]>=6)
				{
					MIN[1]=0;
					HOUR[0]=HOUR[0]+1;
					if ((HOUR[0]>=10))
					{
						HOUR[0]=0;
						HOUR[1]=HOUR[1]+1;
					}
					if ((HOUR[0]+HOUR[1]*10)>=24)
					{
						HOUR[0]=0;
						HOUR[1]=0;
						OBNOV();
					}
				}
				
			}
		}
	}
}			
ISR (INT1_vect)
{}

ISR (TIMER2_COMP_vect)
{
	sei();
	PORTD=0xFF;
}
ISR (TIMER2_OVF_vect)
{
	sei();
	spi_out(0);
	PORTD=~(0b00010000<<cloc);//0b00010000- ‡‰ÂÒ ÔÂ‚Ó„Ó ·ËÚ‡
//	volatile char c;
//	c=;
	if((cloc==2)&((PIND&0b00000100)==0b00000100))
	{
		spi_out((NUMBER[(GO_OUT[cloc])])|0b00100000);
//		PORTB|=(1<<PB7);
	}
	else
	{
		spi_out(NUMBER[(GO_OUT[cloc])]);
	}	
	cloc++;
	if (cloc>3)
	{
		cloc=0;
	}
	sei();
}


ISR(TIMER0_OVF_vect)
{
	sei();
	//Œ—ÕŒ¬ÕŒ… ÷» À
	ADCSRA=1<<ADEN|1<<ADPS2|1<<ADPS1|1<<ADPS0|1<<ADSC|1<<ADIE;// «¿œ”—  ¿÷œ
	
	//¬ «¿¬»—»ÃŒ—“» Œ“ —Œ—“ŒﬂÕ»ﬂ ¡»“¿ CLOC ¬ FLAG ¬€—“¿¬Àﬂ≈Ã À»¡Œ “≈Ãœ≈–¿“”–”, À»¡Œ ¬–≈Ãﬂ Õ¿ ƒ»—œÀ≈…
	if((FLAG & (1<<CLOC))==(1<<CLOC))
	{
		GO_OUT[0]=MIN[0];
		GO_OUT[1]=MIN[1];
		GO_OUT[2]=HOUR[0];
		GO_OUT[3]=HOUR[1];
	}
	else
	{
		volatile char CC;
 		CC=0;
		while ((TEMPO[CC]<=ADC_TEMP)&(CC<0x6F))
		{
			CC++;
		}
		CC=NOMER[CC];
		GO_OUT[1]=11;
		GO_OUT[0]=10;
		GO_OUT[3]=(CC/10);
		GO_OUT[2]=CC-(CC/10)*10;
	}
// }	
// 	//œ≈–≈—◊®“ OCR1A ¬ «¿¬»—»ÃŒ—“» Œ“ ¬Õ≈ÿÕ≈√Œ Œ—¬≈Ÿ≈Õ»ﬂ » —Œ—“ŒﬂÕ»ﬂ: –¿——¬≈“, œŒÀƒ≈Õ‹, «¿ ¿“, À”Õ¿, œŒÀÕŒ◊‹

		switch(FLAG&~(1<<CLOC))
		{
			case 0:
			TIMSK&=~(1<<OCIE1A);
			TIMSK&=~(1<<OCIE1B);
			if((HOUR[0]+HOUR[1]*10)>=8)
			{
				TIMSK|=1<<OCIE1A;
				FLAG=(FLAG&(1<<CLOC))+(1<<PLUS);
			}
			
			break;
			case (1<<PLUS):
			ICR1_BUF--;
			TIMSK|=1<<OCIE1A;
			if (ICR1_BUF==0)
			{
				FLAG=(FLAG&(1<<CLOC))+(1<<SVET);
			}
	//		OCR1A=x;//+ADC_LIGHT*0xFF;
			
			break;
			case (1<<SVET):
			OCR1A=0xFF*5-ADC_LIGHT*0x5;
			if (((HOUR[0]+HOUR[1]*10)==21)&((MIN[0]+MIN[1]*10)==12))
			{
				FLAG=(FLAG&(1<<CLOC))+(1<<MINUS);
			}
			break;
			case (1<<MINUS):
			ICR1_BUF++;
			if (ICR1_BUF==0x4FB)
			{
				FLAG=(FLAG&(1<<CLOC))+(1<<LUNA_EN);
				TIMSK&=~(1<<OCIE1A);
				TIMSK|=1<<OCIE1B;
				OCR1B=0xFF*5;
			}
			break;
			case (1<<LUNA_EN):
			if (HOUR[0]+HOUR[1]==0)
			{
				FLAG=FLAG&(1<<CLOC);
			}
			break;
		}
}

ISR (TIMER1_COMPA_vect)
{
	PORTC|=1<<PC1|1<<PC0;
}

ISR (TIMER1_COMPB_vect)
{
	PORTC|=1<<PC0;
//	PORTD=0xFF;
}

ISR (TIMER1_OVF_vect)
{
	if ((FLAG&(1<<PLUS))==(1<<PLUS)|(FLAG&(1<<MINUS))==(1<<MINUS))
		{
		OCR1A=ICR1_BUF;
		}
	PORTC&=~(1<<PC0|1<<PC1);
	
}




ISR (ADC_vect)
{
 	volatile char CC;
 	CC=ADCH;
	if ((ADMUX&(1<<MUX0))==1)
	{
		ADC_TEMP=CC;
		ADMUX=1<<ADLAR|1<<MUX1|0<<MUX0|1<<REFS0;
	}
	else
	{
		ADMUX=1<<ADLAR|1<<MUX1|1<<MUX0|1<<REFS0;
		ADC_LIGHT=CC;
	}	
}

// void INCREMENT (void)//ÔÎ‡‚ÌÓÂ Û‚ÂÎË˜ÂÌËÂ ˇÍÓÒÚË
// {
// 	FLAG|=1<<PLUS;
// 	switch(FLAG&(1<<PLUS|1<<MINUS|1<<SVET|1<<CLOC|1<<CLOC_EN|1<<LUNA_EN))
// 	case:
// }	