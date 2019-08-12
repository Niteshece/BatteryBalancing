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
#include "BatteryOperation.h"
#include <math.h>

/*****************************************************************************/
/*                           STATIC FUNCTION AND VARIABLE                   */
/*****************************************************************************/
static void app_EffectiveVoltageCalculate(void);

static void app_Calculate_Currents_OneSwitch(int n);
static void app_Calculate_Currents_TwoSwitch(int n,int m);
static void app_Calculate_Currents_ThreeSwitch();


static void app_CalculateVolatgesLeft_OneSwitch(int n);
static void app_CalculateVolatgesLeft_TwoSwitch(int n,int m);
static void app_CalculateVolatgesLeft_ThreeSwitch(void);
static void app_UpdateDischargeSlope(void);

static void app_CalculateEffectiveTime(int n,float *effectivetime_factor);
static void app_EffectiveVoltageCompare(float* effective_volatge_cell1, float* effective_volatge_cell2, float* effective_volatge_cell3);

/*******************************************************************************/
/*                          LOCAL MODULE VARIABLES                             */
/*******************************************************************************/

/*States of switches*/
static t_switchstates switches_state_present = state_000;
static t_switchstates switches_state_next = state_000;
tswitch_state present_switch_state[3] = {OFF}; /*This variable will be used to update the running switch state*/

/*Battery Cut OFF indication*/
int Battery_Low = FALSE;

/*Cell related Graphical Parameters*/
cell_coordinates cell[3] = {0};

/* Presently used discharge slope will be updated in this variable*/
float sf_discharge_slope[0];


/**************************************************************************
* Function name : void app_SwitchStateManager(void)
* Description   : This API is the state manager for all the switch states 
				  and is supported by other functions of this file.
* Parameter     : void
* Return value  : void
***************************************************************************/
void app_SwitchStateManager(void)
{
    /*This will update check the Present volatges and select the slope to be used accordingly*/
    app_UpdateDischargeSlope();
    
    switch(switches_state_present)
    {
        case state_000 :
            battery[0].current = 0;
            battery[1].current = 0;
            battery[2].current = 0;
            present_switch_state[0] = OFF;
            present_switch_state[1] = OFF;
            present_switch_state[2] = OFF;
            Set_SwitchState(present_switch_state);
            usleep(VOLTAGE_UPDATE_TIME);
            
            /*Decide next state on the basis of above calculation*/
            app_EffectiveVoltageCalculate();
            switches_state_present = switches_state_next;
            switches_state_next = state_000;
            break;
        case state_001 :
            present_switch_state[0] = ON;
            present_switch_state[1] = OFF;
            present_switch_state[2] = OFF;
            Set_SwitchState(present_switch_state);
            battery[1].current = 0;
            battery[2].current = 0;
            app_Calculate_Currents_OneSwitch(0);
            usleep(VOLTAGE_UPDATE_TIME);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_OneSwitch(0);
            /*Decide next state on the basis of above calculation*/
            app_EffectiveVoltageCalculate();
            switches_state_present = switches_state_next;
            switches_state_next = state_000;
            break;
        case state_010 :
            present_switch_state[0] = OFF;
            present_switch_state[1] = ON;
            present_switch_state[2] = OFF;
            Set_SwitchState(present_switch_state);
            battery[0].current = 0;
            battery[2].current = 0;
            app_Calculate_Currents_OneSwitch(1);
            usleep(VOLTAGE_UPDATE_TIME);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_OneSwitch(1);
            /*Decide next state on the basis of above calculation*/
            app_EffectiveVoltageCalculate();
            switches_state_present = switches_state_next;
            switches_state_next = state_000;            
            break;
        case state_011 :
            present_switch_state[0] = ON;
            present_switch_state[1] = ON;
            present_switch_state[2] = OFF;
            Set_SwitchState(present_switch_state);
            app_Calculate_Currents_TwoSwitch(0,1);
            battery[2].current = 0;
            usleep(VOLTAGE_UPDATE_TIME);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_TwoSwitch(0,1);
            /*Decide next state on the basis of above calculation*/
            app_EffectiveVoltageCalculate();
            switches_state_present = switches_state_next;
            switches_state_next = state_000;            
            break;
        case state_100 :
            present_switch_state[0] = OFF;
            present_switch_state[1] = OFF;
            present_switch_state[2] = ON;
            Set_SwitchState(present_switch_state);
            battery[0].current = 0;
            battery[1].current = 0;
            app_Calculate_Currents_OneSwitch(2);
            usleep(VOLTAGE_UPDATE_TIME);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_OneSwitch(2);
            /*Decide next state on the basis of above calculation*/
            app_EffectiveVoltageCalculate();
            switches_state_present = switches_state_next;
            switches_state_next = state_000;            
            break;
        case state_101 :
            present_switch_state[0] = ON;
            present_switch_state[1] = OFF;
            present_switch_state[2] = ON;
            Set_SwitchState(present_switch_state);
            app_Calculate_Currents_TwoSwitch(0,2);
            battery[1].current = 0;
            usleep(VOLTAGE_UPDATE_TIME);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_TwoSwitch(0,2);
            /*Decide next state on the basis of above calculation*/
            app_EffectiveVoltageCalculate();
            switches_state_present = switches_state_next;
            switches_state_next = state_000;            
            break;
        case state_110 :
            present_switch_state[0] = OFF;
            present_switch_state[1] = ON;
            present_switch_state[2] = ON;
            Set_SwitchState(present_switch_state);
            app_Calculate_Currents_TwoSwitch(1,2);
            battery[0].current = 0;
            usleep(VOLTAGE_UPDATE_TIME);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_TwoSwitch(1,2);
            /*Decide next state on the basis of above calculation*/
            app_EffectiveVoltageCalculate();
            switches_state_present = switches_state_next;
            switches_state_next = state_000;            
            break;
        case state_111 :
            present_switch_state[0] = ON;
            present_switch_state[1] = ON;
            present_switch_state[2] = ON;
            Set_SwitchState(present_switch_state);
            app_Calculate_Currents_ThreeSwitch();
            usleep(VOLTAGE_UPDATE_TIME);
            /*calculate Parameter after 1 sec*/
            app_CalculateVolatgesLeft_ThreeSwitch();
            /*Decide next state on the basis of above calculation*/
            app_EffectiveVoltageCalculate();
            switches_state_present = switches_state_next;
            switches_state_next = state_000;            
            break;
            
        default        :
            printf("Some Error Occured... Reset the system!");
            break;  
        
    }
    
}

