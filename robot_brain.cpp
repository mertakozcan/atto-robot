//
// Created by Mert Aközcan on 11/27/17.
//

#include <random>
#include <iostream>
#include "robot_brain.h"

void RobotBrain::Work() {
  motorController->Configure();
  //distanceController->Configure();
  /*
  // Random configuration
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);
  */
  while (1) {
    //bool forward = distanceController->CheckObstacleForward();
    //bool left = distanceController->CheckObstacleLeft();
    //bool right = distanceController->CheckObstacleRight();
    bool forward = true;
    bool left = false;
    bool right = false;
    if (forward) {
      motorController->MoveForward();
    } else if (left && right) {
      // Randomly select one
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

/*
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
*/