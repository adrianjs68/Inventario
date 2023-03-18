///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Text Area Class Implementation
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
#include "PxsBase/Header Files/TextArea.h"

// 2. C System Files

// 3. C++ System Files

// 4. Other Libraries

// 5. This Project

///////////////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
TextArea::TextArea()
         :TextField()
{
    // Creation parameters
    m_CreateStruct.cy     = 200;
    m_CreateStruct.cx     = 200;
    m_CreateStruct.style |= ES_MULTILINE;
}

// Copy constructor - not allowed so no implementation

// Destructor
TextArea::~TextArea()
{
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
//      Set the left margin
//
//  Parameters:
//      leftMargin  - the left margin in pixels
//      rightMargin - the right margin in pixels
//
//  Remarks:
//      Window must have been created
//
//  Returns:
//      void
//===============================================================================================//
void TextArea::SetMargins( WORD leftMargin, WORD rightMargin )
{
    if ( m_hWindow == nullptr )
    {
        throw FunctionException( L"m_hWindow", __FUNCTION__ );
    }
    SendMessage( m_hWindow,
                 EM_SETMARGINS,
                 EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM( leftMargin, rightMargin ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Protected Methods
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Private Methods
///////////////////////////////////////////////////////////////////////////////////////////////////
