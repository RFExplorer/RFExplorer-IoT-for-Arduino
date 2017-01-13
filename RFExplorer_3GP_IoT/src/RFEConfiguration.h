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

//------------------------- Includes------------------------------------------

#include "RFEUtilities.h"
#include "CommonValues.h"

#ifndef RFECONFIGURATION_H_
#define RFECONFIGURATION_H_
//------------------------- Enumerations---------------------------------------

enum eModel
{
    MODEL_433 = 0,  //0
    MODEL_868,      //1
    MODEL_915,      //2
    MODEL_WSUB1G,   //3
    MODEL_2400,     //4
    MODEL_WSUB3G,   //5
    MODEL_6G,       //6

    MODEL_RFGEN=60, //60
    MODEL_NONE = 0xFF //0xFF
};

enum eMode
{
    MODE_SPECTRUM_ANALYZER = 0,
    MODE_TRANSMITTER = 1,
    MODE_WIFI_ANALYZER = 2,
    MODE_TRACKING = 5,
    MODE_SNIFFER = 6,
    MODE_NONE = 0xFF
};

enum eCalculator
{
    NORMAL = 0,
    MAX = 1,
    AVG = 2,
    OVERWRITE = 3,
    MAX_HOLD = 4,
    MAX_HISTORICAL = 5,
    UNKNOWN = 0xff
};

//------------------------- Class----------------------------------------------

class RFEConfiguration
{
    private:
    //Value of frequency span start (lower)
    uint32_t m_nStartKHZ;
    //Value of frequency of sample step
    uint32_t m_nStepHZ;
    //Number of sweep steps with values which follows
    uint16_t m_nFreqSpectrumSteps;
    //1 if the Expansion module is the active circuit, 0 otherwise (the main board is active)
    bool m_bExpansionBoardActive;
    //Codified values are SPECTRUM_ANALYZER:0, RF_GENERATOR:1, WIFI_ANALYZER:2, UNKNOWN:255
    eMode m_eModeRFE;
    //Min supported frequency value for the selected RF circuit and mode
    uint32_t m_nMinFreqKHZ;
    //Max supported frequency value for the selected RF circuit and mode
    uint32_t m_nMaxFreqKHZ;
    //Max supported Span value for the selected RF circuit and mode
    uint32_t m_nMaxSpanKHZ;
    //Resolution Bandwidth currently selected
    uint32_t m_nRBWKHZ;
    //OffsetDB manually specified in the unit, including sign
    uint16_t m_nOffset_dB;
    //Mode use to process results 0=NORMAL, 1=MAX, 2=AVG, 3=OVERWRITE, 4=MAX_HOLD
    eCalculator m_eCalculatorRFE; 

    //Codified values for devices RF Explorer are 433M:0, 868M:1, 915M:2, WSUB1G:3, 2.4G:4, WSUB3G:5
    eModel m_eMainBoardModel;
    //Standard format xx.yy, may change format for betas or custom versions
    char m_sRFExplorerFirmware[11];
    
    //Serial Number of devices RF Explorer
    char m_sRFExplorerSerialNumber[17];
    

    //Variable defining whether object RFEConfiguration contains valid data received for processing
    boolean m_bValid;
    

    public:
    RFEConfiguration()
    {
        m_nStartKHZ = 0;
        m_nStepHZ = 0;
        m_nFreqSpectrumSteps = 0;
        m_bExpansionBoardActive = false;
        m_eModeRFE = MODE_NONE;
        m_nMinFreqKHZ = 0;
        m_nMaxFreqKHZ = 0;
        m_nMaxSpanKHZ = 0;
        m_nRBWKHZ = 0;
        m_nOffset_dB = 0;
        m_eCalculatorRFE = UNKNOWN;
        
        m_eMainBoardModel = MODEL_NONE;
        m_sRFExplorerFirmware[0] = 0x00;        
        m_sRFExplorerSerialNumber[0] = 0x00;
        
        
        m_bValid = false;
    }

    //Check Valid Configuration
    boolean isValidConfig() const;

    //Process messages of serial buffer
    //Returns: 0 if is possible to process
    uint8_t processReceivedString(char* pLine, uint8_t* pLastMessage);
    
//------------------------- Access to private variable members-----------------

    //Get value of frequency span start (lower)
    uint32_t getStartKHZ() const { return m_nStartKHZ; }
    
    //Get Value of frequency span end (higher)  
    uint32_t getEndKHZ() const { return (m_nStartKHZ + ((m_nStepHZ * m_nFreqSpectrumSteps)/1000)); }
    
    //Get Value of frequency of sample step
    uint32_t getStepHZ() const { return m_nStepHZ; }
        
    //Get Number of sweep steps with values which follows
    uint16_t getFreqSpectrumSteps() const { return m_nFreqSpectrumSteps; }
        
    //Get 1 if the Expansion module is the active circuit, 0 otherwise (the main board is active)    
    bool getExpansionBoardActive() const { return m_bExpansionBoardActive; }
    
    //Get Mode of codified values are SPECTRUM_ANALYZER:0, RF_GENERATOR:1, WIFI_ANALYZER:2, UNKNOWN:255 
    eMode getModeRFE() const { return m_eModeRFE; }
        
    //Get Min supported frequency value for the selected RF circuit and mode
    uint32_t getMinFreqKHZ() const { return m_nMinFreqKHZ; }
        
    //Get Max supported frequency value for the selected RF circuit and mode
    uint32_t getMaxFreqKHZ() const { return m_nMaxFreqKHZ; }
        
    //Get Resolution Bandwidth currently selected  
    uint32_t getRBWKHZ() const { return m_nRBWKHZ; }
        
    //Get Max supported Span value for the selected RF circuit and mode
    uint32_t getMaxSpanKHZ() const { return m_nMaxSpanKHZ; }
    
    //Get OffsetDB manually specified in the unit, including sign    
    uint16_t getOffset_dB() const { return m_nOffset_dB; }
        
    //Get Mode use to process results 0=NORMAL, 1=MAX, 2=AVG, 3=OVERWRITE, 4=MAX_HOLD
    eCalculator getCalculatorRFE() const { return m_eCalculatorRFE; }
    
    //Get Codified values for devices RF Explorer are 433M:0, 868M:1, 915M:2, WSUB1G:3, 2.4G:4, WSUB3G:5
    eModel getMainBoardModel() const { return m_eMainBoardModel; }
    
    //Get Standard Version Firmware format xx.yy, may change format for betas or custom versions
    const char* getRFExplorerFirmware() const { return (m_sRFExplorerFirmware); }
    
    //Get Serial Number of devices RF Explorer   
    const char* getRFExplorerSerialNumber() const { return (m_sRFExplorerSerialNumber); }
    
};

#endif
