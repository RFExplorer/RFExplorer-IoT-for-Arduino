//============================================================================
//RF Explorer 3G+ IoT for Arduino - A Spectrum Analyzer for everyone!
//Copyright � 2010-17 Ariel Rocholl, www.rf-explorer.com
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

#include "ErrorCodes.h"

#ifndef COMMONVALUES_H_
#define COMMONVALUES_H_

//------------------------- Common Values--------------------------------------

//brief default time to wait after sending command reset
#define DEF_COMMAND_RESET           1000
//brief default baudrate
#define DEF_BAUD_RATE               2400
#define DEBUG_BAUD_RATE             57600

#define MIN_AMPLITUDE_DBM          -120
#define MAX_AMPLITUDE_DBM           00

#define _MIN_FREQUENCY_KHZ           15000
#define _MAX_FREQUENCY_KHZ           2700000

//------------------------- Array Length - This depends on board used----------

#if defined(_SAM3XA_)
    //Max number of steps in Spectrum Analyzer
    #define MAX_SPECTRUM_STEPS          512
    //Max size of String that storage complete message received
    #define STRING_SIZE                 520
#else
    //Max number of steps in Spectrum Analyzer
    #define MAX_SPECTRUM_STEPS          240 
    //Max size of String that storage complete message received
    #define STRING_SIZE                 248 
#endif

//------------------------- 3G+ IoT connections with Arduino -----------------------------

#define	_RFE_RESET                  A4
#define	_RFE_BUZZER                 A5
#define	_RFE_GPIO0                  13
#define	_RFE_GPIO1                  11
#define	_RFE_GPIO2                  A0
#define	_RFE_GPIO4                  A1
#define	_RFE_GPIO5                  A2
#define	_RFE_GPIO6                  12
#define	_RFE_GPIO7                  10
#define	_RFE_RFGPIO0                 6
#define	_RFE_RFGPIO1                 4
#define	_RFE_RFGPIO2                 5


//------------------------- Messages - See getLastMessage()--------------------

#define	_CONFIG_MESSAGE               1
#define	_SWEEP_MESSAGE                2
#define	_MODEL_MESSAGE                3
#define	_UNKNOWN_MESSAGE              4
#define	_ERROR_MESSAGE                5
#define	_SERIALNUMBER_MESSAGE         6

#endif
