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
#include "Stepper.h"

int main (void)
{
    CyGlobalIntEnable;      /* Enable global interrupts */
    /*
    Timer_1_Start();
    
    Stepper motor;
    Stepper_init(&motor, 2038);
    
    for(;;){
        setSpeed(&motor, 5);
        step(&motor, 2038);
        CyDelay(1000);
        
        setSpeed(&motor, 10);
        step(&motor, -2038);
        CyDelay(1000);
    }
    */
    
    for(;;){
        Timer_1_Start();
        CyDelay(1000);
        unsigned long now = Timer_1_ReadCounter();
    }
}
