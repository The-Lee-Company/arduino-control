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

#include <Arduino.h>
#include "lee_ventus_uart.h"

int device_type = 0;

void setup() 
{
  //Connect two Serial ports 
  Serial.begin(115200);       // initialize UART connected to RX0 TX0 (connected to inbuilt USB)  
  Serial1.begin(115200);       // initialize UART connected to RX1 TX1 (connected to Pump Drive Board)

  delay(100);
  // the GP (on Evaluation kit) and SPM boards use different channels for their pressure sensors
  device_type = uart_read_int16(REGISTER_DEVICE_TYPE);
}

void loop() 
{

// -----------------------------------------------------------------------------
// Manual control example
// -----------------------------------------------------------------------------
  Serial.print("\n\nMANUAL POWER CONTROL");


  //set to manual power control
  uart_setup_manual_power_control();
  //and enable pump
  uart_write_int16(REGISTER_PUMP_ENABLE, 1);


  //cycle through different power levels
  for (int target_power = 100; target_power <= 1000; target_power += 100)
  {
    uart_write_float(REGISTER_SET_VAL, target_power);
    delay(100);

    Serial.print("\nTarget power: ");
    Serial.print(target_power);
    Serial.print("mW");

    //read the power level and pressure
    for(int j=0; j<4; j++)
    {
      float measured_power, measured_pressure;
      measured_power = uart_read_float(REGISTER_MEAS_DRIVE_MILLIWATTS);
      // the GP (on Evaluation kit) and SPM boards use different channels for their pressure sensors
      if(device_type == DEVICE_TYPE_SPM) measured_pressure = uart_read_float(REGISTER_MEAS_DIGITAL_PRESSURE);
      if(device_type == DEVICE_TYPE_GP) measured_pressure = uart_read_float(REGISTER_MEAS_ANA_2);

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
  uart_write_int16(REGISTER_PUMP_ENABLE, 0);

// -----------------------------------------------------------------------------
// PID pressure control example
// -----------------------------------------------------------------------------
  Serial.print("\n\nPID PRESSURE CONTROL");
  Serial.print("\nWaiting for pressure in the system to die down...");
  delay(5000); // wait for the pressure in the system to die down


  //set to PID pressure control
  // the GP (on Evaluation kit) and SPM boards use different channels for their pressure sensors
  if(device_type == DEVICE_TYPE_SPM) uart_spm_setup_PID_pressure_control();
  if(device_type == DEVICE_TYPE_GP) uart_gp_setup_PID_pressure_control();
  //and enable pump
  uart_write_int16(REGISTER_PUMP_ENABLE, 1);


  //cycle through different pressure levels
  for (int target_pressure = 100; target_pressure <= 250; target_pressure += 50)
  {
    uart_write_float(REGISTER_SET_VAL, target_pressure);
    delay(100);

    Serial.print("\nTarget pressure: ");
    Serial.print(target_pressure);
    Serial.print("mbar");

    //read the power level and pressure
    for(int j=0; j<10; j++)
    {
      float measured_power, measured_pressure;
      measured_power = uart_read_float(REGISTER_MEAS_DRIVE_MILLIWATTS);
      // the GP (on Evaluation kit) and SPM boards use different channels for their pressure sensors
      if(device_type == DEVICE_TYPE_SPM) measured_pressure = uart_read_float(REGISTER_MEAS_DIGITAL_PRESSURE);
      if(device_type == DEVICE_TYPE_GP) measured_pressure = uart_read_float(REGISTER_MEAS_ANA_2);

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
  uart_write_int16(REGISTER_PUMP_ENABLE, 0);
}
