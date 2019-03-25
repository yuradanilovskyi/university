// this program enables SPI communication and
// Sets the AVR into Master mode


#include <avr/io.h>


int main (void)
{
    unsigned char data = 253;
     DDRD &= ~(1<<PD2);
     PORTD |= (1<<PD2);  

    DDRB |= (1<<2)|(1<<3)|(1<<5);    // SCK, MOSI and SS as outputs
    DDRB &= ~(1<<4);                 // MISO as input

    SPCR |= (1<<MSTR);               // Set as Master
    SPCR |= (1<<SPR0)|(1<<SPR1);     // divided clock by 128
    SPCR |= (1<<SPE);                // Enable SPI
    
    while(1)
    { 
        if (~PIND & (1<<PD2)) data = 65;
        else data = 253; 
         PORTB &= ~(1<<PB2); 
        SPDR = data;                 // send the data
        while(!(SPSR & (1<<SPIF)));  // wait until transmission is complete
        PORTB |= (1<<PB2);

        _delay_ms(10);

        // if you have multiple slaves, this is where you want to switch
    }
}
