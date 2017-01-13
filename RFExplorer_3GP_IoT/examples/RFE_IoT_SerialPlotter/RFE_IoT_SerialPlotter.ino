/*

  RF Explorer 3G+ IoT for Arduino - A Spectrum Analyzer for everyone!
  Copyright © 2010-17 Ariel Rocholl, www.rf-explorer.com
  
  This sketch is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.
  
  -----------------------------------------------------------------
  
  Example: RFE_IoT_SerialPlotter - Version 1.0.0 - 2017/01/13

  General Description: 
    Capture a complete Sweep for electromagnetic spectrum with RF Explorer 3G+ IoT. 
    Show graph with Serial Plotter Arduino tool.
  
  Software Description:
    * setup() Initialize hardware and communications:
        * MWSUB3G module connects at 115200bps
        * Monitor Serial (USB debug port) remains at 57600bps
        * Wait for default configuration 
     * loop() - Collect a complete Sweep and send to SerialPlotter
        * Wait for user configuration. User frequency configuration in example is between 902 - 928MHz  
        * Received one Sweep. 
        * Send this frequency data step by step to Serial
        * Shutdown unit RF Explorer and Finish program

  Circuit Description:
   * RF Explorer 3G+ IoT for Arduino
   * Arduino DUE
   * USB cable for programming and monitoring
  
  Support Email: contact@rf-explorer.com

  See: http://www.rf-explorer.com/IoT for more information

*/

#include <RFExplorer_3GP_IoT.h>

//Constants of initial and end frequency that display in Serial Plotter. User can change to other values of frequency
#define _START_FREQ_KHZ 902000
#define _STOP_FREQ_KHZ  928000       

#if _START_FREQ_KHZ>=_STOP_FREQ_KHZ
    #error PLEASE SPECIFY VALID FREQUENCY RANGE
#endif

#if ((_START_FREQ_KHZ < _MIN_FREQUENCY_KHZ) || (_STOP_FREQ_KHZ>_MAX_FREQUENCY_KHZ))
    #error PLEASE SPECIFY VALID FREQUENCY RANGE
#endif

RFExplorer_3GP_IoT g_objRF;                      //Global 3G+ object for access to all RF Explorer IoT functionality
unsigned short int g_nProcessResult=_RFE_IGNORE; //Global variable for result in method processReceivedString()

void setup()
{ 
    digitalWrite(_RFE_GPIO2, LOW);      //Set _RFE_GPIO2 as output, LOW -> Device mode to 2400bps
    g_objRF.resetHardware();            //Reset 3G+ board
    
    delay(5000);                        //Wait for 3G+ to complete initialization routines
    g_objRF.init();                     //Initialize 3G+ library - Monitor SerialDebugger set 57600bps
    g_objRF.changeBaudrate(115200);     //Change baudrate to 115Kbps, max reliable in Arduino DUE.
    delay(1000);                        //Wait 1sec to stablish communication
    digitalWrite(_RFE_GPIO2, HIGH);     
    pinMode(_RFE_GPIO2, INPUT_PULLUP);  //Set _RFE_GPIO2 as a general port, no longer needed after start completed
    
    g_objRF.requestConfig();            //Request of current configuration to 3G+ -> Device starts to send it default setup and them SweepData
    
    //Wait for message received is Default Config from 3G+
    do
    {
        g_objRF.updateBuffer();
        g_nProcessResult = g_objRF.processReceivedString();  
    }
    while(!((g_nProcessResult == _RFE_SUCCESS) && (g_objRF.getLastMessage() == _CONFIG_MESSAGE)));
     
    //Send Command to change RF module configuration
    g_objRF.sendNewConfig(_START_FREQ_KHZ, _STOP_FREQ_KHZ);      
}

void loop() 
{   
    //Wait for message received is User Config from 3G+
    do
    {
        g_objRF.updateBuffer();
        g_nProcessResult = g_objRF.processReceivedString();  
    }
    while(!((g_nProcessResult == _RFE_SUCCESS) && (g_objRF.getLastMessage() == _CONFIG_MESSAGE)));

    //Wait for message received is SweepData from 3G+
    do
    {
        g_objRF.updateBuffer();
        g_nProcessResult = g_objRF.processReceivedString();  
    } 
    while (!((g_nProcessResult == _RFE_SUCCESS) && (g_objRF.getLastMessage() == _SWEEP_MESSAGE) && g_objRF.isValid()));
    
    //Put scale Y Axis between -125 to -25 dBm manually (unfortunately Arduino Serial Plotter does not
    //offer yet a different way to normalize axis)
    g_objRF.getMonitorSerial().println(-35);
    g_objRF.getMonitorSerial().println(-105);

    for (unsigned short int nIndexStep = 0; nIndexStep<g_objRF.getConfiguration()->getFreqSpectrumSteps(); nIndexStep++)
    {
        //Print every step of sweep data into Serial Plotter
        g_objRF.getMonitorSerial().println(g_objRF.getSweepData()->getAmplitudeDBM(nIndexStep)); 
    }
    //Put RFExplorer 3G+ IoT in shutdown mode and finish program
    g_objRF.sleep(); 
    while(1);
}
