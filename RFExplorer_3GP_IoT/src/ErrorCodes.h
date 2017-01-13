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

#ifndef ERRORCODES_H_
#define ERRORCODES_H_

//------------------------ Error Codes----------------------------------------

#define _RFE_SUCCESS                        0
#define _RFE_ERROR                          1
#define _RFE_ERROR_UNEXPECTED_CHARACTER     3
#define _RFE_ERROR_UNEXPECTED_SIZE          4
#define _RFE_ERROR_STRING_OVERFLOW          5
#define _RFE_ERROR_CONFIG                   10
#define _RFE_ERROR_SWEEP_BAD_CONFIG         11
#define _RFE_ERROR_DEVICE_INCORRECT         12
#define _RFE_IGNORE                         20
#define _RFE_NOT_MESSAGE                    21

#endif