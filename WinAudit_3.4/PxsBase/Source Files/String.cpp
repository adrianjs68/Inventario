///////////////////////////////////////////////////////////////////////////////////////////////////
//
// String Class Implementation
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
#include "PxsBase/Header Files/StringT.h"

// 2. C System Files
#include <stdint.h>

// 3. C++ System Files

// 4. Other Libraries

// 5. This Project
#include "PxsBase/Header Files/AllocateChars.h"
#include "PxsBase/Header Files/BoundsException.h"
#include "PxsBase/Header Files/ComException.h"
#include "PxsBase/Header Files/ByteArray.h"
#include "PxsBase/Header Files/CharArray.h"
#include "PxsBase/Header Files/MemoryException.h"
#include "PxsBase/Header Files/NameValue.h"
#include "PxsBase/Header Files/NullException.h"
#include "PxsBase/Header Files/ParameterException.h"
#include "PxsBase/Header Files/StringArray.h"
#include "PxsBase/Header Files/SystemException.h"
#include "PxsBase/Header Files/TArray.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
String::String()
       :MINIMUM_ALLOCATION( 32 ),
        m_uLengthChars( 0 ),
        m_uCharsAllocated( 0 ),
        m_pwzString( nullptr )
{
}

// Copy constructor
String::String( const String& oString )
       :String()

{
    *this = oString;
}

