#include "graphics.h"

#include <string>
#include <cmath>

std::wstring NSGraphics::CGraphics::m_sApplicvationFontsDirectory;

namespace NSGraphics
{
void CGraphics::init(double width_px, double height_px, double width_mm, double height_mm)
{
    m_pApplicationFonts = NSFonts::NSApplication::Create();
    m_pApplicationFonts->InitializeFromFolder(m_sApplicvationFontsDirectory.empty() ? NSFile::GetProcessDirectory() : m_sApplicvationFontsDirectory);

    NSFonts::IFontManager* pManager = m_pApplicationFonts->GenerateFontManager();

    m_pRenderer = NSGraphics::Create();
    m_pRenderer->SetFontManager(pManager);

    int nRasterW = (int)width_px;
    int nRasterH = (int)height_px;
    BYTE* pData = new BYTE[4 * nRasterW * nRasterH];

    unsigned int back = 0xffffff;
    unsigned int* pData32 = (unsigned int*)pData;
    unsigned int* pData32End = pData32 + nRasterW * nRasterH;
    while (pData32 < pData32End)
        *pData32++ = back;

    CBgraFrame oFrame;
    oFrame.put_Data(pData);
    oFrame.put_Width(nRasterW);
    oFrame.put_Height(nRasterH);
    oFrame.put_Stride(4 * nRasterW);

    m_pRenderer->CreateFromBgraFrame(&oFrame);
    m_pRenderer->SetSwapRGB(false);

    m_pRenderer->put_Width(width_mm);
    m_pRenderer->put_Height(height_mm);
}
void CGraphics::put_GlobalAlpha(bool enable, double alpha)
{
    m_pRenderer->put_GlobalAlphaEnabled(enable, alpha);
}
void CGraphics::End_GlobalAlpha()
{
    bool bIsInteger = m_pRenderer->get_IntegerGrid();
    m_pRenderer->put_IntegerGrid(true);

    m_pRenderer->PathCommandEnd();
    b_color1(255, 255, 255, 140);

    m_pRenderer->AddRect(0.0, 0.0, m_pRenderer->GetPixW(), m_pRenderer->GetPixH());
    m_pRenderer->Fill();
    m_pRenderer->PathCommandEnd();

    m_pRenderer->put_IntegerGrid(bIsInteger);
}
void CGraphics::p_color(int r, int g, int b, int a)
{
    m_pRenderer->put_PenColor(r | (g << 8) | (b << 16));
    m_pRenderer->put_PenAlpha(a);
}
void CGraphics::p_width(double w)
{
    m_pRenderer->put_PenSize(w);
}
void CGraphics::p_dash(size_t length, double* dash)
{
    if(length > 0)
    {
        for(size_t i = 0; i < length; i++)
            dash[i] = dash[i] * 72.0 / 25.4 * 2.0;

        m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
        m_pRenderer->PenDashPattern(dash, length);
    }
    else
        m_pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);
}
void CGraphics::b_color1(int r, int g, int b, int a)
{    
    m_pRenderer->put_BrushType(c_BrushTypeSolid);
    m_pRenderer->put_BrushColor1(r | (g << 8) | (b << 16));
    m_pRenderer->put_BrushAlpha1(a);
}
void CGraphics::b_color2(int r, int g, int b, int a)
{
    m_pRenderer->put_BrushColor2(r | (g << 8) | (b << 16));
    m_pRenderer->put_BrushAlpha2(a);
}
void CGraphics::transform(double sx, double shy, double shx, double sy, double tx, double ty)
{
    m_pRenderer->SetTransform(sx, shy, shx, sy, tx, ty);
}
void CGraphics::_s()
{
    m_pRenderer->PathCommandEnd();
}
void CGraphics::_e()
{
    m_pRenderer->PathCommandEnd();
}
void CGraphics::_z()
{
    m_pRenderer->PathCommandClose();
}
void CGraphics::_m(double x, double y)
{
    if (!m_pRenderer->get_IntegerGrid())
        m_pRenderer->PathCommandMoveTo(x, y);
    else
    {
        m_pRenderer->GetFullTransform()->TransformPoint(x, y);
        m_pRenderer->PathCommandMoveTo((int)x + 0.5, (int)y + 0.5);
    }
}
void CGraphics::_l(double x, double y)
{
    if (!m_pRenderer->get_IntegerGrid())
        m_pRenderer->PathCommandLineTo(x, y);
    else
    {
        m_pRenderer->GetFullTransform()->TransformPoint(x, y);
        m_pRenderer->PathCommandLineTo((int)x + 0.5, (int)y + 0.5);
    }
}
void CGraphics::_c(double x1, double y1, double x2, double y2, double x3, double y3)
{
    m_pRenderer->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
}
void CGraphics::_c2(double x1, double y1, double x2, double y2)
{
    m_pRenderer->PathCommandCurveTo(x1, y1, x1, y1, x2, y2);
}
void CGraphics::ds()
{
    m_pRenderer->Stroke();
}
void CGraphics::df()
{
    m_pRenderer->Fill();
}

