void INICIAL(void)
{
		START();
	TWI_OUT(0xD0);
	TWI_OUT(0x07);
	TWI_OUT(0b00010000);
		STOP();

}

void CLOC_OUT(volatile unsigned char SEC_OUT,volatile unsigned char MIN_OUT,volatile unsigned char HOUR_OUT,volatile unsigned char WEEK_OUT,volatile unsigned char DATA_OUT,volatile unsigned char MONTH_OUT)
{
		START();
	TWI_OUT(0xD0);
	TWI_OUT(0);
	TWI_OUT(SEC_OUT);
	TWI_OUT(MIN_OUT);
	TWI_OUT(HOUR_OUT);
	TWI_OUT(WEEK_OUT);
	TWI_OUT(DATA_OUT);
	TWI_OUT(MONTH_OUT);
	STOP();
	START();
	TWI_OUT(0xD0);
	TWI_OUT(0);
	STOP();
	START();
	TWI_OUT(0xD1);
	TWI_IN();
	NACK();
	STOP();////
	

	OBNOV();
}

void OBNOV(void)
{
	PORTD&=~(1<<PD1|1<<PD0);
		START();
	TWI_OUT(0xD0);
	TWI_OUT(0);
	STOP();
		START();
	TWI_OUT(0xD1);
	TWI_IN();
	SEC[1]=GO_IN[0];
	SEC[0]=GO_IN[1];
	ACK();
	TWI_IN();
	MIN[0]=GO_IN[1];
	MIN[1]=GO_IN[0];
	ACK();
	TWI_IN();
	HOUR[1]=GO_IN[0];
	HOUR[0]=GO_IN[1];
	ACK();
	TWI_IN();
	WEEK[1]=GO_IN[0];
	WEEK[0]=GO_IN[1];
	ACK();
	TWI_IN();
	DAY[1]=GO_IN[0];
	DAY[0]=GO_IN[1];
	ACK();
	TWI_IN();
	MONTH[1]=GO_IN[0];
	MONTH[0]=GO_IN[1];
	NACK();
	STOP();
}

