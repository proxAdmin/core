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

#include "KinsokuContainer.h"
#include "FontCollectionContainer.h"
#include "TextCFException.h"
#include "TextPFException.h"
#include "DefaultRulerAtom.h"
#include "TextSIException.h"
#include "TextMasterStyleAtom.h"

namespace PPT_FORMAT
{
class CRecordDocumentTextInfoContainer : public CUnknownRecord
{
public:
    nullable<CRecordKinsokuContainer> m_kinsoku;
    nullable<CRecordFontCollectionContainer> m_fontCollection;
    nullable<CRecordTextCFExceptionAtom> m_textCFDefaultsAtom;
    nullable<CRecordTextPFExceptionAtom> m_textPFDefaultsAtom;
    nullable<CRecordDefaultRulerAtom> m_defaultRulerAtom;
    nullable<CRecordTextSIExceptionAtom> m_textSIDefaultsAtom;
    nullable<CRecordTextMasterStyleAtom> m_textMasterStyleAtom;


    virtual void ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
    {
        m_oHeader			=	oHeader;
        LONG lPos			=	0;
        StreamUtils::StreamPosition ( lPos, pStream );

        UINT lCurLen		=	0;

        SRecordHeader ReadHeader;

        while ( lCurLen < m_oHeader.RecLen )
        {
            if ( ReadHeader.ReadFromStream(pStream) == false)
                break;

            lCurLen += 8 + ReadHeader.RecLen;

            switch (m_oHeader.RecType)
            {
            case RT_Kinsoku:
            {
                m_kinsoku = new CRecordKinsokuContainer;
                m_kinsoku->ReadFromStream(ReadHeader, pStream);
                break;
            }
            case RT_FontCollection:
            {
                m_fontCollection = new CRecordFontCollectionContainer;
                m_fontCollection->ReadFromStream(ReadHeader, pStream);
                break;
            }
            case RT_TextCharFormatExceptionAtom:
            {
                m_textCFDefaultsAtom = new CRecordTextCFExceptionAtom;
                m_textCFDefaultsAtom->ReadFromStream(ReadHeader, pStream);
                break;
            }
            case RT_TextParagraphFormatExceptionAtom:
            {
                m_textPFDefaultsAtom = new CRecordTextPFExceptionAtom;
                m_textPFDefaultsAtom->ReadFromStream(ReadHeader, pStream);
                break;
            }
            case RT_DefaultRulerAtom:
            {
                m_defaultRulerAtom = new CRecordDefaultRulerAtom;
                m_defaultRulerAtom->ReadFromStream(ReadHeader, pStream);
                break;
            }
            case RT_TextSpecialInfoDefaultAtom:
            {
                m_textSIDefaultsAtom = new CRecordTextSIExceptionAtom;
                m_textSIDefaultsAtom->ReadFromStream(ReadHeader, pStream);
                break;
            }
            case RT_TextMasterStyleAtom:
            {
                m_textMasterStyleAtom = new CRecordTextMasterStyleAtom;
                m_textMasterStyleAtom->ReadFromStream(ReadHeader, pStream);
                break;
            }
            default:
                StreamUtils::StreamSkip(ReadHeader.RecLen, pStream);
                break;
            }
        }
        StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
    }
};
}
