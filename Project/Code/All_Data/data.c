/****************************************************************************
 * File Name: data.c
 * Author:Nitesh Garg
 * Version:1.0
 * Date:Aug,2,2019
 * Operating Environment:C
 * Description: This .c provides API for data get and set
 * List of functions used:
 * void Get_SystemVolatge(float *voltage)
 *void Set_SystemCurrent(float *current)
 *void Get_CellVolatages(float *voltage)
 *void Get_Systemcurrent(float *current)
 *void Set_CellCurrent(float *current)
 *void Get_Cellcurrent(float *current)
 *void Get_SwitchState(tswitch_state *switch_state)
 *void Set_SwitchState(tswitch_state *switch_state)
 *void Set_CellVolatages(float *voltage);
 *void Set_InternalResistance(float *internalresistance)
 *void Set_LoadValue(float loadohm)
 ****************************************************************************/

 
/*****************************************************************************/
/*                           INCLUDE FILES                               */
/*****************************************************************************/
#include "data.h"
#include <stdlib.h>
#include <stdio.h>


/*****************************************************************************/
/*                           STATIC FUNCTION AND VARIABLE                   */
/*****************************************************************************/



float sfa_cellvoltage[3];        
float sfa_cellresistance[3];     
float sf_loadr;
tswitch_state ts_switchvalue[3];
float sf_systemvoltage;
float sf_systemcurrent;
float sfa_cellcurrent[3];

/*******************************************************************************/
/*                          LOCAL MODULE VARIABLES                             */
/*******************************************************************************/

void Set_CellVolatages(float *voltage)
{
    sfa_cellvoltage[0] = voltage[0];
    sfa_cellvoltage[1] = voltage[1];
    sfa_cellvoltage[2] = voltage[2];
}

void Get_CellVolatages(float *voltage)
{
    voltage[0] = sfa_cellvoltage[0];
    voltage[1] = sfa_cellvoltage[1];
    voltage[2] = sfa_cellvoltage[2];
}


void Get_SystemVolatge(float *voltage)
{
    *voltage = sf_systemvoltage;
}


void Get_Systemcurrent(float *current)
{
    *current = sf_systemcurrent;
}

void Get_Cellcurrent(float *current)
{
    current[0] = sfa_cellcurrent[0];
    current[1] = sfa_cellcurrent[1];
    current[2] = sfa_cellcurrent[2];
}


void Get_SwitchState(tswitch_state *switch_state)
{
    switch_state[0] = ts_switchvalue[0];
    switch_state[1] = ts_switchvalue[1];
    switch_state[2] = ts_switchvalue[2];
}


void Set_LoadValue(float loadohm)
{
    sf_loadr = loadohm;
}

void Set_InternalResistance(float *internalresistance)
{
    sfa_cellresistance[0] = internalresistance[0];
    sfa_cellresistance[1] = internalresistance[1];
    sfa_cellresistance[2] = internalresistance[2];
}


void Set_SwitchState(tswitch_state *switch_state)
{
    ts_switchvalue[0] = switch_state[0];
    ts_switchvalue[1] = switch_state[1];
    ts_switchvalue[2] = switch_state[2];
}