/**************************************************************************
* Function name : app_EffectiveVoltageCalculate()
* Description   : This API calculates the effective voltage and does the 
				  comparison for next state decision. 
* Parameter     : void
* Return value  : void
***************************************************************************/
static void app_EffectiveVoltageCalculate(void)
{
    float temp_voltageIR=0;     /*For Temp Volatge across internal Resistance*/
    float temp_resistance = 0;  /*Equivalent Resistance of circuit*/
    float temp_current = 0;
    static float effective_volatge_cell1 =0;
    static float effective_volatge_cell2 =0;
    static float effective_volatge_cell3 =0;
    
    /*For previous cycle*/
    printf("\nEffective Voltages = %f\t,%f\t,%f\n",effective_volatge_cell1,effective_volatge_cell2,effective_volatge_cell3);
    
    
    
    /*For next cycle*/
    effective_volatge_cell1 =0;
    effective_volatge_cell2 =0;
    effective_volatge_cell3 =0;
    
    /*Consider If only 1st cell is connected*/
    temp_resistance = battery[0].resistance+sf_loadr;
    temp_current = battery[0].voltage/temp_resistance;
    /*drop across Internal resistence*/
    temp_voltageIR = temp_current*battery[0].resistance;
    /*Actual Voltage for RL load*/
    effective_volatge_cell1 = battery[0].voltage - temp_voltageIR;
    
    temp_resistance=0;
    temp_current = 0;
    temp_voltageIR =0;
    
    /*Consider If only 2nd cell is connected*/
    temp_resistance = battery[1].resistance+sf_loadr;
    temp_current = battery[1].voltage/temp_resistance;
    /*drop across Internal resistence*/
    temp_voltageIR = temp_current*battery[1].resistance;
    /*Actual Voltage for RL load*/
    effective_volatge_cell2 = battery[1].voltage - temp_voltageIR;
    
    
    temp_resistance=0;
    temp_current = 0;
    temp_voltageIR =0;
    sf_systemvoltage = 0;
    
    /*Consider If only 2nd cell is connected*/
    temp_resistance = battery[2].resistance+sf_loadr;
    temp_current = battery[2].voltage/temp_resistance;
    /*drop across IR*/
    temp_voltageIR = temp_current*battery[2].resistance;
    /*Actual Voltage for RL load*/
    effective_volatge_cell3 = battery[2].voltage - temp_voltageIR;
    
    app_EffectiveVoltageCompare(&effective_volatge_cell1, &effective_volatge_cell2, &effective_volatge_cell3);
}


