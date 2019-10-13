#include "AnalogToDigital.h"

void ADCInit()
{
	ADCSR = (1 << ADEN) | (1 << ADIF) | (1 << ADIE) | 0b101;
	ADMUX = (1 << REFS0) | (1 << ADLAR);
}
