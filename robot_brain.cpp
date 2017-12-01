//
// Created by Mert Aközcan on 11/27/17.
//

#include <random>
#include "robot_brain.h"

void RobotBrain::Work() {
  motorController->Configure();
  distanceController->Configure();
  uartController->Configure();
  /*
  // Random configuration
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);
  */
  uartController->Write("Hello, world. This is Group 12!\n");
  while (1) {
    bool forward = distanceController->CheckObstacleForward();
    bool left = distanceController->CheckObstacleLeft();
    bool right = distanceController->CheckObstacleRight();
    //bool forward = true;
    //bool left = false;
    //bool right = false;
    if (forward) {
      motorController->MoveForward();
    } else if (left && right) {
      // Randomly select one, now it selects right.
      /*
      if (dis(gen) <= 0.5) {
        motorController->TurnLeft();
        motorController->MoveForward();
      } else {
        motorController->TurnRight();
        motorController->MoveForward();
      }
      */
      motorController->TurnRight();
      motorController->MoveForward();
    } else if (left) {
      motorController->TurnLeft();
      motorController->MoveForward();
    } else if (right) {
      motorController->TurnRight();
      motorController->MoveForward();
    } else {
      motorController->TurnBack();
      motorController->MoveForward();
    }
  }
}