/**************************************************************************
* Function name : app_EffectiveVoltageCompare()
* Description   : This API calculates the voltage left in one cell if one 
				  was ON. 
* Parameter     : float, flat, float
* Return value  : void
***************************************************************************/
static void app_EffectiveVoltageCompare(float* effective_volatge_cell1, float* effective_volatge_cell2, float* effective_volatge_cell3)
{
    int temp_state = 0;
    
#if ROUNDOFFENABLE==1
    *effective_volatge_cell1 = floorf(*effective_volatge_cell1*10)/10;
    *effective_volatge_cell2 = floorf(*effective_volatge_cell2*10)/10;
    *effective_volatge_cell3 = floorf(*effective_volatge_cell3*10)/10;   
#endif
    
    
    /*effective_volatge_cell1,effective_volatge_cell2, effective_volatge_cell3 are the volatge drop of individual cell 
     * across Load Rl. Which ever will be higher, the corresponding cell 
     * shall be switched ON*/
    if((*effective_volatge_cell1>=*effective_volatge_cell2)&&(*effective_volatge_cell1>=*effective_volatge_cell3)&&(*effective_volatge_cell1>CUT_OFF_VOLTAGE))
    {
        temp_state = temp_state|(1<<0);
        sf_systemvoltage = *effective_volatge_cell1;
    }
    if((*effective_volatge_cell2>=*effective_volatge_cell1)&&(*effective_volatge_cell2>=*effective_volatge_cell3)&&(*effective_volatge_cell2>CUT_OFF_VOLTAGE))
    {
        temp_state = temp_state|(1<<1);
        sf_systemvoltage = *effective_volatge_cell2;
    }
    if((*effective_volatge_cell3>=*effective_volatge_cell1)&&(*effective_volatge_cell3>=*effective_volatge_cell2)&&(*effective_volatge_cell3>CUT_OFF_VOLTAGE))
    {
        temp_state = temp_state|(1<<2);
        sf_systemvoltage = *effective_volatge_cell3;
    }
    
    /*If All switches are off i.e all cell at cut off voltage*/
    if(temp_state == 0 && switches_state_present == state_000)
    {
        Battery_Low = TRUE;
        sf_systemcurrent = 0;
    }
    switches_state_next = temp_state;
    
}


/**************************************************************************
* Function name : app_CalculateVolatgesLeft_OneSwitch()
* Description   : This API calculates the voltage left in one cell if one 
				  was ON. 
* Parameter     : int n
* Return value  : void
***************************************************************************/
static void app_CalculateVolatgesLeft_OneSwitch(int n)
{
    float temp_voltage_leftover = 0;
    float effectivetime_factor =0;
    
    app_CalculateEffectiveTime(n,&effectivetime_factor);
    temp_voltage_leftover = battery[n].voltage + ((effectivetime_factor)*(sf_discharge_slope[n])*(battery[n].voltage));
    battery[n].voltage = temp_voltage_leftover;
}

