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

#include "../../Structures/IStruct.h"
#include "TextPFException.h"
#include "TextCFException.h"

namespace PPT_FORMAT
{
struct STextPFRun : public IStruct
{
    _UINT32             m_count;
    USHORT              m_indentLevel;
    STextPFException    m_pf;

    virtual void ReadFromStream (POLE::Stream* pStream)
    {
        m_count         = StreamUtils::ReadDWORD(pStream);
        m_indentLevel   = StreamUtils::ReadWORD(pStream);
        m_pf.ReadFromStream(pStream);
    }
};

struct STextCFRun : public IStruct
{
    _UINT32             m_count;
    STextCFException    m_cf;

    virtual void ReadFromStream (POLE::Stream* pStream)
    {
        m_count         = StreamUtils::ReadDWORD(pStream);
        m_cf.ReadFromStream(pStream);
    }
};

class CRecordStyleTextPropAtom : public CUnknownRecord
{
public:
    // нужно еще хранить позицию в стриме,
    // указатель на начало
    LONG m_lOffsetInStream;

    // просто данные. будут обновляться по мере прибытия
    // Text.Bytes/Chars.Atom
    _UINT32 m_count;

    std::vector<STextPFRun> m_arrPFs;
    std::vector<STextCFRun> m_arrCFs;

    CRecordStyleTextPropAtom()
    {
        m_count = 0;
        m_lOffsetInStream = 0;
    }

    ~CRecordStyleTextPropAtom()
    {
        m_count = 0;
    }

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;
        StreamUtils::StreamPosition(m_lOffsetInStream, pStream);

        _UINT32 lMemCount = 0;
        _UINT32 lCountItems = 0;

        if (0 == m_count)
        {
            StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
            return;
        }

        while (lMemCount < m_count + 1)
        {
            STextPFRun elm;
            m_arrPFs.push_back(elm);
            m_arrPFs[lCountItems].ReadFromStream(pStream);
            lMemCount += m_arrPFs[lCountItems].m_count;

            ++lCountItems;
        }

        lMemCount = 0;
        lCountItems = 0;
        while (lMemCount < m_count + 1)
        {
            STextCFRun elm;
            m_arrCFs.push_back(elm);

            m_arrCFs[lCountItems].ReadFromStream(pStream);
            lMemCount += m_arrCFs[lCountItems].m_count;

            ++lCountItems;
        }

        // на всякий случай...
        // здесь когда текст сначала другой (т.е. например - placeholder в мастере) -
        // то у нас неправильно выставился m_count...
        StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
    }

};
}
