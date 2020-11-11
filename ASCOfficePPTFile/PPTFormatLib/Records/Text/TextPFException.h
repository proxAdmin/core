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
#include "PFMasks.h"
#include "TabStops.h"
#include "../../Structures/ColorIndexStruct.h"

namespace PPT_FORMAT
{
struct SBulletFlags : public IStruct
{
    bool m_fHasBullet;
    bool m_fBulletHasFont;
    bool m_fBulletHasColor;
    bool m_fBulletHasSize;

    virtual void ReadFromStream(POLE::Stream* pStream)
    {
        USHORT flags = StreamUtils::ReadWORD(pStream);

        m_fHasBullet        = 0x01 & flags;
        m_fBulletHasFont    = 0x02 & flags;
        m_fBulletHasColor   = 0x04 & flags;
        m_fBulletHasSize    = 0x08 & flags;
    }
};

struct SPFWrapFlags : public IStruct
{
    bool m_charWrap;
    bool m_wordWrap;
    bool m_overflow;

    virtual void ReadFromStream(POLE::Stream* pStream)
    {
        USHORT flags = StreamUtils::ReadWORD(pStream);
        m_charWrap  = 0x01 & flags;
        m_wordWrap  = 0x02 & flags;
        m_overflow  = 0x04 & flags;
    }
};

struct STextPFException : public IStruct
{
    PFMasks m_masks;

    nullable<SBulletFlags>  m_bulletFlags;
    nullable<WCHAR>         m_bulletChar;
    nullable<USHORT>        m_bulletFontRef;
    nullable<SHORT>         m_bulletSize;

    nullable<ColorIndexStruct>  m_bulletColor;

    nullable<TextAlignmentEnum> m_textAlignment;
    nullable<SHORT>             m_lineSpacing;
    nullable<SHORT>             m_spaceBefore;
    nullable<SHORT>             m_spaceAfter;
    nullable<SHORT>             m_leftMargin;
    nullable<SHORT>             m_indent;
    nullable<SHORT>             m_defaultTabSize;

    nullable<STabStops>             m_tabStops;

    nullable<TextFontAlignmentEnum> m_fontAlign;
    nullable<SPFWrapFlags>          m_wrapFlags;
    nullable<TextDirectionEnum>     m_textDirection;


    void ReadFromStream(POLE::Stream* pStream)
    {
        m_masks.ReadFromStream(pStream);

        if (    m_masks.m_hasBullet || m_masks.m_bulletHasFont ||
                m_masks.m_bulletHasColor || m_masks.m_bulletHasSize)
        {
            m_bulletFlags = new SBulletFlags;
            m_bulletFlags->ReadFromStream(pStream);
        }
        if (m_masks.m_bulletChar)
            m_bulletChar = new WCHAR(StreamUtils::ReadWORD(pStream));
        if (m_masks.m_bulletHasFont)
            m_bulletFontRef = new USHORT(StreamUtils::ReadWORD(pStream));
        if (m_masks.m_bulletSize)
            m_bulletSize = new SHORT(StreamUtils::ReadWORD(pStream));
        if (m_masks.m_bulletColor)
        {
            m_bulletColor = new ColorIndexStruct;
            m_bulletColor->ReadFromStream(pStream);
        }
        if (m_masks.m_align)
            m_textAlignment     = new TextAlignmentEnum(
                        (TextAlignmentEnum)StreamUtils::ReadSHORT(pStream));
        if (m_masks.m_lineSpacing)
            m_lineSpacing       = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_masks.m_spaceBefore)
            m_spaceBefore       = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_masks.m_spaceAfter)
            m_spaceAfter        = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_masks.m_leftMargin)
            m_indent            = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_masks.m_defaultTabSize)
            m_defaultTabSize    = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_masks.m_tabStops)
        {
            m_tabStops = new STabStops;
            m_tabStops->ReadFromStream(pStream);
        }
        if (m_masks.m_fontAlign)
            m_fontAlign = new TextFontAlignmentEnum(
                        (TextFontAlignmentEnum)StreamUtils::ReadWORD(pStream));
        if (m_masks.m_charWrap || m_masks.m_wordWrap || m_masks.m_overflow)
        {
            m_wrapFlags = new SPFWrapFlags;
            m_wrapFlags->ReadFromStream(pStream);
        }
        if (m_masks.m_textDirection)
            m_textDirection = new TextDirectionEnum(
                        (TextDirectionEnum)StreamUtils::ReadWORD(pStream));
    }
};

class CRecordTextPFExceptionAtom : public CUnknownRecord
{
public:
    STextPFException m_pf;

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;

        StreamUtils::StreamSkip(2, pStream);

        m_pf.ReadFromStream(pStream);
    }
};
}
