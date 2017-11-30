//
// Created by Mert Akozcan on 11/26/17.
//

#include "motor_controller.h"

void MotorController::MoveForward() {
  // In order to move forward, left motor should turn cw,
  // and right motor should turn ccw.
  SetMotorDirection(1,0);
  // Find required time interval for our robot to
  // move one virtual grid. (Current value assigned randomly.)
  StartTimer(0x1312D00); // 20 Million
  // Find an appropriate motor speed (considering time)
  // for our robot to move one virtual grid. (Current value assigned randomly.)
  SetMotorSpeed(300,300);
  // Wait for timer.
  while ((TIMER0_RIS_R & 0x00000001) == 0);
  SetMotorSpeed(50,50);
  EndTimer();
}

void MotorController::TurnLeft() {
  // In order to turn left, both motors should turn ccw.
  SetMotorDirection(0,0);
  // Find required time interval for our robot to
  // turn left.
  StartTimer(0xA037A0); // 10.5 million
  // Find an appropriate motor speed (considering time)
  // for our robot to turn left.
  SetMotorSpeed(300,300);
  // Wait for timer.
  while ((TIMER0_RIS_R & 0x00000001) == 0);
  SetMotorSpeed(50,50);
  EndTimer();
}

void MotorController::TurnRight() {
  // In order to turn right, both motors should turn cw.
  SetMotorDirection(1,1);
  // Find required time interval for our robot to
  // turn right.
  StartTimer(0xA037A0); // 10.5 million
  // Find an appropriate motor speed (considering time)
  // for our robot to turn right.
  SetMotorSpeed(300,300);
  // Wait for timer.
  while ((TIMER0_RIS_R & 0x00000001) == 0);
  SetMotorSpeed(50,50);
  EndTimer();
}

void MotorController::TurnBack() {
  TurnRight();
  TurnRight();
}

void MotorController::Configure() {
  ConfigurePWM();
  ConfigureGPIO();
  ConfigureTimer();
}

void MotorController::ConfigurePWM() {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
  SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
  (*(volatile unsigned long *)(GPIO_PORTF_BASE + GPIO_O_LOCK)) = GPIO_LOCK_KEY;
  (*(volatile unsigned long *)(GPIO_PORTF_BASE + GPIO_O_CR)) |= 0x01;
  GPIOPinConfigure(GPIO_PF2_M1PWM6);
  GPIOPinConfigure(GPIO_PF3_M1PWM7);
  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);
  PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 400);
  SetMotorSpeed(0,0);
}

void MotorController::ConfigureGPIO() {
  // Use GPIO Port D for motor direction.
  SYSCTL_RCGCGPIO_R |= 0x08;
  GPIO_PORTD_DIR_R = 0x0F;
  GPIO_PORTD_DEN_R = 0x0F;
  GPIO_PORTD_DATA_R &= ~0x0F;
}

void MotorController::ConfigureTimer() {
  SYSCTL_RCGCTIMER_R |= 0x01;
  TIMER0_CTL_R &= ~0x00000001;
  TIMER0_CFG_R = 0x00000000;
  TIMER0_TAMR_R = 0x00000002;
  TIMER0_ICR_R |= 0x00000001;
}

void MotorController::SetMotorSpeed(int left, int right) {
  // PF2 controls speed of the left motor,
  // PF3 controls speed of the right motor.
  // Maximum value is 400, but values close to 400 may cause problems.
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, left > 395 ? 395 : left);
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, right > 395 ? 395 : right);
  PWMGenEnable(PWM1_BASE, PWM_GEN_3);
  PWMOutputState(PWM1_BASE, (PWM_OUT_6_BIT | PWM_OUT_7_BIT ), true);
}

void MotorController::SetMotorDirection(int left, int right) {
  // PD0 and PD1 controls the direction of left motor,
  // PD2 and PD3 controls the direction of right motor.
  // PD0 and PD2 are connected to ccw, PD1 and PD3 are
  // connected to cw.
  GPIO_PORTD_DATA_R &= ~0x0F;
  // Left motor direction
  if (left == 0) GPIO_PORTD_DATA_R |= 0x01;
  else if (left == 1) GPIO_PORTD_DATA_R |= 0x02;
  // Right motor direction
  if (right == 0) GPIO_PORTD_DATA_R |= 0x04;
  else if (right == 1) GPIO_PORTD_DATA_R |= 0x08;
}

void MotorController::StartTimer(int load_value) {
  // Load count-down value
  TIMER0_TAILR_R = load_value; // 0x00F42400
  // Enable timer
  TIMER0_CTL_R |= 0x00000001;
}

void MotorController::EndTimer() {
  // Disable timer
  TIMER0_CTL_R &= ~0x00000001;
  // Clear flag
  TIMER0_ICR_R |= 0x00000001;
}