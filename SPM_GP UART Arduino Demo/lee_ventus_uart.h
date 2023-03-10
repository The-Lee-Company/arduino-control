/*
DISCLAIMER
This Arduino demo is provided "as is" and without any warranty of any kind, and its use is at your 
own risk. LEE Ventus does not warrant the performance or results that you may obtain by using 
this Arduino demo. LEE Ventus makes no warranties regarding this Arduino demo, express 
or implied, including as to non-infringement, merchantability, or fitness for any particular purpose. 
To the maximum extent permitted by law LEE Ventus disclaims liability for any loss or damage 
resulting from use of this Arduino demo, whether arising under contract, tort (including 
negligence), strict liability, or otherwise, and whether direct, consequential, indirect, or otherwise, 
even if LEE Ventus has been advised of the possibility of such damages, or for any claim from any 
third party.
*/

/*
Lee Ventus UART Arduino demo
Date: 27/02/2023
Author: Ruzhev, Dimitar
Arduino IDE version: 2.0.2

For up to date information on the I2C commands and functionality please refer to:
Technical Note TN003: Communications Guide
*/


#ifndef _LEE_VENTUS_UART_
#define _LEE_VENTUS_UART_

#include <Arduino.h>
#include "lee_ventus_register.h"


// example function to set the board settings to do manual power control
// use register REGISTER_PUMP_ENABLE to enable the pump
// use register REGISTER_SET_VAL to set the pump power (<1000mW)
void uart_setup_manual_power_control();

// example function to set the board settings to do PID pressure control
// use register REGISTER_PUMP_ENABLE to enable the pump
// use register REGISTER_SET_VAL to set the target pressure
void uart_spm_setup_PID_pressure_control();
void uart_gp_setup_PID_pressure_control();



// -----------------------------------------------------------------------------
// Low level commands for sending and receiving commands
// -----------------------------------------------------------------------------

//command to write an integer value to the board
//returns if the write was successful or not
bool uart_write_int16(int register_id, int16_t value_to_write);

//command to write a float value to the board
//returns if the write was successful or not
bool uart_write_float(int register_id, float value_to_write);



//command to read a float value to the board
int16_t uart_read_int16(int register_id);

//command to read an integer value to the board
float uart_read_float(int register_id);


#endif