/****************************************************************************
 * File Name: application.c
 * Author:Nitesh Garg
 * Version:1.0
 * Date:Aug,02,2019
 * Operating Environment:C
 * Description: This .c contains the app for actual managing the main.
 * List of functions and routines used:
 static void app_processing_routine()
 static void app_DataRoutine()
 static void app_GatherData(void)
 int main()
 
 ****************************************************************************/

 
/*****************************************************************************/
/*                           INCLUDE FILES                               */
/*****************************************************************************/
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "Battery_processing/BatteryOperation.h"
/*****************************************************************************/
/*                           STATIC FUNCTION AND VARIABLE                   */
/*****************************************************************************/
static void app_DataRoutine(void);
static void app_processing_routine(void);
static void app_GatherData(void);

/*******************************************************************************/
/*                          LOCAL MODULE VARIABLES                             */
/*******************************************************************************/


/*******************************************************************************/
/*                          External VARIABLES                             */
/*******************************************************************************/
extern int Battery_Low; /*To indicate that Theshold Level has been acheived*/

/**************************************************************************
* Function name : main()
* Description   : This function is entry point of the whole application.
* Parameter     : void
* Return value  : int
***************************************************************************/
int main()
{  
    
    /*Take User Inputs*/ 
    app_GatherData();  
    
    
    /*Simulation Started*/
	printf("Simulation Started\n");
    app_processing_routine();
    
    /*sIMULATION OVER*/
    printf("Processing done\n");
    
    return 0;
}


/**************************************************************************
* Function name : app_GatherData
* Description   : This API Gaathers the data from user.
* Parameter     : void
* Return value  : void
***************************************************************************/
static void app_GatherData(void)
{
    /*Local Variables to take user inputs*/
    float fa_volatge[3]= {0};
    float f_loadohm = 0;        /*Load Value in Ohms*/
    float fa_internalresistances[3]= {0};
    char a[100];
    /*Take User Inputs*/ 
    
    
    /*Taking in cell volatges from the user*/ 
    printf("Enter Volatge for cell 1 in volts\n");
    fgets(a,100,stdin);
    fa_volatge[0] = strtof(a,NULL);
    while((fa_volatge[0]==0) ||  (((70*fa_volatge[0])/100)<=CUT_OFF_VOLTAGE))
    {
        printf("Invalid Input, or the voltage is too less to process Please re-enter the correct voltage of cell 1\n");
        memset(a,0,100);
        fgets(a,100,stdin);
        fa_volatge[0] = strtof(a,NULL);
    }
    
    
    
    printf("Enter Volatge for cell 2 in volts\n");
    fgets(a,100,stdin);
    fa_volatge[1] = strtof(a,NULL);
    while((fa_volatge[1]==0) || (((70*fa_volatge[1])/100)<=CUT_OFF_VOLTAGE))
    {
        printf("Invalid Input, or the voltage is too less to process Please re-enter the correct voltage of cell 2\n");
        memset(a,0,100);
        fgets(a,100,stdin);
        fa_volatge[1] = strtof(a,NULL);
    }
    
    
    
    printf("Enter Volatge for cell 3 in volts\n");
    fgets(a,100,stdin);
    fa_volatge[2] = strtof(a,NULL);
    while((fa_volatge[2]==0) || (((70*fa_volatge[2])/100)<=CUT_OFF_VOLTAGE))
    {
        printf("Invalid Input, or the voltage is too less to process Please re-enter the correct voltage of cell 3\n");
        memset(a,0,100);
        fgets(a,100,stdin);
        fa_volatge[2] = strtof(a,NULL);
    }
    
    Set_CellVolatages(fa_volatge);

    
    
    /*Taking in Load Value from the user*/
    printf("Enter Load Value in ohms\n");
    fgets(a,100,stdin);
    f_loadohm = strtof(a,NULL);
    while(f_loadohm == 0)
    {
        printf("Invalid Input, Please re-enter valid Load Resistance value\n");
        fgets(a,100,stdin);
        f_loadohm = strtof(a,NULL);
    }
    Set_LoadValue(f_loadohm);
    memset(a,0,100);
    
    /*Taking in Cell internal resistances from the user*/
    printf("Enter internal Resistance of cell 1 in ohms\n");
    fgets(a,100,stdin);
    fa_internalresistances[0] = strtof(a,NULL);
    while(fa_internalresistances[0] == 0)
    {
        printf("Invalid Input, Please re-enter valid cell 1 internal Resistance value\n");
        fgets(a,100,stdin);
        fa_internalresistances[0] = strtof(a,NULL);
    }
    memset(a,0,100);
    
    
    printf("Enter internal Resistance of cell 2 in ohms\n");
    fgets(a,100,stdin);
    fa_internalresistances[1] = strtof(a,NULL);
    while(fa_internalresistances[1] == 0)
    {
        printf("Invalid Input, Please re-enter valid cell 2 internal Resistance value\n");
        fgets(a,100,stdin);
        fa_internalresistances[1] = strtof(a,NULL);
    }
    memset(a,0,100);
    
    
    
    printf("Enter internal Resistance of cell 3 in ohms\n");
    fgets(a,100,stdin);
    fa_internalresistances[2] = strtof(a,NULL);
    while(fa_internalresistances[2] == 0)
    {
        printf("Invalid Input, Please re-enter valid cell 3 internal Resistance value\n");
        fgets(a,100,stdin);
        fa_internalresistances[2] = strtof(a,NULL);
    }
    memset(a,0,100);
    
    
    Set_InternalResistance(fa_internalresistances);
}


