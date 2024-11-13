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

#pragma once

typedef struct {
    int number_of_steps;
    int direction;
    int last_step_time;
    int step_number;
    int step_delay;
} Stepper;


void Stepper_init(Stepper*, int);
void setSpeed(Stepper*, long);
void step(Stepper*, int);
void stepMotor(Stepper*, int);


/* [] END OF FILE */