// Destructor
String::~String()
{
    // Clean up
    if ( m_pwzString )
    {
        delete[] m_pwzString;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////////////////////////

// Assignment operator
String& String::operator=( const String& oString )
{
    if ( this == &oString ) return *this;

    *this = oString.m_pwzString;

    return *this;
}

// Assignment operator
String& String::operator= ( wchar_t ch )
{
    wchar_t szString[ 2 ] = { ch, PXS_CHAR_NULL };

    *this = szString;

    return *this;
}

// Assignment operator
String& String::operator= ( LPCWSTR pszString )
{
    size_t charsNeeded;

    if ( m_pwzString == pszString )
    {
        return *this;
    }

    // Preserve NULL or ""
    if ( pszString == nullptr )
    {
        SetNull();
        return *this;
    }

    if ( *pszString == PXS_CHAR_NULL )
    {
        SetEmpty();
        return *this;
    }

    // See if can put into the existing buffer
    charsNeeded = wcslen( pszString );
    charsNeeded = PXSAddSizeT( charsNeeded, 1 );    // Null terminator
    Allocate( charsNeeded );
    PXSStringCchCopy( m_pwzString, charsNeeded, pszString, __FUNCTION__ );
    m_uLengthChars = charsNeeded - 1;

    return *this;
}

// Addition assignment operator
String& String::operator+=( const String& oString )
{
    Append( oString.m_pwzString );

    return *this;
}

// Addition assignment operator
String& String::operator+= ( LPCWSTR pszString )
{
    Append( pszString );

    return *this;
}

// Addition assignment operator
String& String::operator+= ( wchar_t ch )
{
    AppendChar( ch, 1 );

    return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Public Methods
///////////////////////////////////////////////////////////////////////////////////////////////////

//===============================================================================================//
//  Description:
//      Allocate memory for this string.
//
//  Parameters:
//        numChars - number of characters to allocate
//
//  Remarks:
//      Has no effect if the requested size numChars is smaller that the
//      amount currently allocated.
//      Any existing string is preserved.
//
//  Returns:
//      void
//===============================================================================================//
void String::Allocate( size_t numChars )
{
    wchar_t* pszNew;

    if ( numChars <= m_uCharsAllocated )
    {
        return;     // Nothing to do
    }

    // Will grow the string by 50% more than requested amount
    numChars = PXSMultiplySizeT( numChars, 3 ) / 2;
    if ( numChars < MINIMUM_ALLOCATION )    // Enforce a minimum
    {
        numChars = MINIMUM_ALLOCATION;
    }
    pszNew = new wchar_t[ numChars ];
    if ( pszNew == nullptr )
    {
        throw MemoryException( __FUNCTION__ );
    }
    wmemset( pszNew, 0, numChars );

    // Copy in any existing string
    if ( m_pwzString )
    {
        HRESULT hr = StringCchCopy( pszNew, numChars, m_pwzString );
        if ( FAILED( hr ) )
        {
            delete [] pszNew;
            throw ComException( hr, L"StringCchCopy", __FUNCTION__ );
        }
    }

    // Replace
    delete[] m_pwzString;
    m_pwzString       = pszNew;
    m_uCharsAllocated = numChars;
}

//===============================================================================================//
//  Description:
//      Append another string to this string.
//
//  Parameters:
//      pszString - pointer to string to append
//
//  Returns:
//      void
//===============================================================================================//
void String::Append( LPCWSTR pszString )
{
    size_t length;

    if ( pszString == nullptr )
    {
        return;   // Nothing to do
    }
    length = wcslen( pszString );
    AppendChars( pszString, length );

    return;
}

//===============================================================================================//
//  Description:
//      Append a character to this string.
//
//  Parameters:
//      ch - character to append
//
//  Returns:
//      void
//===============================================================================================//
void String::AppendChar( wchar_t ch )
{
    AppendChar( ch, 1 );
}

//===============================================================================================//
//  Description:
//      Append a repeating character to this string.
//
//  Parameters:
//      ch     - character to append
//      repeat - number of times to repeat the character
//
//  Returns:
//      void
//===============================================================================================//
void String::AppendChar( wchar_t ch, size_t repeat )
{
    size_t i = 0, charsNeeded = 0, lenThis = m_uLengthChars;

    charsNeeded = PXSAddSizeT( lenThis, repeat );
    charsNeeded = PXSAddSizeT( charsNeeded, 1 );        // Terminator
    Allocate( charsNeeded );
    for ( i = 0; i < repeat; i++ )
    {
        m_pwzString[ lenThis + i ] = ch;
    }
    m_uLengthChars = charsNeeded - 1;
    m_pwzString[ m_uLengthChars ] = PXS_CHAR_NULL;
}

//===============================================================================================//
//  Description:
//      Append a given number of characters from input string to this string.
//
//  Parameters:
//      pszString - pointer to string to append
//      numChars  - number of characters to append
//
//  Returns:
//      void
//===============================================================================================//
void String::AppendChars( LPCWSTR pszString, size_t numChars )
{
    if ( pszString == nullptr )
    {
        return;   // Nothing to do
    }

    // Disallow appending a string to itself
    if ( pszString == m_pwzString )
    {
        throw FunctionException( L"pszString=m_pwzString", __FUNCTION__ );
    }

    size_t charsNeeded = PXSAddSizeT( m_uLengthChars, numChars );
    charsNeeded        = PXSAddSizeT( charsNeeded, 1 );        // Terminator
    Allocate( charsNeeded );
    size_t i = 0;
    while ( i < numChars )
    {
        m_pwzString[ m_uLengthChars + i ] = pszString[ i ];
        i++;
    }
    m_uLengthChars = ( m_uLengthChars + numChars );
    m_pwzString[ m_uLengthChars ] = PXS_CHAR_NULL;
}

//===============================================================================================//
//  Description:
//      Get the pointer to the internal string
//
//  Parameters:
//      None
//
//  Returns:
//      Constant string pointer
//===============================================================================================//
LPCWSTR String::c_str() const
{
    return m_pwzString;
}

//===============================================================================================//
//  Description:
//      Get the character at the specified zero-based index
//
//  Parameters:
//      index - the index
//
//  Returns:
//      The character at the index, else 0
//===============================================================================================//
wchar_t String::CharAt( size_t index ) const
{
    // Return terminator if no string
    if ( m_pwzString == nullptr )
    {
        return PXS_CHAR_NULL;
    }

    if ( index >= m_uLengthChars )      // Inclusive
    {
        throw BoundsException( L"index", __FUNCTION__ );
    }

    return m_pwzString[ index ];
}
//===============================================================================================//
//  Description:
//      Append the characters from this string to input string
//
//  Parameters:
//      idxFrom - zero based index to append from
//      pInput  - receives the appended characters
//
//  Returns:
//      void
//===============================================================================================//
void String::AppendFrom( size_t idxFrom, String* pInput ) const
{
    if ( m_pwzString == nullptr )
    {
        return;     // Nothing to do
    }

    if ( pInput == nullptr )
    {
        throw NullException( L"pInput", __FUNCTION__ );
    }

    if ( idxFrom >= m_uLengthChars )
    {
        return;     // Nothing to do or bounds exception
    }
    pInput->AppendChars( m_pwzString + idxFrom, m_uLengthChars - idxFrom );
}

//===============================================================================================//
//  Description:
//      Append the specified input string to this string
//
//  Parameters:
//      Input - string to append
//
//  Returns:
//      void
//===============================================================================================//
void String::AppendString( const String& Input )
{
    Append( Input.c_str() );
}

//===============================================================================================//
//  Description:
//      Compare the input string with this string
//
//  Parameters:
//      Input         - string to compare with
//      caseSensitive - true if want a case sentitive comaparison
//
//  Returns:
//        0 if the strings are identical
//      < 0 if this string is less than pszString
//      > 0 if this string is greater than pszString
//===============================================================================================//
int String::Compare( const String& Input, bool caseSensitive ) const
{
    return PXSCompareString( m_pwzString, Input.c_str(), caseSensitive );
}

//===============================================================================================//
//  Description:
//      Compare the input string with this string
//
//  Parameters:
//      pszString     - string to compare with
//      caseSensitive - true if want a case sentitive comaparison
//
//  Returns:
//        0 if the strings are identical
//      < 0 if this string is less than pszString
//      > 0 if this object is greater than pszString
//===============================================================================================//
int String::Compare( LPCWSTR pszString, bool caseSensitive ) const
{
    return PXSCompareString( m_pwzString, pszString, caseSensitive );
}

//===============================================================================================//
//  Description:
//      Case insensitive comparison
//
//  Parameters:
//      Input - string to compare with
//
//  Returns:
//        0 if the strings are identical
//      < 0 if this string is less than pszString
//      > 0 if this object is greater than pszString
//===============================================================================================//
int String::CompareI( const String& Input ) const
{
    return PXSCompareString( m_pwzString, Input.c_str(), false );
}

//===============================================================================================//
//  Description:
//      Case insensitive comparison
//
//  Parameters:
//      pszString - string to compare with
//
//  Returns:
//        0 if the strings are identical
//      < 0 if this string is less than pszString
//      > 0 if this object is greater than pszString
//===============================================================================================//
int String::CompareI( LPCWSTR pszString ) const
{
    return PXSCompareString( m_pwzString, pszString, false );
}

//===============================================================================================//
//  Description:
//      Get the number of occurrences of the specified character in this string
//
//  Parameters:
//      wch - the character to look for
//
//  Returns:
//      number of occurrences
//===============================================================================================//
size_t String::CountOfChar( wchar_t wch ) const
{
    size_t i = 0, count = 0;

    if ( m_pwzString == nullptr )
    {
        return 0;
    }

    while ( i < m_uLengthChars )
    {
        if ( m_pwzString[ i ] == wch )
        {
            count++;
        }
        i++;
    }
    return count;
}

//===============================================================================================//
//  Description:
//      Empty this string by freeing memory.
//
//  Parameters:
//      None
//
//  Returns:
//      void
//===============================================================================================//
void String::Delete()
{
    if ( m_pwzString )
    {
        delete[] m_pwzString;
    }
    m_pwzString       = nullptr;
    m_uLengthChars    = 0;
    m_uCharsAllocated = 0;
}

//===============================================================================================//
//  Description:
//      Determines if a this string object ends a given character. The test is case insensitive.
//
//  Parameters:
//      ch - character to test for
//
//  Returns:
//      true if this string ends with the character, else false.
//===============================================================================================//
bool String::EndsWithCharacterI( wchar_t ch ) const
{
    wchar_t szString[ 2 ] = { ch, PXS_CHAR_NULL };

    return EndsWithStringI( szString );
}

//===============================================================================================//
//  Description:
//      Determine if this string ends with the input string.
//
//  Parameters:
//      pszString     - string to test for
//      caseSensitive - true if want a case insentive match othewise false
//
//  Returns:
//      true if this string object ends with pszString else false.
//===============================================================================================//
bool String::EndsWithString( LPCWSTR pszString, bool caseSensitive ) const
{
    size_t numChars;

    if ( ( pszString == nullptr ) || ( m_pwzString == nullptr ) )
    {
        return false;
    }

    numChars = wcslen( pszString );
    if ( numChars > m_uLengthChars )
    {
        return false;
    }

    if ( PXSCompareString( pszString, m_pwzString + (m_uLengthChars - numChars), caseSensitive ) )
    {
       return false;
    }

    return true;
}

//===============================================================================================//
//  Description:
//      Determine if this string ends with the input string. The comparison
//      is case insensitive.
//
//  Parameters:
//      pszString - string to test for
//
//  Returns:
//      true if this string object ends with pszString else false.
//===============================================================================================//
bool String::EndsWithStringI( LPCWSTR pszString ) const
{
    return EndsWithString( pszString, false );
}

//===============================================================================================//
//  Description:
//      Escape the string for use as html text
//
//  Parameters:
//      none
//
//  Returns:
//      void
//===============================================================================================//
void String::EscapeForHtml()
{
    wchar_t  ch;
    wchar_t  szEntity[ 16 ] = { 0 };
    wchar_t* psz    = m_pwzString;
    wchar_t* pszNew = nullptr;
    size_t numEscaped = 0, totalEscaped = 0, numAllocated = 0, idx = 0;

    if ( psz == nullptr )
    {
        return;
    }

    // Count characters needed, for simplicity will not deduct by the space
    // taken by the unescaped characters, hence will overestimate actual
    // chars required.
    ch = *psz;
    while ( ch )
    {
        memset( szEntity, 0, sizeof ( szEntity ) );
        numEscaped   = PXSGetHtmlCharacterEntity( ch, szEntity,  ARRAYSIZE( szEntity ) );
        totalEscaped = PXSAddSizeT( totalEscaped, numEscaped );

        psz++;
        ch = *psz;
    }

    // Test if need to escape
    if ( totalEscaped ==  0 )
    {
        return;     // Nothing to do
    }

    // Allocate
    numAllocated = GetLength();
    numAllocated = PXSAddSizeT( numAllocated, totalEscaped );
    numAllocated = PXSAddSizeT( numAllocated, 1 );      // Terminator
    pszNew = new wchar_t[ numAllocated ];
    if ( pszNew == nullptr )
    {
        throw MemoryException( __FUNCTION__ );
    }
    wmemset( pszNew, 0, numAllocated );

    // Second pass
    try
    {
        psz = m_pwzString;
        idx = 0;
        ch  = *psz;
        while ( ch && ( idx < numAllocated ) )
        {
            memset( szEntity, 0, sizeof ( szEntity ) );
            numEscaped = PXSGetHtmlCharacterEntity( ch, szEntity, ARRAYSIZE( szEntity ) );
            if ( numEscaped && ( (idx + numEscaped) < numAllocated ) )
            {
                PXSStringCchCopy( pszNew + idx, numEscaped + 1, szEntity, __FUNCTION__ );
                idx += numEscaped;
            }
            else
            {
                pszNew[ idx ] = ch;
                idx++;
            }
            psz++;
            ch = *psz;
        }
    }
    catch ( const Exception& )
    {
        delete [] pszNew;
        throw;
    }
    pszNew[ numAllocated - 1 ] = PXS_CHAR_NULL;

    // Replace
    delete[] m_pwzString;
    m_pwzString = pszNew;
    m_uCharsAllocated = numAllocated;
    m_uLengthChars    = idx;
}

//===============================================================================================//
//  Description:
//      Escape the string for use as rich text
//
//  Parameters:
//      none
//
//  Returns:
//      void
//===============================================================================================//
void String::EscapeForRichText()
{
    wchar_t  ch;
    wchar_t  szRtf[ 16 ] = { 0 };
    wchar_t* psz      = m_pwzString;
    wchar_t* pszNew   = nullptr;
    size_t numEscaped = 0, totalEscaped = 0, numAllocated = 0, idx = 0;

    if ( psz == nullptr )
    {
        return;
    }

    // Count characters needed, for simplicity will not deduct by the space
    // taken by the unescaped characters, hence will overestimate actual
    // chars required.
    ch  = *psz;
    while ( ch )
    {
        memset( szRtf, 0, sizeof ( szRtf ) );
        numEscaped   = PXSEscapeRichTextChar( ch, szRtf, ARRAYSIZE( szRtf ) );
        totalEscaped = PXSAddSizeT( totalEscaped, numEscaped );
        psz++;
        ch = *psz;
    }

    // Test if need to escape
    if ( totalEscaped ==  0 )
    {
        return;     // Nothing to do
    }

    // Allocate
    numAllocated = GetLength();
    numAllocated = PXSAddSizeT( numAllocated, totalEscaped );
    numAllocated = PXSAddSizeT( numAllocated, 1 );              // Terminator
    pszNew   = new wchar_t[ numAllocated ];
    if ( pszNew == nullptr )
    {
        throw MemoryException( __FUNCTION__ );
    }
    wmemset( pszNew, 0, numAllocated );

    // Second pass
    try
    {
        psz = m_pwzString;
        idx = 0;
        ch  = *psz;
        while ( ch && ( idx < numAllocated ) )
        {
            numEscaped = PXSEscapeRichTextChar( ch, szRtf, ARRAYSIZE( szRtf ) );
            if (numEscaped && ((idx + numEscaped) < numAllocated))
            {
                PXSStringCchCopy( pszNew + idx, numEscaped + 1, szRtf, __FUNCTION__ );
                idx += numEscaped;
            }
            else
            {
                // No escape required, use the original char
                pszNew[ idx ] = ch;
                idx++;
            }
            psz++;
            ch = *psz;
        }
    }
    catch ( const Exception& )
    {
        delete [] pszNew;
        throw;
    }
    pszNew[ numAllocated - 1 ] = PXS_CHAR_NULL;

    // Replace
    delete[] m_pwzString;
    m_pwzString = pszNew;
    m_uCharsAllocated = numAllocated;
    m_uLengthChars    = idx;
}

//===============================================================================================//
//  Description:
//      Format a string so that it has a fixed width in characters. If need
//      to will either truncate or pad with the specified character
//
//  Parameters:
//      maxWidth - maximum number of characters in string
//      ch       - character to pad string with
//
//  Returns:
//      void
//===============================================================================================//
void String::FixedWidth( size_t maxWidth, wchar_t ch )
{
    // Cannot pad with NULLS
    if ( ch == PXS_CHAR_NULL )
    {
        return;
    }

    if ( maxWidth == m_uLengthChars )
    {
        return;     // Nothing to do
    }

    if ( maxWidth > m_uLengthChars )
    {
        AppendChar( ch, maxWidth - m_uLengthChars );
    }
    else
    {
       Truncate( maxWidth );
    }
}

//===============================================================================================//
//  Description:
//      Get the number characters required for use with WideCharToMultiByte
//      using the system default ANSI code page.
//
//  Parameters:
//      None
//
//  Remarks:
//      The returned size includes room for the NULL terminator
//
//  Returns:
//      The length of the buffer required to hold the multi-byte string.
//===============================================================================================//
size_t String::GetAnsiMultiByteLength() const
{
    int cbMultiByte =0;

    if ( m_pwzString )
    {
        cbMultiByte = WideCharToMultiByte( CP_ACP,
                                           WC_NO_BEST_FIT_CHARS,  // Security
                                           m_pwzString,
                                           -1,
                                           nullptr, 0, nullptr, nullptr );
    }
    cbMultiByte = PXSAddInt32( cbMultiByte, 1 );    // NULL terminator

    return PXSCastInt32ToSizeT( cbMultiByte );
}

//===============================================================================================//
//  Description:
//      Get this string as a byte array
//
//  Parameters:
//      pBytes - receives the bytes
//
//  Returns:
//      void
//===============================================================================================//
void String::GetBytes( ByteArray* pBytes ) const
{
    size_t numBytes;

    if ( pBytes == nullptr )
    {
        throw ParameterException( L"pBytes", __FUNCTION__ );
    }
    pBytes->Zero();

    if ( ( m_pwzString == nullptr ) || ( m_uLengthChars == 0 ) )
    {
        return;     // nothing to do
    }
    numBytes = PXSMultiplySizeT( m_uLengthChars, sizeof ( *m_pwzString ) );
    pBytes->Append( reinterpret_cast< const BYTE* >( m_pwzString ), numBytes );
}

//===============================================================================================//
//  Description:
//      Get the length in characters of this string.
//
//  Parameters:
//      None
//
//  Returns:
//      The length of the string characters.
//===============================================================================================//
size_t String::GetLength() const
{
    return m_uLengthChars;
}

//===============================================================================================//
//  Description:
//      Get the zero-based index positions of the specified string in this
//      string
//
//  Parameters:
//      pszString     - string to search for
//      caseSensitive - true if case sensitive string search
//      pIndexes      - receives the index offsets
//
//  Returns:
//      Number of occurrences of the string
//===============================================================================================//
size_t String::IndexesOf( LPCWSTR pszString,
                          bool caseSensitive, TArray< size_t >* pIndexes ) const
{
    size_t idx = 0, numCharsString = 0, counter = 0;

    if ( pIndexes == nullptr )
    {
        throw ParameterException( L"pIndexes", __FUNCTION__ );
    }
    pIndexes->RemoveAll();

    if ( ( m_pwzString == nullptr ) ||
         ( pszString   == nullptr ) ||
         ( *pszString  == PXS_CHAR_NULL ) )
    {
        return 0;       // Nothing to do
    }

    numCharsString = wcslen( pszString );
    while ( ( idx + numCharsString ) <= m_uLengthChars )
    {
        if ( PXSCompareStringN( pszString,
                                m_pwzString + idx, numCharsString, caseSensitive ) == 0 )
        {
            pIndexes->Add( idx );
            counter++;
            idx += numCharsString;
        }
        else
        {
            idx++;
        }
    }

    return pIndexes->GetSize();
}

//===============================================================================================//
//  Description:
//      Find the location of the first occurrence of a character.
//
//  Parameters:
//      ch        - character to look for.
//      fromIndex - position to start searching from
//
//  Returns:
//      The zero based index otherwise PXS_MINUS_ONE
//===============================================================================================//
size_t String::IndexOf( wchar_t ch, size_t fromIndex ) const
{
    size_t i    = 0;
    size_t found = PXS_MINUS_ONE;    // Assume not found

    if ( m_pwzString == nullptr )
    {
        return PXS_MINUS_ONE;
    }

    for ( i = fromIndex; i < m_uLengthChars; i++ )
    {
        if ( m_pwzString[ i ] == ch )
        {
            found = i;
            break;
        }
    }

    return found;
}

//===============================================================================================//
//  Description:
//      Find the location of the first occurrence of a string.
//
//  Parameters:
//      pszString     - string to search for.
//      caseSensitive - flag to indicate if the search is case sensitive
//      fromIndex     - position to start searching from
//
//  Returns:
//      Index where found or PXS_MINUS_ONE.
//===============================================================================================//
size_t String::IndexOf( LPCWSTR pszString, bool caseSensitive, size_t fromIndex ) const
{
    size_t numChars = 0, idx = 0;
    size_t found    = PXS_MINUS_ONE;     // Assume not found

    if ( ( m_pwzString == nullptr       ) ||
         ( pszString   == nullptr       ) ||
         ( *pszString  == PXS_CHAR_NULL )  )
    {
        return PXS_MINUS_ONE;
    }

    idx = fromIndex;
    numChars = wcslen( pszString );
    while ( ( found == PXS_MINUS_ONE ) && ( PXSAddSizeT( idx, numChars ) <= m_uLengthChars ) )
    {
        if ( PXSCompareStringN( pszString, m_pwzString + idx, numChars, caseSensitive ) == 0 )
        {
            found = idx;
        }
        idx++;
    };

    return found;
}

//===============================================================================================//
//  Description:
//      Determines the zero based offset of the first position of a substring.
//      The search is case insensitive.
//
//  Parameters:
//      pszString - pointer to string to find
//
//  Returns:
//      if substring found, returns zero based index, else PXS_MINUS_ONE;
//===============================================================================================//
size_t String::IndexOfI( LPCWSTR pszString ) const
{
    return IndexOf( pszString, false, 0 );
}

//===============================================================================================//
//  Description:
//      Determine if the string is ""
//
//  Parameters:
//      None
//
//  Returns:
//    true if if the string is of zero length
//===============================================================================================//
bool String::IsEmpty() const
{
    if ( ( m_pwzString == nullptr ) || ( m_uLengthChars == 0 ) )
    {
        return true;
    }
    return false;
}

//===============================================================================================//
//  Description:
//      Determines if this string is NULL.
//
//  Parameters:
//      None
//
//  Returns:
//      true if NULL otherwise false
//===============================================================================================//
bool String::IsNull( ) const
{
    if ( m_pwzString == nullptr )
    {
        return true;
    }
    return false;
}

//===============================================================================================//
//  Description:
//      Determine if the string contains characters beyond 8-bits
//
//  Parameters:
//      None
//
//  Returns:
//    true if only contains a characters in the range 0-255
//===============================================================================================//
bool String::IsOnly8Bit() const
{
    bool only8Bit = true;
    wchar_t* psw  = m_pwzString;

    if ( psw == nullptr )
    {
        return false;
    }

    wchar_t ch  = *psw;
    while ( ch && only8Bit )
    {
        if ( ch > 0xFF )
        {
            only8Bit = false;
        }
        psw++;
        ch = *psw;
    }

    return only8Bit;
}

//===============================================================================================//
//  Description:
//      Test if this string is comprised of only the specified character
//
//  Parameters:
//      ch - the repeated character to test for
//
//  Returns:
//      true if all characters are the same as the input
//===============================================================================================//
bool String::IsOnlyChar( wchar_t ch ) const
{
    return PXSIsOnlyChar( m_pwzString, m_uLengthChars, ch );
}

//===============================================================================================//
//  Description:
//      Test if this string comprises of only digits 0..9
//
//  Parameters:
//      none
//
//  Returns:
//      true if only digits, otherwise false
//===============================================================================================//
bool String::IsOnlyDigits() const
{
    return PXSIsDigitsOnly( m_pwzString, m_uLengthChars );
}

//===============================================================================================//
//  Description:
//      Determine if the string contains only US-ASCII characters
//
//  Parameters:
//      None
//
//  Returns:
//    true if only contains only characters in the range 0-127
//===============================================================================================//
bool String::IsOnlyUSAscii() const
{
    bool     isUSAscii = true;  // Assume there are only USE ASCII characters
    wchar_t  ch  = 0;
    wchar_t* psz = m_pwzString;

    if ( psz == nullptr )
    {
        return false;
    }

    ch = *psz;
    while ( ch && isUSAscii )
    {
        if ( ch > 127 )
        {
            isUSAscii = false;
        }
        psz++;
        ch = *psz;
    }

    return isUSAscii;
}

//===============================================================================================//
//  Description:
//      Determine if the string contains only visible US-ASCII characters
//
//  Parameters:
//      None
//
//  Remarks:
//      Space character is considered to be visible.
//
//  Returns:
//    true if only contains visible characters in the range 0-127
//===============================================================================================//
bool String::IsOnlyUSAsciiVisible() const
{
    bool     isVChar = true;  // Assume there are only USE ASCII characters
    wchar_t  ch  = 0;
    wchar_t* psz = m_pwzString;

    if ( psz == nullptr )
    {
        return false;
    }

    ch = *psz;
    while ( ch && isVChar )
    {
        if ( ( ch < 32 ) || ( ch > 126 ) )
        {
            isVChar = false;
        }
        psz++;
        ch = *psz;
    }

    return isVChar;
}

//===============================================================================================//
//  Description:
//      Get the last index of the specified character
//
//  Parameters:
//      ch - the character to search for
//
//  Returns:
//      zero based index, otherwise PXS_MINUS_ONE if not found
//===============================================================================================//
size_t String::LastIndexOf ( wchar_t ch ) const
{
    size_t index = PXS_MINUS_ONE;
    wchar_t* ptr;

    if ( m_pwzString == nullptr )
    {
        return PXS_MINUS_ONE;
    }

    if ( ch == PXS_CHAR_NULL )
    {
        return PXS_MINUS_ONE;
    }

    ptr = wcsrchr( m_pwzString, ch );
    if ( ptr == nullptr )
    {
        return PXS_MINUS_ONE;
    }

    if ( ptr >= m_pwzString )
    {
        index = static_cast< size_t >( ptr - m_pwzString );
    }

    return index;
}

//===============================================================================================//
//  Description:
//      Left trim a string, characters up to x20 are removed.
//
//  Parameters:
//      None
//
//  Returns:
//      void
//===============================================================================================//
void String::LeftTrim()
{
    size_t shift = 0;

    if ( m_pwzString == nullptr )
    {
        return;
    }

    // Determine how far to shift
    wchar_t ch = *m_pwzString;
    while ( ch && PXSIsWhiteSpace( ch ) )
    {
        shift++;
        ch = m_pwzString[ shift ];
    }

    if ( shift && ( shift <= m_uLengthChars ) )
    {
        size_t newLength = m_uLengthChars - shift;
        for ( size_t i = 0; i < newLength; i++ )
        {
            m_pwzString[ i ] = m_pwzString[ i + shift ];
        }
        m_uLengthChars = newLength;
        m_pwzString[ m_uLengthChars ] = PXS_CHAR_NULL;
    }
}

//===============================================================================================//
//  Description:
//      Replace all occurrences of a string with another string/ Comparison
//      is case sensitive
//
//  Parameters:
//      pszOld - pointer to string to be replaced
//      pszNew - pointer to the replacement string
//
//  Returns:
//        Number of replacements
//===============================================================================================//
size_t String::ReplaceI( LPCWSTR pszOld, LPCWSTR pszNew )
{
    size_t   numCharsNew = 0, numCharsOld = 0, numCharsReplace = 0, numCharsTemp = 0;
    wchar_t* pszReplace  = nullptr;
    TArray< size_t >Indexes;

    if ( ( pszOld      == nullptr       ) ||
         ( *pszOld     == PXS_CHAR_NULL ) ||
         ( pszNew      == nullptr       ) ||
         ( m_pwzString == nullptr       )  )
    {
        return 0;
    }

    // Cannot replace with itself
    if ( m_pwzString == pszNew )
    {
        throw FunctionException( L"m_pwzString=pszNew", __FUNCTION__ );
    }

    numCharsOld = wcslen( pszOld );
    numCharsNew = wcslen( pszNew );
    IndexesOf( pszOld, false, &Indexes );
    if ( Indexes.GetSize() == 0 )
    {
        return 0;   // Nothing to do
    }

    // Allocate a buffer big enough this string, all the replacements and
    // a NULL terminator
    numCharsTemp    = PXSMultiplySizeT( numCharsNew, Indexes.GetSize() );
    numCharsReplace = m_uLengthChars;
    numCharsReplace = PXSAddSizeT( numCharsReplace, numCharsTemp );
    numCharsReplace = PXSAddSizeT( numCharsReplace, 1 );     // + terminator
    pszReplace      = new wchar_t[ numCharsReplace ];
    if ( pszReplace == nullptr )
    {
        throw MemoryException( __FUNCTION__ );
    }
    wmemset( pszReplace, 0, numCharsReplace );

    try
    {
        // Replace the old strings
        size_t idx        = 0;
        size_t idxStart   = 0;
        size_t numIndexes = Indexes.GetSize();
        for ( size_t i = 0; i < numIndexes; i++ )
        {
            // Copy in characters up to pszOld
            size_t idxEnd = Indexes.Get( i );
            memcpy( pszReplace + idx,
                    m_pwzString + idxStart, ( idxEnd - idxStart ) * sizeof ( wchar_t ) );
            idx = PXSAddSizeT( idx, ( idxEnd - idxStart ) );

            // Append the new string, should always fit but make sure
            memcpy( pszReplace + idx, pszNew, numCharsNew * sizeof ( wchar_t ) );
            idx = PXSAddSizeT( idx, numCharsNew );

            // Set new start, skipping length of pszOld
            idxStart = idxEnd;
            idxStart = PXSAddSizeT( idxStart, numCharsOld );
        }
        pszReplace[ numCharsReplace - 1 ] = PXS_CHAR_NULL;

        // Pick up any characters after the last pszOld
        if ( idxStart < m_uLengthChars )
        {
           StringCchCat( pszReplace, numCharsReplace, m_pwzString + idxStart );
        }
    }
    catch ( const Exception& )
    {
        delete [] pszReplace;
        throw;
    }

    // Replace
    delete[]  m_pwzString;
    m_pwzString       = pszReplace;
    m_uLengthChars    = wcslen( m_pwzString );
    m_uCharsAllocated = numCharsReplace;
    return Indexes.GetSize();
}

//===============================================================================================//
//  Description:
//      Replace all occurrences of a given character with another, case sensitive.
//
//  Parameters:
//      oldChar  - character to be replaced
//      newChar  - new character
//
//  Returns:
//      void
//===============================================================================================//
void String::ReplaceChar( wchar_t oldChar, wchar_t newChar )
{
    size_t i = 0;

    if ( m_pwzString == nullptr )
    {
        return;
    }

    while ( m_pwzString[ i ] )
    {
        if ( oldChar == m_pwzString[ i ] )
        {
            m_pwzString[ i ] = newChar;
        }

        i++;
    }
}

//===============================================================================================//
//  Description:
//      Replace all occurrences of the specified character character with the
//      specified string. Case insensitive
//
//  Parameters:
//      oldChar - character to replace
//      pszNew  - pointer to string that will replace character
//
//  Returns:
//      Number of instances of the character that were replaced
//===============================================================================================//
size_t String::ReplaceChar( wchar_t oldChar, LPCWSTR pszNew )
{
    wchar_t szString[ 2 ] = { oldChar, PXS_CHAR_NULL };

    return ReplaceI( szString, pszNew );
}

//===============================================================================================//
//  Description:
//      Replace the character at the specifified index with the new character
//
//  Parameters:
//      index  - the zero-based index of the character to replace
//      pszNew - the new character
//
//  Returns:
//      void
//===============================================================================================//
void String::ReplaceCharAt( size_t index, wchar_t newChar )
{
    if ( m_pwzString == nullptr )
    {
        throw NullException( L"m_pwzString", __FUNCTION__ );
    }

    if ( index >= m_uLengthChars )      // Inclusive
    {
        throw BoundsException( L"index", __FUNCTION__ );
    }
    m_pwzString[ index ] = newChar;
}

//===============================================================================================//
//  Description:
//      Replace any invalid UTF-16 characters with the specified character
//
//  Parameters:
//      newChar - new character
//
//  Remarks:
//      Only relevant for a  Unicode compilation
//
//  Returns:
//      The count of invalid UTF16 chars replaced
//===============================================================================================//
size_t String::ReplaceInvalidUTF16( wchar_t newChar ) const
{
    size_t   count    = 0;
    wchar_t* lpString = m_pwzString;

    if ( lpString == nullptr )
    {
        return 0;   // Nothing to do
    }

    while ( *lpString )
    {
        wchar_t chCurr = *lpString;
        wchar_t chNext = *( lpString + 1 );
        if ( PXSIsValidUtf16( chCurr ) == false )
        {
            *lpString = newChar;
            count++;
        }

        // Test for un-paired high surrogate
        if ( IS_HIGH_SURROGATE( chCurr ) )
        {
            if ( IS_LOW_SURROGATE( chNext ) == FALSE )
            {
                *lpString = newChar;
                count++;

                // Replace chNext as well but only if its not the terminator
                if ( chNext != PXS_CHAR_NULL )
                {
                    *( lpString + 1 ) = newChar;
                    count++;
                }
            }
        }

        // If find a high surrogate will advance by two chars so should
        // never encounter a low surrogate as the current character
        if ( IS_LOW_SURROGATE( chCurr ) )
        {
            *lpString = newChar;
            count++;

            // Replace chNext
            *( lpString + 1 ) = newChar;
            count++;
        }

        lpString++;
    }

    return count;
}

//===============================================================================================//
//  Description:
//      Replace all white spaces (<= ASCII code 32) with another character
//
//  Parameters:
//      newChar - the new character, NULL is allowed
//
//  Returns:
//      Number of replacements effected
//===============================================================================================//
size_t String::ReplaceWhiteSpaces( wchar_t newChar )
{
    size_t i = 0, counter = 0;

    if ( m_pwzString == nullptr )
    {
        return 0;   // Nothing to do
    }

    while ( m_pwzString[ i ] )
    {
        if ( PXSIsWhiteSpace( m_pwzString[ i ] ) )
        {
            m_pwzString[ i ] = newChar;
            counter++;
        }
        i++;
    }

    return counter;
}

//===============================================================================================//
//  Description:
//      Reverse the order of a string.
//
//  Parameters:
//      None
//
//  Returns:
//      void
//===============================================================================================//
void String::Reverse()
{
    if ( m_pwzString )
    {
        m_pwzString = _wcsrev( m_pwzString );
    }
}

//===============================================================================================//
//  Description:
//      Right trim a string, characters up to x20 are removed.
//
//  Parameters:
//      None
//
//  Returns:
//      void
//===============================================================================================//
void String::RightTrim()
{
    size_t i = 0;

    if ( m_pwzString == nullptr )
    {
        return;   // Nothing to do
    }

    while ( ( i < m_uLengthChars ) &&
            ( PXSIsWhiteSpace( m_pwzString[ m_uLengthChars - 1 - i ] ) ) )
    {
        m_pwzString[ m_uLengthChars - 1 - i ] = PXS_CHAR_NULL;
        i++;
    }
    m_uLengthChars -= i;

    return;
}

//===============================================================================================//
//  Description:
//      Right trim the string of the specfied character.
//
//  Parameters:
//      None
//
//  Returns:
//      void
//===============================================================================================//
void String::RightTrim( wchar_t wch )
{
    size_t i = 0;

    if ( m_pwzString == nullptr )
    {
        return;   // Nothing to do
    }

    while ( ( i < m_uLengthChars ) && ( m_pwzString[ m_uLengthChars - 1 - i ] == wch ) )
    {
        m_pwzString[ m_uLengthChars - 1 - i ] = PXS_CHAR_NULL;
        i++;
    }
    m_uLengthChars -= i;

    return;
}

//===============================================================================================//
//  Description:
//      Set this string using the specified ANSI string
//
//  Parameters:
//      pszAnsi - constant pointer to the ANSI string
//
//  Returns:
//      void
//===============================================================================================//
void String::SetAnsi( const char* pszAnsi )
{
    size_t numChars = 0;

    // Preserve NULL or ""
    if ( pszAnsi == nullptr )
    {
        SetNull();
        return;
    }

    if ( *pszAnsi == PXS_CHAR_NULL )
    {
        SetEmpty();
        return;
    }
    numChars = strlen( pszAnsi );
    SetAnsiChars( pszAnsi, numChars );
}

//===============================================================================================//
//  Description:
//      Set this string using the specified ANSI string and length
//
//  Parameters:
//      pszAnsi  - constant pointer to the ANSI string
//      numChars - number of characters
//
//  Returns:
//      void
//===============================================================================================//
void String::SetAnsiChars( const char* pszAnsi, size_t numChars )
{
    SetMultibyte( pszAnsi, numChars, CP_ACP);
}

//===============================================================================================//
//  Description:
//      Set this string using the specified character array
//
//  Parameters:
//      Chars - the array, not necessarily NULL terminated
//
//  Returns:
//      void
//===============================================================================================//
void String::SetCharArray( const CharArray& Chars )
{
    char*  pBuffer;
    size_t numChars = Chars.GetSize();
    AllocateChars NewChars;

    // Copy into a terminated buffer
    numChars = PXSAddSizeT( numChars, 1 );       // Terminator
    pBuffer  = NewChars.New( numChars );
    Chars.Get( 0, pBuffer, numChars - 1 );
    pBuffer[ numChars - 1 ] = 0x00;
    SetAnsi( pBuffer );
}

//===============================================================================================//
//  Description:
//      Set the character at the specified index
//
//  Parameters:
//      index - zero based index
//      ch    -  the character
//
//  Returns:
//      void
//===============================================================================================//
void String::SetCharAt( size_t index, wchar_t ch )
{
    if ( m_pwzString == nullptr )
    {
        throw NullException( L"m_pwzString", __FUNCTION__ );
    }

    if ( index >= m_uLengthChars )      // Inclusive
    {
        throw BoundsException( L"index", __FUNCTION__ );
    }
    m_pwzString[ index ] = ch;
}

//===============================================================================================//
//  Description:
//      Set this to the empty string ""
//
//  Parameters:
//      none
//
//  Returns:
//      void
//===============================================================================================//
void String::SetEmpty()
{
    if ( m_pwzString == nullptr )
    {
        Allocate( 1 );
    }
    Zero();
}

//===============================================================================================//
//  Description:
//      Set this string using the input byte array of wchar_t characters
//
//  Parameters:
//      WCharBytes - the bytes, size must be a whole number of wchar_t characters
//
//  Returns:
//      void
//===============================================================================================//
void String::SetFromWCharBytes( const ByteArray& WCharBytes )
{
    size_t      numBytes, numChars;
    const BYTE* pBytes;

    pBytes   = WCharBytes.GetPtr();
    numBytes = WCharBytes.GetSize();
    if ( numBytes % sizeof ( wchar_t ) )
    {
        throw ParameterException( L"WCharBytes", __FUNCTION__ );
    }
    Zero();
    numChars = numBytes / sizeof ( wchar_t );
    AppendChars( reinterpret_cast< const wchar_t* >( pBytes ), numChars );
}

//===============================================================================================//
//  Description:
//      Set this string to NULL. Same as Delete
//
//  Parameters:
//      none
//
//  Returns:
//      void
//===============================================================================================//
void String::SetNull()
{
    Delete();
}

//===============================================================================================//
//  Description:
//      Determines if this string starts with the specified character. Test is case sensitive
//
//  Parameters:
//      wch - character to test for
//
//  Returns:
//      bool
//===============================================================================================//
bool String::StartsWith( wchar_t wch ) const
{
    if ( ( m_pwzString == nullptr ) || ( wch == PXS_CHAR_NULL ) )
    {
        return false;
    }

    if ( *m_pwzString == wch )
    {
        return true;
    }

    return false;
}

//===============================================================================================//
//  Description:
//      Determines if this string starts with the specified string.
//
//  Parameters:
//      Prefix        - string to test.
//      caseSensitive - flag to determine if the test is case sensitive.
//
//  Returns:
//      bool
//===============================================================================================//
bool String::StartsWith( const String& Prefix, bool caseSensitive ) const
{
    return StartsWith( Prefix.c_str(), caseSensitive );
}

//===============================================================================================//
//  Description:
//      Determines if this string starts with the specified string.
//
//  Parameters:
//      pszPrefix     - string to test.
//      caseSensitive - flag to determine if the test is case sensitive.
//
//  Returns:
//      bool
//===============================================================================================//
bool String::StartsWith( LPCWSTR pszPrefix, bool caseSensitive ) const
{
    if ( ( m_pwzString == nullptr       ) ||
         ( pszPrefix   == nullptr       ) ||
         ( *pszPrefix  == PXS_CHAR_NULL )  )
    {
        return false;
    }

    size_t numChars = wcslen( pszPrefix );
    if ( numChars > m_uLengthChars )
    {
        return false;
    }

    if ( PXSCompareStringN( pszPrefix, m_pwzString, numChars, caseSensitive ) == 0 )
    {
        return true;
    }

    return false;
}

//===============================================================================================//
//  Description:
//      Determine if this string starts with the specified string. The match is case insensitive.
//
//  Parameters:
//      pszPrefix - string to test.
//
//  Returns:
//      bool
//===============================================================================================//
bool String::StartsWithI( LPCWSTR pszPrefix ) const
{
    return StartsWith( pszPrefix, false );
}

//===============================================================================================//
//  Description:
//      Get a substring from this string
//
//  Parameters:
//      start  - zero-based start position
//      length - number of characters
//      pSub   - string object to receive new string
//
//  Remarks:
//      If length runs beyond this string, no error is raised. The entire
//      amount beyond idxStart is put into the output. This avoids the
//      caller having to determine this string's length.
//
//  Returns:
//      void, zero length string "" if arguments out of range
//===============================================================================================//
void String::SubString( size_t start, size_t length, String* pSub ) const
{
    if ( pSub == nullptr )
    {
        throw ParameterException( L"pSub", __FUNCTION__ );
    }
    pSub->Zero();

    // Preserve NULL
    if ( IsNull() )
    {
        pSub->Delete();
        return;     // Nothing to do
    }

    if ( IsEmpty() )
    {
        pSub->SetEmpty();
        return;
    }

    // Bounds check
    if ( start < m_uLengthChars )
    {
        length = PXSMinSizeT( m_uLengthChars - start, length );
        pSub->AppendChars( m_pwzString + start, length );
    }
}

//===============================================================================================//
//  Description:
//      Break up a string into tokens that are separated by the specified
//      character
//
//  Parameters:
//      ch      - character separator, case sensitive
//      pTokens - string array to receive the tokens
//
//  Remarks:
//      Will avoid strtok because it does not return zero-length strings.
//
//  Returns:
//      Number of tokens found
//===============================================================================================//
size_t String::ToArray( wchar_t ch, StringArray* pTokens ) const
{
    size_t i = 0, counter = 0, idxStart = 0;
    String Token;

    if ( pTokens == nullptr )
    {
        throw ParameterException( L"pTokens", __FUNCTION__ );
    }
    pTokens->RemoveAll();

    if ( ( m_pwzString == nullptr ) || ( m_uLengthChars == 0 ) )
    {
        return 0;   // Nothing to do
    }

    // Get number of tokens
    for ( i = 0; i < m_uLengthChars; i++ )
    {
        if ( ch == m_pwzString[ i ] )
        {
            counter++;
        }
    }

    // If no separators, return the whole string. This also handles the case if
    // the token is NULL
    if ( counter == 0 )
    {
        pTokens->Add( m_pwzString );
        return 1;
    }

    Token.Allocate( 1024 );
    idxStart = 0;
    for ( i = 0; i < m_uLengthChars; i++ )
    {
        if ( ch == m_pwzString[ i ] )
        {
            // Extract the string
            Token = PXS_STRING_EMPTY;
            if ( i > idxStart )
            {
                Token.AppendChars( m_pwzString + idxStart, i - idxStart );
            }
            pTokens->Add( Token );
            idxStart = PXSAddSizeT( i, 1 );     // Advance
        }
    }

    // Pick up the last token, note it could be of zero length
    if ( m_uLengthChars >= idxStart )
    {
        Token = PXS_STRING_EMPTY;
        Token.AppendChars( m_pwzString + idxStart, m_uLengthChars - idxStart );
        pTokens->Add( Token );
    }

    return pTokens->GetSize();
}

//===============================================================================================//
//  Description:
//      Break up a string into tokens that are separated by the specified
//      string
//
//  Parameters:
//      pSeparator - string separator, case sensitive
//      pTokens    - string array to receive the tokens
//
//  Remarks:
//      Will avoid strtok because it does not return zero-length strings.
//
//  Returns:
//      Number of tokens found
//===============================================================================================//
size_t String::ToArray( const wchar_t* pSeparator, StringArray* pTokens ) const
{
    if ( ( pSeparator == nullptr ) || ( *pSeparator == PXS_CHAR_NULL ) )
    {
        throw ParameterException( L"pSeparator", __FUNCTION__ );
    }

    if ( pTokens == nullptr )
    {
        throw ParameterException( L"pTokens", __FUNCTION__ );
    }
    pTokens->RemoveAll();

    if ( ( m_pwzString == nullptr ) || ( m_uLengthChars == 0 ) )
    {
        return 0;   // Nothing to do
    }

    size_t   length = wcslen( pSeparator );
    wchar_t* ptr    = wcsstr( m_pwzString, pSeparator );
    if ( ptr == nullptr )
    {
        // No separators, return the whole string.
        pTokens->Add( m_pwzString );
        return 1;
    }

    size_t idxStart = 0;
    String Token;
    Token.Allocate( 1024 );
    while ( ptr )
    {
        size_t idx   = PXSCastPtrDiffToSizeT( ptr - m_pwzString );
        Token = PXS_STRING_EMPTY;
        Token.AppendChars( m_pwzString + idxStart, idx - idxStart );
        pTokens->Add( Token );
        idxStart = PXSAddSizeT( idx, length );
        if ( idxStart < m_uLengthChars )
        {
            ptr = wcsstr( m_pwzString + idxStart, pSeparator );
        }
        else
        {
            ptr = nullptr;
        }
    }

    // Pick up the last token, note it could be of zero length
    if ( idxStart < m_uLengthChars )
    {
        Token = PXS_STRING_EMPTY;
        Token.AppendChars( m_pwzString + idxStart, m_uLengthChars - idxStart );
        pTokens->Add( Token );
    }
    return pTokens->GetSize();
}

//===============================================================================================//
//  Description:
//      Convert the string to lowercase
//
//  Parameters:
//      None
//
//  Returns:
//      void
//===============================================================================================//
void String::ToLowercase()
{
    if ( m_pwzString )
    {
        CharLower( m_pwzString );        // In- place conversion
    }
}

//===============================================================================================//
//  Description:
//      Convert the string to uppercase
//
//  Parameters:
//      None
//
//  Remarks:
//      Unicode advice suggests returning a separate string as converting
//      a character to uppercase does not necessarily return a single
//      character e.g. uppercase for small letter Shape S is "SS".
//
//  Returns:
//      void
//===============================================================================================//
void String::ToUppercase()
{
    if ( m_pwzString )
    {
        CharUpper( m_pwzString );
    }
}

//===============================================================================================//
//  Description:
//      Convert thsi string to single byte US-ASCII. All chars in this string must be <= 127
//
//  Parameters:
//      None
//
//  Returns:
//      void
//===============================================================================================//
void String::ToUSAscii( CharArray* pUSAscii ) const
{
    if ( pUSAscii == nullptr )
    {
        throw ParameterException( L"pUSAscii", __FUNCTION__ );
    }
    pUSAscii->Zero();

    if ( ( m_pwzString == nullptr ) || ( m_uLengthChars == 0 ) )
    {
        return;     // Nothing to do
    }

    if ( pUSAscii->GetNumAllocated() < m_uLengthChars )
    {
        pUSAscii->Allocate( m_uLengthChars );
    }

    for ( size_t i = 0; i < m_uLengthChars; i++ )
    {
        wchar_t wch = m_pwzString[ i ];
        if ( wch > 127 )
        {
            throw BoundsException( L"wch > 127", __FUNCTION__ );
        }
        pUSAscii->Append( static_cast<char>( 0xFF & wch ) );
    }
}

//===============================================================================================//
//  Description:
//      Trim a string, characters up to x20 are removed from beginning
//      and end of string.
//
//  Parameters:
//      None
//
//  Returns:
//      void
//===============================================================================================//
void String::Trim()
{
    LeftTrim();
    RightTrim();
}

//===============================================================================================//
//  Description:
//      Truncate the string to the specified number of characters.
//
//  Parameters:
//      numChars - specifies maximum length of truncated string
//                 if numChars is greater then the string's length then this
//                 method has no effect
//  Returns:
//      void
//===============================================================================================//
void String::Truncate( size_t numChars )
{
    if ( m_pwzString == nullptr )
    {
        return;     // Nothing to do
    }

    if ( numChars < m_uLengthChars )
    {
        m_pwzString[ numChars ] = PXS_CHAR_NULL;
        m_uLengthChars = numChars;
    }
}

//===============================================================================================//
//  Description:
//      Zero any memory allocated for this string. Preserves any allocated memory
//
//  Parameters:
//      none
//
//  Returns:
//      void
//===============================================================================================//
void String::Zero()
{
    if ( m_pwzString && m_uCharsAllocated )
    {
        memset( m_pwzString, 0, sizeof (wchar_t) * m_uCharsAllocated );  // no arithmetic overflow
    }                                                                    // as already allocated.
    m_uLengthChars = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Protected Methods
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Private Methods
///////////////////////////////////////////////////////////////////////////////////////////////////

//===============================================================================================//
//  Description:
//      Set this string using the specified mutlibyte and code page
//
//  Parameters:
//      pszMB    - constant pointer to the multibyte string
//      numChars - number of characters
//      codePage - code page
//
//  Returns:
//      void
//===============================================================================================//
void String::SetMultibyte( const char* pszMB, size_t numChars, UINT codePage )
{
    int    cbMultiByte, cchWideChar;
    size_t uWideChar;

    // Preserve NULL or ""
    if ( pszMB == nullptr )
    {
        SetNull();
        return;
    }

    if ( numChars == 0 )
    {
        SetEmpty();
        return;
    }

    // Allocate the wide chars required to hold the result
    cbMultiByte = PXSCastSizeTToInt32( numChars );
    cchWideChar = MultiByteToWideChar( codePage,
                                       MB_ERR_INVALID_CHARS, pszMB, cbMultiByte, nullptr, 0 );
    if ( cchWideChar == 0 )
    {
        throw SystemException( GetLastError(), L"MultiByteToWideChar", __FUNCTION__ );
    }
    cchWideChar = PXSAddInt32( cchWideChar, 1 );    // Null Terminator
    uWideChar   = PXSCastInt32ToSizeT( cchWideChar );
    Allocate( uWideChar );

    if ( MultiByteToWideChar( codePage,
                              MB_ERR_INVALID_CHARS,
                              pszMB, cbMultiByte, m_pwzString, cchWideChar ) == 0 )
    {
        throw SystemException( GetLastError(), L"MultiByteToWideChar", __FUNCTION__ );
    }
    m_uLengthChars = uWideChar - 1;
    m_pwzString[ m_uLengthChars ] = PXS_CHAR_NULL;
}