/**************************************************************************
* Function name : app_CalculateVolatgesLeft_TwoSwitch()
* Description   : This API calculates the voltage left in two cells if two 
				  were ON. 
* Parameter     : int, int
* Return value  : void
***************************************************************************/
static void app_CalculateVolatgesLeft_TwoSwitch(int n,int m)
{
    float temp_voltage_leftover_celln = 0;
    float temp_voltage_leftover_cellm = 0;
    float effectivetime_factor =0;
    
    app_CalculateEffectiveTime(n,&effectivetime_factor);
    temp_voltage_leftover_celln = battery[n].voltage + ((effectivetime_factor)*(sf_discharge_slope[n])*battery[n].voltage)/2;
    
    effectivetime_factor = 0;
    app_CalculateEffectiveTime(m,&effectivetime_factor);
    temp_voltage_leftover_cellm = battery[m].voltage + ((effectivetime_factor)*(sf_discharge_slope[m])*battery[m].voltage)/2;
    
    
    battery[n].voltage=temp_voltage_leftover_celln;
    battery[m].voltage=temp_voltage_leftover_cellm;
}

/**************************************************************************
* Function name : app_CalculateVolatgesLeft_ThreeSwitch()
* Description   : This API calculates the voltage left in three cells if three 
				  were ON. 
* Parameter     : void
* Return value  : void
***************************************************************************/
static void app_CalculateVolatgesLeft_ThreeSwitch()
{
    float temp_voltage_leftover_cell1 = 0;
    float temp_voltage_leftover_cell2 = 0;
    float temp_voltage_leftover_cell3 = 0;
    float effectivetime_factor = 0;
    app_CalculateEffectiveTime(0,&effectivetime_factor);
    
    temp_voltage_leftover_cell1 = battery[0].voltage + (((effectivetime_factor)*(sf_discharge_slope[0])*battery[0].voltage)/3);
    
    effectivetime_factor = 0;
    app_CalculateEffectiveTime(1,&effectivetime_factor);
    temp_voltage_leftover_cell2 = battery[1].voltage + (((effectivetime_factor)*(sf_discharge_slope[1])*battery[1].voltage)/3);
    
    effectivetime_factor = 0;
    app_CalculateEffectiveTime(2,&effectivetime_factor);
    temp_voltage_leftover_cell3 = battery[2].voltage + (((effectivetime_factor)*(sf_discharge_slope[2])*battery[2].voltage)/3);
    
    battery[0].voltage = temp_voltage_leftover_cell1;
    battery[1].voltage = temp_voltage_leftover_cell2;
    battery[2].voltage = temp_voltage_leftover_cell3;
}


/**************************************************************************
* Function name : app_Calculate_Currents_OneSwitch()
* Description   : This API calculates the current of individual cell and 
				  system when one switch is ON. 
* Parameter     : int 
* Return value  : void
***************************************************************************/
static void app_Calculate_Currents_OneSwitch(int n)
{
    /*here nth Resistance is in series with Load Resistance*/
    /*So net Resistance will be addition of Load and nth Resistance*/
    /* Voltage will be the system voltage as In current is flowing between GND and
     * system voltage point*/
    
    battery[n].current = (sf_systemvoltage)/(battery[n].resistance+sf_loadr);
    sf_systemcurrent = battery[n].current;    
}

/**************************************************************************
* Function name : app_Calculate_Currents_TwoSwitch()
* Description   : This API calculates the current of individual cell and 
				  system when two switches are ON. 
* Parameter     : int, int 
* Return value  : void
***************************************************************************/
static void app_Calculate_Currents_TwoSwitch(int n ,int m)
{
    float temp_netResistence=0;
    /*Consider calculation for nth Resistance-*/
    /*nth resistence is in series with a parallel combination of mth and Load Resistance*/
    temp_netResistence = (battery[n].resistance + ((battery[m].resistance*sf_loadr)/(sf_loadr+battery[m].resistance)));
    /* Voltage will be the system voltage as In current is flowing between GND and
     * system voltage point*/
    battery[n].current = sf_systemvoltage/temp_netResistence;
    
    
    temp_netResistence=0;
    /*Consider calculation for mth Resistance-*/
    /*mth resistence is in series with a parallel combination of nth and Load Resistance*/
    temp_netResistence = (battery[m].resistance + ((battery[n].resistance*sf_loadr)/(sf_loadr+battery[n].resistance)));
    /* Voltage will be the system voltage as In current is flowing between GND and
     * system voltage point*/
    battery[m].current = sf_systemvoltage/temp_netResistence;
    
    
    sf_systemcurrent = battery[n].current + battery[m].current;
    
}

