///////////////////////////////////////////////////////////////////////////////////////////////////
//
// System Management BIOS Information Class Header
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

#ifndef WINAUDIT_SMBIOS_INFORMATION_H_
#define WINAUDIT_SMBIOS_INFORMATION_H_

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

class SmbiosInformation
{
    public:
        // Default constructor
        SmbiosInformation();

        // Destructor
        ~SmbiosInformation();

        // Methods
        void  GetAuditRecords( BYTE structureType, TArray< AuditRecord >* pRecords );
        void  GetChassisAssetTag( String* pChassisAssetTag ) const;
 static void  GetItemName( DWORD itemID, String* pItemName );
        void  GetProductName( String* pProductName ) const;
        bool  GetSmbiosRecord( BYTE structureType,
                               WORD structureNumber, AuditRecord* pRecord ) const;
        void  GetSmbiosMajorDotMinor( String* pMajorDotMinor ) const;
        bool  GetStructureOffset( BYTE structureType,
                                  WORD structureNumber, WORD* pStructureOffset ) const;
        void  GetSystemManufacturer( String* pSystemManufacturer ) const;
        void  GetSystemSerialNumber( String* pSystemSerialNumber ) const;
        void  GetSystemUUID( String* pSystemUUID ) const;
        DWORD GetTotalRamMB() const;
        void  ReadSmbiosData();
        void  ReadSmbiosDataFromFile();

    protected:
        // Methods

        // Data members

    private:
        typedef struct  _TYPE_SMBIOS_VALUE
        {
            bool    boolean;
            BYTE    byte;
            WORD    word;
            DWORD   dword;
            QWORD   qword;
            char    szString[ 260 ];  // SMBIOS data is stored as ASCII strings
        }
        TYPE_SMBIOS_VALUE;

        // Firmware data is stored with single byte alignment
        #pragma pack( 1 )
        typedef struct _TYPE_SMBIOS_DATA
        {
            // Entry Point structure
            BYTE    checksum;               // Offset 04h
            BYTE    length;                 // Offset 05h
            BYTE    majorVersion;           // Offset 06h
            BYTE    minorVersion;           // Offset 07h
            WORD    maxStructureSize;       // Offset 08h
            BYTE    revision;               // Offset 0Ah
            BYTE    formattedArea[ 5 ];     // Offset 0Bh - 0Fh (5 BYTES)
            BYTE    intermediate[ 5 ];      // Offset 10h - 14h (i.e _DMI_)
            BYTE    intermediateChecksum;   // Offset 15h
            WORD    tableLength;            // Offset 16h
            DWORD   tableAddress;           // Offset 18h
            WORD    numberStructures;       // Offset 1Ch
            BYTE    BCDRevision;            // Offset 1Eh

            // Data
            bool    validData;              // Data looks OK
            DWORD   epsAddress;             // In range 0x000F0000 - 0x000FFFFF
            BYTE*   pDataTable;             // SMBIOS Data
        } TYPE_SMBIOS_DATA;
        #pragma pack()  // Reset alignment packing

        // Copy constructor - not allowed
        SmbiosInformation( const SmbiosInformation& oSmbios );

        // Assignment operator - not allowed
        SmbiosInformation& operator= ( const SmbiosInformation& oSmbios );

