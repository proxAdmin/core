#include "CXmlElement.h"

CXmlElement::CXmlElement()
{

}

CXmlElement::CXmlElement(std::wstring sNameDefaultElement)
{
    CreateDefaultElement(sNameDefaultElement);
}

void CXmlElement::CreateDefaultElement(std::wstring sNameDefaultElement)
{
    Clear();
    if (sNameDefaultElement == L"normal")
    {
        SetType(L"paragraph");
        SetStyleId(L"normal");
        SetS(L"1");
        SetName(L"Normal");
        SetQFormat(true);
    }
    else if (sNameDefaultElement == L"li")
    {
        SetType(L"paragraph");
        SetStyleId(L"li");
        SetName(L"List Paragraph");
        SetBasedOn(L"normal");
        SetQFormat(true);
        SetUiPriority(L"34");
        SetContextualSpacing(L"true");
        SetInd(L"w:left=\"720\"");
    }
    else if (sNameDefaultElement == L"h1")
    {
        SetType(L"paragraph");
        SetStyleId(L"h1");
        SetName(L"Heading 1");
        SetBasedOn(L"normal");
        SetLink(L"title1-c");
        SetQFormat(true);
        SetB(true);
        SetColor(L"000000");
        SetSz(L"48");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"480\"");
        SetOutlineLvl(L"0");
    }
    else if (sNameDefaultElement == L"h2")
    {
        SetType(L"paragraph");
        SetStyleId(L"h2");
        SetName(L"Heading 2");
        SetBasedOn(L"normal");
        SetLink(L"title2-c");
        SetQFormat(true);
        SetUnhideWhenUsed(true);
        SetB(true);
        SetColor(L"000000");
        SetSz(L"40");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"200\"");
        SetOutlineLvl(L"1");
    }
    else if (sNameDefaultElement == L"h3")
    {
        SetType(L"paragraph");
        SetStyleId(L"h3");
        SetName(L"Heading 3");
        SetBasedOn(L"normal");
        SetLink(L"title3-c");
        SetQFormat(true);
        SetUnhideWhenUsed(true);
        SetB(true);
        SetI(true);
        SetColor(L"000000");
        SetSz(L"36");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"200\"");
        SetOutlineLvl(L"2");
    }
    else if (sNameDefaultElement == L"h4")
    {
        SetType(L"paragraph");
        SetStyleId(L"h4");
        SetName(L"Heading 4");
        SetBasedOn(L"normal");
        SetLink(L"title4-c");
        SetQFormat(true);
        SetUnhideWhenUsed(true);
        SetColor(L"232323");
        SetSz(L"32");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"200\"");
        SetOutlineLvl(L"3");
    }
    else if (sNameDefaultElement == L"h5")
    {
        SetType(L"paragraph");
        SetStyleId(L"h5");
        SetName(L"Heading 5");
        SetBasedOn(L"normal");
        SetLink(L"title5-c");
        SetQFormat(true);
        SetUnhideWhenUsed(true);
        SetB(true);
        SetColor(L"444444");
        SetSz(L"28");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"200\"");
        SetOutlineLvl(L"4");
    }
    else if (sNameDefaultElement == L"h6")
    {
        SetType(L"paragraph");
        SetStyleId(L"h6");
        SetName(L"Heading 6");
        SetBasedOn(L"normal");
        SetLink(L"title6-c");
        SetQFormat(true);
        SetUnhideWhenUsed(true);
        SetI(true);
        SetColor(L"232323");
        SetSz(L"28");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"200\"");
        SetOutlineLvl(L"5");
    }
    else if (sNameDefaultElement == L"title1-c")
    {
        SetType(L"character");
        SetStyleId(L"title1-c");
        SetCustomStyle(L"1");
        SetName(L"Title 1 Sign");
        SetLink(L"h1");
        SetUiPriority(L"9");;
        SetRFonts(L"Arial");
        SetSz(L"40");
    }
    else if (sNameDefaultElement == L"title2-c")
    {
        SetType(L"character");
        SetStyleId(L"title2-c");
        SetCustomStyle(L"1");
        SetName(L"Title 2 Sign");
        SetLink(L"h2");
        SetUiPriority(L"9");;
        SetRFonts(L"Arial");
        SetSz(L"34");
    }
    else if (sNameDefaultElement == L"title3-c")
    {
        SetType(L"character");
        SetStyleId(L"title3-c");
        SetCustomStyle(L"1");
        SetName(L"Title 3 Sign");
        SetLink(L"h3");
        SetUiPriority(L"9");;
        SetRFonts(L"Arial");
        SetSz(L"30");
    }
    else if (sNameDefaultElement == L"title4-c")
    {
        SetType(L"character");
        SetStyleId(L"title4-c");
        SetCustomStyle(L"1");
        SetName(L"Title 4 Sign");
        SetLink(L"h4");
        SetUiPriority(L"9");;
        SetRFonts(L"Arial");
        SetSz(L"26");
    }
    else if (sNameDefaultElement == L"title5-c")
    {
        SetType(L"character");
        SetStyleId(L"title5-c");
        SetCustomStyle(L"1");
        SetName(L"Title 5 Sign");
        SetLink(L"h5");
        SetUiPriority(L"9");;
        SetRFonts(L"Arial");
        SetSz(L"24");
    }
    else if (sNameDefaultElement == L"title6-c")
    {
        SetType(L"character");
        SetStyleId(L"title6-c");
        SetCustomStyle(L"1");
        SetName(L"Title 6 Sign");
        SetLink(L"h6");
        SetUiPriority(L"9");;
        SetRFonts(L"Arial");
        SetSz(L"22");
    }
    else if (sNameDefaultElement == L"p")
    {
        SetType(L"paragraph");
        SetStyleId(L"p");
        SetCustomStyle(L"1");
        SetName(L"Paragraph");
        SetBasedOn(L"normal");
        SetLink(L"p-c");
        SetQFormat(true);
        SetInd(L"w:firstLine=\"567\"");
        SetJc(L"both");
    }
    else if (sNameDefaultElement == L"div-c")
    {
        SetType(L"character");
        SetStyleId(L"div-c");
        SetCustomStyle(L"1");
        SetName(L"Paragraph_character");
        SetLink(L"div");
    }
    else if (sNameDefaultElement == L"div")
    {
        SetType(L"paragraph");
        SetStyleId(L"div");
        SetCustomStyle(L"1");
        SetName(L"Paragraph");
        SetBasedOn(L"normal");
        SetLink(L"div-c");
        SetQFormat(true);
        SetInd(L"w:firstLine=\"567\"");
        SetJc(L"both");
    }
    else if (sNameDefaultElement == L"a")
    {
        SetType(L"character");
        SetStyleId(L"a");
        SetName(L"Hyperlink");
        SetUiPriority(L"99");
        SetUnhideWhenUsed(true);
        SetColor(L"0563C1");
        SetU(L"single");
    }
}

