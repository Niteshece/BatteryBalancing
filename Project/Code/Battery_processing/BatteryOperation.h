/****************************************************************************
 * File Name: BatteryOperation.h
 * Author:Nitesh Garg
 * Version:1.0
 * Date:Aug,02,2019
 * Operating Environment:C
 * Description:This .h Files is for providing API functionalities which are 
 * defined by Battery Operation.c for manipulations.
*****************************************************************************/
#ifndef BATTEYOPERATION_H_
#define BATTEYOPERATION_H_

/**************************************************************************/
/*                            INCLUDES                                    */
/**************************************************************************/
#include "../All_Data/data.h"
#include <unistd.h>


typedef enum
{
    state_000 = 0,/*All Switched are OFF*/
    state_001 = 1,/*Switch 1 is ON 2 and 3 are OFF*/
    state_010 = 2,
    state_011 = 3,
    state_100 = 4,
    state_101 = 5,
    state_110 = 6,
    state_111 = 7,/*All Switched are ON*/
    
}t_switchstates;

/*****************************************************************************/
/*                           MACRO DEFINITIONS                               */
/*****************************************************************************/




/*****************************************************************************/
/*                       TYPE DEFINITION DECLARATIONS                         */
/*****************************************************************************/




/*****************************************************************************/
/*                      EXTERNAL FUNCTIONS DECLARATIONS                      */
/*****************************************************************************/
void app_SwitchStateManager(void);
extern float sfa_cellvoltage[3];        
extern float sfa_cellresistance[3];     
extern float sf_loadr;
extern tswitch_state ts_switchvalue[3];
extern float sf_systemvoltage;
extern float sf_systemcurrent;
extern float sfa_cellcurrent[3];

#endif/*BATTEYOPERATION_H_*/
