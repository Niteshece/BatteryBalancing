/****************************************************************************
 * File Name: data.h
 * Author:Nitesh Garg
 * Version:1.0
 * Date:Aug,02,2019
 * Operating Environment:C
 * Description:This .h file is for data.c
*****************************************************************************/
#ifndef DATA_H_
#define DATA_H_

/**************************************************************************/
/*                            INCLUDES                                    */
/**************************************************************************/
#include <stdlib.h>
#include <stdio.h>

/*****************************************************************************/
/*                           MACRO DEFINITIONS                               */
/*****************************************************************************/


/*****************************************************************************/
/*                       TYPE DEFINITION DECLARATIONS                         */
/*****************************************************************************/

/*Enum Type created for two state of a switch*/
typedef enum
{
    OFF  =0, /*OFF state*/
    ON   =1,/* ON State*/
}tswitch_state;


typedef struct
{
    float resistance;
    float current;
    float voltage;
    tswitch_state switchstate;    
}Battery_data;             /*Structure to represent the data of a particular Battery*/


/*****************************************************************************/
/*                      EXTERNAL FUNCTIONS DECLARATIONS                      */
/*****************************************************************************/

void Get_SystemVolatge(float *voltage);
void Set_SystemCurrent(float *current);
void Get_CellVolatages(float *voltage);
void Get_Systemcurrent(float *current);
void Set_CellCurrent(float *current);
void Get_Cellcurrent(float *current);
void Get_SwitchState(tswitch_state *switch_state);
void Set_SwitchState(tswitch_state *switch_state);
void Set_CellVolatages(float *voltage);
void Set_InternalResistance(float *internalresistance);
void Set_LoadValue(float loadohm);


#endif/*DATA_H_*/