void CXmlElement::Clear()
{
    m_sType.clear();
    m_sStyleId.clear();
    m_sDefault.clear();
    m_sCustomStyle.clear();
    m_sS.clear();

    m_sName.clear();
    m_sBasedOn.clear();
    m_sLink.clear();
    m_sUiPriority.clear();
    m_bQFormat = false;
    m_bUnhideWhenUsed = false;

    // <rPr>
        m_bB = false;
        m_bI = false;
        m_sU.clear();
        m_sRFonts.clear();
        m_sColor.clear();
        m_sSz.clear();
    // </rPr>

    // <pPr>
        m_bKeepLines = false;
        m_bKeepNext = false;
        m_sSpacing.clear();
        m_sContextualSpacing.clear();
        m_sOutlineLvl.clear();
        m_sInd.clear();
        m_sJc.clear();
    // </pPr>
}

void CXmlElement::SetType(std::wstring sType)
{
    m_sType = sType;
}

void CXmlElement::SetStyleId(std::wstring sStyleId)
{
    m_sStyleId = sStyleId;
}

void CXmlElement::SetDefault(std::wstring sDefault)
{
    m_sDefault = sDefault;
}

void CXmlElement::SetCustomStyle(std::wstring sCustomStyle)
{
    m_sCustomStyle = sCustomStyle;
}

