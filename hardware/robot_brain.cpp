//
// Created by Mert Akozcan on 11/27/17.
//

#include <iostream>
#include "robot_brain.h"

void RobotBrain::Work() {
  bool autoMode = true;
  // Configure controllers
  motorController->Configure();
  distanceController->Configure();
  uartController->Configure();
  uartController->Write("Hello, world. This is Group 12!\r\n");
  uartController->Write("Please press k for automatic or l for manual mode.\r\n");
  // Get driving mode
  char mode = uartController->Read();
  if (mode == 'l') {
    autoMode = false;
    uartController->Write("Manual controls: a -> left, s -> back, d -> right\r\n");
  }
  // Start driving
  while (1) {
    bool forward = distanceController->CheckObstacleForward();
    if (forward) {
      // Whether it's in automatic or manual mode, move forward if there isn't
      // any obstacle.
      motorController->MoveForward();
      uartController->Write("Forward!\r\n");
    } else {
      bool left = distanceController->CheckObstacleLeft();
      bool right = distanceController->CheckObstacleRight();
      if (autoMode) {
        if (right) {
          motorController->TurnRight();
          uartController->Write("Right!\r\n");
        } else if (left) {
          motorController->TurnLeft();
          uartController->Write("Left!\r\n");
        } else {
          motorController->TurnBack();
          uartController->Write("Back!\r\n");
        }
        motorController->MoveForward();
      } else {
        // Send available directions to user
        uartController->Write("Available directions:");
        if (left) uartController->Write(" left(a)");
        if (right) uartController->Write(" right(d)");
        uartController->Write(" back(s). Please select one.\r\n");
        // Get direction
        char direction = uartController->Read();
        if (direction == 'd' && right) {
          motorController->TurnRight();
          motorController->MoveForward();
        } else if (direction == 'a' && left) {
          motorController->TurnLeft();
          motorController->MoveForward();
        } else if (direction == 's') {
          motorController->TurnBack();
          motorController->MoveForward();
        } else {
          uartController->Write("Please select a valid direction.\r\n");
        }
      }
    }
  }
}