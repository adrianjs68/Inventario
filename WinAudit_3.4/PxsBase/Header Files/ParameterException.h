///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Invalid Function/Method Parameter Exception Class Header
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

#ifndef PXSBASE_PARAMETER_EXCEPTION_H_
#define PXSBASE_PARAMETER_EXCEPTION_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// Remarks
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////

// 1. Own Interface
#include "PxsBase/Header Files/PxsBase.h"

// 2. C System Files

// 3. C++ System Files

// 4. Other Libraries
#include "PxsBase/Header Files/Exception.h"

// 5. This Project

// 6. Forwards

///////////////////////////////////////////////////////////////////////////////////////////////////
// Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

class ParameterException : public Exception
{
    public:
        // Default constructor
        ParameterException();

        // Constructor with additional details and the throwing method/function
        ParameterException( LPCWSTR pszDetails, const char* pszFunction );

        // Copy constructor
        ParameterException( const ParameterException& oParameter );

        // Destructor
        ~ParameterException();

        // Assignment operator
        ParameterException& operator= (  const ParameterException& oParameter );

        // Methods

    protected:
        // Method

        // Data members

    private:
        // Method

        // Data members
};

#endif  // PXSBASE_PARAMETER_EXCEPTION_H_
