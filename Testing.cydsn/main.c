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
#include <stdio.h>
#include <stdint.h>

#include "HindsightCAN/CANPacket.h"
#include "HindsightCAN/CANScience.h"
#include "HindsightCAN/Port.h"

#include "Stepper_Functions.h"
#include "FSM.h"

#define Print(message) DBG_UART_UartPutString(message)

#define DEVICE_SERIAL_SCIENCE_STEPPER 0x03 // TODO: REMOVE WHEN ADDED CANSerialNumbers.h

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    // INIT CAN
    CANPacket can_receive;
    CANPacket can_send;
    uint8 address = 0;
    uint16_t id = 0;
    
    // address = Status_Reg_DIP_Read();
    // address = DEVICE_SERIAL_SCIENCE_STEPPER;
    InitCAN(DEVICE_GROUP_SCIENCE, DEVICE_SERIAL_SCIENCE_STEPPER);
    
    // INIT UART
    char txData[200];
    DBG_UART_Start();
    sprintf(txData, "DG: %x, Dip Addr: %x \r\n", DEVICE_GROUP_SCIENCE, address);
    Print(txData);
    
    // INIT TIMER
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
    uint8_t motor_idx;
    uint8_t speed;
    int16_t total_steps;
    
    // reset LEDS
    LED_ERR_Write(1);
    
    for(;;)
    {
        /* Place your application code here. */
        
        switch (stepper_driver_state) {
            
            case IDLE:
                
                // DECODE CAN
                if (PollAndReceiveCANPacket(&can_receive) == ERROR_NONE) {
                    sprintf(txData, "Received Packet: %x | %x | %x | %x | %x | %x | ", can_receive.data[0],
                            can_receive.data[1], can_receive.data[2], can_receive.data[3], can_receive.data[4], can_receive.data[5]);
                    Print(txData);
                    
                    LED_ERR_Write(1);
                    
                    // DECODE Motor Idx
                    motor_idx = GetScienceStepperIDFromPacket(&can_receive);
                    if (motor_idx < 1 || motor_idx > 6) {
                        LED_ERR_Write(0);
                        sprintf(txData, "Motor Index Not Valid: %d, Aborting", motor_idx);
                        Print(txData);
                        // Send CAN ERROR PACKET ?
                        stepper_driver_state = IP;
                        break;
                    } else {
                        sprintf(txData, "Motor Index: %d ", motor_idx); 
                        Print(txData);
                    }
                    
                    // DECODE speed
                    speed = GetStepperSpeedFromPacket(&can_receive);
                    if (speed < 0) {
                        LED_ERR_Write(0);
                        sprintf(txData, "Motor Speed: %d can't be less then 0, using DEFAULT_SPEED: %d ", speed, DEFAULT_SPEED); 
                        Print(txData);
                        
                    } else {
                        sprintf(txData, "Motor Speed: %d ", speed); 
                        Print(txData);
                    }
                    
                    // DECODE total steps
                    if (GetPacketID(&can_receive) == ID_SCIENCE_STEPPER_TURN_ANGLE) {
                        int16_t angle = GetStepperAngleFromPacket(&can_receive);
                        total_steps = degree_to_step((int)angle);
                        sprintf(txData, "Turn Angle: %d \r \n", GetStepperAngleFromPacket(&can_receive));
                        Print(txData);
                    } else if (GetPacketID(&can_receive) == ID_SCIENCE_STEPPER_TURN_STEPS) {
                        total_steps = GetStepperStepsFromPacket(&can_receive);
                        sprintf(txData, "Turn Steps: %d \r \n", GetStepperStepsFromPacket(&can_receive)); 
                        Print(txData);
               
                    } else {
                        LED_ERR_Write(0);
                        total_steps = 0;
                        sprintf(txData, "Wrong Packet ID: %x \n", GetPacketID(&can_receive)); 
                        Print(txData);
                    }
                    
                    motors[motor_idx - 1].last_tick_time = PP_Timer_ReadCounter();
                    motors[motor_idx - 1].active_idx = 0;
                    motors[motor_idx - 1].vals[0] = 0;
                    motors[motor_idx - 1].vals[1] = 0;
                    motors[motor_idx - 1].vals[2] = 0;
                    motors[motor_idx - 1].vals[3] = 0;
                    
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
                    
                        if (motors[i].direction == FORWARD) {
                            forward_step(&motors[i]);
                        } else if (motors[i].direction == BACKWARD) {
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
