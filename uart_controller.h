//
// Created by Cloe Lee on 11/29/17.
//

#ifndef UART_CONTROLLER_H
#define UART_CONTROLLER_H

#include "tm4c123gh6pm.h"

class UARTController {

public:
  UARTController() {}

  // Configure registers relevant to UART.
  void Configure();

  // Print string to UART console.
  void Write(const char *myString);
  // Read char from UART console.
  char Read();

private:
  void ConfigureUART();
  
  char ReadChar();
  void PrintString(const char *myString);
  void PrintChar(char myChar);
};

#endif //uART_CONTROLLER_H
