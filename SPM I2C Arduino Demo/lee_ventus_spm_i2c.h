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
Lee Ventus SPM I2C Arduino demo
Date: 02/02/2023
Author: Ruzhev, Dimitar
Arduino IDE version: 2.0.2

For up to date information on the I2C commands and functionality please refer to:
Technical Note TN003: Communications Guide
*/


#ifndef _LEE_VENTUS_SPM_I2C_
#define _LEE_VENTUS_SPM_I2C_

#include <Arduino.h>
#include "lee_ventus_register.h"

#define SPM_DEFAULT_I2C_ADDRESS (37)

// example function to set the SPM settings to do manual power control
// use register REGISTER_PUMP_ENABLE to enable the pump
// use register REGISTER_SET_VAL to set the pump power (<1000mW)
void spm_i2c_setup_manual_power_control(int i2c_address);

// example function to set the SPM settings to do PID pressure control
// use register REGISTER_PUMP_ENABLE to enable the pump
// use register REGISTER_SET_VAL to set the target pressure
void spm_i2c_setup_PID_pressure_control(int i2c_address);



// -----------------------------------------------------------------------------
// Low level commands for sending and receiving commands
// -----------------------------------------------------------------------------

//command to write an integer value to the SPM
//returns if the write was successful or not
bool spm_i2c_write_int16(int i2c_address, int register_id, int16_t value_to_write);

//command to write a float value to the SPM
//returns if the write was successful or not
bool spm_i2c_write_float(int i2c_address, int register_id, float value_to_write);



//command to read a float value to the SPM
//returns -32,768 if there is an error
int16_t spm_i2c_read_int16(int i2c_address, int register_id);

//command to read an integer value to the SPM
//returns -32,768 if there is an error
float spm_i2c_read_float(int i2c_address, int register_id);


#endif