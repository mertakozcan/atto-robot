//
// Created by Cloe Lee on 11/29/17.
//

#include "uart_controller.h"

void UARTController::Write(const char *myString) {
  PrintString(myString);
}

char UARTController::Read() {
  return ReadChar();
}

void UARTController::Configure() {
  ConfigureUART();
}

void UARTController::PrintString(const char *myString) {
  while (*myString) {
    PrintChar(*(myString++));
  }
}

void UARTController::PrintChar(char myChar) {
  while((UART1_FR_R & (1<<5)) != 0);
  UART1_DR_R = myChar;
}

char UARTController::ReadChar() {
  char c;
  while((UART1_FR_R & 0x0010) != 0); // Wait until RXFE is 0
  c = UART1_DR_R;
  return c; 
}

void UARTController::ConfigureUART() {
  SYSCTL_RCGCUART_R |= (1<<1); // Enable the UART Module 1
  SYSCTL_RCGCGPIO_R |= (1<<1); // Enable GPIO Port B
  
  GPIO_PORTB_AFSEL_R  = (1<<1)|(1<<0); // Enable alternative function  
  GPIO_PORTB_PCTL_R = (1<<0)|(1<<4); // UART1 Column 1, PB0 & PB1
  
  GPIO_PORTB_DEN_R = (1<<0)|(1<<1); // Enable Digital

  UART1_CTL_R &= ~(1<<0); // Disable UART by clearing UARTEN bit

  // Find the Baud rate 
  // BRD = clk / (clkdiv * baud rate) = 16,000,000/(16 * 115,200) = 8.6805555
  // UARTFBRD[DIVFRAC] = integer (BRDF * 64 + 0.5) = integer (.6805555 * 64 + 0.5) = 44
  UART1_IBRD_R = 8; // integer of BRD 
  UART1_FBRD_R = 44; // fractional of BRD
  
  UART1_LCRH_R = (0x3<<5)|(1<<4); //8 bit, no parity, 1-stop bit
  UART1_CC_R = 0x0; // Configure UART clock - system clk
  // Enable UART, bit8: transmit, bit9: receive
  UART1_CTL_R = (1<<0) | (1<<8) | (1<<9); 
}
