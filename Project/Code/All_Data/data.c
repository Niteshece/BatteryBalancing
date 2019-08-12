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

/*****************************************************************************/
/*                           STATIC FUNCTION AND VARIABLE                   */
/*****************************************************************************/


/*******************************************************************************/
/*                          LOCAL MODULE VARIABLES                             */
/*******************************************************************************/
    
float sf_loadr;
float sf_systemvoltage;
float sf_systemcurrent;
Battery_data battery[3] = {0};

/**************************************************************************
* Function name : void Set_CellVolatages(float *voltage)
* Description   : This API sets the cell voltages..
* Parameter     : float
* Return value  : void
***************************************************************************/
void Set_CellVolatages(float *voltage)
{
    battery[0].voltage = voltage[0];
    battery[1].voltage = voltage[1];
    battery[2].voltage = voltage[2];
}

/**************************************************************************
* Function name : void Get_CellVolatages(float *voltage)
* Description   : This API is to get the cell voltages..
* Parameter     : float*
* Return value  : void
***************************************************************************/ 
void Get_CellVolatages(float *voltage)
{
    voltage[0] = battery[0].voltage;
    voltage[1] = battery[1].voltage;
    voltage[2] = battery[2].voltage;
}

/**************************************************************************
* Function name : void Get_SystemVolatge(float *voltage)
* Description   : This API is to get the cell system voltage.
* Parameter     : float*
* Return value  : void
***************************************************************************/ 
void Get_SystemVolatge(float *voltage)
{
    *voltage = sf_systemvoltage;
}

/**************************************************************************
* Function name : void Get_Systemcurrent(float *current)
* Description   : This API is to get the  system current.
* Parameter     : float*
* Return value  : void
***************************************************************************/ 
void Get_Systemcurrent(float *current)
{
    *current = sf_systemcurrent;
}

/**************************************************************************
* Function name : void Get_Cellcurrent(float *current)
* Description   : This API is to get the cell current.
* Parameter     : float*
* Return value  : void
***************************************************************************/ 
void Get_Cellcurrent(float *current)
{
    current[0] = battery[0].current;
    current[1] = battery[1].current;
    current[2] = battery[2].current;
}

/**************************************************************************
* Function name : Get_SwitchState(tswitch_state *switch_state)
* Description   : This API is to get the switches state.
* Parameter     : tswitch_state*
* Return value  : void
***************************************************************************/ 
void Get_SwitchState(tswitch_state *switch_state)
{
    switch_state[0] = battery[0].switchstate;
    switch_state[1] = battery[1].switchstate;
    switch_state[2] = battery[2].switchstate;
}

/**************************************************************************
* Function name : void Set_LoadValue(float loadohm)
* Description   : This API is to set the value of load Rl.
* Parameter     : float
* Return value  : void
***************************************************************************/
void Set_LoadValue(float loadohm)
{
    sf_loadr = loadohm;
}

/**************************************************************************
* Function name : void Set_InternalResistance(void)
* Description   : This API is to set the value of load Rl.
* Parameter     : float*
* Return value  : void
***************************************************************************/
void Set_InternalResistance(float *internalresistance)
{
    battery[0].resistance = internalresistance[0];
    battery[1].resistance = internalresistance[1];
    battery[2].resistance = internalresistance[2];
}

/**************************************************************************
* Function name : void Set_SwitchState(tswitch_state *switch_state)
* Description   : This API is to get the value of switch status.
* Parameter     : tswitch_state*
* Return value  : void
***************************************************************************/
void Set_SwitchState(tswitch_state *switch_state)
{
    battery[0].switchstate = switch_state[0];
    battery[1].switchstate = switch_state[1];
    battery[2].switchstate = switch_state[2];
}