/**************************************************************************
* Function name : app_Calculate_Currents_ThreeSwitch()
* Description   : This API calculates the current of individual cell and 
				  system when three switches are ON. 
* Parameter     : void
* Return value  : void
***************************************************************************/
static void app_Calculate_Currents_ThreeSwitch()
{
    float temp_netResistence = 0;
    /*Cosider 1st cell, Its internal resitance R1 is in series with parallel combination
     * of R2||R3||RLoad*/
    temp_netResistence = battery[0].resistance+
                        ((battery[1].resistance*battery[2].resistance*sf_loadr)/
                        ((sf_loadr*battery[1].resistance)+
                        (sf_loadr*battery[2].resistance)+
                        (battery[1].resistance*battery[2].resistance)));
    battery[0].current = sf_systemvoltage/temp_netResistence;
    
    temp_netResistence = 0;
    /*Cosider 2nd cell, Its internal resitance R2 is in series with parallel combination
     * of R1||R3||RLoad*/
    temp_netResistence = battery[1].resistance+
                        ((battery[0].resistance*battery[2].resistance*sf_loadr)/
                        ((sf_loadr*battery[0].resistance)+
                        (sf_loadr*battery[2].resistance)+
                        (battery[0].resistance*battery[2].resistance)));
    battery[1].current = sf_systemvoltage/temp_netResistence;
    
    temp_netResistence = 0;
    /*Cosider 2nd cell, Its internal resitance R3 is in series with parallel combination
     * of R1||R2||RLoad*/
    temp_netResistence = battery[2].resistance+
                        ((battery[0].resistance*battery[1].resistance*sf_loadr)/
                        ((sf_loadr*battery[0].resistance)+
                        (sf_loadr*battery[1].resistance)+
                        (battery[0].resistance*battery[1].resistance)));
    battery[2].current = sf_systemvoltage/temp_netResistence;
    
    sf_systemcurrent = battery[0].current+battery[1].current+battery[2].current;
    
}

/**************************************************************************
* Function name : app_Clalculate_Currents_ThreeSwitch()
* Description   : This API calculates the current of individual cell and 
				  system when three switches are ON. 
* Parameter     : void
* Return value  : void
***************************************************************************/
static void app_CalculateEffectiveTime(int n,float *effectivetime_factor)
{
    /*here We will find the effective Asec lost by battery*/
    /*this time will be used to find the voltage fall as per 
     * the volatge discharge curve*/
    
    /*So, our batter is 360000Ampsec battery. This means It shall give 1Amp
     * for 360000sec. Now if we run system for 1Amp and half sec then effective
     * Ampsec lost is .5. So effective dissipation time is .5sec. Similarly
     *if we draw .5Amp for .5sec then effectve ampsec are .25ampsec and effective
     * time is .25sec*/
    
    
    *effectivetime_factor = ((VOLTAGE_UPDATE_TIME)*battery[n].current)/1000000;
                                    /*In sec*/  /*Current from that cell*/
}

/**************************************************************************
* Function name : app_UpdateDischargeSlope()
* Description   : This API calculates the current of individual cell and 
				  system when three switches are ON. 
* Parameter     : void
* Return value  : void
***************************************************************************/
static void app_UpdateDischargeSlope(void)
{
    if(battery[0].voltage<=(cell[0].point_B.coordinate_y))
    {
        sf_discharge_slope[0] = cell[0].slope_m2;
    }
    else
    {
        sf_discharge_slope[0] = cell[0].slope_m1;
    }
    
    if(battery[1].voltage<=cell[1].point_B.coordinate_y)
    {
        sf_discharge_slope[1] = cell[1].slope_m2;
    }
    else
    {
        sf_discharge_slope[1] = cell[1].slope_m1;
    }
    
    if(battery[2].voltage<=cell[2].point_B.coordinate_y)
    {
        sf_discharge_slope[2] = cell[2].slope_m2;
    }
    else
    {
        sf_discharge_slope[2] = cell[2].slope_m1;
    }
    
}