        // Methods
 static void FormatUUID( const BYTE* pData,
                         DWORD dataLen, char* pszUUID, size_t charLenUUID );
        bool GetBooleanValue( DWORD itemID, WORD structureNumber, bool* pValue ) const;
        bool GetByteValue( DWORD itemID, WORD structureNumber, BYTE* pValue ) const;
        bool GetDWordValue( DWORD itemID, WORD structureNumber, DWORD* pValue ) const;
        bool GetQWordValue( DWORD itemID, WORD structureNumber, QWORD* pValue ) const;
        bool GetStringValue( DWORD itemID, WORD structureNumber, String* pValue ) const;
        bool GetWordValue( DWORD itemID, WORD structureNumber, WORD* pValue ) const;
 static void GetItemSpecification( DWORD itemID, PXS_TYPE_SMBIOS_SPECIFICATION* pSmbiosSpec );
        bool GetSmbiosValue( DWORD itemID,
                             WORD structureNumber, TYPE_SMBIOS_VALUE* pSmbiosValue ) const;
 static void GetString( BYTE stringNumber,
                        const BYTE* pStructure,
                        WORD  bufferLen, char* pszBuffer, size_t bufferChars );
        bool GetType0_BiosRecord( AuditRecord* pRecord ) const;
        bool GetType1_SystemRecord( AuditRecord* pRecord ) const;
        bool GetType2_BaseBoardRecord( WORD boardNumber, AuditRecord* pRecord ) const;
        bool GetType3_ChassisRecord( AuditRecord* pRecord ) const;
        bool GetType4_ProcessorRecord( WORD processorNumber, AuditRecord* pRecord ) const;
        bool GetType5_MemoryControllerRecord( WORD controllerNumber, AuditRecord* pRecord ) const;
        bool GetType6_MemoryModuleRecord( WORD moduleNumber, AuditRecord* pRecord ) const;
        bool GetType7_CpuCacheRecord( WORD cacheNumber, AuditRecord* pRecord ) const;
        bool GetType8_PortConnectorRecord( WORD portNumber, AuditRecord* pRecord ) const;
        bool GetType9_SystemSlotRecord( WORD slotNumber, AuditRecord* pRecord ) const;
        bool GetType16_MemoryArrayRecord( WORD arrayNumber, AuditRecord* pRecord ) const;
        bool GetType17_MemoryDeviceRecord( WORD deviceNumber, AuditRecord* pRecord ) const;
 static void TranslateBaseBoardType( BYTE type, String* pTranslation );
 static void TranslateBoardFeaturesFlags( BYTE flags, String* pTranslation );
 static void TranslateChassisSecurityStatus( BYTE status, String* pTranslation );
 static void TranslateChassisState( BYTE state, String* pTranslation );
 static void TranslateChassisType( BYTE type, String* pTranslation );
 static void TranslateCpuCacheConfiguration( WORD configuration, String* pTranslation );
 static void TranslateCpuCacheAssociativity( BYTE associativity, String* pTranslation );
 static void TranslateCpuCacheErrCorrection( BYTE correction, String* pTranslation );
 static void TranslateCpuCacheSramType( WORD type, String* pTranslation );
 static void TranslateCpuCacheType( BYTE type, String* pTranslation );
 static void TranslateMemoryArrayErrCorrection( BYTE correction, String* pTranslation );
 static void TranslateMemoryArrayLocation( BYTE location, String* pTranslation );
 static void TranslateMemoryArrayUse( BYTE use, String* pTranslation );
 static void TranslateMemoryControllerErrCorrection( BYTE correction, String* pTranslation );
 static void TranslateMemoryControllerInterleave( BYTE interleave, String* pTranslation );
 static void TranslateMemoryControllerSpeed( WORD speed, String* pTranslation );
 static void TranslateMemoryControllerVoltage( BYTE voltage, String* pTranslation );
 static void TranslateMemoryDeviceFormFactor( BYTE form, String* pTranslation );
 static void TranslateMemoryDeviceSet( BYTE set, String* pTranslation );
 static void TranslateMemoryDeviceType( BYTE type, String* pTranslation );
 static void TranslateMemoryDeviceTypeDetail( WORD detail, String* pTranslation );
 static void TranslateMemoryModuleBank( BYTE bank, String* pTranslation );
 static void TranslateMemoryModuleErrorStatus( BYTE status, String* pTranslation );
 static void TranslateMemoryModuleSize( BYTE size, String* pTranslation );
 static void TranslateMemoryModuleType( WORD type, String* pTranslation );
 static void TranslatePortConnectionType( BYTE type, String* pTranslation );
 static void TranslatePortType( BYTE type, String* pTranslation );
 static void TranslateProcessorCharacteristics( WORD characteristics, String* pTranslation );
 static void TranslateProcessorFamily( BYTE family, String* pTranslation );
 static void TranslateProcessorFamily2( WORD family2, String* pTranslation );
 static void TranslateProcessorStatus( BYTE status, String* pTranslation );
 static void TranslateProcessorType( BYTE type, String* pTranslation );
 static void TranslateProcessorUpgrade( BYTE upgrade, String* pTranslation );
 static void TranslateProcessorVoltage( BYTE voltage, String* pTranslation );
 static void TranslateSystemSlotCharac1( BYTE characteristics1, String* pTranslation );
 static void TranslateSystemSlotCharac2( BYTE characteristics2, String* pTranslation );
 static void TranslateSystemSlotLength( BYTE length, String* pTranslation );
 static void TranslateSystemSlotType( BYTE type, String* pTranslation );
 static void TranslateSystemSlotUsage( BYTE Usage, String* pTranslation );
 static void TranslateSystemSlotWidth( BYTE width, String* pTranslation );
 static void TranslateWakeUpType( BYTE type, String* pTranslation );

        // Data members
        TYPE_SMBIOS_DATA  m_SmBiosData;
};

#endif  // WINAUDIT_SMBIOS_INFORMATION_H_
