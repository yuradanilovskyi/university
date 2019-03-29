// this program enables SPI communication and
// Sets the AVR into Master mode


#include <avr/io.h>
volatile unsigned char data = 253;

int main (void)
{
     DDRD &= ~(1<<PD2);
     PORTD |= (1<<PD2);  
     DDRD |= (1<<PD6);

    DDRB |= (1<<2)|(1<<3)|(1<<5);    // SCK, MOSI and SS as outputs
    DDRB &= ~(1<<4);                 // MISO as input

    SPCR |= (1<<MSTR);               // Set as Master
    SPCR |= (1<<SPR0)|(1<<SPR1);     // divided clock by 128
    SPCR |= (1<<SPE);                // Enable SPI

    ADMUX = 0;                // use ADC0
    ADMUX |= (1 << REFS0);    // use AVcc as the reference
    ADMUX |= (1 << ADLAR);    // Right adjust for 8 bit resolution

    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale for 16Mhz
    ADCSRA |= (1 << ADATE);   // Set ADC Auto Trigger Enable
    
    ADCSRB = 0;               // 0 for free running mode

    ADCSRA |= (1 << ADEN);    // Enable the ADC
    ADCSRA |= (1 << ADIE);    // Enable Interrupts 

    ADCSRA |= (1 << ADSC);    // Start the ADC conversion

    sei();    // Thanks N, forgot this the first time =P
    
    while(1)
    { 
 
         PORTB &= ~(1<<PB2); 
        SPDR = data;                 // send the data
        while(!(SPSR & (1<<SPIF)));  // wait until transmission is complete
        PORTB |= (1<<PB2);

        _delay_ms(10);

        // if you have multiple slaves, this is where you want to switch
    }
}


ISR(ADC_vect)
{
    data = ADCH;         // only need to read the high value for 8 bit
    // REMEMBER: once ADCH is read the ADC will update
    //if (data > 125) PORTD |= (1<<PD6);

}