void CGraphics::restore()
{
    m_pRenderer->BeginCommand(c_nResetClipType);
    m_pRenderer->EndCommand(c_nResetClipType);
}

void CGraphics::clip()
{
    m_pRenderer->BeginCommand(c_nClipType);
    m_pRenderer->EndCommand(c_nClipType);
}

void CGraphics::reset()
{
    m_pRenderer->ResetTransform();
}

void CGraphics::transform3(double sx, double shy, double shx, double sy, double tx, double ty)
{
    m_pRenderer->SetTransform(sx, shy, shx, sy, tx, ty);
}

void CGraphics::drawImage(const std::wstring& img, double x, double y, double w, double h, BYTE alpha)
{
    std::wstring strImage = (0 == img.find(L"theme") ? m_sThemesDirectory : m_sImagesDirectory) + img;
    m_pRenderer->DrawImageFromFile(strImage, x, y, w, h, alpha);
}

void CGraphics::SetFont(const std::wstring& path, int face, double size, int style)
{
    m_pRenderer->put_FontPath((std::string::npos == path.find(L'/') ? m_sFontsDirectory : L"") + path);
    m_pRenderer->put_FontFaceIndex(face);
    m_pRenderer->put_FontSize(size);
    m_pRenderer->put_FontStyle(style);
}

void CGraphics::FillText(double x, double y, int text)
{
    m_pRenderer->CommandDrawTextCHAR(text, x, y, 0, 0);
}

void CGraphics::FillText2(double x, double y, int text, double cropX, double cropW)
{
    m_pRenderer->CommandDrawTextCHAR(text, x, y, 0, 0);
}

void CGraphics::FillTextCode(double x, double y, int lUnicode)
{
    m_pRenderer->CommandDrawTextCHAR(lUnicode, x, y, 0, 0);
}

void CGraphics::tg(int text, double x, double y)
{
    m_pRenderer->put_FontStringGID(TRUE);
    m_pRenderer->CommandDrawTextCHAR(text, x, y, 0, 0);
    m_pRenderer->put_FontStringGID(FALSE);
}

void CGraphics::SetIntegerGrid(bool param)
{
    m_pRenderer->put_IntegerGrid(param);
}

bool CGraphics::GetIntegerGrid()
{
    return m_pRenderer->get_IntegerGrid();
}

void CGraphics::DrawHeaderEdit(double yPos, BYTE lock_type)
{
    m_pRenderer->PathCommandEnd();

    Aggplus::CMatrix* pFull = m_pRenderer->GetFullTransform();
    double dPenSize = 0;
    m_pRenderer->get_PenSize(&dPenSize);

    double _width;
    m_pRenderer->get_Width(&_width);
    pFull->TransformPoint(_width, yPos);

    BYTE nPenDashStyle = 0;
    m_pRenderer->get_PenDashStyle(&nPenDashStyle);

    bool bIsIntegerGrid = m_pRenderer->get_IntegerGrid();
    if (!bIsIntegerGrid)
        m_pRenderer->put_IntegerGrid(true);

    m_pRenderer->put_PenSize(2);
    m_pRenderer->PathCommandStart();

    double dash[2] = { 6, 3 };
    m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
    m_pRenderer->PenDashPattern(dash, 2);

    m_pRenderer->put_PenColor(0xBBBEC2);

    m_pRenderer->PathCommandMoveTo(0, (int)(yPos));
    m_pRenderer->PathCommandLineTo(_width, (int)(yPos));

    m_pRenderer->Stroke();
    m_pRenderer->PathCommandEnd();

    if (!bIsIntegerGrid)
        m_pRenderer->put_IntegerGrid(false);

    m_pRenderer->put_PenSize(dPenSize);
    m_pRenderer->put_PenDashStyle(nPenDashStyle);
}

