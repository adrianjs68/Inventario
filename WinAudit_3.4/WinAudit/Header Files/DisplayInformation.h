///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Display/Monitor Information Class Header
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

#ifndef WINAUDIT_DISPLAY_INFORMATION_H_
#define WINAUDIT_DISPLAY_INFORMATION_H_

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

// 5. This Project

// 6. Forwards
class AuditRecord;
template< class T > class TArray;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

class DisplayInformation
{
    public:
        // Default constructor
        DisplayInformation();

        // Destructor
        ~DisplayInformation();

        // Methods
 static void GetAdapterRecords( TArray< AuditRecord >* pRecords );
 static void GetDescriptionString( String* pDescriptionString );
 static void GetDiagnostics( String* pDiagnostics );
 static void GetIdentificationRecords( TArray< AuditRecord >* pRecords );
 static void ReadEdidHexFile( const String& FilePath, BYTE* pEdid, size_t bufferSize );

    protected:
        // Methods

        // Data members

    private:
        // Copy constructor - not allowed
        DisplayInformation( const DisplayInformation& oDisplay );

        // Assignment operator - not allowed
        DisplayInformation& operator= ( const DisplayInformation& oDisplay );

        // Methods
 static void GetDesktopCapabilityRecords( DWORD monitorNumber, TArray< AuditRecord >* pRecords );
 static void GetDesktopMonitorIDs( StringArray* pMonitorIDs, StringArray* pDeviceNames );
 static void GetDisplayRecord( size_t monitorNumber,
                               const String& MonitorID,
                               const String& DeviceName, AuditRecord* pRecord );
 static void GetPrimaryMonitorRecord( AuditRecord* pRecord );
 static void GetProductIdSizeString( String* pProductIdSizeString );
 static void GetResolutionString( String* pResolutionString );
 static bool IsValidEdidHeader( const BYTE* pEdid, size_t bufferSize );
 static bool ReadEdidDataBlock( const String& MonitorID,
                                BYTE* pEdid, size_t bufferSize, String* pDeviceDesc );
 static void TranslateEdidData( const BYTE*   pEdid, size_t  bufferSize,
                                bool*   pValidHeader, bool*   pValidCheckSum,
                                String* pVersionRev , String* pManufacturerID,
                                String* pProductID  , String* pSerialNumber,
                                String* pManufacDate, String* pDigDisplay,
                                String* pDisplaySize, String* pGamma,
                                String* pDisplayType, String* pMonitorSerial,
                                String* pTextData   , String* pMonitorName,
                                String* pFeatures   , String* pEstTimings,
                                String* pStdTimings , String* pHexData );
 static void TranslateEdidDescriptorBlock( const BYTE* pEdid,
                                           size_t  bufferSize,
                                           String* pMonitorSerial,
                                           String* pTextData, String* pMonitorName );
 static void TranslateIdManufacturerCode( const String& PnPID, String* pManufacturerName );
        // Data members
};

#endif  // WINAUDIT_DISPLAY_INFORMATION_H_
