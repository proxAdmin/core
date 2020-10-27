#ifndef CGRAPHICS_H
#define CGRAPHICS_H

#include <string>
#include <vector>

#include "../common/Types.h"
#include "../graphics/pro/Graphics.h"

namespace NSGraphics
{
    enum EGrStateType
    {
        gstPen		= 0,
        gstBrush	= 1,
        gstPenBrush	= 2,
        gstState	= 3,
        gstNone		= 4
    };

    class IGrState
    {
    public:
        EGrStateType m_eType;
        IGrState() { m_eType = gstNone; }
        virtual ~IGrState() {}
    };

    class CGrStatePen : public IGrState
    {
    public:
        NSStructures::CPen m_oPen;
        CGrStatePen() { m_eType = gstPen; }
    };

    class CGrStateBrush : public IGrState
    {
    public:
        NSStructures::CBrush m_oBrush;
        CGrStateBrush() { m_eType = gstBrush; }
    };

    class CGrStatePenBrush : public IGrState
    {
    public:
        NSStructures::CPen   m_oPen;
        NSStructures::CBrush m_oBrush;
        CGrStatePenBrush() { m_eType = gstPenBrush; }
    };

    class CGraphics;
    class CHist_Clip
    {
    public:
        bool  IsPath;
        void* Path;

        Aggplus::CDoubleRect Rect;
        bool				 IsIntegerGrid;
        Aggplus::CMatrix	 Transform;

    public:
        CHist_Clip() : IsPath(false), Path(NULL), IsIntegerGrid(false) {}

        void ToRenderer(CGraphics* pRenderer);
    };

    class CGrStateState : public IGrState
        {
        public:
            Aggplus::CMatrix	        Transform;
            bool				        IsIntegerGrid;
            std::vector<CHist_Clip*>	Clips;

        public:
            CGrStateState()
            {
                m_eType = gstState;
                IsIntegerGrid = false;
            }
            ~CGrStateState()
            {
                size_t nCount = Clips.size();
                for (size_t i = 0; i < nCount; ++i)
                {
                    CHist_Clip* pClip = Clips[i];
                    RELEASEOBJECT(pClip);
                }
                Clips.clear();
            }

            template <typename T>
            void ApplyClips(T* pRenderer)
            {
                size_t nCount = Clips.size();
                for (size_t i = 0; i < nCount; ++i)
                {
                    Clips[i]->ToRenderer(pRenderer);
                }
            }
        };

    class CGrState
    {
    public:
        std::vector<IGrState*>   States;
        std::vector<CHist_Clip*> Clips;

    public:
        CGrState() {}
        ~CGrState()
        {
            for(IGrState* pState : States)
                RELEASEOBJECT(pState);
            States.clear();

            for(CHist_Clip* pState : Clips)
                RELEASEOBJECT(pState);
            Clips.clear();
        }

        void Save()
        {
            SavePen();
            SaveBrush();
            SaveGrState();
        }
        void Restore()
        {
            RestoreGrState();
            RestoreBrush();
            RestorePen();
        }

        void StartClipPath()
        {
            // реализовать, как понадобится
        }
        void EndClipPath()
        {
            // реализовать, как понадобится
        }
    };

    class CGraphics
    {
    private:
        NSFonts   ::IApplicationFonts* m_pApplicationFonts; // этот объект - один на приложение
        NSGraphics::IGraphicsRenderer* m_pRenderer;         // drawer interface
        CBgraFrame m_oFrame;                                // image data
        CGrState oGrState;

        std::wstring m_sFontsDirectory;
        std::wstring m_sImagesDirectory;
        std::wstring m_sThemesDirectory;

    public:
        CGraphics()
        {
        }
        ~CGraphics()
        {
        }