void CGraphics::DrawFooterEdit(double yPos, BYTE lock_type)
{
    m_pRenderer->PathCommandEnd();

    Aggplus::CMatrix* pFull = m_pRenderer->GetFullTransform();
    double dPenSize = 0;
    m_pRenderer->get_PenSize(&dPenSize);

    double _width;
    m_pRenderer->get_Width(&_width);
    pFull->TransformPoint(_width, yPos);

    BYTE nPenDashStyle = 0;
    m_pRenderer->get_PenDashStyle(&nPenDashStyle);

    bool bIsIntegerGrid = m_pRenderer->get_IntegerGrid();
    if (!bIsIntegerGrid)
        m_pRenderer->put_IntegerGrid(true);

    m_pRenderer->put_PenSize(2);
    m_pRenderer->PathCommandStart();

    double dash[2] = { 6, 3 };
    m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
    m_pRenderer->PenDashPattern(dash, 2);

    m_pRenderer->put_PenColor(0xBBBEC2);

    m_pRenderer->PathCommandMoveTo(0, (int)(yPos));
    m_pRenderer->PathCommandLineTo(_width, (int)(yPos));

    m_pRenderer->Stroke();
    m_pRenderer->PathCommandEnd();

    if (!bIsIntegerGrid)
        m_pRenderer->put_IntegerGrid(false);

    m_pRenderer->put_PenSize(dPenSize);
    m_pRenderer->put_PenDashStyle(nPenDashStyle);
}

void CGraphics::DrawEmptyTableLine(double x1, double y1, double x2, double y2)
{
    m_pRenderer->PathCommandEnd();

    Aggplus::CMatrix* pFull = m_pRenderer->GetFullTransform();
    double dPenSize = 0;
    m_pRenderer->get_PenSize(&dPenSize);

    BYTE nPenDashStyle = 0;
    m_pRenderer->get_PenDashStyle(&nPenDashStyle);

    double _x1 = x1;
    double _y1 = y1;
    double _x2 = x2;
    double _y2 = y2;
    pFull->TransformPoint(_x1, _y1);
    pFull->TransformPoint(_x2, _y2);

    if (fabs(_x1 - _x2) < 0.001 || fabs(_y1 - _y2) < 0.001)
    {
        bool bIsIntegerGrid = m_pRenderer->get_IntegerGrid();
        if (!bIsIntegerGrid)
            m_pRenderer->put_IntegerGrid(true);

        m_pRenderer->put_PenSize(1);
        m_pRenderer->PathCommandStart();

        double dash[2] = { 2, 2 };
        m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
        m_pRenderer->PenDashPattern(dash, 2);

        m_pRenderer->put_PenColor(0xBFA28A);

        if (fabs(_x1 - _x2) < 0.001)
        {
            double _dx = ((int)_x1) + 0.5;
            double _dy1 = ((int)_y1);
            double _dy2 = ((int)_y2);
            m_pRenderer->PathCommandMoveTo(_dx, _dy1);
            m_pRenderer->PathCommandLineTo(_dx, _dy2);
        }
        else
        {
            double _dy = ((int)_y1) + 0.5;
            double _dx1 = ((int)_x1);
            double _dx2 = ((int)_x2);
            m_pRenderer->PathCommandMoveTo(_dx1, _dy);
            m_pRenderer->PathCommandLineTo(_dx2, _dy);
        }

        m_pRenderer->Stroke();
        m_pRenderer->PathCommandEnd();

        if (!bIsIntegerGrid)
            m_pRenderer->put_IntegerGrid(false);
    }
    else
    {
        bool bIsIntegerGrid = m_pRenderer->get_IntegerGrid();
        if (bIsIntegerGrid)
            m_pRenderer->put_IntegerGrid(false);

        m_pRenderer->put_PenSize(0);
        m_pRenderer->PathCommandStart();

        double dash[2] = { 2, 2 };
        m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
        m_pRenderer->PenDashPattern(dash, 2);

        m_pRenderer->put_PenColor(0xBFA28A);
        m_pRenderer->PathCommandMoveTo(x1, y1);
        m_pRenderer->PathCommandLineTo(x2, y2);
        m_pRenderer->Stroke();
        m_pRenderer->PathCommandEnd();

        if (bIsIntegerGrid)
            m_pRenderer->put_IntegerGrid(true);
    }

    m_pRenderer->put_PenSize(dPenSize);
    m_pRenderer->put_PenDashStyle(nPenDashStyle);
}

