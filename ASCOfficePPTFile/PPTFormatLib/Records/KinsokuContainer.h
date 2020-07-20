/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once
#include "../Reader/Records.h"
#include "KinsokuAtom.h"
#include "KinsokuFollowingAtom.h"
#include "KinsokuLeadingAtom.h"

class CRecordKinsokuContainer : public CUnknownRecord
{
public:
    _UINT32 m_nLevel;

    CRecordKinsokuContainer()
    {
    }

    ~CRecordKinsokuContainer()
    {
    }

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
        if( IsCorrect () == false ) return;
#endif
        LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

        UINT res = 0;
        SRecordHeader ReadHeader;
        if ( ReadHeader.ReadFromStream(pStream) )
            setBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

        //if ( m_oSetBehaviorAtom.m_bToPropertyUsed )
        //{
        if ( ReadHeader.ReadFromStream(pStream) )
            varTo.ReadFromStream ( ReadHeader, pStream );
        //}

        if ( ReadHeader.ReadFromStream(pStream) )
            behavior.ReadFromStream ( ReadHeader, pStream );

        StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
    }


    virtual bool IsCorrect ()
    {
        return  m_oHeader.RecVersion == 0xF	&&
                m_oHeader.RecInstance == 0x2 &&
                m_oHeader.RecType == RT_Kinsoku;
    }

public:

    CRecordKinsokuAtom          m_KinsokuAtom;
    CRecordKinsokuLeadingAtom   m_KinsokuLeadingAtom;
    CRecordKinsokuFollowingAtom	m_KinsokuFollowingAtom;

};