        void init(double width_px, double height_px, double width_mm, double height_mm);
        void EndDraw() {}
        void put_GlobalAlpha(bool enable, double globalAlpha);
        void Start_GlobalAlpha() {}
        void End_GlobalAlpha();
        // pen methods
        void p_color(int r, int g, int b, int a);
        void p_width(double w);
        void p_dash(size_t length, double* dash);
        // brush methods
        void b_color1(int r, int g, int b, int a);
        void b_color2(int r, int g, int b, int a);
        void transform(double sx, double shy, double shx, double sy, double tx, double ty);
        void CalculateFullTransform(bool isInvertNeed) {}
        // path commands
        void _s();
        void _e();
        void _z();
        void _m(double x, double y);
        void _l(double x, double y);
        void _c(double x1, double y1, double x2, double y2, double x3, double y3);
        void _c2(double x1, double y1, double x2, double y2);
        void ds();
        void df();
        // canvas state
        void save() {}
        void restore();
        void clip();
        void reset();
        void transform3(double sx, double shy, double shx, double sy, double tx, double ty);
        void FreeFont() {}
        void ClearLastFont() {}
        // images
        void drawImage(const std::wstring& img, double x, double y, double w, double h, BYTE alpha);
        // text
        void GetFont() {}
        void font(int font_id, int font_size) {}
        void SetFont(const std::wstring& path, int face, double size, int style);
        void SetTextPr() {}
        void GetTextPr() {}
        void FillText(double x, double y, int text);
        void t() {}
        void FillText2(double x, double y, int text, double cropX, double cropW);
        void t2() {}
        void FillTextCode(double x, double y, int lUnicode);
        void tg(int text, double x, double y);
        void charspace() {}
        // private methods
        void private_FillGlyph()  {}
        void private_FillGlyphC() {}
        void private_FillGlyph2() {}
        void SetIntegerGrid(bool param);
        bool GetIntegerGrid() {}
        void DrawStringASCII() {}
        void DrawStringASCII2() {}
        void DrawHeaderEdit(double yPos, BYTE lock_type);
        void DrawFooterEdit(double yPos, BYTE lock_type);
        void DrawLockParagraph (BYTE lock_type, double x, double y1, double y2) {}
        void DrawLockObjectRect(BYTE lock_type, double x, double y,  double w, double h) {}
        void DrawEmptyTableLine(double x1, double y1, double x2, double y2);
        void DrawSpellingLine(double y0, double x0, double x1, double w);
        // smart methods for horizontal / vertical lines
        void drawHorLine (BYTE align, double y, double x, double r, double penW);
        void drawHorLine2(BYTE align, double y, double x, double r, double penW);
        void drawVerLine (BYTE align, double x, double y, double b, double penW);
        // мега крутые функции для таблиц
        void drawHorLineExt(BYTE align, double y, double x, double r, double penW, double leftMW, double rightMW);
        void rect(double x, double y, double w, double h);
        void TableRect(double x, double y, double w, double h);
        // функции клиппирования
        void AddClipRect(double x, double y, double w, double h);
        void RemoveClipRect() {}
        void SetClip(double x, double y, double w, double h) {}
        void RemoveClip() {}
        void drawCollaborativeChanges(double x, double y, double w, double h, int r, int g, int b, int a);
        void drawMailMergeField(double x, double y, double w, double h);
        void drawSearchResult(double x, double y, double w, double h);
        void drawFlowAnchor(double x, double y) {}
        void SavePen();
        void RestorePen();
        void SaveBrush();
        void RestoreBrush();
        void SavePenBrush();
        void RestorePenBrush();
        void SaveGrState();
        void RestoreGrState();
        void StartClipPath();
        void EndClipPath();
        void StartCheckTableDraw() {}
        void EndCheckTableDraw(bool bIsRestore) {}
        void SetTextClipRect(double _l, double _t, double _r, double _b) {}
        void AddSmartRect   (double x,  double y,  double w,  double h, double pen_w);
        void CheckUseFonts2() {}
        void UncheckUseFonts2() {}
        void Drawing_StartCheckBounds() {}
        void Drawing_EndCheckBounds() {}
        void DrawPresentationComment() {}
        void DrawPolygon() {}
        void DrawFootnoteRect() {}
    };

    void CHist_Clip::ToRenderer(CGraphics* pRenderer)
    {
        if (!IsPath)
        {
            pRenderer->StartClipPath();
            pRenderer->rect(Rect.left, Rect.top, Rect.GetWidth(), Rect.GetHeight());
            pRenderer->EndClipPath();
        }
    }
}

#endif // CGRAPHICS_H