/**************************************************************************
* Function name : app_processing_routine
* Description   : This API takes care of the whole simulation
* Parameter     : void
* Return value  : void
***************************************************************************/
static void app_processing_routine()
{
    /*Calculating the Slopes m1 and m1 bothh for cell1, cell2, cell3 on the 
     * basis of the user provided input volatges*/
    app_CalculateSlope();
    
    /*Check for all cell at 8V*/
	while((Battery_Low != TRUE))
    {
        /*This is call to state machine managerto manage the switches S1,S2,S3*/
		app_SwitchStateManager();
        
        /*This is call to routine to display data to users*/
        app_DataRoutine();
	}
}



/**************************************************************************
* Function name : app_DataRoutine
* Description   : This API takes care of display data part
* Parameter     : void
* Return value  : void
***************************************************************************/
static void app_DataRoutine(void)
{
    /*Temporary Variables used for diplaying the data*/
    float fa_cellvoltage[3] = {0};
    float fa_cellcurrent[3] = {0};
    float f_systemvoltage = 0;
    float f_systemcurrent = 0;
    
    /*Initial Switch state is Off*/
    tswitch_state switchstate[3] = {OFF};

    /*Get Switch State*/
    Get_SwitchState(switchstate);    
    /*Get Cell Voltages*/
    Get_CellVolatages(fa_cellvoltage);    
    /*Get Cell Currents*/
    Get_Cellcurrent(fa_cellcurrent);    
    /*Get System Voltage*/
    Get_SystemVolatge(&f_systemvoltage);    
    /*Get System Current*/   
    Get_Systemcurrent(&f_systemcurrent);
    
    
	/*Display all data as standard output*/ 
    
    /*Display Switch States*/
    printf("Switch 1 = %d\t\t\t", switchstate[0]);      /*Cell 1 Switch Status*/ 
    printf("Switch 2 = %d\t\t\t", switchstate[1]);      /*Cell 1 Switch Status*/
    printf("Switch 3 = %d\n", switchstate[2]);          /*Cell 1 Switch Status*/
    
    /*Display Cell Voltages*/
    printf("Cell 1 Voltage = %f \t", fa_cellvoltage[0]);/*Cell 1 voltage diaplay*/  
    printf("Cell 2 Voltage = %f \t", fa_cellvoltage[1]);/*Cell 2 voltage diaplay*/
    printf("Cell 3 Voltage = %f \n", fa_cellvoltage[2]);/*Cell 3 voltage diaplay*/
    
    /*Display Cell Currents*/
    printf("Cell 1 Current = %f \t", fa_cellcurrent[0]);/*Cell 1 Current diaplay*/ 
    printf("Cell 2 Current = %f \t", fa_cellcurrent[1]);/*Cell 2 Current diaplay*/ 
    printf("Cell 3 Current = %f \n", fa_cellcurrent[2]);/*Cell 3 Current diaplay*/ 
    
    /*Display Voltage across load and Current through Load*/
    printf("System Load Voltage = %f\t", f_systemvoltage);    
    printf("System Load current = %f\n\n\n", f_systemcurrent);
}
