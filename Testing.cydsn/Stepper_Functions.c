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

#include "project.h"
#include "Stepper_Functions.h"


int degree_to_step(int degree) {
    return (int)((double)(degree / DEGREES_PER_REVOLUTION) * STEPS_PER_REVOLUTION);
}

void forward_step(Motor* motor) {
    
    motor->vals[motor->active_idx] = 1;
    
    if (motor->motor_idx == 1) {
        step_1(motor->vals);
    } else if (motor->motor_idx == 2) {
        step_2(motor->vals);
    } else if (motor->motor_idx == 3) {
        step_3(motor->vals);
    } else if (motor->motor_idx == 4) {
        step_4(motor->vals);
    } else if (motor->motor_idx == 5) {
        step_5(motor->vals);
    } else if (motor->motor_idx == 6) {
        step_6(motor->vals);
    }
    
    motor->vals[motor->active_idx] = 0;
    
    motor->active_idx = (motor->active_idx + 1) % 4;
    
}
void backward_step(Motor* motor) {

    motor->vals[3 - motor->active_idx] = 1;
    
    if (motor->motor_idx == 1) {
        step_1(motor->vals);
    } else if (motor->motor_idx == 2) {
        step_2(motor->vals);
    } else if (motor->motor_idx == 3) {
        step_3(motor->vals);
    } else if (motor->motor_idx == 4) {
        step_4(motor->vals);
    } else if (motor->motor_idx == 5) {
        step_5(motor->vals);
    } else if (motor->motor_idx == 6) {
        step_6(motor->vals);
    }
        
    motor->vals[3 - motor->active_idx] = 0;
           
    motor->active_idx = (motor->active_idx + 1) % 4;

}

void step_1(int vals[]) {
    Motor_1_1_Write(vals[0]);
    Motor_1_2_Write(vals[1]);
    Motor_1_3_Write(vals[2]);
    Motor_1_4_Write(vals[3]);
}
void step_2(int vals[]) {
    Motor_2_1_Write(vals[0]);
    Motor_2_2_Write(vals[1]);
    Motor_2_3_Write(vals[2]);
    Motor_2_4_Write(vals[3]);
}
void step_3(int vals[]) {
    Motor_3_1_Write(vals[0]);
    Motor_3_2_Write(vals[1]);
    Motor_3_3_Write(vals[2]);
    Motor_3_4_Write(vals[3]);
}
void step_4(int vals[]) {
    Motor_4_1_Write(vals[0]);
    Motor_4_2_Write(vals[1]);
    Motor_4_3_Write(vals[2]);
    Motor_4_4_Write(vals[3]);
}
void step_5(int vals[]) {
    Motor_5_1_Write(vals[0]);
    Motor_5_2_Write(vals[1]);
    Motor_5_3_Write(vals[2]);
    Motor_5_4_Write(vals[3]);
}
void step_6(int vals[]) {
    Motor_6_1_Write(vals[0]);
    Motor_6_2_Write(vals[1]);
    Motor_6_3_Write(vals[2]);
    Motor_6_4_Write(vals[3]);
}

/* [] END OF FILE */
