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
#include "OutlineTextRefAtom.h"
#include "TextHeaderAtom.h"
#include "TextCharsAtom.h"
#include "TextBytesAtom.h"
#include "StyleTextPropAtom.h"
#include "SlideNumberMCAtom.h"
#include "DateTimeMCAtom.h"
#include "GenericDateMCAtom.h"
#include "HeaderMCAtom.h"
#include "FooterMCAtom.h"
#include "RTFDateTimeMCAtom.h"
#include "TextBookmarkAtom.h"
#include "TextSpecialInfoAtom.h"
#include "../SlideShow/InteractiveInfoInstance.h"
#include "TextInteractiveInfoInstance.h"
#include "TextRulerAtom.h"
#include "MasterTextPropAtom.h"


namespace PPT_FORMAT
{
IRecord* TextClientDataSubContainerOrAtomFactory(SRecordHeader & oHeader)
{
    switch (oHeader.RecType)
    {
    case RT_OutlineTextRefAtom:
    {
        return new CRecordOutlineTextRefAtom;

        break;
    }
    case RT_TextHeaderAtom:
    {
        return new CRecordTextHeaderAtom;
        break;
    }
    case RT_TextCharsAtom:
    {
        return new CRecordTextCharsAtom;
        break;
    }
    case RT_TextBytesAtom:
    {
        return new CRecordTextBytesAtom;
        break;
    }
    case RT_StyleTextPropAtom:
    {
        return new CRecordStyleTextPropAtom;
        break;
    }
    case RT_SlideNumberMetaCharAtom:
    {
        return new CRecordSlideNumberMCAtom;
        break;
    }
    case RT_DateTimeMetaCharAtom:
    {
        return new CRecordDateTimeMCAtom;
        break;
    }
    case RT_GenericDateMetaCharAtom:
    {
        return new CRecordGenericDateMCAtom;
        break;
    }
    case RT_HeaderMetaCharAtom:
    {
        return new CRecordHeaderMCAtom;
        break;
    }
    case RT_FooterMetaCharAtom:
    {
        return new CRecordOutlineTextRefAtom;
        break;
    }
    case RT_RtfDateTimeMetaCharAtom:
    {
        return new CRecordRTFDateTimeMCAtom;
        break;
    }
    case RT_TextBookmarkAtom:
    {
        return new CRecordTextBookmarkAtom;
        break;
    }
    case RT_TextSpecialInfoAtom:
    {
        return new CRecordTextSpecialInfoAtom;
        break;
    }
    case RT_InteractiveInfo:
    {
        return InteractiveInfoInstanceFactory(oHeader);
        break;
    }
    case RT_TextRulerAtom:
    {
        return new CRecordTextRulerAtom;
        break;
    }
    case RT_MasterTextPropAtom:
    {
        return new CRecordMasterTextPropAtom;
        break;
    }
    default:
        return new CUnknownRecord;
    }
}

class CRecordOfficeArtClientTextbox : public CUnknownRecord
{
public:
    std::vector<IRecord*> m_rgChildRec;

    ~CRecordOfficeArtClientTextbox()
    {
        for (auto pRec : m_rgChildRec)
            RELEASEOBJECT(pRec)
    }

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;
        m_rgChildRec.clear();

        LONG lPosition = 0;
        StreamUtils::StreamPosition(lPosition, pStream);

        m_oHeader = oHeader;

        UINT lCurLen = 0;
        SRecordHeader oRec;
        while (lCurLen < m_oHeader.RecLen)
        {
            if (oRec.ReadFromStream(pStream) == FALSE )
            {
                break;
            }

            IRecord* pRecord = TextClientDataSubContainerOrAtomFactory(oRec);
            pRecord->ReadFromStream(oRec, pStream);

            m_rgChildRec.push_back(pRecord);
            lCurLen += (8 + oRec.RecLen);
        }

        StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
    }
};
}
