void EEPROM_write(unsigned int Address, unsigned char Data)
{
while(EECR & (1<<EEWE));
/* Set up address and data registers */
EEAR = Address;
EEDR = Data;
/* Write logical one to EEMWE */
EECR |= (1<<EEMWE);
/* Start eeprom write by setting EEWE */
EECR |= (1<<EEWE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEWE))
;
/* Set up address register */
EEAR = MAX_LIGHT;
/* Start eeprom read by writing EERE */
EECR |= (1<<EERE);
/* Return data from data register */
LIGHT[0]=EEDR;

while(EECR & (1<<EEWE));
EEAR = MIN_LIGHT;
EECR |= (1<<EERE);
LIGHT[1]=EEDR;
return EEDR;
}