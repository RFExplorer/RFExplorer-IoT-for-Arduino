/*

  RF Explorer 3G+ IoT for Arduino - A Spectrum Analyzer for everyone!
  Copyright © 2010-17 Ariel Rocholl, www.rf-explorer.com
  
  This sketch is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.
  
  -----------------------------------------------------------------
  
  Example: RFE_IoT_GetPeak - Version 1.0.0 - 2016/12/01

  General Description: 
    Simplest example with RF Explorer 3G+ IoT capturing external Signal Peak.
    Use the Arduino Terminal to detect results
  
  Software Description:
    * setup() initialize hardware and communications:
        * MWSUB3G module connects at 115200bps
        * Monitor Serial (USB debug port) remains at 57600bps
    * loop() continuous scan frequency range 1.180 to 1.2 GHz to detect peak signal
        * Everytime a new sweep scan is received, the Terminal will display frequency and power of peak signal

  Circuit Description:
   * RF Explorer 3G+ IoT for Arduino
   * Arduino DUE
   * USB cable for programming and monitoring
  
  Support Email: contact@rf-explorer.com

  See: http://www.rf-explorer.com/IoT for more information

*/

#include <RFExplorer_3GP_IoT.h>

RFExplorer_3GP_IoT g_objRF;   //Global 3G+ object for access to all RF Explorer IoT functionality
 
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
    
    g_objRF.requestConfig();            //Request of current configuration to 3G+ -> Device starts to send it setup and them SweepData
}

void loop() 
{
    //Call to these two functions to refresh library received data.
    //If this function is not being called regularly, data received from 3G+ may be lost
    //and keep unprocessed
    g_objRF.updateBuffer();             
    unsigned short int nProcessResult = g_objRF.processReceivedString(); 
    
    //If received data processing was correct, we can use it
    if (nProcessResult == _RFE_SUCCESS) 
    {
        if ((g_objRF.getLastMessage() == _CONFIG_MESSAGE)) 
        {
            //Message received is a new configuration from 3G+
            //We show new Start/Stop KHZ range here from the new configuration
            g_objRF.getMonitorSerial().println("New Config");
            g_objRF.getMonitorSerial().print("StartKHz: "); 
            g_objRF.getMonitorSerial().println(g_objRF.getConfiguration()->getStartKHZ());
            g_objRF.getMonitorSerial().print("StopKHz:  "); 
            g_objRF.getMonitorSerial().println(g_objRF.getConfiguration()->getEndKHZ());  
        }
        else if((g_objRF.getLastMessage() == _SWEEP_MESSAGE) && g_objRF.isValid()) 
        {
            //Message received was actual sweep data, we can now use internal functions
            //to get sweep data parameters
            unsigned long int nFreqPeakKHZ=0;                                      
            int16_t nPeakDBM=0;
            if (g_objRF.getPeak(&nFreqPeakKHZ, &nPeakDBM) ==_RFE_SUCCESS)           
            {
                //Display frequency and amplitude of the signal peak
                g_objRF.getMonitorSerial().print(nFreqPeakKHZ);
                g_objRF.getMonitorSerial().print(" KHz to ");
                g_objRF.getMonitorSerial().print(nPeakDBM);
                g_objRF.getMonitorSerial().println(" dBm"); 
            }
        }
    }
    else
    {
        //Message or Data was not received, or was wrong. Note:
        // _RFE_IGNORE or _RFE_NOT_MESSAGE are not errors, it just mean a new message was not available
        if ((nProcessResult != _RFE_IGNORE) && (nProcessResult != _RFE_NOT_MESSAGE))
        {
            //Other error codes were received, report for information
            //Check library error codes for more details
            g_objRF.getMonitorSerial().print("Error:");
            g_objRF.getMonitorSerial().println(nProcessResult);
        }
    }
}
