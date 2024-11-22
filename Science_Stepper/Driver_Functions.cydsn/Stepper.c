/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "Stepper.h"
#include <stdlib.h>
#include <project.h>

volatile unsigned long curr_time = 0;



void Stepper_init(Stepper* motor, int number_of_steps){
  motor->step_number = 0;    // which step the motor is on
  motor->direction = 0;      // motor direction
  motor->last_step_time = 0; // timestamp in us of the last step taken
  motor->number_of_steps = number_of_steps; // total number of steps for this motor
  //Timer_1_Start();
  //isr_1_StartEx(isr_1__Handler);
}

/*
 * Sets the speed in revs per minute
 */
void setSpeed(Stepper *motor, long whatSpeed){
  motor->step_delay = 60L * 1000L * 1000L / motor->number_of_steps / whatSpeed;
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void step(Stepper* motor, int steps_to_move){
  int steps_left = abs(steps_to_move);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { motor->direction = 1; }
  if (steps_to_move < 0) { motor->direction = 0; }


  // decrement the number of steps, moving one step each time:
  while (steps_left > 0){ 
    
    unsigned long now = Timer_1_ReadCounter();
    // move only if the appropriate delay has passed:
    if (now - motor->last_step_time >= motor->step_delay){
      // get the timeStamp of when you stepped:
      motor->last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (motor->direction == 1){
        motor->step_number++;
        if (motor->step_number == motor->number_of_steps) {
          motor->step_number = 0;
        }
      }else{
        if (motor->step_number == 0) {
          motor->step_number = motor->number_of_steps;
        }
        motor->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, ..., {3 or 10}
 
        stepMotor(motor->step_number % 4);
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
void stepMotor(int thisStep){
    switch (thisStep) {
      case 0:  // 1010
        IN1_Write(1);
        IN2_Write(0);
        IN3_Write(1);
        IN4_Write(0);
        break;
      case 1:  // 0110
        IN1_Write(0);
        IN2_Write(1);
        IN3_Write(1);
        IN4_Write(0);
        break;
      case 2:  //0101
        IN1_Write(0);
        IN2_Write(1);
        IN3_Write(0);
        IN4_Write(1);
        break;
      case 3:  //1001
        IN1_Write(1);
        IN2_Write(0);
        IN3_Write(0);
        IN4_Write(1);
        break;
    }
}


/* [] END OF FILE */
