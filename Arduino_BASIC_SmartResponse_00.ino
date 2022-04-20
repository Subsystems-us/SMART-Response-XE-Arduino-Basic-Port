#include "basic.h"
#include "host.h"
#include "SPI.h"
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include "SmartResponseXEa.h"
#include <Arduino.h>

// Define in host.h if using an external EEPROM e.g. 24LC256
// Should be connected to the I2C pins
// SDA -> Analog Pin 4, SCL -> Analog Pin 5
// See e.g. http://www.hobbytronics.co.uk/arduino-external-eeprom

// BASIC
unsigned char mem[MEMORY_SIZE];
#define TOKEN_BUF_SIZE    77
unsigned char tokenBuf[TOKEN_BUF_SIZE];

const char welcomeStr[] PROGMEM = "Arduino BASIC";
char autorun = 0;

void setup() {
  pinMode(INT2, INPUT_PULLUP); // Power Button

  TRXPR = 1 << SLPTR; // send transceiver to sleep
  initADC();
  SRXEInit(0xe7, 0xd6, 0xa2); // initialize and clear display CS, D/C, RESET
  SRXEFill(0);

  reset();
  host_init();
  host_cls();
  host_outputProgMemString(welcomeStr);
  // show memory size
  host_outputFreeMem(sysVARSTART - sysPROGEND);
  host_showBuffer();
}

void loop() {
  int ret = ERROR_NONE;

  if (!autorun) {
    // get a line from the user
    char *input = host_readLine();
    // special editor commands
    if (input[0] == '?' && input[1] == 0) {
      host_outputFreeMem(sysVARSTART - sysPROGEND);
      host_showBuffer();
      return;
    }
    // otherwise tokenize
    ret = tokenize((unsigned char*)input, tokenBuf, TOKEN_BUF_SIZE);
  }
  else {
    // host_loadProgram();
    // tokenBuf[0] = TOKEN_RUN;
    // tokenBuf[1] = 0;
    // autorun = 0;
  }
  // execute the token buffer
  if (ret == ERROR_NONE) {
    host_newLine();
    ret = processInput(tokenBuf);
  }
  if (ret != ERROR_NONE) {
    host_newLine();
    if (lineNumber != 0) {
      host_outputInt(lineNumber);
      host_outputChar('-');
    }
    host_outputProgMemString((char *)pgm_read_word(&(errorTable[ret])));
  }
}

// Setup the ADC
void initADC() {
  ADMUX = 0xC0; // Int ref 1.6V
  ADCSRA = 0x87; // Enable ADC
  ADCSRB = 0x00; // MUX5= 0, freerun
  ADCSRC = 0x54; // Default value
  ADCSRA = 0x97; // Enable ADC
  //delay(5);
  ADCSRA |= (1 << ADSC); // start conversion
}
