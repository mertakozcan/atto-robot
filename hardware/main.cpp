#include "robot_brain.h"

int main()
{
  RobotBrain *robotBrain = new RobotBrain();
  robotBrain->Work();
  delete robotBrain;
  return 0;
}
