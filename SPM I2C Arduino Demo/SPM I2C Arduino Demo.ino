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

#include <Arduino.h>
#include <Wire.h> // Arduino library for I2C
#include "lee_ventus_spm_i2c.h"


void setup() 
{
  Serial.begin(9600); //initialize serial communication
  Serial.println("SPM I2C demo");

  Wire.begin(); // join i2c bus (address optional for master)
}

void loop() 
{

// -----------------------------------------------------------------------------
// Manual control example
// -----------------------------------------------------------------------------
  Serial.print("\n\nMANUAL POWER CONTROL");


  //set to manual power control
  spm_i2c_setup_manual_power_control(SPM_DEFAULT_I2C_ADDRESS);
  //and enable pump
  spm_i2c_write_int16(SPM_DEFAULT_I2C_ADDRESS, REGISTER_PUMP_ENABLE, 1);


  //cycle through different power levels
  for (int target_power = 100; target_power <= 1000; target_power += 100)
  {
    spm_i2c_write_float(SPM_DEFAULT_I2C_ADDRESS, REGISTER_SET_VAL, target_power);
    delay(100);

    Serial.print("\nTarget power: ");
    Serial.print(target_power);
    Serial.print("mW");

    //read the power level and pressure
    for(int j=0; j<4; j++)
    {
      float measured_power, measured_pressure;
      measured_power = spm_i2c_read_float(SPM_DEFAULT_I2C_ADDRESS, REGISTER_MEAS_DRIVE_MILLIWATTS);
      measured_pressure = spm_i2c_read_float(SPM_DEFAULT_I2C_ADDRESS, REGISTER_MEAS_DIGITAL_PRESSURE);

      Serial.print("\nPower: ");
      Serial.print(measured_power);
      Serial.print("mW, ");
      Serial.print("Pressure: ");
      Serial.print(measured_pressure);
      Serial.print("mbar");

      delay(500);
    }
  }

  //disable pump
  spm_i2c_write_int16(SPM_DEFAULT_I2C_ADDRESS, REGISTER_PUMP_ENABLE, 0);

// -----------------------------------------------------------------------------
// PID pressure control example
// -----------------------------------------------------------------------------
  Serial.print("\n\nPID PRESSURE CONTROL");
  Serial.print("\nWaiting for pressure in the system to die down...");
  delay(5000); // wait for the pressure in the system to die down


  //set to PID pressure control
  spm_i2c_setup_PID_pressure_control(SPM_DEFAULT_I2C_ADDRESS);
  //and enable pump
  spm_i2c_write_int16(SPM_DEFAULT_I2C_ADDRESS, REGISTER_PUMP_ENABLE, 1);


  //cycle through different pressure levels
  for (int target_pressure = 100; target_pressure <= 250; target_pressure += 50)
  {
    spm_i2c_write_float(SPM_DEFAULT_I2C_ADDRESS, REGISTER_SET_VAL, target_pressure);
    delay(100);

    Serial.print("\nTarget pressure: ");
    Serial.print(target_pressure);
    Serial.print("mbar");

    //read the power level and pressure
    for(int j=0; j<10; j++)
    {
      float measured_power, measured_pressure;
      measured_power = spm_i2c_read_float(SPM_DEFAULT_I2C_ADDRESS, REGISTER_MEAS_DRIVE_MILLIWATTS);
      measured_pressure = spm_i2c_read_float(SPM_DEFAULT_I2C_ADDRESS, REGISTER_MEAS_DIGITAL_PRESSURE);

      Serial.print("\nPower: ");
      Serial.print(measured_power);
      Serial.print("mW, ");
      Serial.print("Pressure: ");
      Serial.print(measured_pressure);
      Serial.print("mbar");

      delay(500);
    }
  }

  //disable pump
  spm_i2c_write_int16(SPM_DEFAULT_I2C_ADDRESS, REGISTER_PUMP_ENABLE, 0);
}
