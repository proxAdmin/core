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
#include "../../Structures/IStruct.h"
#include "TabStops.h"


namespace PPT_FORMAT
{

struct STextRuler : public IStruct
{
    bool m_fDefaultTabSize;
    bool m_fCLevels;
    bool m_fTabStops;

    bool m_fLeftMargin1;
    bool m_fLeftMargin2;
    bool m_fLeftMargin3;
    bool m_fLeftMargin4;
    bool m_fLeftMargin5;

    bool m_fIndent1;
    bool m_fIndent2;
    bool m_fIndent3;
    bool m_fIndent4;
    bool m_fIndent5;

    nullable<SHORT>     m_cLevels;
    nullable<SHORT>     m_defaultTabSize;
    nullable<STabStops> m_tabs;

    nullable<SHORT> m_leftMargin1;
    nullable<SHORT> m_leftMargin2;
    nullable<SHORT> m_leftMargin3;
    nullable<SHORT> m_leftMargin4;
    nullable<SHORT> m_leftMargin5;

    nullable<SHORT> m_indent1;
    nullable<SHORT> m_indent2;
    nullable<SHORT> m_indent3;
    nullable<SHORT> m_indent4;
    nullable<SHORT> m_indent5;


    virtual void ReadFromStream (POLE::Stream* pStream)
    {
        USHORT flags = StreamUtils::ReadDWORD(pStream);

        m_fDefaultTabSize   = 0x1 & flags;
        m_fCLevels          = 0x2 & flags;
        m_fTabStops         = 0x4 & flags;

        m_fLeftMargin1      = 0x08 & flags;
        m_fLeftMargin2      = 0x10 & flags;
        m_fLeftMargin3      = 0x20 & flags;
        m_fLeftMargin4      = 0x40 & flags;
        m_fLeftMargin5      = 0x80 & flags;

        m_fIndent1          = 0x0100 & flags;
        m_fIndent2          = 0x0200 & flags;
        m_fIndent3          = 0x0400 & flags;
        m_fIndent4          = 0x0800 & flags;
        m_fIndent5          = 0x1000 & flags;


        if (m_fCLevels)
            m_cLevels           = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_fDefaultTabSize)
            m_defaultTabSize    = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_fTabStops)
        {
            m_tabs = new STabStops;
            m_tabs->ReadFromStream(pStream);
        }
        if (m_fLeftMargin1)
            m_leftMargin1       = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_fIndent1)
            m_indent1           = new SHORT(StreamUtils::ReadSHORT(pStream));

        if (m_fLeftMargin2)
            m_leftMargin2       = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_fIndent2)
            m_indent2           = new SHORT(StreamUtils::ReadSHORT(pStream));

        if (m_fLeftMargin3)
            m_leftMargin3       = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_fIndent3)
            m_indent3           = new SHORT(StreamUtils::ReadSHORT(pStream));

        if (m_fLeftMargin4)
            m_leftMargin4       = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_fIndent4)
            m_indent4           = new SHORT(StreamUtils::ReadSHORT(pStream));

        if (m_fLeftMargin5)
            m_leftMargin5       = new SHORT(StreamUtils::ReadSHORT(pStream));
        if (m_fIndent5)
            m_indent5           = new SHORT(StreamUtils::ReadSHORT(pStream));
    }
};

}
