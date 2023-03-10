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


#include "lee_ventus_uart.h"

//buffer for reading incoming messages
const byte numChars = 32;
char receivedChars[numChars];

// example function to set the board settings to do manual power control
// use register REGISTER_PUMP_ENABLE to enable the pump
// use register REGISTER_SET_VAL to set the pump power (<1000mW)
void uart_setup_manual_power_control()
{
  uart_write_int16(REGISTER_PUMP_ENABLE                  ,0);            //pump off
  uart_write_int16(REGISTER_STREAM_MODE_ENABLE           ,0);            //streaming off
  uart_write_int16(REGISTER_CONTROL_MODE                 ,MODE_MANUAL);  //manual mode
  uart_write_int16(REGISTER_MANUAL_MODE_SETPOINT_SOURCE  ,SOURCE_SETVAL);//target power source (0 = set val register 23) 
  uart_write_float(REGISTER_SET_VAL                      ,0);            //register 23 (power target). Set to zero initially 
}

// example function to set the SPM settings to do PID pressure control
// use register REGISTER_PUMP_ENABLE to enable the pump
// use register REGISTER_SET_VAL to set the target pressure
void uart_spm_setup_PID_pressure_control()
{
  uart_write_int16(REGISTER_PUMP_ENABLE                  ,0);            //pump off
  uart_write_int16(REGISTER_STREAM_MODE_ENABLE           ,0);            //streaming off
  uart_write_int16(REGISTER_CONTROL_MODE                 ,MODE_PID);     //PID mode
  uart_write_int16(REGISTER_PID_MODE_SETPOINT_SOURCE     ,SOURCE_SETVAL);//PID setpoint source (0=Set val (register 23), 1=Analog A,  2=Analog B, 3= Analog C
  uart_write_int16(REGISTER_PID_MODE_MEAS_SOURCE         ,SOURCE_DIGITAL_PRESSURE);//PID measurement source (0=Set val (register 23), 1=Analog A  2=Analog B, 3= Analog C, 4=Flow sensor, 5= digital pressure sensor
  uart_write_float(REGISTER_PID_PROPORTIONAL_COEFF       ,5);            //PID  'P=5'
  uart_write_float(REGISTER_PID_INTEGRAL_COEFF           ,10);           //PID  'I=10'
  uart_write_float(REGISTER_PID_DIFFERENTIAL_COEFF       ,0);            //PID  'D=0' Advisable to keep this at zero
  uart_write_float(REGISTER_SET_VAL                      ,0);            // register 23 (PID target). Set to zero initially 
}

// example function to set the GP board settings to do PID pressure control
// use register REGISTER_PUMP_ENABLE to enable the pump
// use register REGISTER_SET_VAL to set the target pressure
void uart_gp_setup_PID_pressure_control()
{
  uart_write_int16(REGISTER_PUMP_ENABLE                  ,0);            //pump off
  uart_write_int16(REGISTER_STREAM_MODE_ENABLE           ,0);            //streaming off
  uart_write_int16(REGISTER_CONTROL_MODE                 ,MODE_PID);     //PID mode
  uart_write_int16(REGISTER_PID_MODE_SETPOINT_SOURCE     ,SOURCE_SETVAL);//PID setpoint source (0=Set val (register 23), 1=Analog A,  2=Analog B, 3= Analog C
  uart_write_int16(REGISTER_PID_MODE_MEAS_SOURCE         ,SOURCE_ANA2);  //PID measurement source (0=Set val (register 23), 1=Analog A  2=Analog B (pressure sensor on the eval kit), 3= Analog C, 4=Flow sensor, 5= digital pressure sensor
  uart_write_float(REGISTER_PID_PROPORTIONAL_COEFF       ,5);            //PID  'P=5'
  uart_write_float(REGISTER_PID_INTEGRAL_COEFF           ,10);           //PID  'I=10'
  uart_write_float(REGISTER_PID_DIFFERENTIAL_COEFF       ,0);            //PID  'D=0' Advisable to keep this at zero
  uart_write_float(REGISTER_SET_VAL                      ,0);            // register 23 (PID target). Set to zero initially 
}