/**************************************************************************
* Function name : app_CalculateSlope()
* Description   : This API calculates the current of individual cell and 
				  system when three switches are ON. 
* Parameter     : void
* Return value  : void
***************************************************************************/
void app_CalculateSlope(void)
{
    /*x coordinates are the points on time/capacity axis(ampsec)*/
    /*Y coordinates are the points on Volatge percentage axis*/
    
    /* Now there are two slopes m1 & m2. m1 is the slope between point A
     * (100% volatge(y) , 0% of initial capacity(x)) & point B (70% voltage(y) , 60% of inital capacity(Ampsec)(x))*/
    /*Slope m2 is between point B(70% voltage & 60%(y) of inital capacity(Ampsec)(x)) & point
     * C (8volts(y), 100% of initial capacity(Ampsec)(x)), Refer document for more understanding*/
    
    /*First finding the coordinates of point A,B,C*/
    
    /*Coordinates of A-Cell1*/
    cell[0].point_A.coordinate_y = battery[0].voltage;
    cell[0].point_A.coordinate_x = 0;
    
    /*Coordinates of B-Cell1*/
    cell[0].point_B.coordinate_y = ((70)*battery[0].voltage)/100;
    cell[0].point_B.coordinate_x = ((60)*CELL_CAPACITY)/100;
    
    
    /*Coordinates of C-Cell1*/
    cell[0].point_C.coordinate_y = CUTOFF_VOLTAGE;
    cell[0].point_C.coordinate_x = CELL_CAPACITY;
    
    /*Slope Cell1_m1*/
    cell[0].slope_m1 = ((cell[0].point_B.coordinate_y - cell[0].point_A.coordinate_y)/(cell[0].point_B.coordinate_x - cell[0].point_A.coordinate_x));
    /*Slope Cell1_m2*/
    cell[0].slope_m2 = ((cell[0].point_C.coordinate_y - cell[0].point_B.coordinate_y)/(cell[0].point_C.coordinate_x - cell[0].point_B.coordinate_x));
    
 /*==================================================================*/   
    /*Coordinates of A-Cell2*/
    cell[1].point_A.coordinate_y = battery[1].voltage;
    cell[1].point_A.coordinate_x = 0;
    
    /*Coordinates of B-Cell2*/
    cell[1].point_B.coordinate_y = ((70)*battery[1].voltage)/100;
    cell[1].point_B.coordinate_x = ((60)*CELL_CAPACITY)/100;
    
    
    /*Coordinates of C-Cell2*/
    cell[1].point_C.coordinate_y = CUTOFF_VOLTAGE;
    cell[1].point_C.coordinate_x = CELL_CAPACITY;
    
    
    /*Slope Cell2_m1*/
    cell[1].slope_m1 = ((cell[1].point_B.coordinate_y - cell[1].point_A.coordinate_y)/(cell[1].point_B.coordinate_x - cell[1].point_A.coordinate_x));
    /*Slope Cell1_m2*/
    cell[1].slope_m2 = ((cell[1].point_C.coordinate_y - cell[1].point_B.coordinate_y)/(cell[1].point_C.coordinate_x - cell[1].point_B.coordinate_x));
    
 /*==================================================================*/   
    /*Coordinates of A-Cell3*/
    cell[2].point_A.coordinate_y = battery[2].voltage;
    cell[2].point_A.coordinate_x = 0;
    
    /*Coordinates of B-Cell3*/
    cell[2].point_B.coordinate_y = ((70)*battery[2].voltage)/100;
    cell[2].point_B.coordinate_x = ((60)*CELL_CAPACITY)/100;
    
    
    /*Coordinates of C-Cell3*/
    cell[2].point_C.coordinate_y = CUTOFF_VOLTAGE;
    cell[2].point_C.coordinate_x = CELL_CAPACITY;
    
    /*Slope Cell2_m1*/
    cell[2].slope_m1 = ((cell[2].point_B.coordinate_y - cell[2].point_A.coordinate_y)/(cell[2].point_B.coordinate_x - cell[2].point_A.coordinate_x));
    /*Slope Cell1_m2*/
    cell[2].slope_m2 = ((cell[2].point_C.coordinate_y - cell[2].point_B.coordinate_y)/(cell[2].point_C.coordinate_x - cell[2].point_B.coordinate_x));
    
}
