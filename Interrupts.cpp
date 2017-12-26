#include "Interrupts.h"
#include <Arduino.h>

Buffer<unsigned long, 10> intBuffer;
const int BUTTON_PIN = 2;
const int MILLIS_PERIOD = 250;

void timer1_init() {
  TCCR1A = 0; // set entire TCCR1A register to 0
//  OCR1A = 62500/DESIRED_HZ-1; // = (ARDUINO_HZ = 16*10^6) / ( DESIRED_HZ * 256) - 1
  OCR1A = 62.5*MILLIS_PERIOD-1;
  TCCR1B = _BV(WGM12);  // turn on CTC mode and timer stopped
  TIMSK1 |= _BV(OCIE1A); // enable timer compare interrupt
}

void timer1_start() {
  TCNT1  = 0; //set counter value to 0;
  TCCR1B |= _BV(CS12); // set CS11 bit for 256 prescaler (start the timer)
}

void timer1_stop() {
  TCCR1B &= ~_BV(CS12);
}

void int0_init() {
  EICRA &= ~_BV(ISC00);
  EICRA |= _BV(ISC01); // set INT0 to trigger on falling edge
}

void int0_enable() {
  EIFR |= _BV(INTF0); // disable last triggered event
  EIMSK |= _BV(INT0);
}

void int0_disable() {
  EIMSK &= ~_BV(INT0);
}

void interruptsInit() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  cli(); //stop interrupts
  int0_init();
  int0_enable();
  timer1_init();
  sei(); //allow interrupts
}

ISR(TIMER1_COMPA_vect) {
  timer1_stop();
  int0_enable();
}

ISR(INT0_vect) {
  int0_disable();
  intBuffer.put(millis());
  timer1_start();
}
