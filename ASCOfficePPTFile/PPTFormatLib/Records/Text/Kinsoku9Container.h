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
#include "KinsokuFollowingAtom.h"
#include "KinsokuLeadingAtom.h"


namespace PPT_FORMAT
{
class CRecordKinsoku9Atom : public CUnknownRecord
{
public:
    BYTE m_korLevel;    // 2 bits
    BYTE m_scLevel;     // 2 bits
    BYTE m_tcLevel;     // 2 bits
    BYTE m_jpnLevel;    // 2 bits
    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;

        _UINT32 flags = StreamUtils::ReadDWORD(pStream);
        m_korLevel = 0x03 & flags;
        m_scLevel  = 0x0C & flags;
        m_tcLevel  = 0x30 & flags;
        m_jpnLevel = 0xC0 & flags;
    }
};

class CRecordKinsoku9Container : public CUnknownRecord
{
public:
    CRecordKinsoku9Atom                     m_kinsoku9Atom;
    nullable<CRecordKinsokuLeadingAtom>     m_kinsokuLeadingAtom;
    nullable<CRecordKinsokuFollowingAtom>   m_kinsokuFollowingAtom;

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;

        SRecordHeader ReadHeader;
        ReadHeader.ReadFromStream(pStream);
        m_kinsoku9Atom.ReadFromStream(ReadHeader, pStream);

        if (
                m_kinsoku9Atom.m_korLevel == 2 ||
                m_kinsoku9Atom.m_scLevel  == 2 ||
                m_kinsoku9Atom.m_tcLevel  == 2 ||
                m_kinsoku9Atom.m_jpnLevel == 2
           )
        {
            ReadHeader.ReadFromStream(pStream);
            m_kinsokuLeadingAtom = new CRecordKinsokuLeadingAtom;
            m_kinsokuLeadingAtom->ReadFromStream(ReadHeader, pStream);

            ReadHeader.ReadFromStream(pStream);
            m_kinsokuFollowingAtom = new CRecordKinsokuFollowingAtom;
            m_kinsokuFollowingAtom->ReadFromStream(ReadHeader, pStream);
        }
    }
};
}
