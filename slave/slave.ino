#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char data;

int main (void)
{
    DDRD |= (1<<6);
    DDRB &= ~((1<<2)|(1<<3)|(1<<5));   // SCK, MOSI and SS as inputs
    DDRB |= (1<<4);                    // MISO as output

    SPCR &= ~(1<<MSTR);                // Set as slave 
    SPCR |= (1<<SPR0)|(1<<SPR1);       // divide clock by 128
    SPCR |= (1<<SPIE);                 // Enable SPI Interrupt
    SPCR |= (1<<SPE);                  // Enable SPI

    DDRD |= (1 << DDD6);
    // PD6 is now an output

    OCR0A = 0;
    // set PWM for 50% duty cycle


    TCCR0A |= (1 << COM0A1);
    // set none-inverting mode

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

    TCCR0B |= (1 << CS02) | (1 << CS00); 
    // set prescaler to 1024 and starts PWM
    sei();
  
    while(1)
    {
        ;
    }
}

ISR (SPI_STC_vect)
{
    data = SPDR;
    OCR0A = data;
}