void CXmlElement::SetS(std::wstring sS)
{
    m_sS = sS;
}

void CXmlElement::SetName(std::wstring sName)
{
    m_sName = sName;
}

void CXmlElement::SetBasedOn(std::wstring sBasedOn)
{
    m_sBasedOn = sBasedOn;
}

void CXmlElement::SetLink(std::wstring sLink)
{
    m_sLink = sLink;
}

void CXmlElement::SetUiPriority(std::wstring sUiPriority)
{
    m_sUiPriority = sUiPriority;
}

void CXmlElement::SetQFormat(bool bQFormat)
{
    m_bQFormat = bQFormat;
}

void CXmlElement::SetUnhideWhenUsed(bool bUnhideWhenUsed)
{
    m_bUnhideWhenUsed = bUnhideWhenUsed;
}

void CXmlElement::SetB(bool bB)
{
    m_bB = bB;
}

void CXmlElement::SetI(bool bI)
{
    m_bI = bI;
}

void CXmlElement::SetU(std::wstring sU)
{
    m_sU = sU;
}

void CXmlElement::SetRFonts(std::wstring sRFonts)
{
    m_sRFonts = sRFonts;
}

void CXmlElement::SetColor(std::wstring sColor)
{
    m_sColor = sColor;
}

void CXmlElement::SetSz(std::wstring sSz)
{
    m_sSz = sSz;
}

void CXmlElement::SetKeepLines(bool bKeepLines)
{
    m_bKeepLines = bKeepLines;
}

void CXmlElement::SetKeepNext(bool bKeepNext)
{
    m_bKeepNext = bKeepNext;
}

void CXmlElement::SetSpacing(std::wstring sSpacing)
{
    m_sSpacing = sSpacing;
}

void CXmlElement::SetOutlineLvl(std::wstring sOutlineLvl)
{
    m_sOutlineLvl = sOutlineLvl;
}

void CXmlElement::SetContextualSpacing(std::wstring sContextualSpacing)
{
    m_sContextualSpacing = sContextualSpacing;
}

void CXmlElement::SetInd(std::wstring sInd)
{
    m_sInd = sInd;
}

void CXmlElement::SetJc(std::wstring sJc)
{
    m_sJc = sJc;
}

CXmlElement& CXmlElement::operator+=(const CXmlElement &oElement)
{
    if (!oElement.m_sType.empty())
        m_sType = oElement.m_sType;

    if (!oElement.m_sStyleId.empty())
        m_sStyleId = oElement.m_sStyleId;

    if (!oElement.m_sDefault.empty())
        m_sDefault = oElement.m_sDefault;

    if (!oElement.m_sCustomStyle.empty())
        m_sCustomStyle = oElement.m_sCustomStyle;

    if (!oElement.m_sS.empty())
        m_sS = oElement.m_sS;

    if (!oElement.m_sName.empty())
        m_sName = oElement.m_sName;

    if (!oElement.m_sBasedOn.empty())
        m_sBasedOn = oElement.m_sBasedOn;

    if (!oElement.m_sLink.empty())
        m_sLink = oElement.m_sLink;

    if (!oElement.m_sUiPriority.empty())
        m_sUiPriority = oElement.m_sUiPriority;

    if (oElement.m_bQFormat)
        m_bQFormat = true;

    if (oElement.m_bUnhideWhenUsed)
        m_bUnhideWhenUsed = true;

    if (oElement.m_bB)
        m_bB = true;

    if (oElement.m_bI)
        m_bI = true;

    if (!oElement.m_sRFonts.empty())
        m_sRFonts = oElement.m_sRFonts;

    if (!oElement.m_sColor.empty())
        m_sColor = oElement.m_sColor;

    if (!oElement.m_sU.empty())
        m_sU = oElement.m_sU;

    if (!oElement.m_sSz.empty())
        m_sSz = oElement.m_sSz;

    if (oElement.m_bKeepLines)
        m_bKeepLines = true;

    if (oElement.m_bKeepNext)
        m_bKeepNext = true;

    if (!oElement.m_sSpacing.empty())
        m_sSpacing = oElement.m_sSpacing;

    if (!oElement.m_sOutlineLvl.empty())
        m_sOutlineLvl = oElement.m_sOutlineLvl;

    if (!oElement.m_sContextualSpacing.empty())
        m_sContextualSpacing = oElement.m_sContextualSpacing;

    if (!oElement.m_sInd.empty())
        m_sInd = oElement.m_sInd;

    if (!oElement.m_sJc.empty())
        m_sJc = oElement.m_sJc;

    return *this;
}

