//
// Created by Mert Aközcan on 11/27/17.
//

#include "robot_brain.h"

void RobotBrain::Work() {
    motorController = new MotorController();
    motorController->Move(Direction::NORTH);
    distanceController = new DistanceController();
    distanceController->CheckObstacleForward();
    distanceController->CheckObstacleLeft();
    distanceController->CheckObstacleRight();
}