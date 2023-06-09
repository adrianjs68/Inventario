///////////////////////////////////////////////////////////////////////////////////////////////////
//
// BSTR String Class Implementation
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Include Files
///////////////////////////////////////////////////////////////////////////////////////////////////

// 1. Own Interface
#include "PxsBase/Header Files/BStr.h"

// 2. C System Files

// 3. C++ System Files

// 4. Other Libraries

// 5. This Project
#include "PxsBase/Header Files/AllocateWChars.h"
#include "PxsBase/Header Files/Formatter.h"
#include "PxsBase/Header Files/FunctionException.h"
#include "PxsBase/Header Files/MemoryException.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
BStr::BStr()
     :m_BinaryString( nullptr )
{
}

// Copy constructor - not allowed so no implementation

// Destructor
BStr::~BStr()
{
    if ( m_BinaryString )
    {
        SysFreeString( m_BinaryString );    // OK to pass in NULL
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////////////////////////

// Assignment operator - not allowed so no implementation

///////////////////////////////////////////////////////////////////////////////////////////////////
// Public Methods
///////////////////////////////////////////////////////////////////////////////////////////////////

//===============================================================================================//
//  Description:
//      Allocate a BSTR string with SysAllocString
//
//  Parameters:
//      Text - the input string
//
//  Returns:
//      void
//===============================================================================================//
void BStr::Allocate( const String& Text )
{
    // Only allow one-shot usage
    if ( m_BinaryString )
    {
        throw FunctionException( L"m_BinaryString", __FUNCTION__ );
    }

    if ( Text.IsNull() )
    {
        return;     // Nothing to do
    }

    m_BinaryString = SysAllocString( Text.c_str() );
    if ( m_BinaryString == nullptr )
    {
        throw MemoryException( __FUNCTION__ );
    }
}

//===============================================================================================//
//  Description:
//      Return a pointer to the BSTR string
//
//  Parameters:
//      None
//
//  Returns:
//      pointer to the string
//===============================================================================================//
BSTR BStr::b_str()
{
    return m_BinaryString;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Protected Methods
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Private Methods
///////////////////////////////////////////////////////////////////////////////////////////////////
