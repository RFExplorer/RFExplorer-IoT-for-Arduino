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

//------------------------- Includes-------------------------------------------

#include "RFEUtilities.h"
#include "CommonValues.h"

#ifndef RFESWEEPDATA_H_
#define RFESWEEPDATA_H_

//------------------------- Class----------------------------------------------

class RFESweepData
{
    private:
    //Start frequency
    uint32_t m_nStartFrequencyKHZ;    
    //Step frequency between each sweep step
    uint32_t m_nStepFrequencyHZ;
    //Total number of sweep steps captured
    uint16_t m_nTotalSteps;
    //The actual data container, a consecutive set of dBm amplitude values
    byte m_arrAmplitude[MAX_SPECTRUM_STEPS];
    //Variable defining whether object RFESweepData contains valid data for processing
    boolean m_bValid;

    public:
    RFESweepData()
    {
        m_nTotalSteps = MAX_SPECTRUM_STEPS;
        m_bValid = false;
        memset(m_arrAmplitude,0x0,sizeof(m_arrAmplitude));
    }

    //obtain current frequency of select step on current configuration
    uint32_t getFrequencyKHZ(uint16_t nStep) const;

    //Check if is possible process current SweepData
    boolean isValidSweep() const;
    
    //Set externally when Sweep DAtas are valid
    void setValidSweep(boolean bValid);

    //Process messages of serial buffer
    uint8_t processReceivedString(char* pLine, uint8_t* pLastMessage);

    //Returns amplitude data in dBm. This is the value as it was read from the device or from a file
    //nStep: Internal frequency step or bucket to read data from
    //Returns: Value in dBm
    int16_t getAmplitudeDBM(uint16_t nStep) const;

    // Storage amplitude data in dBm.
    //nStep: Number of step
    //nDBM: Value in dBm for that step of frequency
    void setAmplitudeDBM(uint16_t nStep, int8_t nDBM);
    
    //Returns the step of the highest amplitude value found
    uint16_t getPeakStep() const;

//------------------------- Access to private variable members-----------------

    //Get/Set value of frequency span start (lower)
    uint32_t getStartFrequencyKHZ() const { return m_nStartFrequencyKHZ; }      
    void setStartFrequencyKHZ(uint32_t nStartFrequencyKHZ) { m_nStartFrequencyKHZ = nStartFrequencyKHZ; }
     
    //Get Value of frequency span end (higher)  
    uint32_t getEndFrequencyKHZ() const { return (getFrequencyKHZ((uint32_t)(m_nTotalSteps - 1))); }
        
    //Get/Set Value of frequency of sample step  
    uint32_t getStepFrequencyHZ() const { return m_nStepFrequencyHZ; }
    void setStepFrequencyHZ(uint32_t nStepFrequencyHZ) { m_nStepFrequencyHZ = nStepFrequencyHZ; }
        
    //Get/Set Number of sweep steps with values which follows
    uint16_t getTotalSteps() const { return m_nTotalSteps;}
    void setTotalSteps(uint16_t nTotalSteps) { m_nTotalSteps = nTotalSteps; }
};

#endif