std::wstring CXmlElement::GetStyle()
{
    std::wstring sStyle = L"<w:style ";

    if (!m_sType.empty())
        sStyle += L"w:type=\"" + m_sType + L"\" ";

    if (!m_sStyleId.empty())
        sStyle += L"w:styleId=\"" + m_sStyleId + L"\" ";

    if (!m_sDefault.empty())
        sStyle += L"w:default=\"" + m_sDefault + L"\" ";

    if (!m_sCustomStyle.empty())
        sStyle += L"w:customStyle=\"" + m_sCustomStyle + L"\" ";

    if (!m_sS.empty())
        sStyle += L"w:S=\"" + m_sS + L"\" ";

    sStyle += L">";

    if (!m_sName.empty())
        sStyle += L"<w:name w:val=\"" + m_sName + L"\"/>";

    if (!m_sBasedOn.empty())
        sStyle += L"<w:basedOn w:val=\"" + m_sBasedOn + L"\"/>";

    if (!m_sLink.empty())
        sStyle += L"<w:link w:val=\"" + m_sLink + L"\"/>";

    if (!m_sUiPriority.empty())
        sStyle += L"<w:uiPriority w:val=\"" + m_sUiPriority + L"\"/>";

    if (m_bQFormat)
        sStyle += L"<w:qFormat/>";

    if (m_bUnhideWhenUsed)
        sStyle += L"<w:unhideWhenUsed/>";

    std::wstring sRPr = L"<w:rPr>";

        if (m_bB)
            sRPr += L"<w:b/><w:bCs/>";

        if (m_bI)
            sRPr += L"<w:i/><w:iCs/>";

        if (!m_sRFonts.empty())
            sRPr += L"<w:rFonts w:ascii=\"" + m_sRFonts + L"\" w:hAnsi=\"" + m_sRFonts + L"\" w:cs=\"" +
                      m_sRFonts + L"\" w:eastAsia=\"" + m_sRFonts + L"\"/>";

        if (!m_sColor.empty())
            sRPr += L"<w:color w:val=\"" + m_sColor + L"\"/>";

        if (!m_sU.empty())
            sRPr += L"<w:u w:val=\"" + m_sU + L"\"/>";

        if (!m_sSz.empty())
            sRPr += L"<w:sz w:val=\"" + m_sSz + L"\"/>" + L"<w:szCs w:val=\"" + m_sSz + L"\"/>";

    sRPr += L"</w:rPr>";

    std::wstring sPPr = L"<w:pPr>";

        if (m_bKeepLines)
            sPPr += L"<w:keepLines/>";

        if (m_bKeepNext)
            sPPr += L"<w:keepNext/>";

        if (!m_sSpacing.empty())
            sPPr += L"<w:spacing " + m_sSpacing + L"/>";

        if (!m_sOutlineLvl.empty())
            sPPr += L"<w:outlineLvl w:val=\"" + m_sOutlineLvl + L"\"/>";

        if (!m_sContextualSpacing.empty())
            sPPr += L"<w:contextualSpacing w:val=\"" + m_sContextualSpacing + L"\"/>";

        if (!m_sInd.empty())
            sPPr += L"<w:ind " + m_sInd + L"/>";

        if (!m_sJc.empty())
            sPPr += L"<w:jc w:val=\"" + m_sJc + L"\"/>";

    sPPr += L"</w:pPr>";

//    std::wstring sTbl

    if (sPPr.length() > 15)
        sStyle += sPPr;

    if (sRPr.length() > 15)
        sStyle += sRPr;

    sStyle += L"</w:style>";
    if (sStyle.length() > 15)
        return sStyle;

    return L"";
}
