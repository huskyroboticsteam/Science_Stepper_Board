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
#include "cytypes.h"

#include "CANPacket.h"
#include "CANScience.h"
#include "Port.h"

#include "Stepper_Functions.h"
#include "FSM.h"


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    CANPacket buffer = {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};
    InitCAN(0x07, 0x04);
    
    PP_Timer_Start();
    
    // IDLE, IP, DONE
    State stepper_driver_state = IDLE; 
    
    // motor_idx, remaining_steps, direction, last_tick_time, active_idx, vals[4]
    Motor motors[6] = {
        {1, 0, 0, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}},
        {2, 0, 0, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}},
        {3, 0, 0, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}},
        {4, 0, 0, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}},
        {5, 0, 0, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}},
        {6, 0, 0, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}}
    };
    int motor_idx;
    int total_steps;
    int degree;
    
    LED_ERR_Write(1);
    
    for(;;)
    {
        /* Place your application code here. */
        
        switch (stepper_driver_state) {
            
            case IDLE: // DECODE?

                if (PollAndReceiveCANPacket(&buffer) == ERROR_NONE) {
                    
                    motor_idx = GetScienceStepperIDFromPacket(&buffer);
                    if (GetPacketID(&buffer) == ID_SCIENCE_STEPPER_TURN_ANGLE) {
                        degree = (int)GetStepperAngleFromPacket(&buffer);
                        total_steps = (double)(degree / 360.0) * 2048;
                    } else if (GetPacketID(&buffer) == ID_SCIENCE_STEPPER_TURN_STEPS) {
                        total_steps = GetStepperStepsFromPacket(&buffer);
                    } else {
                        LED_ERR_Write(0);
                        total_steps = 0;
                    }
                    motors[motor_idx - 1].remaining_steps = (total_steps > 0) ? total_steps : -1 * total_steps;
                    motors[motor_idx - 1].direction = (total_steps > 0) ? 0 : 1;
                    
                }
                
                stepper_driver_state = IP;
                break;
            
            case IP:
                for (int i = 0; i < 6; i++) {
                    if ((motors[i].remaining_steps > 0) && ((motors[i].last_tick_time - delay) > PP_Timer_ReadCounter())) {
                        if (motors[i].direction == 0) {
                            forward_step(&motors[i]);
                        } else if (motors[i].direction == 1) {
                            backward_step(&motors[i]);
                        }
                        motors[i].remaining_steps -= 1;
                        
                        motors[i].last_tick_time = PP_Timer_ReadCounter();
                    }
                }
                
                stepper_driver_state = IDLE;
                break;

            case DONE:
                break;
            
            default:
                stepper_driver_state = IDLE;
                break;
        }
    }
}
/* [] END OF FILE */
