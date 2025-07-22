/*
 * GccApplication2.c
 *
 * Created: 21.07.2025 18:38:43
 * Author : memor
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

uint8_t count = 0;

void foo()
{
	//while (1)
	//{
	PORTB |= (1 << PORTB5); // Toggle pin D13 (PB5)
	_delay_ms(3000);

	count = 0; // Reset count
	//}
}

uint16_t fooFunctionPointer;
uint8_t * myStackPointer;
void fooboi()
{
  
	fooFunctionPointer = (uint16_t)&foo; // überschreiben von ret addresse
	myStackPointer = (uint8_t*)SP; // lel

// 3) -> 32bit = 4 byte (1 byte = 8 bit) -> 3 2 1 0
	*(myStackPointer) = (uint8_t)(fooFunctionPointer); // cancel out first 3 bytes msb -> 0
	myStackPointer--;

	*(myStackPointer) = (uint8_t)((fooFunctionPointer >> 8)); // same but shift right to extract -> 1
	myStackPointer--;

	//*(myStackPointer) = (uint8_t)((fooFunctionPointer >> 16 )); // now get last -> 2
	//myStackPointer--;

	SP = (uint8_t*)myStackPointer; // feels illegal  
}

void setup()
{
	// put your setup code here, to run once:
	// int result = myFunction(2, 3);
	DDRB |= (1 << PORTB5);   // Set pin D13 (PB5) as output
	PORTB &= ~(1 << PORTB5); // Clear pin D13 (PB5)
}

int main(void)
{
  setup();

  while (1)
  {
	  PORTB ^= (1 << PORTB5); // Toggle pin D13 (PB5)
	  _delay_ms(250);

	  count++;

	  if (count >= 5)
	  {
		  fooboi(); // Manipulate stack pointer to call foo
		  //foo();
	  }
  }

  return 0;
}

