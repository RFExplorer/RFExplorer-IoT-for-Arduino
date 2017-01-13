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

#ifndef ByteBuffer_RFE_h
#define ByteBuffer_RFE_h

#ifdef _SAM3XA_
    #define m_obj3GPSerial Serial3
    #define BUFFER_CIRCULAR_SIZE 1024
#else
    #define m_obj3GPSerial Serial
    #define BUFFER_CIRCULAR_SIZE 256
#endif


class ByteBuffer_RFE
{
    private:
        
    byte data[BUFFER_CIRCULAR_SIZE];

    uint16_t m_nCapacity;
    uint16_t m_nPosition;
    uint16_t m_nLength;
    
    public:
    
    // Counter for all o bytes reading since start application
    uint16_t m_CounterBytes;
    
    ByteBuffer_RFE();

    // This method initializes the data store of the buffer to a certain size the buffer should NOT be used before this call is made
    void init();

    // This method resets the buffer into an original state (with no data)
    void clear();

    // Returns how much space is left in the buffer for more data
    uint16_t getSize();
    
    // Returns the maximum capacity of the buffer
    uint16_t getCapacity();

    // This method returns the byte that is located at index in the buffer but doesn't modify the buffer like the get methods (doesn't remove the retured byte from the buffer)
    byte peek(unsigned int index);

    // Put methods, either a regular put in back or put in front
    uint8_t putInFront(byte in);
    uint8_t put(byte in);

    // Get methods, either a regular get from front or from back. Even a active wait like getWait
    byte get();
    byte getFromBack();
    byte getWait();
    
};
#endif
