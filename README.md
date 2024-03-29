Directory & File Structure : 
Project--|	
               |-CSR_Requirements - Battery_Balancing_C++.pdf				
               |-Code
               	|-All_Data
                       	|-data.c
                       	|-data.h 
                       |- Battery_processing
			|- Battery_Operation.c
                                  |- BatteryOperation.h
                       |- application.c

1.	Battery_Balancing_C++.pdf – This document is Gram Power provided assessment document.
2.	application.c – This .c file contains the main entry point of the code. There are three APIs in the file :
•	Function main: Here the data is collected from the user, stored into the buffers and further simulation is started.
•	Function app_processing_routine : This API check for 8v min. level and executes the API which simulates battery cells switch states.
•	Function app_data_routine : This API prints the updated data(Voltage ,Current & Switch states) on console for User.
3.	Battery_Operation.c – This file contains the state machine for switch state sand calculation of currents & voltages.
•	Function app_SwitchStateManager : This API is the state manager for all the switch states and is supported by other functions of this file.
•	Function app_Calculate_Currents_OneSwitch: This API calculates the voltage left in one cell if one was ON.
•	Function app_Calculate_Currents_TwoSwitch: This API calculates the voltage left in two cells if two were ON.
•	Function app_Calculate_Currents_ThreeSwitch: This API calculates the voltage left in three cells if three were ON.
•	Function app_CalculateVolatgesLeft_OneSwitch: This API calculates the current of individual cell and system when one switch is ON.
•	Function app_CalculateVolatgesLeft_TwoSwitch: This API calculates the current of individual cell and system when two switches are ON.
•	Function app_CalculateVolatgesLeft_ThreeSwitch: This API calculates the current of individual cell and system when three switches are ON.
•	Function app_VoltageCompare: This API calculates the effective voltage and does the	  comparison for next state decision.

4.	data.c – Tis files contains the getter setter methods for all the buffers and variable to keep the data abstracted from user read and write.
Implementation : 
1.	High Level Design :
This big picture of the implementation contains a state machine which always keeps updating the state of the switches. After every 1sec in a state it recalculates the remaining voltages of the cells in order to understand the next probable state of the switches i.e. cell whose voltage is highest will have corresponding switch ON. Also further it calculates the voltage that Load will have in next state, currents for present states.  
Data is abstracted in data.c from user and manipulated only through battery_operation.c.
 

2.	Low Level Design & Algorithm: 

Consider any switch state, when the assumed state is set the following things happens:
•	Switches which are open, their respective currents are set to zero.
•	Switched which are closed, their respective currents are calculated. This will have four scenarios:
o	No Switch is ON
o	One Switch is  ON
o	Two Switches are ON
o	All three switches are ON
•	Based on Above calculation system current is calculated as sum of all the available currents.
•	System goes in discharge(sleep) for one second.
•	Now after discharge of 1 sec, New voltages are calculated. This calculation based on switch status will have four scenarios:
o	No Switch is ON
o	One Switch is  ON
o	Two Switches are ON
o	All three switches are ON
•	The switches which were OFF have their corresponding voltage discharge 0 for that discharge cycle.
•	Now with new voltage next switch state is calculated.
Calculation of Respective Currents
1.	No Switch is ON
When no switch is ON the currents will be zero as all the switches are open.
 

2.	One Switch is ON
In case of one switch is on R1 and RL goes in series. So total resistance is (R1+RL). Voltage would be the voltage of only connected cell i.e. Vn. SO current for particular cell will be:
In=Vn/(R1+RL)

 
3.	Two Switches are ON

 

In case of two cell Cn and Cm. Resistance for current In for cell Cn is (Rn series (Rm||RL)) .
Resistance for current Im for cell Cm is (Rm series (Rn||RL)). 
Voltage will be same if both the cells are connected together.  By ohms law we can calculate In & Im individually.

4.	Three Switches are ON
 

This case can be taken as, here cell C1,C2,C3 with current I1,I2,I3, with internal resistance R1,R2,R3 and Load Resistance RL . 
With respect to Cell C1 R1 is in series with parallel combination of (R2 || R3 || RL)
With respect to Cell C2 R2 is in series with parallel combination of (R1 || R3 || RL)
With respect to Cell C3 R3 is in series with parallel combination of (R1 || R2 || RL)
Voltage will be common as all the cells are ON so voltages are same. With Resistances been known and Voltages been known, By Ohms law we can find I1, I1 and I3.
Calculation of Respective Voltages Left
Assumption : The discharge curves of all cells are same with slope = -0.1  (Can be configured through Macro)
1.	No Switch is ON
When no switch is ON, no voltage discharge has taken place so voltages remain same.
 


2.	One Switch is ON
 
