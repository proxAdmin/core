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


namespace PPT_FORMAT
{
class CRecordFontEntityAtom : public CUnknownRecord
{
public:
    std::wstring    m_lfFaceName;
    BYTE            m_lfCharSet;

    bool m_fEmbedSubsetted;
    bool m_rasterFontType;
    bool deviceFontType;
    bool m_truetypeFontType;
    bool m_fNoFontSubstitution;

    BYTE m_lfPitchAndFamily;

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;

        m_lfFaceName.clear();
        LONG lPos(0); StreamUtils::StreamPosition(lPos, pStream);
        for (int i = 0; i < 32; i++)
        {
            WCHAR symbol = StreamUtils::ReadWORD(pStream);
            if (symbol == L'\0')
                break;

            m_lfFaceName.push_back(symbol);
        }
        StreamUtils::StreamSeek(lPos + 64, pStream);

        m_lfCharSet             = StreamUtils::ReadBYTE(pStream);
        m_fEmbedSubsetted = 0x1 & StreamUtils::ReadBYTE(pStream);

        BYTE flags              = StreamUtils::ReadBYTE(pStream);
        m_rasterFontType        = 0x01 & flags;
        deviceFontType          = 0x02 & flags;
        m_truetypeFontType      = 0x04 & flags;
        m_fNoFontSubstitution   = 0x08 & flags;

        m_lfPitchAndFamily      = StreamUtils::ReadBYTE(pStream);

    }

};

class CRecordFontEmbedDataBlob : public CUnknownRecord
{
public:
    std::pair<boost::shared_array<unsigned char>, _INT32> data;
        //font data of an embedded font

    CRecordFontEmbedDataBlob(){}
    ~CRecordFontEmbedDataBlob(){}

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;

        data = std::make_pair(boost::shared_array<unsigned char>(new unsigned char[m_oHeader.RecLen]), m_oHeader.RecLen);
        pStream->read(data.first.get(), data.second);
    }
};

class CRecordFontCollectionEntry : public CUnknownRecord
{
public:
    CRecordFontEntityAtom m_fontEntityAtom;

    nullable<CRecordFontEmbedDataBlob> m_fontEmbedData1;
    nullable<CRecordFontEmbedDataBlob> m_fontEmbedData2;
    nullable<CRecordFontEmbedDataBlob> m_fontEmbedData3;
    nullable<CRecordFontEmbedDataBlob> m_fontEmbedData4;

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;

        LONG lCurPos(0); StreamUtils::StreamPosition(lCurPos, pStream);

        SRecordHeader ReadHeader;
        ReadHeader.ReadFromStream(pStream);
        m_fontEntityAtom.ReadFromStream(ReadHeader, pStream);
        lCurPos += 8 + ReadHeader.RecLen;

        if (lCurPos < m_oHeader.RecLen)
        {
            ReadHeader.ReadFromStream(pStream);
            m_fontEmbedData1 = new CRecordFontEmbedDataBlob;
            m_fontEmbedData1->ReadFromStream(ReadHeader, pStream);
            lCurPos += 8 + ReadHeader.RecLen;
        }

        if (lCurPos < m_oHeader.RecLen)
        {
            ReadHeader.ReadFromStream(pStream);
            m_fontEmbedData2 = new CRecordFontEmbedDataBlob;
            m_fontEmbedData2->ReadFromStream(ReadHeader, pStream);
            lCurPos += 8 + ReadHeader.RecLen;
        }

        if (lCurPos < m_oHeader.RecLen)
        {
            ReadHeader.ReadFromStream(pStream);
            m_fontEmbedData3 = new CRecordFontEmbedDataBlob;
            m_fontEmbedData3->ReadFromStream(ReadHeader, pStream);
            lCurPos += 8 + ReadHeader.RecLen;
        }

        if (lCurPos < m_oHeader.RecLen)
        {
            ReadHeader.ReadFromStream(pStream);
            m_fontEmbedData4 = new CRecordFontEmbedDataBlob;
            m_fontEmbedData4->ReadFromStream(ReadHeader, pStream);
            lCurPos += 8 + ReadHeader.RecLen;
        }
    }
};
}
