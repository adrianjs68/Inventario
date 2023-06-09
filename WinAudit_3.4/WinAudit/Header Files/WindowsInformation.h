///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Windows Information Class Header
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

#ifndef WINAUDIT_WINDOWS_INFORMATION_H_
#define WINAUDIT_WINDOWS_INFORMATION_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// Remarks
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////

// 1. Own Interface
#include "WinAudit/Header Files/WinAudit.h"

// 2. C System Files

// 3. C++ System Files

// 4. Other Libraries
#include "PxsBase/Header Files/SystemInformation.h"

// 5. This Project

// 6. Forwards

///////////////////////////////////////////////////////////////////////////////////////////////////
// Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

class WindowsInformation : public SystemInformation
{
    public:
        // Default constructor
        WindowsInformation();

        // Copy constructor
        WindowsInformation( const WindowsInformation& oWindows );

        // Destructor
        ~WindowsInformation();

        // Assignment operator
        WindowsInformation& operator= ( const WindowsInformation& oWindows );

        // Methods
        void GetCSDVersion( String* pCSDVersion ) const;
 static void GetDirectXVersion( String* pDirectXVersion );
 static void GetDirectXVersionXP( String* pDirectXVersion );
 static void GetFirmwareDataNT6( String* pFirmwareDataNT6 );
 static void GetFirmwareDataXP( String* pFirmwareDataXP );
 static void GetLanguage( String* pLanguage );
        void GetName( String* pName ) const;
 static void GetRegistrationDetails( String* pRegisteredOwner,
                                     String* pRegisteredOrgName,
                                     String* pProductID,
                                     String* pPlusVersionNumber, String* pInstallDate );
        void GetWin10OSBuild( String* pWin10OSBuild ) const;
        void GetWin10Version( String* pWin10Version ) const;
        bool IsWin2003() const;
        bool IsWindows10() const;
        bool IsWinVistaOrNewer() const;
        bool IsWinXP() const;
        bool IsWinXPorNewer() const;
 static void LanguageIDToName( WORD langID, String* pName );

    protected:
        // Methods

        // Data members

    private:
        // Methods
        bool    IsMajorMinor( DWORD major, DWORD minor ) const;

        // Data members
};

#endif  // WINAUDIT_WINDOWS_INFORMATION_H_