In case of one switch is ON 0.1part of total voltage will be discharged(approx10%). So Left voltage can be calculated by linear subtraction.
3.	Two Switches are ON

 

In case of two cell Cn and Cm, Since the load requirement is same and the voltages of both the cells are same discharge will be approx 5%(0.1/2).

4.	Three Switches are ON
 

This case can be taken as, here cell C1,C2,C3, since voltage is same and load is again common as for above cases the discharge time would be 3times less compare to case 2. So discharge will be approx.3.33% (0.1/3).
Calculation of Voltages for Comparison:
NEED : This voltage calculations are needed to considered as voltage drop on Load will not be same as that of a cell voltage. There will be a minimal voltage drop on the internal resistances too. 
This can be a considerable loss too if in case of old(or deteriorated) cell. So this cannot be neglected in system voltage calculation. Once the individual drops are calculated across the internal resistances then we can have the three nodal resistances for all the three cells. 
These potentials can then be compared with each other to find the maximum potential across the load.
Calculation goes as:
Assumption : Cells do not charge each other.
The Aim is to find potential drop across internal resistance Rn from cell Cn. There will not be any effect of other cell as per the mentioned assumption. So with this we can consider circuit as below : 


Internal Resistance Rn and the Load RL are in series. Net resistance will be Rn+RL. Voltage drop would be same as potential of cell Cn viz.. Vn. By ohms law Current In :
= Vn/(Rn+RL)
With this we can find drop across Rn as:
Vntemp = In*Rn
By this way we can find effective cell potential for Load RL from Lth and mth cell as VLtemp& Vmtemp These three Voltages can be compares, and cell with highest effective voltage can be connected into the circuit. Rest all the cells can be kept with opened switch. In this way switch positions can be determined.
Enhancements:
1.	Effective voltages are approximated to one decimal place and then compared. This feature can be disabled by changing the value of macro in ROUNDOFFENABLE to 0. This macro can be found in BatteryOperation.h.
2.	Earlier the voltage for reduced with respect to time. Now the voltage is reduced with respect to time and current discharged both.
For example:
Consider a battery of capacity 100Ahr. This is equivalent to 100*60*60Asec = 360000Asec. By this it means this battery can draw 1Ampere current at constant voltage for 360000seconds.
Now if run the system for 1sec but current drawn is .5Amp Then,
•	In previous case : Voltage was reduced with respect to time only viz.:
Volatge_remaining = volatge_present-(Time(insec))*(slope)*volatge_present
•	In new case the current drawn in that 1 second is  also taken in consideration. current drawn was 0.5Amp, so by this effective time/capacity lost by batter is .5 only, Therefore:
volatge_remaining = volatge_present-(EffectiveTime(insec))*(slope)*volatge_present.
Reference  : https://www.electronicdesign.com/test-measurement/measuring-cell-capacity
New function introduced in order to provide this functionality is :
app_CalculateEffectiveTime – This function will convert the Amp lost parameter in terms of time and an effective time is provided to calculate the voltage delivered by battery.
3.	Use of Two step linear discharge curve
Using above reference(Enhancement 2) it is understood for const. current time is fixed by battery capacity. So assuming that our each battery is of 360000Asec, It will discharge to cut off voltage (8V) within 360000sec. The discharge curve is distributed in two parts.
•	Slow Discharge : It is initial phase of system discharge where it will try to maintain a close to constant voltage. In this the voltage remaining would be slowly decreasing.
•	Fast discharge : In this phase of system, the voltage will reduce much rapidly as compared to the previous scenario.

 

From 100% to 70% the output voltage reduces at a slower pace. From 70% to cut off voltage reduces at comparatively faster pace.
These slopes are m1 & m2.
They are found with the help of formula:
m = y2-y1/x2-x1
New functions introduced in order to perform this job are :
o	static void app_UpdateDischargeSlope(void) : This function keeps checking the activities of cell voltages and switches the slope for voltage calculation as per monitoring.
o	app_CalculateSlope : based on the user provided inputs this function calculates the slope m1&m2 using the above mentioned method. When to use which slope is decided by app_UpdateDischargeSlope.
4.	To increase the modularity & readability of code, all the battery data are organized in a structure instead of operating on them on isolated basis.
5.	Some Variables are renamed in order to make the code more understandable.
6.	Improved the commenting in code.
7.	VoltageCompare function is split into two functions to increase the modularity and enable the better ease to test the code.
•	app_EffectiveVoltageCalculate : This function calculates the effective voltages of all the cells across the Load RL. 
•	app_EffectiveVoltageCompare : The above calculated effective voltages are compared and used to decide the next state of switches by this function. 
8.	Input validation check has been incorporated in the code now. With this check one new function is introduced in application.c file :
a.	app_GatherData() : This API will take care of gathering and validating the data.


