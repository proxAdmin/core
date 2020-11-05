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
#include "CFMasks.h"
#include "../../Structures/ColorIndexStruct.h"


namespace PPT_FORMAT
{
struct SCFStyle : public IStruct
{
    bool m_bold;
    bool m_italic;
    bool m_underline;
    // unused
    bool m_shadow;
    bool m_fehint;
    // unused
    bool m_kumi;
    // unused
    bool m_emboss;
    BYTE m_pp9rt;       // 4 bits

    virtual void ReadFromStream(POLE::Stream* pStream)
    {
        USHORT flags = StreamUtils::ReadWORD(pStream);

        m_bold          = 0x1 & flags;
        m_italic        = 0x2 & flags;
        m_underline     = 0x3 & flags;
        // unused
        m_shadow        = 0x08 & flags;
        m_fehint        = 0x10 & flags;
        // unused
        m_kumi          = 0x40 & flags;
        // unused
        m_emboss        = 0x0100 & flags;
        m_pp9rt         = 0x3C00 & flags;
    }

};

struct STextCFException : public IStruct
{
    SCFMasks                    m_masks;
    nullable<SCFStyle>          m_fontStyle;
    nullable<USHORT>            m_fontRef;
    nullable<USHORT>            m_oldEAFontRef;
    nullable<USHORT>            m_ansiFontRef;
    nullable<USHORT>            m_symbolFontRef;
    nullable<SHORT>             m_fontSize;
    nullable<ColorIndexStruct>  m_color;
    nullable<SHORT>             m_position;

    virtual void ReadFromStream(POLE::Stream* pStream)
    {
        m_masks.ReadFromStream(pStream);

        if (   m_masks.m_bold   || m_masks.m_italic || m_masks.m_underline ||
               m_masks.m_shadow || m_masks.m_fehint || m_masks.m_kumi ||
               m_masks.m_emboss || m_masks.m_fHasStyle)
        {
            m_fontStyle = new SCFStyle;
            m_fontStyle->ReadFromStream(pStream);
        }

        if (m_masks.m_typeface)
            m_fontRef       = new USHORT(StreamUtils::ReadWORD(pStream));
        if (m_masks.m_oldEATypeface)
            m_oldEAFontRef  = new USHORT(StreamUtils::ReadWORD(pStream));
        if (m_masks.m_ansiTypeface)
            m_ansiFontRef   = new USHORT(StreamUtils::ReadWORD(pStream));
        if (m_masks.m_symbolTypeface)
            m_symbolFontRef = new USHORT(StreamUtils::ReadWORD(pStream));
        if (m_masks.m_size)
            m_fontSize      = new  SHORT(StreamUtils::ReadWORD(pStream));

        if (m_masks.m_color)
        {
            m_color = new ColorIndexStruct;
            m_color->ReadFromStream(pStream);
        }
        if (m_masks.m_position)
            m_position = new SHORT(StreamUtils::ReadSHORT(pStream));
    }
};

class CRecordTextCFExceptionAtom : public CUnknownRecord
{
public:
    STextCFException m_cf;

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;

        m_cf.ReadFromStream(pStream);
    }
};
}
