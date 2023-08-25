

void TWI_OUT(volatile char data)
{
	volatile char b;
	b=0;
	while(b<8)
	{
		SREG=0;
		data=data<<1;
		if((SREG&1)==1)
		{
			DDRD&=~(1<<sda);
			DDRD&=~(1<<scl);
			DELAY_TWI();
		}
		else
		{
			DDRD|=(1<<sda);
			DDRD&=~(1<<scl);
			DELAY_TWI();
		}
		DDRD|=(1<<scl);
		DDRD|=(1<<sda);
		DELAY_TWI();
		b++;
	}
	DDRD&=~(1<<scl);
	DELAY_TWI();
	DDRD|=1<<scl;
}

void TWI_IN(void)
{
	GO_IN[0]=0;
	GO_IN[1]=0;
	volatile char b;
	b=0;
	DDRD&=~(1<<sda);
	while(b<4)
	{
		DDRD&=~(1<<scl);
		DELAY_TWI();
		if ((PIND&1)==0)
		{
			GO_IN[0]=((GO_IN[0])<<1);
		}
		else
		{
			GO_IN[0]=(GO_IN[0]<<1)+1;
		}
		b++;
		DDRD|=(1<<scl);
		DELAY_TWI();
	}	
	b=0;
	while(b<4)
	{
		DDRD&=~(1<<scl);
		DELAY_TWI();
		if ((PIND&1)==0)
		{
			GO_IN[1]=(GO_IN[1]<<1);
		}
		else
		{
			GO_IN[1]=(GO_IN[1]<<1)+1;
		}
		b++;
		DDRD|=(1<<scl);
		DELAY_TWI();
	}
}

void START(void)
{
	DDRD|=(1<<sda);
	DELAY_TWI();
	DDRD|=(1<<scl);
	DELAY_TWI();
}

void STOP (void)
{
	DDRD&=~(1<<scl);
	DELAY_TWI();
	DDRD&=~(1<<sda);
	DELAY_TWI();
}

void DELAY_TWI (void)
{
	volatile char c;
	c=0;
	while (c<0x2)
	{
		c++;
	}
}

void ACK (void)
{
	DDRD|=(1<<sda);
	DDRD&=~(1<<scl);
	DELAY_TWI();
	DDRD|=1<<scl;
	DELAY_TWI();
}

void NACK (void)
{
	DDRD&=~(1<<scl);
	DDRD&=~(1<<sda);
	DELAY_TWI();
	DDRD|=1<<scl|(1<<sda);
	DELAY_TWI();
}