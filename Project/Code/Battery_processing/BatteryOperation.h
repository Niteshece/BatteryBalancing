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
#include <stdio.h>

/*****************************************************************************/
/*                           MACRO DEFINITIONS                               */
/*****************************************************************************/
#define CUT_OFF_VOLTAGE         8           /*Min operating volatge for a cell*/
#define VOLTAGE_UPDATE_TIME     1000000     /*in micro-secs , 1sec*/
#define CELL_CAPACITY           360    /*in Ampsechere- .001Amphr - */
#define FAST_DISCHARGE_VOLTAGE  11
#define ROUNDOFFENABLE          1
#define CUTOFF_VOLTAGE          8

#define TRUE                    1
#define FALSE                   0


/*****************************************************************************/
/*                       TYPE DEFINITION DECLARATIONS                        */
/*****************************************************************************/
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
    
    
}t_switchstates; /*Enum to define all possible combination of cell switches*/


typedef struct
{
    float coordinate_x;
    float coordinate_y;
}pointcoordinate;       /*Structure for a particular Point on Graph*/


typedef struct
{
    pointcoordinate point_A;
    pointcoordinate point_B;
    pointcoordinate point_C;
    float slope_m1;
    float slope_m2;
}cell_coordinates;      /*Structure to represent the graphical paramenters of a particular cell*/




/*****************************************************************************/
/*                      EXTERNAL FUNCTIONS DECLARATIONS                      */
/*****************************************************************************/

void app_SwitchStateManager(void);
void app_CalculateSlope(void);

/*cell paramenters*/
extern Battery_data battery[3];
 


/*Load Value*/
extern float sf_loadr;

/*System paramenters*/
extern float sf_systemvoltage;
extern float sf_systemcurrent;


#endif/*BATTEYOPERATION_H_*/
