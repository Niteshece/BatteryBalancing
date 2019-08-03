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
static void app_VoltageCompare();
static void app_CalculateVolatgesLeft_OneSwitch(int n);
static void app_Calculate_Currents_ThreeSwitch();
static void app_Calculate_Currents_TwoSwitch(int n,int m);
static void app_Calculate_Currents_OneSwitch(int n);
static void app_CalculateVolatgesLeft_ThreeSwitch();
static void app_CalculateVolatgesLeft_TwoSwitch(int n,int m);


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
            sfa_cellcurrent[0] = 0;
            sfa_cellcurrent[1] = 0;
            sfa_cellcurrent[2] = 0;
            current_state_set[0] = OFF;
            current_state_set[1] = OFF;
            current_state_set[2] = OFF;
            Set_SwitchState(current_state_set);
            sleep(1);
            
            /*Decide next state on the basis of above calculation*/
            app_VoltageCompare();
            switches_state_current = switches_state_next;
            switches_state_next = state_000;
            break;
        case state_001 :
            current_state_set[0] = ON;
            current_state_set[1] = OFF;
            current_state_set[2] = OFF;
            Set_SwitchState(current_state_set);
            sfa_cellcurrent[1] = 0;
            sfa_cellcurrent[2] = 0;
            app_Calculate_Currents_OneSwitch(0);
            sleep(1);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_OneSwitch(0);
            /*Decide next state on the basis of above calculation*/
            app_VoltageCompare();
            switches_state_current = switches_state_next;
            switches_state_next = state_000;

            break;
        case state_010 :
            current_state_set[0] = OFF;
            current_state_set[1] = ON;
            current_state_set[2] = OFF;
            Set_SwitchState(current_state_set);
            sfa_cellcurrent[0] = 0;
            sfa_cellcurrent[2] = 0;
            app_Calculate_Currents_OneSwitch(1);
            sleep(1);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_OneSwitch(1);
            /*Decide next state on the basis of above calculation*/
            app_VoltageCompare();
            switches_state_current = switches_state_next;
            switches_state_next = state_000;
            
            break;
        case state_011 :
            current_state_set[0] = ON;
            current_state_set[1] = ON;
            current_state_set[2] = OFF;
            Set_SwitchState(current_state_set);
            app_Calculate_Currents_TwoSwitch(0,1);
            sfa_cellcurrent[2] = 0;
            sleep(1);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_TwoSwitch(0,1);
            /*Decide next state on the basis of above calculation*/
            app_VoltageCompare();
            switches_state_current = switches_state_next;
            switches_state_next = state_000;
            
            break;
        case state_100 :
            current_state_set[0] = OFF;
            current_state_set[1] = OFF;
            current_state_set[2] = ON;
            Set_SwitchState(current_state_set);
            sfa_cellcurrent[0] = 0;
            sfa_cellcurrent[1] = 0;
            app_Calculate_Currents_OneSwitch(2);
            sleep(1);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_OneSwitch(2);
            /*Decide next state on the basis of above calculation*/
            app_VoltageCompare();
            switches_state_current = switches_state_next;
            switches_state_next = state_000;
            
            break;
        case state_101 :
            current_state_set[0] = ON;
            current_state_set[1] = OFF;
            current_state_set[2] = ON;
            Set_SwitchState(current_state_set);
            app_Calculate_Currents_TwoSwitch(0,2);
            sfa_cellcurrent[1] = 0;
            sleep(1);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_TwoSwitch(0,2);
            /*Decide next state on the basis of above calculation*/
            app_VoltageCompare();
            switches_state_current = switches_state_next;
            switches_state_next = state_000;
            
            break;
        case state_110 :
            current_state_set[0] = OFF;
            current_state_set[1] = ON;
            current_state_set[2] = ON;
            Set_SwitchState(current_state_set);
            app_Calculate_Currents_TwoSwitch(1,2);
            sfa_cellcurrent[0] = 0;
            sleep(1);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_TwoSwitch(1,2);
            /*Decide next state on the basis of above calculation*/
            app_VoltageCompare();
            switches_state_current = switches_state_next;
            switches_state_next = state_000;
            
            break;
        case state_111 :
            current_state_set[0] = ON;
            current_state_set[1] = ON;
            current_state_set[2] = ON;
            Set_SwitchState(current_state_set);
            app_Calculate_Currents_ThreeSwitch();
            sleep(1);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_ThreeSwitch();
            /*Decide next state on the basis of above calculation*/
            app_VoltageCompare();
            switches_state_current = switches_state_next;
            switches_state_next = state_000;
            
            break;
            
        default        :
            printf("Some Error Occured... Reset the system!");
            break;  
        
    }
    
}

