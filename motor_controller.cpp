//
// Created by Mert Akozcan on 11/26/17.
//

#include "motor_controller.h"

void MotorController::Move(Direction direction) {
  if (currentDirection == Direction::START) currentDirection = direction;
  if (direction == currentDirection) {
    MoveOneStep();
  } else {
    if (currentDirection == Direction::NORTH) {
      // ******
      if (direction == Direction::EAST) TurnRight();
      else if (direction == Direction::WEST) TurnLeft();
      else if (direction == Direction::SOUTH) TurnBack();
      currentDirection = direction;
      MoveOneStep();
      // ******
    } else if (currentDirection == Direction::SOUTH) {
      // ******
      if (direction == Direction::EAST) TurnLeft();
      else if (direction == Direction::WEST) TurnRight();
      else if (direction == Direction::NORTH) TurnBack();
      currentDirection = direction;
      MoveOneStep();
      // ******
    } else if (currentDirection == Direction::EAST) {
      // ******
      if (direction == Direction::NORTH) TurnLeft();
      else if (direction == Direction::WEST) TurnBack();
      else if (direction == Direction::SOUTH) TurnRight();
      currentDirection = direction;
      MoveOneStep();
      // ******
    } else if (currentDirection == Direction::WEST) {
      // ******
      if (direction == Direction::NORTH) TurnRight();
      else if (direction == Direction::EAST) TurnBack();
      else if (direction == Direction::SOUTH) TurnLeft();
      currentDirection = direction;
      MoveOneStep();
      // ******
    }
  }
}

void MotorController::MoveOneStep() {
  // In order to move forward, left motor should turn cw,
  // and right motor should turn ccw.
  SetMotorDirection(1,0);
  // TODO: Find required time interval for our robot to
  // move one virtual grid. (Current value assigned randomly.)
  StartTimer(0x00F42400);
  // TODO: Find an appropriate motor speed (considering time)
  // for our robot to move one virtual grid. (Current value assigned randomly.)
  SetMotorSpeed(300,300);
  // Wait for timer.
  while ((TIMER0_RIS_R & 0x00000001) == 0);
  SetMotorSpeed(0,0);
  EndTimer();
}

void MotorController::TurnLeft() {
  // In order to turn left, both motors should turn ccw.
  SetMotorDirection(0,0);
  // TODO: Find required time interval for our robot to
  // turn left. (Current value assigned randomly.)
  StartTimer(0x00F42400);
  // TODO: Find an appropriate motor speed (considering time)
  // for our robot to turn left. (Current value assigned randomly.)
  SetMotorSpeed(300,300);
  // Wait for timer.
  while ((TIMER0_RIS_R & 0x00000001) == 0);
  SetMotorSpeed(0,0);
  EndTimer();
}

void MotorController::TurnRight() {
  // In order to turn right, both motors should turn cw.
  SetMotorDirection(1,1);
  // TODO: Find required time interval for our robot to
  // turn right. (Current value assigned randomly.)
  StartTimer(0x00F42400);
  // TODO: Find an appropriate motor speed (considering time)
  // for our robot to turn right. (Current value assigned randomly.)
  SetMotorSpeed(300,300);
  // Wait for timer.
  while ((TIMER0_RIS_R & 0x00000001) == 0);
  SetMotorSpeed(0,0);
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
  // TODO: Use another GPIO Port (not E!)
  SYSCTL_RCGCGPIO_R = 0x10;
  GPIO_PORTE_DIR_R = 0x1E;
  GPIO_PORTE_DEN_R = 0x1E;
  GPIO_PORTE_DATA_R &= ~0x1E;
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
  // PE1 and PE2 controls the direction of left motor,
  // PE3 and PE4 controls the direction of right motor.
  // PE1 and PE3 are connected to ccw, PE2 and PE4 are
  // connected to cw.
  GPIO_PORTE_DATA_R &= ~0x1E;
  // Left motor direction
  if (left == 0) GPIO_PORTE_DATA_R |= 0x02;
  else if (left == 1) GPIO_PORTE_DATA_R |= 0x04;
  // Right motor direction
  if (right == 0) GPIO_PORTE_DATA_R |= 0x08;
  else if (right == 1) GPIO_PORTE_DATA_R |= 0x10;
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