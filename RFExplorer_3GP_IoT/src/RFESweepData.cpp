//============================================================================
//RF Explorer 3G+ IoT for Arduino - A Spectrum Analyzer for everyone!
//Copyright © 2010-17 Ariel Rocholl, www.rf-explorer.com
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

#include "RFESweepData.h"

uint32_t RFESweepData::getFrequencyKHZ(uint16_t nStep) const
{
    if (nStep <= m_nTotalSteps)
    {
        return (m_nStartFrequencyKHZ + ((m_nStepFrequencyHZ * nStep)/1000));
    }
    else
    {
        return 0;
    }        
}

boolean RFESweepData::isValidSweep() const
{
    return m_bValid;
}

void  RFESweepData::setValidSweep(boolean bValid)
{
    m_bValid = bValid;
}

uint8_t RFESweepData::processReceivedString(char* pLine, uint8_t* pLastMessage)
{
    uint8_t nReturnCode = _RFE_SUCCESS;

    uint16_t nSizeString = strlen(pLine);

    if (nSizeString >= 2) 
    {
        if ((StartWith(pLine,"$s") && (nSizeString - 3 == (pLine[2] * 16))) || (StartWith(pLine,"$S") && (nSizeString - 3 == pLine[2])))
        {
            memcpy(m_arrAmplitude, pLine + 3, m_nTotalSteps);
            m_bValid = true;
            *pLastMessage = _SWEEP_MESSAGE;
        }
        else
        {
            nReturnCode = _RFE_ERROR_UNEXPECTED_SIZE;
            m_bValid = false;
        }
    }
    else
    {
        nReturnCode = _RFE_IGNORE;
        m_bValid = false;
    }

    return nReturnCode;
}

int16_t RFESweepData::getAmplitudeDBM(uint16_t nStep) const
{
    if (nStep < m_nTotalSteps)
    {
        return ((int16_t)(m_arrAmplitude[nStep] + 1)/-2);
    }
    else
    {
        return MIN_AMPLITUDE_DBM;
    }        
}

void RFESweepData::setAmplitudeDBM(uint16_t nStep, int8_t nDBM)
{
    if (nStep < m_nTotalSteps)
    {
        m_arrAmplitude[nStep] = nDBM;
    }        
}

uint16_t RFESweepData::getPeakStep() const
{
    uint16_t nStep = 0;
    byte nPeak = 255;
    
    for (uint16_t nInd = 0; nInd < m_nTotalSteps; nInd++)
    {
        if (nPeak > m_arrAmplitude[nInd])
        {
            nPeak = m_arrAmplitude[nInd];
            nStep = nInd;
        }
    }
    
    return nStep;
}
