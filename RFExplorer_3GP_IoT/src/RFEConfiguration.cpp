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

#include "RFEConfiguration.h"

boolean RFEConfiguration::isValidConfig() const
{
    return m_bValid;
}

uint8_t RFEConfiguration::processReceivedString(char* pLine, uint8_t* pLastMessage)
{
    uint8_t nReturnCode = _RFE_SUCCESS;

    uint16_t nSizeString = strlen(pLine);

    if ((nSizeString >= 6) && StartWith(pLine,"#C2-F:")) //Received Configuration of Analyzer
    {
         *pLastMessage = _CONFIG_MESSAGE;
        if (nSizeString >= 65)
        {
            pLine+=6;
            pLine[7]=0x00;
            m_nStartKHZ=atol(pLine);
            pLine+=8;
            pLine[7]=0x00;
            m_nStepHZ=atol(pLine);
            pLine+=18;
            pLine[4]=0x00;
            m_nFreqSpectrumSteps=atol(pLine);
            pLine+=5;
            pLine[1]=0x00;
            m_bExpansionBoardActive=atoi(pLine);//((*pLine)[37] == '1');
            pLine+=2;
            pLine[3]=0x00;
            m_eModeRFE = (eMode) atoi(pLine);
            pLine+=4;
            pLine[7]=0x00;
            m_nMinFreqKHZ = atol(pLine);
            pLine+=8;
            pLine[7]=0x00;
            m_nMaxFreqKHZ = atol(pLine);
            pLine+=8;
            pLine[7]=0x00;
            m_nMaxSpanKHZ = atol(pLine);

            m_bValid = true;     
          
            if (nSizeString >= 66)
            {
                pLine+=8;
                pLine[5]=0x00;
                m_nRBWKHZ = atol(pLine);
            }
            if (nSizeString >= 73)
            {
                pLine+=6;
                pLine[4]=0x00;
                m_nOffset_dB = atol(pLine);
            }
            if (nSizeString >= 78)
            {
                pLine+=5;
                pLine[3]=0x00;
                m_eCalculatorRFE = (eCalculator)atoi(pLine);
            }
        }
        else
        {
            m_bValid = false;
        }
    }
    else if ((nSizeString > 5) && StartWith(pLine,"#C2-M:")) //Received Model + Firmware
    {
        if (nSizeString >= 13)
        {
            pLine+=6;
            pLine[3]=0x00;
            m_eMainBoardModel=(eModel)atoi(pLine);
            if (m_eMainBoardModel != MODEL_WSUB3G)
                nReturnCode = _RFE_ERROR_DEVICE_INCORRECT;
            pLine+=8;
            strncpy(m_sRFExplorerFirmware, pLine, 10);
            m_sRFExplorerFirmware[10] = 0x00;
            *pLastMessage = _MODEL_MESSAGE;
        }
        else
            *pLastMessage=_ERROR_MESSAGE;
    }
    else if ((nSizeString > 2) && StartWith(pLine,"#Sn")) //Received Serial Number
    {
        if (nSizeString >= 18)
        {
            pLine+=3;
            pLine[16]=0x00;
            strncpy(m_sRFExplorerSerialNumber, pLine, 16);
            m_sRFExplorerSerialNumber[16]=0x00;
            *pLastMessage = _SERIALNUMBER_MESSAGE;
        } 
        else
            *pLastMessage=_ERROR_MESSAGE;  
    } 
    else
    {
        nReturnCode = _RFE_IGNORE; 
    }

    return nReturnCode;
}

