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


#include <project.h>
#pragma once

typedef struct {
    int number_of_steps;
    int direction;
    unsigned long last_step_time;
    int step_number;
    unsigned long step_delay;
} Stepper;


void Stepper_init(Stepper*, int);;
void setSpeed(Stepper*, long);
void step(Stepper*, int);
void stepMotor(int);

CY_ISR(isr_1_Handler);


/* [] END OF FILE */