static void app_VoltageCompare()
{
    float temp_voltageIR=0;
    float temp_resistance = 0;
    float temp_current = 0;
    float temp_v1 =0;
    float temp_v2 =0;
    float temp_v3 =0;
    int temp_state = 0;
    
    /*Cosider If only 1st cell is connected*/
    temp_resistance = sfa_cellresistance[0]+sf_loadr;
    temp_current = sfa_cellvoltage[0]/temp_resistance;
    /*drop across IR*/
    temp_voltageIR = temp_current*sfa_cellresistance[0];
    /*Actual Voltage for RL load*/
    temp_v1 = sfa_cellvoltage[0] - temp_voltageIR;
    
    temp_resistance=0;
    temp_current = 0;
    temp_voltageIR =0;
    
        /*Cosider If only 2nd cell is connected*/
    temp_resistance = sfa_cellresistance[1]+sf_loadr;
    temp_current = sfa_cellvoltage[1]/temp_resistance;
    /*drop across IR*/
    temp_voltageIR = temp_current*sfa_cellresistance[1];
    /*Actual Voltage for RL load*/
    temp_v2 = sfa_cellvoltage[1] - temp_voltageIR;
    
    
    temp_resistance=0;
    temp_current = 0;
    temp_voltageIR =0;
    sf_systemvoltage = 0;
    
        /*Cosider If only 2nd cell is connected*/
    temp_resistance = sfa_cellresistance[2]+sf_loadr;
    temp_current = sfa_cellvoltage[2]/temp_resistance;
    /*drop across IR*/
    temp_voltageIR = temp_current*sfa_cellresistance[2];
    /*Actual Voltage for RL load*/
    temp_v3 = sfa_cellvoltage[2] - temp_voltageIR;
    
    /*temp_v1,temp_v2, temp_v3 are the volatge drop of individual cell 
     * across Load Rl. Which ever will be higher, the corresponding cell 
     * shall be switched ON*/
    if((temp_v1>=temp_v2)&&(temp_v1>=temp_v3)&&(temp_v1>8))
    {
        temp_state = temp_state|(1<<0);
        sf_systemvoltage = temp_v1;
    }
    if((temp_v2>=temp_v1)&&(temp_v2>=temp_v3)&&(temp_v2>8))
    {
        temp_state = temp_state|(1<<1);
        sf_systemvoltage = temp_v2;
    }
    if((temp_v3>=temp_v1)&&(temp_v3>=temp_v2)&&(temp_v3>8))
    {
        temp_state = temp_state|(1<<2);
        sf_systemvoltage = temp_v3;
    }
    
    if(temp_state == 0 && switches_state_current == state_000)
    {
        Battery_Low = 1;
        sf_systemcurrent = 0;
    }
    switches_state_next = temp_state;
}

static void app_CalculateVolatgesLeft_OneSwitch(int n)
{
    float temp_voltage_leftover = 0;
    temp_voltage_leftover = sfa_cellvoltage[n] - (VOLTAGE_LOST_CONST)*sfa_cellvoltage[n];
    sfa_cellvoltage[n] = temp_voltage_leftover;
}

static void app_CalculateVolatgesLeft_TwoSwitch(int n,int m)
{
    float temp_voltage_leftover_celln = 0;
    float temp_voltage_leftover_cellm = 0;
    
    temp_voltage_leftover_celln = sfa_cellvoltage[n] - ((VOLTAGE_LOST_CONST)*sfa_cellvoltage[n])/2;
    
    temp_voltage_leftover_cellm = sfa_cellvoltage[m] -((VOLTAGE_LOST_CONST)*sfa_cellvoltage[m])/2;
    
    
    sfa_cellvoltage[n]=temp_voltage_leftover_celln;
    sfa_cellvoltage[m]=temp_voltage_leftover_cellm;
}

