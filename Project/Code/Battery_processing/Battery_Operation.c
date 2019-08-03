/****************************************************************************
 * File Name: p_queue.c
 * Author:Nitesh Garg
 * Version:1.0
 * Date:Aug,02,2019
 * Operating Environment:C
 * Description: 
 * List of functions used:
 * void app_SwitchStateManager(void)
 ****************************************************************************/

 
/*****************************************************************************/
/*                           INCLUDE FILES                               */
/*****************************************************************************/
#include <stdio.h>
#include "BatteryOperation.h"



#define MIN_OPERATING_VOLTAGE 8
#define VOLTAGE_LOST_CONST    0.1


/*****************************************************************************/
/*                           STATIC FUNCTION AND VARIABLE                   */
/*****************************************************************************/


/*******************************************************************************/
/*                          LOCAL MODULE VARIABLES                             */
/*******************************************************************************/
t_switchstates switches_state_current= state_000;
t_switchstates switches_state_next = state_000;
tswitch_state current_state_set[3] = {OFF}; 
int Battery_Low = 0;

void app_SwitchStateManager(void)
{
    switch(switches_state_current)
    {
        case state_000 :

            break;
        case state_001 :
            

            break;
        case state_010 :
            
            
            break;
        case state_011 :
            
            
            break;
        case state_100 :
            
            
            break;
        case state_101 :            
            
            break;
        case state_110 :
            
            
            break;
        case state_111 :           
            
            break;
            
        default        :
            printf("Some Error Occured... Reset the system!");
            break;  
        
    }
    
}