void CGraphics::DrawSpellingLine(double y0, double x0, double x1, double w)
{
    Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
    if (!m_pRenderer->get_IntegerGrid())
    {
        if (pMatrix->IsIdentity2())
        {
            m_pRenderer->put_IntegerGrid(true);
            m_pRenderer->drawHorLine(1, y0, x0, x1, w);
            m_pRenderer->put_IntegerGrid(false);
        }
        else
        {
            m_pRenderer->put_PenSize(w);
            m_pRenderer->PathCommandEnd();
            m_pRenderer->PathCommandMoveTo(x0, y0);
            m_pRenderer->PathCommandLineTo(x1, y0);
            m_pRenderer->Stroke();
        }
    }
    else
    {
        if(pMatrix->IsIdentity2())
        {
            m_pRenderer->drawHorLine(1, y0, x0, x1, w);
        }
        else
        {
            m_pRenderer->put_IntegerGrid(false);
            m_pRenderer->put_PenSize(w);
            m_pRenderer->PathCommandEnd();
            m_pRenderer->PathCommandMoveTo(x0, y0);
            m_pRenderer->PathCommandLineTo(x1, y0);
            m_pRenderer->Stroke();
            m_pRenderer->put_IntegerGrid(true);
        }

    }
}

void CGraphics::drawHorLine(BYTE align, double y, double x, double r, double penW)
{
    Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
    if (!m_pRenderer->get_IntegerGrid())
    {
        if (pMatrix->IsIdentity2())
        {
            m_pRenderer->put_IntegerGrid(true);
            m_pRenderer->drawHorLine(align, y, x, r, penW);
            m_pRenderer->put_IntegerGrid(false);
        }
        else
        {
            m_pRenderer->put_PenSize(penW);
            m_pRenderer->PathCommandEnd();
            m_pRenderer->PathCommandMoveTo(x, y);
            m_pRenderer->PathCommandLineTo(r, y);
            m_pRenderer->Stroke();
        }
    }
    else
    {
        if(pMatrix->IsIdentity2())
        {
            m_pRenderer->drawHorLine(align, y, x, r, penW);
        }
        else
        {
            m_pRenderer->put_IntegerGrid(false);
            m_pRenderer->put_PenSize(penW);
            m_pRenderer->PathCommandEnd();
            m_pRenderer->PathCommandMoveTo(x, y);
            m_pRenderer->PathCommandLineTo(r, y);
            m_pRenderer->Stroke();
            m_pRenderer->put_IntegerGrid(true);
        }

    }
}

void CGraphics::drawHorLine2(BYTE align, double y, double x, double r, double penW)
{
    Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
    if (!m_pRenderer->get_IntegerGrid())
    {
        if (pMatrix->IsIdentity2())
        {
            m_pRenderer->put_IntegerGrid(true);
            m_pRenderer->drawHorLine2(align, y, x, r, penW);
            m_pRenderer->put_IntegerGrid(false);
        }
        else
        {
            double _y1 = y - penW / 2;
            double _y2 = _y1 + 2 * penW;

            m_pRenderer->put_PenSize(penW);
            m_pRenderer->PathCommandEnd();
            m_pRenderer->PathCommandMoveTo(x, _y1);
            m_pRenderer->PathCommandLineTo(r, _y1);
            m_pRenderer->Stroke();

            m_pRenderer->PathCommandEnd();
            m_pRenderer->PathCommandMoveTo(x, _y2);
            m_pRenderer->PathCommandLineTo(r, _y2);
            m_pRenderer->Stroke();
        }
    }
    else
    {
        if (pMatrix->IsIdentity2())
        {
            m_pRenderer->drawHorLine2(align, y, x, r, penW);
        }
        else
        {
            m_pRenderer->put_IntegerGrid(false);
            double _y1 = y - penW / 2;
            double _y2 = _y1 + 2 * penW;

            m_pRenderer->put_PenSize(penW);
            m_pRenderer->PathCommandEnd();
            m_pRenderer->PathCommandMoveTo(x, _y1);
            m_pRenderer->PathCommandLineTo(r, _y1);
            m_pRenderer->Stroke();

            m_pRenderer->PathCommandEnd();
            m_pRenderer->PathCommandMoveTo(x, _y2);
            m_pRenderer->PathCommandLineTo(r, _y2);
            m_pRenderer->Stroke();
            m_pRenderer->put_IntegerGrid(true);
        }
    }
}