// -----------------------------------------------------------------------------
// Low level commands for sending and receiving commands
// -----------------------------------------------------------------------------

//command to write an integer value to the board
//returns if the write was successful or not
bool uart_write_int16(int register_id, int16_t value_to_write)
{
  String command = String("#W");
  command = command + register_id;
  command = command + ",";
  command = command + value_to_write;
  command = command + "\n";

  Serial1.print(command);

  return true;
}

//command to write a float value to the board
//returns if the write was successful or not
bool uart_write_float(int register_id, float value_to_write)
{
  String command = String("#W");
  command = command + register_id;
  command = command + ",";
  command = command + value_to_write;
  command = command + "\n";

  Serial1.print(command);

  return true;
}

//command to read a int value from the board
int16_t uart_read_int16(int register_id)
{
  String command = String("#R");
  command = command + register_id;
  command = command + "\n";

  char rc;
  static bool recvInProgress = false; // variable for sensing if we are in the data region of the received command
  static byte ndx = 0;                // variable for indexing the buffer

  while (Serial1.available() > 0) {       //clear buffer 
    rc = Serial1.read();
  }

  // request data
  Serial1.print(command);
  delay(10);

  //read serial character by character and store those between "," and "\n" 
  while (Serial1.available() > 0) { 
    rc = Serial1.read();            // rc stores every character of the received command in sequence
    if (recvInProgress == true) {   // this if triggers if we are in the data region beween "," and "\n"
      if (rc != '\n') {             // if we have not reached the end of the data region which is the "\n"
        receivedChars[ndx] = rc;    // store the data
        ndx++;                      // and increment the counter for which digit we are receiving
        if (ndx >= numChars) {      // make sure we do not exceed the limit of the buffer
          ndx = numChars - 1;
        }
      }
      else {                        // this else triggers when we are past the data region, so at the "\n"
        receivedChars[ndx] = '\0';  // terminate the string. Required to properly handle the string
        recvInProgress = false;     // reset the state variables
        ndx = 0;
      }
    }
    else if (rc == ',') {           // this if senses when we reach the ","
      recvInProgress = true;        // and triggers that we are in the data region
    }
  }

  //convert the string value into int
  String value_txt = receivedChars;
  int16_t value = (int16_t)(value_txt.toInt());

  return value;
}

//command to read an float value from the board
float uart_read_float(int register_id)
{
  String command = String("#R");
  command = command + register_id;
  command = command + "\n";

  char rc;
  static bool recvInProgress = false; // variable for sensing if we are in the data region of the received command
  static byte ndx = 0;                // variable for indexing the buffer

  while (Serial1.available() > 0) {       //clear buffer 
    rc = Serial1.read();
  }

  // request data
  Serial1.print(command);
  delay(10);

  //read serial character by character and store those between "," and "\n" 
  while (Serial1.available() > 0) { 
    rc = Serial1.read();            // rc stores every character of the received command in sequence
    if (recvInProgress == true) {   // this if triggers if we are in the data region beween "," and "\n"
      if (rc != '\n') {             // if we have not reached the end of the data region which is the "\n"
        receivedChars[ndx] = rc;    // store the data
        ndx++;                      // and increment the counter for which digit we are receiving
        if (ndx >= numChars) {      // make sure we do not exceed the limit of the buffer
          ndx = numChars - 1;
        }
      }
      else {                        // this else triggers when we are past the data region, so at the "\n"
        receivedChars[ndx] = '\0';  // terminate the string. Required to properly handle the string
        recvInProgress = false;     // reset the state variables
        ndx = 0;
      }
    }
    else if (rc == ',') {           // this if senses when we reach the ","
      recvInProgress = true;        // and triggers that we are in the data region
    }
  }

  //convert the string value into float
  String value_txt = receivedChars;
  float value = value_txt.toFloat();

  return value;
}