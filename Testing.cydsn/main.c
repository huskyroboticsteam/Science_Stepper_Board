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


#define DEVICE_SERIAL_SCIENCE_STEPPER 0x03 // TODO: REMOVE WHEN ADDED CANSerialNumbers.h


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    // Init CAN
    CANPacket buffer = {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};
    InitCAN(DEVICE_GROUP_SCIENCE, DEVICE_SERIAL_SCIENCE_STEPPER);
    
    // Start Timer
    PP_Timer_Start();
    
    // IDLE, IP, DONE
    State stepper_driver_state = IDLE; 
    
    // motor_idx, remaining_steps, direction, speed, last_tick_time, active_idx, vals[4]
    Motor motors[6] = {
        {1, 0, FORWARD, DEFAULT_SPEED, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}},
        {2, 0, FORWARD, DEFAULT_SPEED, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}},
        {3, 0, FORWARD, DEFAULT_SPEED, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}},
        {4, 0, FORWARD, DEFAULT_SPEED, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}},
        {5, 0, FORWARD, DEFAULT_SPEED, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}},
        {6, 0, FORWARD, DEFAULT_SPEED, PP_Timer_ReadCounter(), 0, {0, 0, 0, 0}}
    };

    // CAN intermediarys
    int motor_idx;
    int speed;
    int total_steps;
    
    // reset LEDS
    LED_ERR_Write(1);
    
    for(;;)
    {
        /* Place your application code here. */
        
        switch (stepper_driver_state) {
            
            case IDLE:
                
                // DECODE CAN
                if (PollAndReceiveCANPacket(&buffer) == ERROR_NONE) {
                    
                    // DECODE Motor Idx
                    motor_idx = GetScienceStepperIDFromPacket(&buffer);

                    // DECODE speed
                    speed = GetStepperSpeedFromPacket(&buffer);
                    
                    // DECODE total steps
                    if (GetPacketID(&buffer) == ID_SCIENCE_STEPPER_TURN_ANGLE) {
                        total_steps = degree_to_step((int)GetStepperAngleFromPacket(&buffer));
                    } else if (GetPacketID(&buffer) == ID_SCIENCE_STEPPER_TURN_STEPS) {
                        total_steps = GetStepperStepsFromPacket(&buffer);
                    } else {
                        LED_ERR_Write(0);
                        total_steps = 0;
                    }

                    motors[motor_idx - 1].remaining_steps = (total_steps > 0) ? total_steps : -1 * total_steps;
                    motors[motor_idx - 1].direction = (total_steps > 0) ? FORWARD : BACKWARD;
                    motors[motor_idx - 1].speed = (speed > 0) ? speed : DEFAULT_SPEED;
                    
                }
                
                stepper_driver_state = IP;
                break;
            
            case IP:
                
                // Step Active Motors
                for (int i = 0; i < 6; i++) {
                    
                    if ((motors[i].remaining_steps > 0) && ((motors[i].last_tick_time - motors[i].speed) > PP_Timer_ReadCounter())) {
                    
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