static void app_CalculateVolatgesLeft_ThreeSwitch()
{
    float temp_voltage_leftover_cell1 = 0;
    float temp_voltage_leftover_cell2 = 0;
    float temp_voltage_leftover_cell3 = 0;
    
    temp_voltage_leftover_cell1 = sfa_cellvoltage[0] - (((VOLTAGE_LOST_CONST)*sfa_cellvoltage[0])/3);
    
    temp_voltage_leftover_cell2 = sfa_cellvoltage[1] - (((VOLTAGE_LOST_CONST)*sfa_cellvoltage[1])/3);
    
    temp_voltage_leftover_cell3 = sfa_cellvoltage[2] - (((VOLTAGE_LOST_CONST)*sfa_cellvoltage[2])/3);
    
    sfa_cellvoltage[0] = temp_voltage_leftover_cell1;
    sfa_cellvoltage[1] = temp_voltage_leftover_cell2;
    sfa_cellvoltage[2] = temp_voltage_leftover_cell3;
}


static void app_Calculate_Currents_OneSwitch(int n)
{
    /*here nth Resistance is in series with Load Resistance*/
    /*So net Resistance will be addition of Load and nth Resistance*/
    /* Voltage will be the system voltage as In current is flowing between GND and
     * system voltage point*/
    
    sfa_cellcurrent[n] = (sf_systemvoltage)/(sfa_cellresistance[n]+sf_loadr);
    sf_systemcurrent = sfa_cellcurrent[n];    
}


static void app_Calculate_Currents_TwoSwitch(int n ,int m)
{
    float temp_netResistence=0;
    /*Consider calculation for nth Resistance-*/
    /*nth resistence is in series with a parallel combination of mth and Load Resistance*/
    temp_netResistence = (sfa_cellresistance[n] + ((sfa_cellresistance[m]*sf_loadr)/(sf_loadr+sfa_cellresistance[m])));
    /* Voltage will be the system voltage as In current is flowing between GND and
     * system voltage point*/
    sfa_cellcurrent[n] = sf_systemvoltage/temp_netResistence;
    
    
    temp_netResistence=0;
    /*Consider calculation for mth Resistance-*/
    /*mth resistence is in series with a parallel combination of nth and Load Resistance*/
    temp_netResistence = (sfa_cellresistance[m] + ((sfa_cellresistance[n]*sf_loadr)/(sf_loadr+sfa_cellresistance[n])));
    /* Voltage will be the system voltage as In current is flowing between GND and
     * system voltage point*/
    sfa_cellcurrent[m] = sf_systemvoltage/temp_netResistence;
    
    
    sf_systemcurrent = sfa_cellcurrent[n] + sfa_cellcurrent[m];
    
}


static void app_Calculate_Currents_ThreeSwitch()
{
    float temp_netResistence = 0;
    /*Cosider 1st cell, Its internal resitance R1 is in series with parallel combination
     * of R2||R3||RLoad*/
    temp_netResistence = sfa_cellresistance[0]+
                        ((sfa_cellresistance[1]*sfa_cellresistance[2]*sf_loadr)/
                        ((sf_loadr*sfa_cellresistance[1])+
                        (sf_loadr*sfa_cellresistance[2])+
                        (sfa_cellresistance[1]*sfa_cellresistance[2])));
    sfa_cellcurrent[0] = sf_systemvoltage/temp_netResistence;
    
    temp_netResistence = 0;
    /*Cosider 2nd cell, Its internal resitance R2 is in series with parallel combination
     * of R1||R3||RLoad*/
    temp_netResistence = sfa_cellresistance[1]+
                        ((sfa_cellresistance[0]*sfa_cellresistance[2]*sf_loadr)/
                        ((sf_loadr*sfa_cellresistance[0])+
                        (sf_loadr*sfa_cellresistance[2])+
                        (sfa_cellresistance[0]*sfa_cellresistance[2])));
    sfa_cellcurrent[1] = sf_systemvoltage/temp_netResistence;
    
    temp_netResistence = 0;
    /*Cosider 2nd cell, Its internal resitance R3 is in series with parallel combination
     * of R1||R2||RLoad*/
    temp_netResistence = sfa_cellresistance[2]+
                        ((sfa_cellresistance[0]*sfa_cellresistance[1]*sf_loadr)/
                        ((sf_loadr*sfa_cellresistance[0])+
                        (sf_loadr*sfa_cellresistance[1])+
                        (sfa_cellresistance[0]*sfa_cellresistance[1])));
    sfa_cellcurrent[2] = sf_systemvoltage/temp_netResistence;
    
    sf_systemcurrent = sfa_cellcurrent[0]+sfa_cellcurrent[1]+sfa_cellcurrent[2];
    
}
