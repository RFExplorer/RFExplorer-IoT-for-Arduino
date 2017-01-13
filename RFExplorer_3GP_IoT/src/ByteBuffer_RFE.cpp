//============================================================================
//RF Explorer 3G+ IoT for Arduino - A Spectrum Analyzer for everyone!
//Copyright © 2010-17 Ariel Rocholl, www.rf-explorer.com
//
//Arduino Buffered Serial
//A library that helps establish buffered serial communication with a
//host application.
//Copyright (C) 2010 Sigurður Örn Aðalgeirsson (siggi@media.mit.edu)
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

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "RFExplorer_3GP_IoT.h"
#include "ByteBuffer_RFE.h"

ByteBuffer_RFE::ByteBuffer_RFE()
{

}

void ByteBuffer_RFE::init()
{
    m_nCapacity = BUFFER_CIRCULAR_SIZE;
    m_nPosition = 0;
    m_nLength = 0;
    m_CounterBytes = 0;
}

void ByteBuffer_RFE::clear()
{
    m_nPosition = 0;
    m_nLength = 0;
}

uint16_t ByteBuffer_RFE::getSize()
{
    return m_nLength;
}

uint16_t ByteBuffer_RFE::getCapacity()
{
    return m_nCapacity;
}

byte ByteBuffer_RFE::peek(unsigned int index)
{
    byte b = data[(m_nPosition+index)%m_nCapacity];
    return b;
}

uint8_t ByteBuffer_RFE::put(byte in)
{
    if(m_nLength < m_nCapacity)
    {
        // save data byte at end of buffer
        data[(m_nPosition+m_nLength) % m_nCapacity] = in;
        // increment the length
        m_nLength++;
        m_CounterBytes++;
        return 1;
    }
    // return failure
    return 0;
}

uint8_t ByteBuffer_RFE::putInFront(byte in)
{
    if(m_nLength < m_nCapacity)
    {
        // save data byte at end of buffer
        if( m_nPosition == 0 )
            m_nPosition = m_nCapacity-1;
        else
            m_nPosition = (m_nPosition-1)%m_nCapacity;
        data[m_nPosition] = in;
        // increment the length
        m_nLength++;
        m_CounterBytes++;
        return 1;
    }
    // return failure
    return 0;
}

byte ByteBuffer_RFE::get()
{
    byte b = 0;

    if(m_nLength > 0)
    {
        b = data[m_nPosition];
        // move index down and decrement length
        m_nPosition = (m_nPosition+1)%m_nCapacity;
        m_nLength--;
    }

    return b;
}

byte ByteBuffer_RFE::getFromBack()
{
    byte b = 0;
    if(m_nLength > 0)
    {
        b = data[(m_nPosition+m_nLength-1)%m_nCapacity];
        m_nLength--;
    }

    return b;
}

byte ByteBuffer_RFE::getWait()
{
    while (getSize() == 0)
    {
        while (m_obj3GPSerial.available()>0)
        {   //Storage all bytesForRead
            put(m_obj3GPSerial.read());
        }
    }
    byte b = get();
    
    return b;
}

