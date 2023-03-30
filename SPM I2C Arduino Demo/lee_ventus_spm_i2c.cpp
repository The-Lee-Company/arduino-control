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


#include "lee_ventus_spm_i2c.h"
#include <Wire.h> // Arduino library for I2C


// example function to set the SPM settings to do manual power control
// use register REGISTER_PUMP_ENABLE to enable the pump
// use register REGISTER_SET_VAL to set the pump power (<1000mW)
void spm_i2c_setup_manual_power_control(int i2c_address)
{
  spm_i2c_write_int16(i2c_address,  REGISTER_PUMP_ENABLE                  ,0);            //pump off
  spm_i2c_write_int16(i2c_address,  REGISTER_STREAM_MODE_ENABLE           ,0);            //streaming off
  spm_i2c_write_int16(i2c_address,  REGISTER_CONTROL_MODE                 ,MODE_MANUAL);  //manual mode
  spm_i2c_write_int16(i2c_address,  REGISTER_MANUAL_MODE_SETPOINT_SOURCE  ,SOURCE_SETVAL);//target power source (0 = set val register 23) 
  spm_i2c_write_float(i2c_address,  REGISTER_SET_VAL                      ,0);            //register 23 (power target). Set to zero initially 
}

// example function to set the SPM settings to do PID pressure control
// use register REGISTER_PUMP_ENABLE to enable the pump
// use register REGISTER_SET_VAL to set the target pressure
void spm_i2c_setup_PID_pressure_control(int i2c_address)
{
  spm_i2c_write_int16(i2c_address,  REGISTER_PUMP_ENABLE                  ,0);            //pump off
  spm_i2c_write_int16(i2c_address,  REGISTER_STREAM_MODE_ENABLE           ,0);            //streaming off
  spm_i2c_write_int16(i2c_address,  REGISTER_CONTROL_MODE                 ,MODE_PID);            //PID mode
  spm_i2c_write_int16(i2c_address,  REGISTER_PID_MODE_SETPOINT_SOURCE     ,SOURCE_SETVAL);//PID setpoint source (0=Set val (register 23), 1=Analog A,  2=Analog B, 3= Analog C
  spm_i2c_write_int16(i2c_address,  REGISTER_PID_MODE_MEAS_SOURCE         ,SOURCE_DIGITAL_PRESSURE);//PID measurement source (0=Set val (register 23), 1=Analog A  2=Analog B, 3= Analog C, 4=Flow sensor, 5= digital pressure sensor
  spm_i2c_write_float(i2c_address,  REGISTER_PID_PROPORTIONAL_COEFF       ,5);            //PID  'P=5'
  spm_i2c_write_float(i2c_address,  REGISTER_PID_INTEGRAL_COEFF           ,10);           //PID  'I=10'
  spm_i2c_write_float(i2c_address,  REGISTER_PID_DIFFERENTIAL_COEFF       ,0);            //PID  'D=0' Advisable to keep this at zero
  spm_i2c_write_float(i2c_address,  REGISTER_SET_VAL                      ,0);            // register 23 (PID target). Set to zero initially 
}



// -----------------------------------------------------------------------------
// Low level commands for sending and receiving commands
// -----------------------------------------------------------------------------

//command to write an integer value to the SPM
//returns if the write was successful or not
bool spm_i2c_write_int16(int i2c_address, int register_id, int16_t value_to_write)
{
  int response;
  uint8_t *ptrToInt;
  ptrToInt = (uint8_t *)&value_to_write; //split the value into MSB and LSB 
  
  Wire.beginTransmission(i2c_address);  //start write process- send address
  Wire.write(register_id);              // send register ID byte      
  Wire.write(ptrToInt[0]);              // LSByte of int16_t         
  Wire.write(ptrToInt[1]);              // MSByte of int16_t 

  response = Wire.endTransmission();
  if (response != 0) {
    Serial.println("Error during write to spm");//error report
    return false;
  }
  return true;
}

//command to write a float value to the SPM
//returns if the write was successful or not
bool spm_i2c_write_float(int i2c_address, int register_id, float value_to_write)
{
  int response;
  uint8_t *ptrToFloat;
  ptrToFloat = (uint8_t *)&value_to_write;//split the value into 4 bytes
   
  Wire.beginTransmission(i2c_address);  //start write process- send address
  Wire.write(register_id);          // send register ID byte        
  Wire.write(ptrToFloat[0]);        // send LSByte of int16_t 
  Wire.write(ptrToFloat[1]);        // send 2nd LSByte of int16_t 
  Wire.write(ptrToFloat[2]);        // send 3rd LSByte of int16_t 
  Wire.write(ptrToFloat[3]);        // send MSByte of int16_t 
  
  response = Wire.endTransmission();
  if (response != 0) {
    Serial.println("Error during write to spm");//error report
    return false;
  }
  return true;  
}

//command to read a float value to the SPM
//returns -32,768 if there is an error
int16_t spm_i2c_read_int16(int i2c_address, int register_id)
{
  
  //write the request to the SPM to report value (See SPM comms guide)
  int response;
  Wire.beginTransmission(i2c_address);//start write process- send address
  Wire.write(128 + register_id);  //Register ID byte (+128 to set the MSB to 1 per SPM Comms guide) 
  response = Wire.endTransmission();
  if (response != 0) 
  {
    Serial.println("Error 1 during read from spm");
    return -32768;
  }
  
  Wire.requestFrom(i2c_address, 2);//request 2 bytes of data encoding the response from the SPM
  if (Wire.available() < 2) 
  {
    Serial.println("Error while reading SPM measurement");//error report
    return -32768;
  }
 
  int16_t value;
  byte byteArray[2];
  for(int i = 0; i < 2; i++)//read the two bytes
  {
    byteArray[i] = Wire.read();
  }

  //construct the int16_t value from the two bytes
  uint8_t *ptrToInt;
  ptrToInt = (uint8_t *)&value;
  ptrToInt[0] = byteArray[0];
  ptrToInt[1] = byteArray[1];
  return value;
}

//command to read an integer value to the SPM
//returns -32,768 if there is an error
float spm_i2c_read_float(int i2c_address, int register_id)
{

  //write the request to the SPM to report value (See SPM comms guide)
  int response;
  Wire.beginTransmission(i2c_address);
  Wire.write(128 + register_id);  //Register ID byte (+128 to set the MSB to 1 per Comms guide) 
  response = Wire.endTransmission();
  if (response != 0) 
  {
    Serial.println("Error 1 during read from spm");//error report
    return -32768.0;
  }

  Wire.requestFrom(i2c_address, 4);//request 4 bytes of data encoding the measurement  
  if (Wire.available() < 4)
  {
    Serial.println("Error while reading SPM measurement");//error report
    return -32768.0;
  }
  
  float value;
  byte byteArray[4];
  for(int i = 0; i < 4; i++)//save the 4 bytes into an array
  {
    byteArray[i] = Wire.read();
  }
  
  //convert the bytes into a float and return 
  uint8_t *ptrToFloat;
  ptrToFloat = (uint8_t *)&value;
  ptrToFloat[0] = byteArray[0];
  ptrToFloat[1] = byteArray[1];
  ptrToFloat[2] = byteArray[2];
  ptrToFloat[3] = byteArray[3];
  return value;
}
