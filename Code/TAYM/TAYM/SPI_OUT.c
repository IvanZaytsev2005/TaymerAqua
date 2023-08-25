void spi_out(volatile unsigned char a)
{
	a=~a;
	PORTB&=~(1<<CS);
	volatile char b;
	b=0;
	while(b<8)
	{
		
		a=a<<1;
		if((SREG&1)==1)
		{
			PORTB|=1<<mosi;
			PORTB&=~(1<<sck);
		}
		else
		{
			PORTB&=~(1<<mosi);
			PORTB&=~(1<<sck);
		}
		PORTB|=1<<sck;
		b++;
	}
	PORTB|=1<<CS;
}
