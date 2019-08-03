/****************************************************************************
 * File Name: application.c
 * Author:Nitesh Garg
 * Version:1.0
 * Date:Aug,02,2019
 * Operating Environment:C
 * Description: This .c contains the app for actual managing the main.
 * List of functions and routines used:
 static void app_processing_routine()
 static void app_data_routine()
 int main()
 
 ****************************************************************************/

 
/*****************************************************************************/
/*                           INCLUDE FILES                               */
/*****************************************************************************/
#include <stdio.h>
#include <signal.h>
#include "Battery_processing/BatteryOperation.h"
/*****************************************************************************/
/*                           STATIC FUNCTION AND VARIABLE                   */
/*****************************************************************************/



/*******************************************************************************/
/*                          LOCAL MODULE VARIABLES                             */
/*******************************************************************************/
static void app_data_routine();
static void app_processing_routine();

/*******************************************************************************/
/*                          External VARIABLES                             */
/*******************************************************************************/
extern int Battery_Low;

/**************************************************************************
* Function name : main()
* Description   : This function is entry point of the whole application.
* Parameter     : void
* Return value  : int
***************************************************************************/
int main()
{
    /*Local Variables to take user inputs*/
    float fa_volatge[3]= {0};
    float f_loadohm = 0;
    float fa_internalr[3]= {0};
    
    
    /*Take User Inputs*/    
    printf("Enter Volatge for cell 1 in volts\n");
    scanf("%f",&fa_volatge[0]);
    printf("Enter Volatge for cell 2 in volts\n");
    scanf("%f",&fa_volatge[1]);
    printf("Enter Volatge for cell 3 in volts\n");
    scanf("%f",&fa_volatge[2]);
    Set_CellVolatages(fa_volatge);
    
    printf("Enter Load Value in Ohms\n");
    scanf("%f",&f_loadohm);
    Set_LoadValue(f_loadohm);
    
    printf("Enter internal Resistance of cell 1 in ohms\n");
    scanf("%f",&fa_internalr[0]);
    printf("Enter internal Resistance of cell 2 in ohms\n");
    scanf("%f",&fa_internalr[1]);
    printf("Enter internal Resistance of cell 3 in ohms\n");
    scanf("%f",&fa_internalr[2]);
    Set_InternalResistance(fa_internalr);
    
    /*Simulation Started*/    
    app_processing_routine();

    printf("Processing done\n");
}


/**************************************************************************
* Function name : app_processing_routine
* Desc	ription   : This API takes care of the whole simulation
* Parameter     : void *arg
* Return value  : 
***************************************************************************/
static void app_processing_routine()
{  
    /*Check for all cell at 8V*/
	while((Battery_Low != 1))
    {
		app_SwitchStateManager();
        app_data_routine();
	}
}



/**************************************************************************
* Function name : app_data_routine
* Description   : This API takes care of display data part
* Parameter     : void *arg
* Return value  : 
***************************************************************************/
static void app_data_routine()
{
    float fa_cellvoltage[3] = {0};
    float fa_cellcurrent[3] = {0};
    float f_sysvoltage = 0;
    float f_syscurrent = 0;
    tswitch_state switchstate[3] = {OFF};

    /*Get Switch State*/
    Get_SwitchState(switchstate);
    
    /*Get Cell Voltages*/
    Get_CellVolatages(fa_cellvoltage);
    
    /*Get Cell Currents*/
    Get_Cellcurrent(fa_cellcurrent);
    
    /*Get System Voltage*/
    Get_SystemVolatge(&f_sysvoltage);
    
    /*Get System Current*/   
    Get_Systemcurrent(&f_syscurrent);
    
    printf("Switch 1 = %d\n", switchstate[0]);
    printf("Cell 1 Voltage = %f \t", fa_cellvoltage[0]);
    printf("Cell 1 Current = %f \n", fa_cellcurrent[0]);
    
    printf("Switch 2 = %d\n", switchstate[1]);
    printf("Cell 2 Voltage = %f \t", fa_cellvoltage[1]);
    printf("Cell 2 Current = %f \n", fa_cellcurrent[1]);
    
    printf("Switch 3 = %d\n", switchstate[2]);
    printf("Cell 3 Voltage = %f \t", fa_cellvoltage[2]);
    printf("Cell 3 Current = %f \n", fa_cellcurrent[2]);
    
    
    printf("System Load Voltage = %f\t", f_sysvoltage);
    printf("System Load current = %f\n", f_syscurrent);
}
