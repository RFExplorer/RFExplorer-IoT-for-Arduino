//============================================================================
//RF Explorer 3G+ IoT for Arduino - A Spectrum Analyzer for everyone!
//Copyright � 2010-17 Ariel Rocholl, www.rf-explorer.com
//
//Contributed by:
//       Manuel Ballesteros
//
//This application is free software; you can redistribute it and/or
//modify it under the terms of the GNU Lesser General Public
//License as published by the Free Software Foundation; either
//version 3.0 of the License, or (at your option) any later version.
//
//This software is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//General Public License for more details.
//
//You should have received a copy of the GNU General Public
//License along with this library; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//=============================================================================


//------------------------- Includes-------------------------------------------

#include "Arduino.h"
#include "RFEConfiguration.h"
#include "RFESweepData.h"
#include "ByteBuffer_RFE.h"
#include "CommonValues.h"
#if !defined (_SAM3XA_)
    #include <SoftwareSerial.h>
#endif

#ifndef RFEXPLORER_3GP_IOT_H_
#define RFEXPLORER_3GP_IOT_H_

//------------------------- Definitions & Declarations-------------------------

#define MONITOR_SERIAL

#define _HEADER_LIB_3GP  "---RFExplorer 3G+ IoT Arduino---"
#define _VERSION_LIB_3GP "-------Library v1.0.1701.1------"

//IOT RF Explorer is only supported by Arduino DUE  
//and Seeeduino UNO (currently version 4.2) platforms, 
//These platforms operate at 3.3V. 
//Any other plate is incompatible because voltage levels that handles 5V.


#if defined(_SAM3XA_)  && defined(__AVR_ATmega328P__)
    #error REVIEW COMPILATION DEFINED BOARD
#endif

#if !defined(_SAM3XA_)  && !defined(__AVR_ATmega328P__)
    #error REVIEW COMPILATION DEFINED BOARD
#endif


#ifdef MONITOR_SERIAL
    #if defined (_SAM3XA_)
        #define m_objMonitorSerial Serial
    #else
        #define m_objMonitorSerial m_objDebugSP
    #endif
#endif


//------------------------- Class----------------------------------------------

class RFExplorer_3GP_IoT
{

//------------------------- Constructor----------------------------------------

    public:
    RFExplorer_3GP_IoT()
    {

    };

//------------------------- Private Attributes---------------------------------

    private:
    // Objects for storage configuration and data of RFE
    RFEConfiguration m_objRFEConfiguration;
    RFESweepData m_objRFESweepData;

    // Circular buffer for Rx data of RFExplorer 3G+ IoT
    ByteBuffer_RFE m_CircularBuffer;
    
    //String of message received from RFExplorer 3G+ IoT
    char m_pLine[STRING_SIZE];
    uint16_t m_nCharCounter;


    //Number of error process at the beginning
    uint16_t m_nCounterError;

    //Current Baud rate
    uint32_t m_nBaudrate;
    
    //Last Message process
    uint8_t m_nLastMessage;
    
//------------------------- Private Methods ------------------------------------
    private:
    //Send physical command to RF Explorer.
    //pCommand: pointer to the buffer with the command to be sent.
    //nLength: length of the buffer.
    void sendCommand(const char* pCommand, int nLength =-1);


    //Methods to dump information through the serial port and check device operation.
    #ifdef MONITOR_SERIAL
    void monitorSerial_Init();
    #endif

    
    //Get the output line already split from m_CircularBuffer.
    //Returns: 0 if is possible to process otherwise ErrorCodes.
    uint8_t processReceivedString_GetNextLine();
    
    //Clean m_pLine and allows new process
    void LineBufferInit();
 
//------------------------- Public Methods ------------------------------------   
    public:

    //Initialize necessary elements in RF Explorer.
    //Call this function one time at the beginning.
    void init();

    // Hardware restart to RF Explorer RFExplorer 3G+ IoT
    // It place Reset pin of RFExplorer 3GP IoT at low level for a certain time.
    void resetHardware() const;
     
    //Change RF Explorer UART module communication baud rate. The change is immediate and it is lost after a reset.
    //nbaudrate: new baudrate for asynchronous UART communication.
    //Available baud rates are (bps) 2400, 4800, 9600, 19200, 38400, 57600.
    //For Arduino DUE is possible to work up to 115200.
    void changeBaudrate(uint32_t nbaudrate);

    //Change SweepData for success process
    //Recommended for Arduino DUE - 512 steps
    //Recommended for Seeeduino - 240 steps
    void changeNumberSteps(uint16_t nSteps);
        
    // Request RF Explorer command to receive Current_Config from RFExplorer 3G+ IoT
    void requestConfig();

    //It will change current configuration for RF.
    //nStartFreqKHZ: Value of frequency span start in kHz (lower).
    //nEndFreqKHZ: Value of frequency span end in kHz (higher).
    //Internal Top and Bottom are set to 0dBm and -120dBm. This units are ignored by RFExplorer 3G+ IoT.
    void sendNewConfig(uint32_t nStartFreqKHZ, uint32_t nEndFreqKHZ);
     
    //Stop spectrum analyzer data dump.
    void setHold();
     
    //Start the device if it was previously stopped.
    void setRun();
     
    //Shutdown RF Explorer unit.
    void sleep();
     

    //Set serial values at buffer member class called m_CircularBuffer.
    //Refresh this buffer periodically to avoid losing information.
    //Returns: number of bytes read in last use.
    uint8_t updateBuffer();

    //Process messages of buffer member class.
    //This method stores information on objects m_objRFEConfiguration (last device configuration)
    //and m_objRFESweepData (last set of valid data).
    //Returns: 0 if is possible to process.
    //See ErrorCodes.h in other case.
    uint8_t processReceivedString();
     
    //Returns the type of message of the last process performed on processReceivedString()
    //Valid types of messages are constants like:
    //_CONFIG_MESSAGE / _SWEEP_MESSAGE / _MODEL_MESSAGE / _UNKNOWN_MESSAGE
    // _ERROR_MESSAGE / _SERIALNUMBER_MESSAGE
    uint8_t getLastMessage() const;

    //Function that determines whether the configuration and data from
    //the module has a correct format that allows the user to perform operations
    boolean isValid() const;
     
    //Obtain Max value of Array Data Amplitude and corresponding frequency.
    //*pFreqKHZ: Pointer that storage frequency at KHz of the maximum value.
    //*pMaxDBM: Pointer that storage max value in dBm.
    //Return 0 if it is success.
    uint8_t getPeak(uint32_t *pFreqKHZ, int16_t *pMaxDBM) const;
     
    //Read access to internally object.
    //Returns object system to use methods and properties of RFE_Configuration class.
    RFEConfiguration* getConfiguration();
     
    //Read access to internally object.
    //Returns object system to use methods and properties of RFE_SweepData class.
    RFESweepData* getSweepData();

    //Access to internal Software serial using for task debugging.
    //Different use between Arduino Due or Seeeduino
    //See in definition of object m_objDebugSerialPort   
    #if defined MONITOR_SERIAL
        #if defined (_SAM3XA_)
            HardwareSerial& getMonitorSerial() const;
        #else
            SoftwareSerial& getMonitorSerial() const;
        #endif
    #endif

};
#endif
