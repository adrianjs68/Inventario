///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Close Handle Class Implementation
//
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright 1987-2022 PARMAVEX SERVICES
//
// Licensed under the European Union Public Licence (EUPL), Version 1.1 or -
// as soon they will be approved by the European Commission - subsequent
// versions of the EUPL (the "Licence"). You may not use this work except in
// compliance with the Licence. You may obtain a copy of the Licence at:
//
// http://ec.europa.eu/idabc/eupl
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Licence is distributed on an "AS IS" basis,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the Licence for the specific language governing permissions and
// limitations under the Licence. This source code is free software. It
// must not be sold, leased, rented, sub-licensed or used for any form of
// monetary recompense whatsoever. This notice must not be removed or altered
// from this source distribution.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Remarks
///////////////////////////////////////////////////////////////////////////////////////////////////

// Convenience class to call CloseHandle when the class goes out of scope.

///////////////////////////////////////////////////////////////////////////////////////////////////
// Include Files
///////////////////////////////////////////////////////////////////////////////////////////////////

// 1. Own Interface
#include "PxsBase/Header Files/AutoCloseHandle.h"

// 2. C System Files

// 3. C++ System Files

// 4. Other Libraries

// 5. This Project

///////////////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
AutoCloseHandle::AutoCloseHandle()
                :m_hHandle( INVALID_HANDLE_VALUE )
{
}

// Parameter Constructor
AutoCloseHandle::AutoCloseHandle( HANDLE hHandle )
                :m_hHandle( hHandle )
{
}

// Copy constructor - not allowed so no implementation

// Destructor
AutoCloseHandle::~AutoCloseHandle()
{
    // Close the handle
    if ( m_hHandle && ( m_hHandle != INVALID_HANDLE_VALUE ) )
    {
        CloseHandle( m_hHandle );
        m_hHandle = INVALID_HANDLE_VALUE;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////////////////////////

// Assignment operator - not allowed so no implementation

///////////////////////////////////////////////////////////////////////////////////////////////////
// Public Methods
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Protected Methods
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Private Methods
///////////////////////////////////////////////////////////////////////////////////////////////////