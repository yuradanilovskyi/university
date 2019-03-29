#include <avr/io.h>
#include <stdint.h>                     // needed for uint8_t

#include <avr/interrupt.h>



#define FOSC 16000000                       // Clock Speed
#define BAUD 9600                
#define MYUBRR FOSC/16/BAUD -1


volatile unsigned char ReceivedChar;


void printuart(char *str) {
        for(int i = 0; str[i] != '\0'; i++) {
        while ( !(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
          UDR0 = str[i];
        }

}

int main( void )
{

  
    DDRD |= (1 << DDD6);
    // PD6 is now an output

    OCR0A = 25;
    // set PWM for 50% duty cycle


    TCCR0A |= (1 << COM0A1);
    // set none-inverting mode

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

    TCCR0B |= (1 << CS02) | (1 << CS00); 
    // set prescaler to 1024 and starts 


    /*Set baud rate */
    UBRR0H = (MYUBRR >> 8);
    UBRR0L = MYUBRR;
    
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);      // Enable receiver and transmitter
    UCSR0B |= (1 << RXCIE0);                    // Enable reciever interrupt
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);    // Set frame: 8data, 1 stp

    sei();                                      // Lets not forget to enable interrupts =P

    while(1)
    {
        printuart("Nice to meet you\n");
        _delay_ms(1000);                                    // Main loop
    }    
}

ISR (USART_RX_vect)
{
    ReceivedChar = UDR0;                       // Read data from the RX buffer
    UDR0 = ReceivedChar;   
    OCR0A = ReceivedChar - 48 + 22;// Write the data to the TX buffer
}

