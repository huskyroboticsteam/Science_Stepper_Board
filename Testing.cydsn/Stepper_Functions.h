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


#include "cytypes.h"


#define DEFAULT_SPEED           2
#define FORWARD                 0
#define BACKWARD                1
#define STEPS_PER_REVOLUTION    2048
#define DEGREES_PER_REVOLUTION  360.0 // KEEP .0 for FLOAT


typedef struct Motor {
    int motor_idx;
    int remaining_steps;
    int direction;          // 0 = forward, 1 = backward
    uint8_t speed;
    uint32_t last_tick_time;
    int active_idx;         // 0 = A, ... 3 = D
    int vals[4];
} Motor;


int degree_to_step(int degree);

void forward_step(Motor* motor);
void backward_step(Motor* motor);

void step_1();
void step_2();
void step_3();
void step_4();
void step_5();
void step_6();

/* [] END OF FILE */
