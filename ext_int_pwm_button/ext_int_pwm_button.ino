#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

volatile bool button = false;

int main(void)
{

    //Config extarnal interruption
    DDRD &= ~(1 << DDD2);     // Clear the PD2 pin
    // PD2 (PCINT0 pin) is now an input

    PORTD |= (1 << PORTD2);    // turn On the Pull-up
    // PD2 is now an input with pull-up enabled



    EICRA |= (1 << ISC01) | (1<<ISC00);    // set INT0 to trigger on FALLING
    EIMSK |= (1 << INT0);     // Turns on INT0


    // Config PWM and timer
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

    TIMSK0 |= (1 << TOIE0);

    sei();                    // turn on interrupts

    while(1)
    {
        /*main program loop here */
    }
}



ISR (INT0_vect)
{
  cli();
  button = true;
  sei();  
}



ISR (TIMER0_OVF_vect)  // timer0 overflow interrupt (16 ms = (16000000/1024)/256))
{
  cli();

    
    if (button == true || (~PIND & (1 << PD2))) {
        button = false;
        OCR0A-=1;
        
      }

   sei();
}

