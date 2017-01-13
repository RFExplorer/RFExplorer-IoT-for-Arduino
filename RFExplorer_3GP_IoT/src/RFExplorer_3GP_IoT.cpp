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

#include "RFExplorer_3GP_IoT.h"
#include "ByteBuffer_RFE.h"

#if !defined (_SAM3XA_)
    SoftwareSerial m_objDebugSP(11, 13);
#endif


void RFExplorer_3GP_IoT::LineBufferInit()
{
    m_pLine[0] = 0X00;
    m_nCharCounter = 0;
}

void RFExplorer_3GP_IoT::init()
{
    m_nCounterError = 0;
    m_nBaudrate = DEF_BAUD_RATE;

    memset(m_pLine,0x0,sizeof(m_pLine));
    m_nCharCounter = 0;
    m_CircularBuffer.init();
    m_obj3GPSerial.begin(DEF_BAUD_RATE);
        
    #ifdef MONITOR_SERIAL
        monitorSerial_Init();
    #endif

}

void RFExplorer_3GP_IoT::resetHardware() const
{
    pinMode(_RFE_RESET,OUTPUT);
    digitalWrite(_RFE_RESET,LOW);
    delay(DEF_COMMAND_RESET);
    digitalWrite(_RFE_RESET,HIGH);
}

void RFExplorer_3GP_IoT::requestConfig()
{
    sendCommand("#\x04" "C0");//#<Size>C0
}

void RFExplorer_3GP_IoT::changeBaudrate(uint32_t nbaudrate)
{
    switch (nbaudrate)
    {
        case 4800:
        sendCommand("#\x04" "c3");
        break;
        case 9600:
        sendCommand("#\x04" "c4");
        break;
        case 19200:
        sendCommand("#\x04" "c5");
        break;
        case 38400:
        sendCommand("#\x04" "c6");
        break;
        case 57600:
        sendCommand("#\x04" "c7");
        break;
        case 115200:
        sendCommand("#\x04" "c8");
        break;
        default:
        case DEF_BAUD_RATE: //2400
        sendCommand("#\x04" "c2");
        break;
    }
    //First send Command to RFExplorer 3G+ IoT, them change baud rate of local Arduino
    m_nBaudrate = nbaudrate;
    delay(50); //Worst case change 2400 bauds to other - 16,6 ms
    m_obj3GPSerial.begin(m_nBaudrate);
}

void RFExplorer_3GP_IoT::changeNumberSteps(uint16_t nSteps)
{
    switch (nSteps)
    {
        case 112:
        sendCommand("#\x05" "CP" "\x06");
        break;
        case 240:
        sendCommand("#\x05" "CP" "\x0E");
        break;
        default:
        case 512:
        sendCommand("#\x05" "CP" "\xFF");
        break;
    }  
}    

void RFExplorer_3GP_IoT::sendNewConfig(uint32_t nStartFreqKHZ, uint32_t nEndFreqKHZ)
{
    char pCommand[35]; //#<Size>C2-F:Sssssss,Eeeeeee,tttt,bbbb
    
    sprintf(pCommand,"# C2-F:%07lu,%07lu,000,-120", nStartFreqKHZ, nEndFreqKHZ);
    
    pCommand[1]=strlen(pCommand);
    sendCommand(pCommand);
}

void RFExplorer_3GP_IoT::setHold()
{
    sendCommand("#\x04" "CH"); //#<Size>CH
}

void RFExplorer_3GP_IoT::setRun()
{
    requestConfig();
}    
    
void RFExplorer_3GP_IoT::sleep()
{
    sendCommand("#\x03" "S");//#<Size>S    
}


void RFExplorer_3GP_IoT::sendCommand(const char* pCommand, int nLength)
{
    if (nLength == -1)
    {
        nLength=strlen(pCommand);
    }
    m_obj3GPSerial.write(pCommand,nLength);
}

uint8_t RFExplorer_3GP_IoT::updateBuffer()
{
    uint8_t nCounterBytesUpdate = 0;
    
    while (m_obj3GPSerial.available() > 0)
    {   //Storage all bytesForRead on m_CircularBuffer
        m_CircularBuffer.put(m_obj3GPSerial.read());
        nCounterBytesUpdate++;
    } 

    return nCounterBytesUpdate;
}    
    

