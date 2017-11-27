//
// Created by Mert Akozcan on 11/26/17.
//

#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#define PART_TM4C123GH6PM

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

enum Direction {
  NORTH,
  SOUTH,
  EAST,
  WEST,
  START,
  STOP
};

class MotorController {

public:
  MotorController() {}

  // Configure registers relevant to motors.
  void Configure();

  // Move the robot one step in the given direction.
  void Move(Direction direction);

protected:
  Direction currentDirection = Direction::START;

  // Configure PWM to be able to change motor speed.    
  void ConfigurePWM();

  // Configure GPIO Port E to be able to change motor direction.
  void ConfigureGPIO();

  // Configure timer for time based moving actions.
  void ConfigureTimer();

  // Set motor speed in range (0,400).
  void SetMotorSpeed(int left, int right);

  // Set motor direction to be ccw(0) or cw(1).
  void SetMotorDirection(int left, int right);

  // Set timer value and enable timer.
  void StartTimer(int load_value);

  // Disable timer.
  void EndTimer();

  // Helper methods for moving action.
  void MoveOneStep();
  void TurnLeft();
  void TurnRight();
  void TurnBack();
};

#endif //MOTOR_CONTROLLER_H
