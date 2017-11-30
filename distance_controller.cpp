//
// Created by Mert Akozcan on 11/26/17.
//

#include "distance_controller.h"

bool DistanceController::CheckObstacleForward() {
  // PE3 is connected to forward facing sensor.
  ADC0_PSSI_R |= 0x8;
  // Wait for conversion to be completed
  while ((ADC0_RIS_R & 0x8) == 0);
  // Clear SS3 flag
  ADC0_ISC_R |= (1<<3);
  volatile int result = ADC0_SSFIFO3_R;
  // Check whether there is an obstacle in forward direction.
  return !IsObstacle(result);
}

bool DistanceController::CheckObstacleLeft() {
  // PE2 is connected to left facing sensor.
  ADC0_PSSI_R |= 0x4;
  // Wait for conversion to be completed
  while ((ADC0_RIS_R & 0x4) == 0);
  // Clear SS2 flag
  ADC0_ISC_R |= (1<<2);
  volatile int result = ADC0_SSFIFO2_R;
  // Check whether there is an obstacle in forward direction.
  return !IsObstacle(result);
}

bool DistanceController::CheckObstacleRight() {
  // PE1 is connected to right facing sensor.
  ADC0_PSSI_R |= 0x2;
  // Wait for conversion to be completed
  while ((ADC0_RIS_R & 0x2) == 0);
  // Clear SS1 flag
  ADC0_ISC_R |= (1<<1);
  volatile int result = ADC0_SSFIFO1_R;
  // Check whether there is an obstacle in forward direction.
  return !IsObstacle(result);
}

bool DistanceController::IsObstacle(volatile int result) {
  double volts = (result * 3.3) / 4095; // Input 3.3V
  double distance = 27.86 * (1 / (pow(volts, 1.15)));
  return (distance < 17.0);
}

void DistanceController::Configure() {
  ConfigureADC();
}

void DistanceController::ConfigureADC() {
  // Enable clock for GPIO Port E.
  SYSCTL_RCGCGPIO_R |= 0x10;
  // Enable clock for ADC0
  SYSCTL_RCGCADC_R |= 0x1;
  while ((SYSCTL_RCGCADC_R & 0x0000001) == 0) {};
  
  // Enable alternate function
  GPIO_PORTE_AFSEL_R |= 0xE;
  // Disable digital function
  GPIO_PORTE_DEN_R &= ~0xE;
  // Enable analog function
  GPIO_PORTE_AMSEL_R |= 0xE;
  
  // Disable SS3, SS2 and SS1 before configuration
  ADC0_ACTSS_R &= ~0xE;
  // Software trigger conversion
  ADC0_EMUX_R &= ~0xFFF0;
  // Set input channels
  ADC0_SSMUX3_R |= 0x0;
  ADC0_SSMUX2_R |= 0x1;
  ADC0_SSMUX1_R |= 0x2;
  
  // Take one sample at a time, set flag at first sample
  // Read data from input channels
  ADC0_SSCTL3_R |= 0x6;
  ADC0_SSCTL3_R &= ~(1<<3); 
  ADC0_SSCTL2_R |= 0x6;
  ADC0_SSCTL2_R &= ~(1<<3);
  ADC0_SSCTL1_R |= 0x6;
  ADC0_SSCTL1_R &= ~(1<<3);
  
  // Enable SS3, SS2 and SS1
  ADC0_ACTSS_R |= 0xE;
  
  // Clear flags
  ADC0_ISC_R |= (1<<3);
  ADC0_ISC_R |= (1<<2);
  ADC0_ISC_R |= (1<<1);
}