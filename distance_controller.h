//
// Created by Mert Akozcan on 11/26/17.
//

#ifndef DISTANCE_CONTROLLER_H
#define DISTANCE_CONTROLLER_H

#include "tm4c123gh6pm.h"
#include <cmath>

class DistanceController {

public:
  DistanceController() {}
  
  // Configure registers relevant to distance sensors.
  void Configure();

  // Check if there is an obstacle.
  bool CheckObstacleForward();
  bool CheckObstacleLeft();
  bool CheckObstacleRight();

private:
  // Configure ADC to read data from distance sensors.
  void ConfigureADC();
  
  // Calculate distance to obstacle
  bool IsObstacle(int result);
};

#endif //DISTANCE_CONTROLLER_H