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
#ifndef STEPPER_FUNCTIONS
#define STEPPER_FUNCTIONS

#include "cytypes.h"
    
#define delay 2
    
typedef struct Motor {
    int motor_idx;
    int remaining_steps;
    int direction;          // 0 = forward, 1 = backward
    uint32_t last_tick_time;
    int active_idx;         // 0 = A, ... 3 = D
    int vals[4];
} Motor;


void forward_step(Motor* motor);
void backward_step(Motor* motor);


void step_1();
void step_2();
void step_3();
void step_4();
void step_5();
void step_6();

#endif
/* [] END OF FILE */
