# Disc pump General purpose driver and Smart pump modules arduino control
This repository contains a Arduino code snippets for The Lee Company Disc Pump General purpose drive board (UEKA0300000AA), Evaluation kit (UEKA05xxxxxxxx) and Smart Pump Module (Uxxxx2xxxxxAA).

## Repository contents
Provided are two examples for communicating with and controlling Ventus Drive PCBs using and Arduino.

The folder “SPM_GP UART Arduino Demo” contains code for communicating with a drive board over UART. The code is compatible with either a General Purpose Drive Board or with a Smart Pump Module. The files “lee_ventus_uart.h” and “lee_ventus_uart.cpp” provide functions for communication with and control of the board. The file “SPM_GP UART Arduino Demo.ino” contains the example Arduino code, running through a demo consisting of multiple setpoints of power and PID pressure control.

The folder “SPM I2C Arduino Demo” contains code for communicating with an SPM board over I2C. The code is only compatible with a Smart Pump Module as the General Purpose Drive Boards do not support control  over I2C. The files “lee_ventus_spm_i2c.h” and “lee_ventus_spm_i2c.cpp” provide functions for communication  with and control of an SPM. The file “SPM I2C Arduino Demo.ino” contains a similar demo to the UART example.

The header file “lee_ventus_register.h” is common between the two projects. It provides a human readable mapping to the drive board register numbers. It also provides human readable mappings for some control values.


## Contact us

For additional support, please visit https://www.theleeco.com/contact/ or you can call our Technical Support Line on 1-800-LEE-PLUG.

We welcome suggestions for how we can improve and build upon this repository; please feel free to share your ideas, feature requests and any bugs you identify with us using the email address above. 

## DISCLAIMER 
These Arduino demos are provided "as is" and without any warranty of any kind, and their use is at your own risk. LEE Ventus does not warrant the performance or results that you may obtain by using these Arduino demos. LEE Ventus makes no warranties regarding these Arduino demos, express or implied, including as to non-infringement, merchantability, or fitness for any particular purpose. To the maximum extent permitted by law LEE Ventus disclaims liability for any loss or damage resulting from use of these Arduino demos, whether arising under contract, tort (including negligence), strict liability, or otherwise, and whether direct, consequential, indirect, or otherwise, even if LEE Ventus has been advised of the possibility of such damages, or for any claim from any third party.