uint8_t RFExplorer_3GP_IoT::processReceivedString_GetNextLine()
{

    uint8_t nReturnCode = _RFE_IGNORE;
    char cBufferRead = 0x00;
    
    if (m_nCharCounter > STRING_SIZE)
    {
        
        LineBufferInit();
        return _RFE_ERROR_STRING_OVERFLOW;
    }

    if(m_CircularBuffer.getSize() > 0)
    {
        if ( m_nCharCounter < 3) //Catch first header received characters
        {
            cBufferRead = m_CircularBuffer.get();
            if ((m_nCharCounter!=0) || ((m_nCharCounter==0) && (('#' == cBufferRead) || ('$' == cBufferRead))))
            {
                strcat(m_pLine, cBufferRead);
                m_nCharCounter++;
            }
        }
        if (m_nCharCounter >= 3)
        {
            if('#' == m_pLine[0]) //#xxxx
            {
                bool bContinue=(m_CircularBuffer.getSize()>0);
                if (bContinue)
                {
                    cBufferRead = m_CircularBuffer.get();
                }
                while (bContinue && ('\r' != cBufferRead) && (m_nCharCounter < STRING_SIZE))
                {
                    strcat(m_pLine,cBufferRead);
                    m_nCharCounter++;
                    if (m_CircularBuffer.getSize()==0)
                    {
                        bContinue = false;
                    }
                    else
                    {
                        cBufferRead = m_CircularBuffer.get();
                    }
                }
                if (bContinue && ('\r' == cBufferRead) && (m_nCharCounter<STRING_SIZE))
                {
                    if ('\n' == m_CircularBuffer.getWait())
                    {
                        nReturnCode = _RFE_SUCCESS;
                    }
                    else
                    {
                        nReturnCode = _RFE_ERROR_UNEXPECTED_CHARACTER; //Unexpected character
                        m_nCounterError++;
                    }
                }
            }
            else if('$' == m_pLine[0]) //$xxxx
            {
                if (('s' == m_pLine[1]) || ('S' == m_pLine[1]))
                {
                    uint16_t nMaxLengthMessage = m_pLine[2];
                    if (nMaxLengthMessage <= MAX_SPECTRUM_STEPS)
                    {
                        bool bContinue = (m_CircularBuffer.getSize() > 0);
                        if (bContinue)
                        {
                            cBufferRead = m_CircularBuffer.get();
                        }
                        while(bContinue && ('\r' != cBufferRead) && (m_nCharCounter < STRING_SIZE))
                        {
                            strcat(m_pLine,cBufferRead);
                            m_nCharCounter++;
                            if (m_CircularBuffer.getSize() == 0)
                            {
                                bContinue = false;
                            }
                            else
                            {
                                cBufferRead = m_CircularBuffer.get();
                            }
                        }
                        if (bContinue && ('\r' == cBufferRead) && (m_nCharCounter<STRING_SIZE))
                        {
                            if ('\n' == m_CircularBuffer.getWait())
                            {
                                nReturnCode = _RFE_SUCCESS;
                            }
                            else
                            {
                                nReturnCode = _RFE_ERROR_UNEXPECTED_CHARACTER; //Unexpected character
                                m_nCounterError++;

                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        nReturnCode = _RFE_IGNORE;
    }

    return nReturnCode;
}

uint8_t RFExplorer_3GP_IoT::processReceivedString()
{
    uint8_t nReturnCode = processReceivedString_GetNextLine();

    if (nReturnCode == _RFE_SUCCESS)
    {
        m_nLastMessage = _UNKNOWN_MESSAGE;
        if (m_objRFEConfiguration.processReceivedString(m_pLine, &m_nLastMessage) == _RFE_SUCCESS) //Is object Config
        {
			nReturnCode = _RFE_SUCCESS; 
            
			if (m_nLastMessage == _CONFIG_MESSAGE)
			{
				m_objRFESweepData.setValidSweep(false);       
				if (m_objRFEConfiguration.isValidConfig() == true)
				{
						//Update Configuration						
						m_objRFESweepData.setStartFrequencyKHZ(m_objRFEConfiguration.getStartKHZ());
						m_objRFESweepData.setStepFrequencyHZ(m_objRFEConfiguration.getStepHZ());
						m_objRFESweepData.setTotalSteps(m_objRFEConfiguration.getFreqSpectrumSteps());
				}
				else
				{
					nReturnCode = _RFE_ERROR_CONFIG; //Configuration Incorrect
					m_nCounterError++;
					requestConfig();
				}
			}
        }
        else if (m_objRFESweepData.processReceivedString(m_pLine, &m_nLastMessage) == _RFE_SUCCESS)
        {
            if (m_objRFEConfiguration.isValidConfig() == true)
            {
                nReturnCode = _RFE_SUCCESS;
            }
            else
            {
                nReturnCode = _RFE_ERROR_SWEEP_BAD_CONFIG; //Sweep is incorrect because Config previous is incorrect
                m_nCounterError++;
            }
        }
        else
        {
            nReturnCode = _RFE_NOT_MESSAGE;
        }

        LineBufferInit();
    }


    return nReturnCode;
}

uint8_t RFExplorer_3GP_IoT::getLastMessage() const
{
    return(m_nLastMessage);
}

boolean RFExplorer_3GP_IoT::isValid() const 
{    
    return (m_objRFEConfiguration.isValidConfig() && m_objRFESweepData.isValidSweep());
}

uint8_t RFExplorer_3GP_IoT::getPeak(uint32_t *pFreqKHZ, int16_t *pMaxDBM) const
{
    uint8_t nReturnCode = _RFE_SUCCESS;//Hard coded - Wait for specification

    uint16_t nStepMax = m_objRFESweepData.getPeakStep();
    *pFreqKHZ = m_objRFESweepData.getFrequencyKHZ(nStepMax);
    *pMaxDBM = m_objRFESweepData.getAmplitudeDBM(nStepMax);

    return nReturnCode;
}

RFEConfiguration* RFExplorer_3GP_IoT::getConfiguration()
{
    return (&m_objRFEConfiguration);
}

RFESweepData* RFExplorer_3GP_IoT::getSweepData()
{
    return (&m_objRFESweepData);
}

#if defined MONITOR_SERIAL
    #if defined (_SAM3XA_)
        HardwareSerial& RFExplorer_3GP_IoT::getMonitorSerial() const
        {
            return m_objMonitorSerial;
        }
    #else
        SoftwareSerial& RFExplorer_3GP_IoT::getMonitorSerial() const
        {
            return m_objMonitorSerial;
        }
    #endif
#endif

#ifdef MONITOR_SERIAL
    void RFExplorer_3GP_IoT::monitorSerial_Init()
    {

        m_objMonitorSerial.begin(DEBUG_BAUD_RATE);
    
        m_objMonitorSerial.println(F(_HEADER_LIB_3GP));
        m_objMonitorSerial.println(F(_VERSION_LIB_3GP));
    }

#endif