void CGraphics::drawVerLine(BYTE align, double x, double y, double b, double penW)
{
    if (!m_pRenderer->get_IntegerGrid())
    {
        Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
        if (pMatrix->IsIdentity2())
        {
            m_pRenderer->put_IntegerGrid(true);
            m_pRenderer->drawVerLine(align, x, y, b, penW);
            m_pRenderer->put_IntegerGrid(false);
        }
        else
        {
            m_pRenderer->put_PenSize(penW);
            m_pRenderer->PathCommandEnd();
            m_pRenderer->PathCommandMoveTo(x, y);
            m_pRenderer->PathCommandLineTo(x, b);
            m_pRenderer->Stroke();
        }
    }
    else
    {
        m_pRenderer->drawVerLine(align, x, y, b, penW);
    }
}

void CGraphics::drawHorLineExt(BYTE align, double y, double x, double r, double penW, double leftMW, double rightMW)
{
    if (!m_pRenderer->get_IntegerGrid())
    {
        Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
        if (pMatrix->IsIdentity2())
        {
            m_pRenderer->put_IntegerGrid(true);
            m_pRenderer->drawHorLineExt(align, y, x, r, penW, leftMW, rightMW);
            m_pRenderer->put_IntegerGrid(false);
        }
        else
        {
            m_pRenderer->put_PenSize(penW);
            m_pRenderer->PathCommandEnd();
            m_pRenderer->PathCommandMoveTo(x, y);
            m_pRenderer->PathCommandLineTo(r, y);
            m_pRenderer->Stroke();
        }
    }
    else
    {
        m_pRenderer->drawHorLineExt(align, y, x, r, penW, leftMW, rightMW);
    }
}

void CGraphics::rect(double x, double y, double w, double h)
{
    m_pRenderer->PathCommandEnd();
    if (m_pRenderer->get_IntegerGrid())
    {
        double r = x + w;
        double b = y + h;

        m_pRenderer->GetFullTransform()->TransformPoint(x, y);
        m_pRenderer->GetFullTransform()->TransformPoint(r, b);

        x = (int)(x + 0.5);
        y = (int)(y + 0.5);
        r = (int)(r + 0.5);
        b = (int)(b + 0.5);

        m_pRenderer->AddRect(x, y, r - x, b - y);
    }
    else
    {
        m_pRenderer->AddRect(x, y, w, h);
    }
}

void CGraphics::TableRect(double x, double y, double w, double h)
{
    m_pRenderer->PathCommandEnd();
    if (m_pRenderer->get_IntegerGrid())
    {
        double r = x + w;
        double b = y + h;

        m_pRenderer->GetFullTransform()->TransformPoint(x, y);
        m_pRenderer->GetFullTransform()->TransformPoint(r, b);

        x = (int)x;
        y = (int)y;
        r = (int)r;
        b = (int)b;

        m_pRenderer->AddRect(x, y, r - x + 1, b - y + 1);
    }
    else
    {
        m_pRenderer->AddRect(x, y, w, h);
    }
    m_pRenderer->Fill();
}

void CGraphics::AddClipRect(double x, double y, double w, double h)
{
    CHist_Clip* _histClip = new CHist_Clip();

    double sx, shy, shx, sy, tx, ty;
    m_pRenderer->GetTransform(&sx, &shy, &shx, &sy, &tx, &ty);
    _histClip->Transform.SetElements(sx, shy, shx, sy, tx, ty);

    _histClip->IsIntegerGrid = m_pRenderer->get_IntegerGrid();
    _histClip->Rect.left	 = x;
    _histClip->Rect.top		 = y;
    _histClip->Rect.right	 = x + w;
    _histClip->Rect.bottom	 = y + h;

    oGrState.Clips.push_back(_histClip);

    StartClipPath();

    _s();
    _m(x, y);
    _l(x + w, y);
    _l(x + w, y + h);
    _l(x, y + h);
    _l(x, y);

    EndClipPath();
}

void CGraphics::drawCollaborativeChanges(double x, double y, double w, double h, int r, int g, int b, int a)
{
    b_color1(r, g, b, a);
    rect(x, y, w, h);
    df();
    m_pRenderer->PathCommandEnd();
}

void CGraphics::drawMailMergeField(double x, double y, double w, double h)
{
    b_color1(206, 212, 223, 204);
    rect(x, y, w, h);
    df();
    m_pRenderer->PathCommandEnd();
}

void CGraphics::drawSearchResult(double x, double y, double w, double h)
{
    b_color1(255, 238, 128, 255);
    rect(x, y, w, h);
    df();
    m_pRenderer->PathCommandEnd();
}

