void DELAY(void)
{
	volatile unsigned int a;
	a=0;
	while(a<0xff)
	{
		a++;
	}
}