void CGraphics::SavePen()
{
    CGrStatePen* pState = new CGrStatePen();
    m_pRenderer->SavePen(pState->m_oPen);
    oGrState.States.push_back(pState);
}

void CGraphics::RestorePen()
{
    if (oGrState.States.empty())
        return;

    IGrState* pState = oGrState.States.back();
    if (pState->m_eType == gstPen)
    {
        m_pRenderer->RestorePen(((CGrStatePen*)pState)->m_oPen);
        oGrState.States.pop_back();
        RELEASEOBJECT(pState);
    }
}

void CGraphics::SaveBrush()
{
    CGrStateBrush* pState = new CGrStateBrush();
    m_pRenderer->SaveBrush(pState->m_oBrush);
    oGrState.States.push_back(pState);
}

void CGraphics::RestoreBrush()
{
    if (oGrState.States.empty())
        return;

    IGrState* pState = oGrState.States.back();
    if (pState->m_eType == gstBrush)
    {
        m_pRenderer->RestoreBrush(((CGrStateBrush*)pState)->m_oBrush);
        oGrState.States.pop_back();
        RELEASEOBJECT(pState);
    }
}

void CGraphics::SavePenBrush()
{
    CGrStatePenBrush* pState = new CGrStatePenBrush();
    m_pRenderer->SavePen(pState->m_oPen);
    m_pRenderer->SaveBrush(pState->m_oBrush);
    oGrState.States.push_back(pState);
}

void CGraphics::RestorePenBrush()
{
    if (oGrState.States.empty())
        return;

    IGrState* pState = oGrState.States.back();
    if (pState->m_eType == gstPenBrush)
    {
        m_pRenderer->RestorePen(((CGrStatePenBrush*)pState)->m_oPen);
        m_pRenderer->RestoreBrush(((CGrStatePenBrush*)pState)->m_oBrush);
        oGrState.States.pop_back();
        RELEASEOBJECT(pState);
    }
}

void CGraphics::SaveGrState()
{
    CGrStateState* pState = new CGrStateState();
    pState->IsIntegerGrid = m_pRenderer->get_IntegerGrid();
    pState->Clips = oGrState.Clips;

    double sx, shy, shx, sy, tx, ty;
    m_pRenderer->GetTransform(&sx, &shy, &shx, &sy, &tx, &ty);
    pState->Transform.SetElements(sx, shy, shx, sy, tx, ty);

    oGrState.Clips.clear();
    oGrState.States.push_back(pState);
}

void CGraphics::RestoreGrState()
{
    if (oGrState.States.empty())
        return;

    IGrState* pState = oGrState.States.back();
    if (pState->m_eType != gstState)
        return;

    CGrStateState* pGrState = (CGrStateState*)pState;

    if (!oGrState.Clips.empty())
    {
        RemoveClip();

        for (IGrState* i : oGrState.States)
        {
            if (i->m_eType == gstState)
            {
                std::vector<CHist_Clip*>& arr = ((CGrStateState*)i)->Clips;

                for (CHist_Clip* j : arr)
                {
                    Aggplus::CMatrix& oMatrix = j->Transform;
                    transform3(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
                    SetIntegerGrid(j->IsIntegerGrid);

                    StartClipPath();

                    double x = j->Rect.left;
                    double y = j->Rect.top;
                    double r = j->Rect.right;
                    double b = j->Rect.bottom;

                    _s();
                    _m(x, y);
                    _l(r, y);
                    _l(r, b);
                    _l(x, b);
                    _l(x, y);

                    EndClipPath();
                }
            }
        }
    }

    for (CHist_Clip* pClip : oGrState.Clips)
        RELEASEOBJECT(pClip);
    oGrState.Clips.clear();

    oGrState.Clips = pGrState->Clips;
    pGrState->Clips.clear();
    oGrState.States.pop_back();

    Aggplus::CMatrix& oMatrix = pGrState->Transform;
    transform3(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
    SetIntegerGrid(pGrState->IsIntegerGrid);

    RELEASEOBJECT(pState);
}

void CGraphics::StartClipPath()
{
    m_pRenderer->BeginCommand(c_nClipType);
}

void CGraphics::EndClipPath()
{
    m_pRenderer->EndCommand(c_nClipType);
}

void CGraphics::AddSmartRect(double x, double y, double w, double h, double pen_w)
{
    m_pRenderer->AddRect(x, y, w, h);
}
}