#include "graphics.h"

#include <string>
#include <cmath>

namespace NSGraphics
{

void CGraphics::init(v8::Local<v8::Value>* context, double width_px, double height_px, double width_mm, double height_mm)
{
    m_oContext   = context;
    m_lHeightPix = height_px; // height_px >> 0;
    m_lWidthPix  = width_px; // width_px >> 0;
    m_dWidthMM   = width_mm;
    m_dHeightMM  = height_mm;
    m_dDpiX      = 25.4 * m_lWidthPix / m_dWidthMM;
    m_dDpiY      = 25.4 * m_lHeightPix / m_dHeightMM;

    /*
    m_oCoordTransform.sx = m_dDpiX / 25.4;
    m_oCoordTransform.sy = m_dDpiY / 25.4;

    TextureFillTransformScaleX = 1 / m_oCoordTransform.sx;
    TextureFillTransformScaleY = 1 / m_oCoordTransform.sy;

    m_oLastFont.Clear();
    m_oContext.save();
    */

    m_bPenColorInit = false;
    m_bBrushColorInit = false;
}

void CGraphics::put_GlobalAlpha(bool enable, int alpha)
{
    if(!enable)
    {
        globalAlpha = 1;
        // m_oContext.globalAlpha = 1;
    }
    else
    {
        globalAlpha = alpha;
        // m_oContext.globalAlpha = alpha;
    }
}

void CGraphics::End_GlobalAlpha()
{
    if (!m_bIntegerGrid)
    {
        // m_oContext.setTransform(1,0,0,1,0,0);
    }

    // ДРУГОЙ МЕТОД КЛАССА
    // b_color1(255, 255, 255, 140);

    // m_oContext.fillRect(0, 0, m_lWidthPix, m_lHeightPix);
    // m_oContext.beginPath();

    if (!m_bIntegerGrid)
    {
        /*
        m_oContext.setTransform(m_oFullTransform.sx, m_oFullTransform.shy, m_oFullTransform.shx,
                                m_oFullTransform.sy, m_oFullTransform.tx, m_oFullTransform.ty);
        */
    }
}

void CGraphics::p_color(int r, int g, int b, int a)
{
    /*
    var _c = m_oPen.Color;
    if (m_bPenColorInit && _c.R == r && _c.G == g && _c.B == b && _c.A == a)
        return;

    m_bPenColorInit = true;
    _c.R = r;
    _c.G = g;
    _c.B = b;
    _c.A = a;

    m_oContext.strokeStyle = "rgba(" + std::to_string(_c.R) + "," + std::to_string(_c.G) + "," + std::to_string(_c.B) +
                             "," + std::to_string(static_cast<int>(static_cast<double>(_c.A) / 255.0)) + ")";
    */
}

void CGraphics::p_width(int w)
{
    // m_oPen.LineWidth = static_cast<int>(static_cast<double>(w) / 1000.0);

    if (!m_bIntegerGrid)
    {
        /*
        if (0 != m_oPen.LineWidth)
        {
            m_oContext.lineWidth = m_oPen.LineWidth;
        }
        else
        {
            double _x1 = m_oFullTransform.TransformPointX(0, 0);
            double _y1 = m_oFullTransform.TransformPointY(0, 0);
            double _x2 = m_oFullTransform.TransformPointX(1, 1);
            double _y2 = m_oFullTransform.TransformPointY(1, 1);

            double _koef = sqrt(((_x2 - _x1) * (_x2 - _x1) + (_y2 - _y1) * (_y2 - _y1)) / 2);
            m_oContext.lineWidth = static_cast<int>(1.0 / _koef);
        }
        */
    }
    else
    {
        /*
        if (0 != m_oPen.LineWidth)
        {
            v8::Local<v8::Value> _m = m_oFullTransform;
            double x = _m.sx + _m.shx;
            double y = _m.sy + _m.shy;

            double koef = sqrt((x * x + y * y) / 2);
            m_oContext.lineWidth = static_cast<int>(m_oPen.LineWidth * koef);
        }
        else
        {
            m_oContext.lineWidth = 1;
        }
        */
    }
}

void CGraphics::p_dash(const v8::Local<v8::Value>& params)
{
    /*
    if (!m_oContext.setLineDash)
        return;

    dash_no_smart = params ? params.slice() : null;
    m_oContext.setLineDash(params ? params : []);
    */
}

void CGraphics::b_color1(int r, int g, int b, int a)
{
    /*
    var _c = m_oBrush.Color1;
    if (m_bBrushColorInit && _c.R == r && _c.G == g && _c.B == b && _c.A == a)
        return;

    m_bBrushColorInit = true;

    _c.R = r;
    _c.G = g;
    _c.B = b;
    _c.A = a;

    m_oContext.fillStyle = "rgba(" + std::to_string(_c.R) + "," + std::to_string(_c.G) + "," + std::to_string(_c.B) +
                           "," + std::to_string(static_cast<int>(static_cast<double>(_c.A) / 255.0)) + ")";
    */
}

void CGraphics::b_color2(int r, int g, int b, int a)
{
    /*
    var _c = m_oBrush.Color2;
    _c.R = r;
    _c.G = g;
    _c.B = b;
    _c.A = a;
    */
}

void CGraphics::transform(double sx, double shy, double shx, double sy, double tx, double ty)
{
    /*
    v8::Local<v8::Value> _t = m_oTransform;
    _t.sx    = sx;
    _t.shx   = shx;
    _t.shy   = shy;
    _t.sy    = sy;
    _t.tx    = tx;
    _t.ty    = ty;

    CalculateFullTransform();
    if (!m_bIntegerGrid)
    {
        v8::Local<v8::Value> _ft = m_oFullTransform;
        m_oContext.setTransform(_ft.sx, _ft.shy, _ft.shx, _ft.sy, _ft.tx, s_ft.ty);
    }

    if (NULL != m_oFontManager)
        m_oFontManager.SetTextMatrix(_t.sx, _t.shy, _t.shx, _t.sy, _t.tx, _t.ty);
    */
}

void CGraphics::CalculateFullTransform(bool isInvertNeed)
{
    /*
    v8::Local<v8::Value> _ft = m_oFullTransform;
    v8::Local<v8::Value> _t  = m_oTransform;
    _ft.sx  = _t.sx;
    _ft.shx = _t.shx;
    _ft.shy = _t.shy;
    _ft.sy  = _t.sy;
    _ft.tx  = _t.tx;
    _ft.ty  = _t.ty;
    global_MatrixTransformer.MultiplyAppend(_ft, m_oCoordTransform);

    v8::Local<v8::Value> _it = m_oInvertFullTransform;
    _it.sx = _ft.sx;
    _it.shx = _ft.shx;
    _it.shy = _ft.shy;
    _it.sy = _ft.sy;
    _it.tx = _ft.tx;
    _it.ty = _ft.ty;

    if (!isInvertNeed)
    {
        global_MatrixTransformer.MultiplyAppendInvert(_it, _t);
    }
    */
}

void CGraphics::_s()
{
    // m_oContext.beginPath();
}

void CGraphics::_e()
{
    // m_oContext.beginPath();
}

void CGraphics::_z()
{
    // m_oContext.closePath();
}

void CGraphics::_m(double x, double y)
{
    /*
    if (!m_bIntegerGrid)
    {
        m_oContext.moveTo(x, y);

        if (ArrayPoints != NULL)
            ArrayPoints[ArrayPoints.length] = {x: x, y: y};
    }
    else
    {
        var _x = (m_oFullTransform.TransformPointX(x, y)) >> 0;
        var _y = (m_oFullTransform.TransformPointY(x, y)) >> 0;
        m_oContext.moveTo(_x + 0.5, _y + 0.5);
    }
    */
}

void CGraphics::_l(double x, double y)
{
    /*
    if (!m_bIntegerGrid)
    {
        m_oContext.lineTo(x, y);

        if (ArrayPoints != NULL)
            ArrayPoints[ArrayPoints.length] = {x: x, y: y};
    }
    else
    {
        var _x = (m_oFullTransform.TransformPointX(x, y)) >> 0;
        var _y = (m_oFullTransform.TransformPointY(x, y)) >> 0;
        m_oContext.lineTo(_x + 0.5, _y + 0.5);
    }
    */
}

void CGraphics::_c(double x1, double y1, double x2, double y2, double x3, double y3)
{
    /*
    if (!m_bIntegerGrid)
    {
        m_oContext.bezierCurveTo(x1, y1, x2 ,y2, x3, y3);

        if (ArrayPoints != NULL)
        {
            ArrayPoints[ArrayPoints.length] = {x: x1, y: y1};
            ArrayPoints[ArrayPoints.length] = {x: x2, y: y2};
            ArrayPoints[ArrayPoints.length] = {x: x3, y: y3};
        }
    }
    else
    {
        var _x1 = (m_oFullTransform.TransformPointX(x1, y1)) >> 0;
        var _y1 = (m_oFullTransform.TransformPointY(x1, y1)) >> 0;

        var _x2 = (m_oFullTransform.TransformPointX(x2, y2)) >> 0;
        var _y2 = (m_oFullTransform.TransformPointY(x2, y2)) >> 0;

        var _x3 = (m_oFullTransform.TransformPointX(x3, y3)) >> 0;
        var _y3 = (m_oFullTransform.TransformPointY(x3, y3)) >> 0;
        m_oContext.bezierCurveTo(_x1 + 0.5, _y1 + 0.5, _x2 + 0.5, _y2 + 0.5, _x3 + 0.5, _y3 + 0.5);
    }
    */
}

void CGraphics::_c2(double x1, double y1, double x2, double y2)
{
    /*
    if (!m_bIntegerGrid)
    {
        m_oContext.quadraticCurveTo(x1, y1, x2, y2);

        if (ArrayPoints != NULL)
        {
            ArrayPoints[ArrayPoints.length] = {x: x1, y: y1};
            ArrayPoints[ArrayPoints.length] = {x: x2, y: y2};
        }
    }
    else
    {
        var _x1 = (m_oFullTransform.TransformPointX(x1, y1)) >> 0;
        var _y1 = (m_oFullTransform.TransformPointY(x1, y1)) >> 0;

        var _x2 = (m_oFullTransform.TransformPointX(x2, y2)) >> 0;
        var _y2 = (m_oFullTransform.TransformPointY(x2, y2)) >> 0;

        m_oContext.quadraticCurveTo(_x1 + 0.5, _y1 + 0.5, _x2 + 0.5, _y2 + 0.5);
    }
    */
}

void CGraphics::ds()
{
    // m_oContext.stroke();
}

void CGraphics::df()
{
    // m_oContext.fill();
}

void CGraphics::save()
{
    // m_oContext.save();
}

void CGraphics::restore()
{
    // m_oContext.restore();

    m_bPenColorInit = false;
    m_bBrushColorInit = false;
}

void CGraphics::clip()
{
    // m_oContext.clip();
}

void CGraphics::reset()
{
    // m_oTransform.Reset();
    CalculateFullTransform(false);

    /*
    if (!m_bIntegerGrid)
        m_oContext.setTransform(m_oCoordTransform.sx, 0, 0, m_oCoordTransform.sy, 0, 0);
    */
}

void CGraphics::transform3(const v8::Local<v8::Value>& m, bool isNeedInvert)
{
    /*
    v8::Local<v8::Value> _t = m_oTransform;
    _t.sx  = m.sx;
    _t.shx = m.shx;
    _t.shy = m.shy;
    _t.sy  = m.sy;
    _t.tx  = m.tx;
    _t.ty  = m.ty;
    CalculateFullTransform(isNeedInvert);

    if (!m_bIntegerGrid)
    {
        var _ft = m_oFullTransform;
        m_oContext.setTransform(_ft.sx, _ft.shy, _ft.shx, _ft.sy, _ft.tx, _ft.ty);
    }
    else
        SetIntegerGrid(false);
    */
}

void CGraphics::FreeFont()
{
    // m_oFontManager.m_pFont = null;
}

void CGraphics::ClearLastFont()
{
    /*
    m_oLastFont  = new AscCommon.CFontSetup();
    m_oLastFont2 = NULL;
    */
}

void CGraphics::drawImage2(const v8::Local<v8::Value>& img, double x, double y, double w, double h, int alpha, const v8::Local<v8::Value>& srcRect)
{
    /*
    if (srcRect)
    {
        // test on need draw:
        if (srcRect.l >= 100 || srcRect.t >= 100)
            return;
        if (srcRect.r <= 0 || srcRect.b <= 0)
            return;
    }

    var isA = (undefined !== alpha && null != alpha && 255 != alpha);
    var _oldGA = 0;
    if (isA)
    {
        _oldGA = this.m_oContext.globalAlpha;
        this.m_oContext.globalAlpha = alpha / 255;
    }

    if (false === this.m_bIntegerGrid)
    {
        if (!srcRect)
        {
            // тут нужно проверить, можно ли нарисовать точно. т.е. может картинка ровно такая, какая нужна.
            if (!global_MatrixTransformer.IsIdentity2(this.m_oTransform))
            {
                this.m_oContext.drawImage(img,x,y,w,h);
            }
            else
            {
                var xx = this.m_oFullTransform.TransformPointX(x, y);
                var yy = this.m_oFullTransform.TransformPointY(x, y);
                var rr = this.m_oFullTransform.TransformPointX(x + w, y + h);
                var bb = this.m_oFullTransform.TransformPointY(x + w, y + h);
                var ww = rr - xx;
                var hh = bb - yy;

                if (Math.abs(img.width - ww) < 2 && Math.abs(img.height - hh) < 2)
                {
                    // рисуем точно
                    this.m_oContext.setTransform(1, 0, 0, 1, 0, 0);
                    this.m_oContext.drawImage(img, xx >> 0, yy >> 0);

                    var _ft = this.m_oFullTransform;
                    this.m_oContext.setTransform(_ft.sx,_ft.shy,_ft.shx,_ft.sy,_ft.tx,_ft.ty);

                }
                else
                {
                    this.m_oContext.drawImage(img,x,y,w,h);
                }
            }
        }
        else
        {
            var _w = img.width;
            var _h = img.height;
            if (_w > 0 && _h > 0)
            {
                var _sx = 0;
                var _sy = 0;
                var _sr = _w;
                var _sb = _h;

                var _l = srcRect.l;
                var _t = srcRect.t;
                var _r = 100 - srcRect.r;
                var _b = 100 - srcRect.b;

                _sx += _l * _w / 100;
                _sr -= _r * _w / 100;
                _sy += _t * _h / 100;
                _sb -= _b * _h / 100;

                var naturalW = _w;
                naturalW -= _sx;
                naturalW += (_sr - _w);

                var naturalH = _h;
                naturalH -= _sy;
                naturalH += (_sb - _h);

                var tmpW = w;
                var tmpH = h;
                if (_sx < 0)
                {
                    x += (-_sx * tmpW / naturalW);
                    w -= (-_sx * tmpW / naturalW);
                    _sx = 0;
                }
                if (_sy < 0)
                {
                    y += (-_sy * tmpH / naturalH);
                    h -= (-_sy * tmpH / naturalH);
                    _sy = 0;
                }
                if (_sr > _w)
                {
                    w -= ((_sr - _w) * tmpW / naturalW);
                    _sr = _w;
                }
                if (_sb > _h)
                {
                    h -= ((_sb - _h) * tmpH / naturalH);
                    _sb = _h;
                }

                if (_sx >= _sr || _sx >= _w || _sr <= 0 || w <= 0)
                    return;
                if (_sy >= _sb || _sy >= _h || _sb <= 0 || h <= 0)
                    return;

                this.m_oContext.drawImage(img,_sx,_sy,_sr-_sx,_sb-_sy,x,y,w,h);
            }
            else
            {
                this.m_oContext.drawImage(img,x,y,w,h);
            }
        }
    }
    else
    {
        var _x1 = (this.m_oFullTransform.TransformPointX(x,y)) >> 0;
        var _y1 = (this.m_oFullTransform.TransformPointY(x,y)) >> 0;
        var _x2 = (this.m_oFullTransform.TransformPointX(x+w,y+h)) >> 0;
        var _y2 = (this.m_oFullTransform.TransformPointY(x+w,y+h)) >> 0;

        x = _x1;
        y = _y1;
        w = _x2 - _x1;
        h = _y2 - _y1;

        if (!srcRect)
        {
            // тут нужно проверить, можно ли нарисовать точно. т.е. может картинка ровно такая, какая нужна.
            if (!global_MatrixTransformer.IsIdentity2(this.m_oTransform))
            {
                this.m_oContext.drawImage(img,_x1,_y1,w,h);
            }
            else
            {
                if (Math.abs(img.width - w) < 2 && Math.abs(img.height - h) < 2)
                {
                    // рисуем точно
                    this.m_oContext.drawImage(img, x, y);
                }
                else
                {
                    this.m_oContext.drawImage(img,_x1,_y1,w,h);
                }
            }
        }
        else
        {
            var _w = img.width;
            var _h = img.height;
            if (_w > 0 && _h > 0)
            {
                var __w = w;
                var __h = h;
                var _delW = Math.max(0, -srcRect.l) + Math.max(0, srcRect.r - 100) + 100;
                var _delH = Math.max(0, -srcRect.t) + Math.max(0, srcRect.b - 100) + 100;

                var _sx = 0;
                if (srcRect.l > 0 && srcRect.l < 100)
                    _sx = Math.min((_w * srcRect.l / 100) >> 0, _w - 1);
                else if (srcRect.l < 0)
                {
                    var _off = ((-srcRect.l / _delW) * __w);
                    x += _off;
                    w -= _off;
                }
                var _sy = 0;
                if (srcRect.t > 0 && srcRect.t < 100)
                    _sy = Math.min((_h * srcRect.t / 100) >> 0, _h - 1);
                else if (srcRect.t < 0)
                {
                    var _off = ((-srcRect.t / _delH) * __h);
                    y += _off;
                    h -= _off;
                }
                var _sr = _w;
                if (srcRect.r > 0 && srcRect.r < 100)
                    _sr = Math.max(Math.min((_w * srcRect.r / 100) >> 0, _w - 1), _sx);
                else if (srcRect.r > 100)
                {
                    var _off = ((srcRect.r - 100) / _delW) * __w;
                    w -= _off;
                }
                var _sb = _h;
                if (srcRect.b > 0 && srcRect.b < 100)
                    _sb = Math.max(Math.min((_h * srcRect.b / 100) >> 0, _h - 1), _sy);
                else if (srcRect.b > 100)
                {
                    var _off = ((srcRect.b - 100) / _delH) * __h;
                    h -= _off;
                }

                if ((_sr-_sx) > 0 && (_sb-_sy) > 0 && w > 0 && h > 0)
                    this.m_oContext.drawImage(img,_sx,_sy,_sr-_sx,_sb-_sy,x,y,w,h);
            }
            else
            {
                this.m_oContext.drawImage(img,x,y,w,h);
            }
        }
    }

    if (isA)
    {
        this.m_oContext.globalAlpha = _oldGA;
    }
    */
}

void CGraphics::drawImage(const v8::Local<v8::Value>& img, double x, double y, double w, double h, int alpha, const v8::Local<v8::Value>& srcRect, const v8::Local<v8::Value>& nativeImage)
{
    /*
    if (nativeImage)
    {
        this.drawImage2(nativeImage,x,y,w,h,alpha,srcRect);
        return;
    }

    var _img = editor.ImageLoader.map_image_index[img];
    if (_img != undefined && _img.Status == AscFonts.ImageLoadStatus.Loading)
    {
        // TODO: IMAGE_LOADING
    }
    else if (_img != undefined && _img.Image != null)
    {
        this.drawImage2(_img.Image,x,y,w,h,alpha,srcRect);
    }
    else
    {
        var _x = x;
        var _y = y;
        var _r = x+w;
        var _b = y+h;

        var ctx = this.m_oContext;
        var old_p = ctx.lineWidth;

        var bIsNoIntGrid = false;

        if (this.m_bIntegerGrid)
        {
            _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;
            _y = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;
            _r = (this.m_oFullTransform.TransformPointX(x+w,y+h) >> 0) + 0.5;
            _b = (this.m_oFullTransform.TransformPointY(x+w,y+h) >> 0) + 0.5;

            ctx.lineWidth = 1;
        }
        else
        {
            if (global_MatrixTransformer.IsIdentity2(this.m_oTransform))
            {
                bIsNoIntGrid = true;

                this.SetIntegerGrid(true);
                _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;
                _y = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;
                _r = (this.m_oFullTransform.TransformPointX(x+w,y+h) >> 0) + 0.5;
                _b = (this.m_oFullTransform.TransformPointY(x+w,y+h) >> 0) + 0.5;

                ctx.lineWidth = 1;
            }
            else
            {
                ctx.lineWidth = 1 / this.m_oCoordTransform.sx;
            }
        }

        //ctx.strokeStyle = "#FF0000";
        ctx.strokeStyle = "#F98C76";

        ctx.beginPath();
        ctx.moveTo(_x,_y);
        ctx.lineTo(_r,_b);
        ctx.moveTo(_r,_y);
        ctx.lineTo(_x,_b);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(_x,_y);
        ctx.lineTo(_r,_y);
        ctx.lineTo(_r,_b);
        ctx.lineTo(_x,_b);
        ctx.closePath();

        ctx.stroke();
        ctx.beginPath();

        if (bIsNoIntGrid)
            this.SetIntegerGrid(false);

        ctx.lineWidth = old_p;
        ctx.strokeStyle = "rgba(" + this.m_oPen.Color.R + "," + this.m_oPen.Color.G + "," +
                this.m_oPen.Color.B + "," + (this.m_oPen.Color.A / 255) + ")";
    }
    */
}

CFont CGraphics::GetFont()
{
    return m_oCurFont;
}

void CGraphics::font(unsigned int font_id, int font_size)
{
    /*
    AscFonts.g_font_infos[AscFonts.g_map_font_index[font_id]].LoadFont(editor.FontLoader, this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager,
    Math.max(font_size, 1), 0, this.m_dDpiX, this.m_dDpiY, this.m_oTransform);
    */
}

void CGraphics::SetFont(const v8::Local<v8::Value>& font)
{
    /*
    if (null == font)
        return;

    this.m_oCurFont.Name        = font.FontFamily.Name;
    this.m_oCurFont.FontSize    = font.FontSize;
    this.m_oCurFont.Bold        = font.Bold;
    this.m_oCurFont.Italic      = font.Italic;

    var bItalic = true === font.Italic;
    var bBold   = true === font.Bold;

    var oFontStyle = FontStyle.FontStyleRegular;
    if ( !bItalic && bBold )
        oFontStyle = FontStyle.FontStyleBold;
    else if ( bItalic && !bBold )
        oFontStyle = FontStyle.FontStyleItalic;
    else if ( bItalic && bBold )
        oFontStyle = FontStyle.FontStyleBoldItalic;

    var _last_font = this.IsUseFonts2 ? this.m_oLastFont2 : this.m_oLastFont;
    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    _last_font.SetUpName = font.FontFamily.Name;
    _last_font.SetUpSize = font.FontSize;
    _last_font.SetUpStyle = oFontStyle;

    g_fontApplication.LoadFont(_last_font.SetUpName, AscCommon.g_font_loader, _font_manager, font.FontSize, oFontStyle, this.m_dDpiX, this.m_dDpiY, this.m_oTransform, this.LastFontOriginInfo);

    var _mD = _last_font.SetUpMatrix;
    var _mS = this.m_oTransform;

    _mD.sx = _mS.sx;
    _mD.sy = _mS.sy;
    _mD.shx = _mS.shx;
    _mD.shy = _mS.shy;
    _mD.tx = _mS.tx;
    _mD.ty = _mS.ty;
    */
}

void CGraphics::SetTextPr(const v8::Local<v8::Value>& textPr, const v8::Local<v8::Value>& theme)
{
    /*
    m_oTextPr = textPr;
    if (theme)
        m_oGrFonts.checkFromTheme(theme.themeElements.fontScheme, m_oTextPr.RFonts);
    else
        m_oGrFonts = m_oTextPr.RFonts;
    */
}

void CGraphics::SetFontSlot(const v8::Local<v8::Value>& slot, double fontSizeKoef)
{
    /*
    var _rfonts = this.m_oGrFonts;
    var _lastFont = this.IsUseFonts2 ? this.m_oLastFont2 : this.m_oLastFont;

    switch (slot)
    {
    case fontslot_ASCII:
    {
        _lastFont.Name   = _rfonts.Ascii.Name;
        _lastFont.Size = this.m_oTextPr.FontSize;
        _lastFont.Bold = this.m_oTextPr.Bold;
        _lastFont.Italic = this.m_oTextPr.Italic;

        break;
    }
    case fontslot_CS:
    {
        _lastFont.Name   = _rfonts.CS.Name;
        _lastFont.Size = this.m_oTextPr.FontSizeCS;
        _lastFont.Bold = this.m_oTextPr.BoldCS;
        _lastFont.Italic = this.m_oTextPr.ItalicCS;

        break;
    }
    case fontslot_EastAsia:
    {
        _lastFont.Name   = _rfonts.EastAsia.Name;
        _lastFont.Size = this.m_oTextPr.FontSize;
        _lastFont.Bold = this.m_oTextPr.Bold;
        _lastFont.Italic = this.m_oTextPr.Italic;

        break;
    }
    case fontslot_HAnsi:
    default:
    {
        _lastFont.Name   = _rfonts.HAnsi.Name;
        _lastFont.Size = this.m_oTextPr.FontSize;
        _lastFont.Bold = this.m_oTextPr.Bold;
        _lastFont.Italic = this.m_oTextPr.Italic;

        break;
    }
    }

    if (undefined !== fontSizeKoef)
        _lastFont.Size *= fontSizeKoef;

    var _style = 0;
    if (_lastFont.Italic)
        _style += 2;
    if (_lastFont.Bold)
        _style += 1;

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    if (_lastFont.Name != _lastFont.SetUpName || _lastFont.Size != _lastFont.SetUpSize || _style != _lastFont.SetUpStyle || !_font_manager.m_pFont)
    {
        _lastFont.SetUpName = _lastFont.Name;
        _lastFont.SetUpSize = _lastFont.Size;
        _lastFont.SetUpStyle = _style;

        g_fontApplication.LoadFont(_lastFont.SetUpName, AscCommon.g_font_loader, _font_manager, _lastFont.SetUpSize, _lastFont.SetUpStyle, this.m_dDpiX, this.m_dDpiY, this.m_oTransform, this.LastFontOriginInfo);

        var _mD = _lastFont.SetUpMatrix;
        var _mS = this.m_oTransform;
        _mD.sx = _mS.sx;
        _mD.sy = _mS.sy;
        _mD.shx = _mS.shx;
        _mD.shy = _mS.shy;
        _mD.tx = _mS.tx;
        _mD.ty = _mS.ty;
    }
    else
    {
        var _mD = _lastFont.SetUpMatrix;
        var _mS = this.m_oTransform;

        if (_mD.sx != _mS.sx || _mD.sy != _mS.sy || _mD.shx != _mS.shx || _mD.shy != _mS.shy || _mD.tx != _mS.tx || _mD.ty != _mS.ty)
        {
            _mD.sx = _mS.sx;
            _mD.sy = _mS.sy;
            _mD.shx = _mS.shx;
            _mD.shy = _mS.shy;
            _mD.tx = _mS.tx;
            _mD.ty = _mS.ty;

            _font_manager.SetTextMatrix(_mD.sx,_mD.shy,_mD.shx,_mD.sy,_mD.tx,_mD.ty);
        }
    }
    */
}

v8::Local<v8::Value> CGraphics::GetTextPr()
{
    return *m_oTextPr;
}

void CGraphics::FillText(double x, double y, const v8::Local<v8::Value>& text)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        var _code = text.charCodeAt(0);

        if (null != this.LastFontOriginInfo.Replace)
            _code = g_fontApplication.GetReplaceGlyph(_code, this.LastFontOriginInfo.Replace);

        _font_manager.LoadString4C(_code,_x,_y);
    }
    catch(err)
    {
    }

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(1,0,0,1,0,0);
    }
    var pGlyph = _font_manager.m_oGlyphString.m_pGlyphsBuffer[0];
    if (null == pGlyph)
        return;

    if (null != pGlyph.oBitmap)
    {
        this.private_FillGlyph(pGlyph);
    }
    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CGraphics::t(const v8::Local<v8::Value>& text, double x, double y, bool isBounds)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        _font_manager.LoadString2(text,_x,_y);
    }
    catch(err)
    {
    }

    this.m_oContext.setTransform(1,0,0,1,0,0);
    var _bounds = isBounds ? {x:100000, y:100000, r:-100000, b:-100000} : null;
    while (true)
    {
        var pGlyph = _font_manager.GetNextChar2();
        if (null == pGlyph)
            break;

        if (null != pGlyph.oBitmap)
        {
            this.private_FillGlyph(pGlyph, _bounds);
        }
    }
    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }

    return _bounds;
    */
}

void CGraphics::FillText2(double x, double y, const v8::Local<v8::Value>& text, double cropX, double cropW)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        var _code = text.charCodeAt(0);

        if (null != this.LastFontOriginInfo.Replace)
            _code = g_fontApplication.GetReplaceGlyph(_code, this.LastFontOriginInfo.Replace);

        _font_manager.LoadString4C(_code,_x,_y);
    }
    catch(err)
    {
    }

    this.m_oContext.setTransform(1,0,0,1,0,0);
    var pGlyph = _font_manager.m_oGlyphString.m_pGlyphsBuffer[0];
    if (null == pGlyph)
        return;

    if (null != pGlyph.oBitmap)
    {
        this.private_FillGlyphC(pGlyph,cropX,cropW);
    }
    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CGraphics::t2(const v8::Local<v8::Value>& text, double x, double y, double cropX, double cropW)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        _font_manager.LoadString2(text,_x,_y);
    }
    catch(err)
    {
    }

    this.m_oContext.setTransform(1,0,0,1,0,0);
    while (true)
    {
        var pGlyph = _font_manager.GetNextChar2();
        if (null == pGlyph)
            break;

        if (null != pGlyph.oBitmap)
        {
            this.private_FillGlyphC(pGlyph,cropX,cropW);
        }
    }

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CGraphics::FillTextCode(double x, double y, const v8::Local<v8::Value>& lUnicode)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        if (null != this.LastFontOriginInfo.Replace)
            lUnicode = g_fontApplication.GetReplaceGlyph(lUnicode, this.LastFontOriginInfo.Replace);

        _font_manager.LoadString4C(lUnicode,_x,_y);
    }
    catch(err)
    {
    }

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(1,0,0,1,0,0);
    }
    var pGlyph = _font_manager.m_oGlyphString.m_pGlyphsBuffer[0];
    if (null == pGlyph)
        return;

    if (null != pGlyph.oBitmap)
    {
        this.private_FillGlyph(pGlyph);
    }
    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CGraphics::tg(const v8::Local<v8::Value>& text, double x, double y)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        _font_manager.LoadString3C(text,_x,_y);
    }
    catch(err)
    {
    }

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(1,0,0,1,0,0);
    }
    var pGlyph = _font_manager.m_oGlyphString.m_pGlyphsBuffer[0];
    if (null == pGlyph)
        return;

    if (null != pGlyph.oBitmap)
    {
        var _a = this.m_oBrush.Color1.A;
        if (255 != _a)
            this.m_oContext.globalAlpha = _a / 255;
        this.private_FillGlyph(pGlyph);

        if (255 != _a)
            this.m_oContext.globalAlpha = 1.0;
    }
    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CGraphics::private_FillGlyph(const v8::Local<v8::Value>& pGlyph, const v8::Local<v8::Value>& _bounds)
{
    /*
    var nW = pGlyph.oBitmap.nWidth;
    var nH = pGlyph.oBitmap.nHeight;

    if (0 == nW || 0 == nH)
        return;

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    var nX = (_font_manager.m_oGlyphString.m_fX >> 0) + (pGlyph.fX + pGlyph.oBitmap.nX) >> 0;
    var nY = (_font_manager.m_oGlyphString.m_fY >> 0) + (pGlyph.fY - pGlyph.oBitmap.nY) >> 0;

    pGlyph.oBitmap.oGlyphData.checkColor(this.m_oBrush.Color1.R,this.m_oBrush.Color1.G,this.m_oBrush.Color1.B,nW,nH);

    if (null == this.TextClipRect)
        pGlyph.oBitmap.draw(this.m_oContext, nX, nY, this.TextClipRect);
    else
        pGlyph.oBitmap.drawCropInRect(this.m_oContext, nX, nY, this.TextClipRect);

    if (_bounds)
    {
        var _r = nX + pGlyph.oBitmap.nWidth;
        var _b = nY + pGlyph.oBitmap.nHeight;
        if (_bounds.x > nX)
            _bounds.x = nX;
        if (_bounds.y > nY)
            _bounds.y = nY;
        if (_bounds.r < _r)
            _bounds.r = _r;
        if (_bounds.b < _b)
            _bounds.b = _b;
    }
    */
}

void CGraphics::private_FillGlyphC(const v8::Local<v8::Value>& pGlyph, double cropX, double cropW)
{
    /*
    var nW = pGlyph.oBitmap.nWidth;
    var nH = pGlyph.oBitmap.nHeight;

    if (0 == nW || 0 == nH)
        return;

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    var nX = (_font_manager.m_oGlyphString.m_fX + pGlyph.fX + pGlyph.oBitmap.nX) >> 0;
    var nY = (_font_manager.m_oGlyphString.m_fY + pGlyph.fY - pGlyph.oBitmap.nY) >> 0;

    var d_koef = this.m_dDpiX / 25.4;

    var cX = Math.max((cropX * d_koef) >> 0, 0);
    var cW = Math.min((cropW * d_koef) >> 0, nW);
    if (cW <= 0)
        cW = 1;

    pGlyph.oBitmap.oGlyphData.checkColor(this.m_oBrush.Color1.R,this.m_oBrush.Color1.G,this.m_oBrush.Color1.B,nW,nH);
    pGlyph.oBitmap.drawCrop(this.m_oContext, nX, nY, cW, nH, cX);
    */
}

void CGraphics::private_FillGlyph2(const v8::Local<v8::Value>& pGlyph)
{
    /*
    var i = 0;
    var j = 0;

    var nW = pGlyph.oBitmap.nWidth;
    var nH = pGlyph.oBitmap.nHeight;

    if (0 == nW || 0 == nH)
        return;

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    var nX = (_font_manager.m_oGlyphString.m_fX + pGlyph.fX + pGlyph.oBitmap.nX) >> 0;
    var nY = (_font_manager.m_oGlyphString.m_fY + pGlyph.fY - pGlyph.oBitmap.nY) >> 0;

    var imageData = this.m_oContext.getImageData(nX,nY,nW,nH);
    var pPixels = imageData.data;

    var _r = this.m_oBrush.Color1.R;
    var _g = this.m_oBrush.Color1.G;
    var _b = this.m_oBrush.Color1.B;

    for (; j < nH; ++j)
    {
        var indx = 4 * j * nW;
        for (i = 0; i < nW; ++i)
        {
            var weight  = pGlyph.oBitmap.pData[j * pGlyph.oBitmap.nWidth + i];

            if (255 == weight)
            {
                pPixels[indx]     = _r;
                pPixels[indx + 1] = _g;
                pPixels[indx + 2] = _b;
                pPixels[indx + 3] = 255;
            }
            else
            {
                var r = pPixels[indx];
                var g = pPixels[indx + 1];
                var b = pPixels[indx + 2];
                var a = pPixels[indx + 3];

                pPixels[indx]     = ((_r - r) * weight + (r << 8)) >>> 8;
                pPixels[indx + 1] = ((_g - g) * weight + (g << 8)) >>> 8;
                pPixels[indx + 2] = ((_b - b) * weight + (b << 8)) >>> 8;
                pPixels[indx + 3] = (weight + a) - ((weight * a + 256) >>> 8);
            }

            indx += 4;
        }
    }
    this.m_oContext.putImageData(imageData,nX,nY);
    */
}

void CGraphics::SetIntegerGrid(bool param)
{
    /*
    if (true == param)
    {
        this.m_bIntegerGrid = true;
        this.m_oContext.setTransform(1,0,0,1,0,0);
    }
    else
    {
        this.m_bIntegerGrid = false;
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

bool CGraphics::GetIntegerGrid()
{
    return m_bIntegerGrid;
}

void CGraphics::DrawStringASCII(const std::string& name, int size, bool bold, bool italic, const v8::Local<v8::Value>& text, double x, double y, bool bIsHeader)
{
    /*
    var _textProp = {
        RFonts : { Ascii : { Name : name, Index : -1 } },
        FontSize : (((size * 2 * 96 / this.m_dDpiY) + 0.5) >> 0) / 2,
        Bold : false,
        Italic : false
    };

    this.m_oTextPr = _textProp;
    this.m_oGrFonts.Ascii.Name = this.m_oTextPr.RFonts.Ascii.Name;
    this.m_oGrFonts.Ascii.Index = -1;

    this.SetFontSlot(fontslot_ASCII, 1);

    this.m_oFontManager.LoadString2(text, 0, 0);
    var measure = this.m_oFontManager.MeasureString2();

    var _ctx = this.m_oContext;
    _ctx.beginPath();
    _ctx.fillStyle = "#E1E1E1";
    _ctx.strokeStyle = GlobalSkin.RulerOutline;
    this.m_bBrushColorInit = false;
    this.m_bPenColorInit = false;

    var _xPxOffset = 10;
    var _yPxOffset = 5;
    if (AscBrowser.isRetina)
    {
        _xPxOffset = (_xPxOffset * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
        _yPxOffset = (_yPxOffset * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
    }

    var __x = this.m_oFullTransform.TransformPointX(x, y) >> 0;
    var __y = this.m_oFullTransform.TransformPointY(x, y) >> 0;
    var __w = (measure.fWidth >> 0) + 2 * _xPxOffset;
    var __h = (Math.abs(measure.fY) >> 0) + 2 * _yPxOffset;

    if (!bIsHeader)
        __y -= __h;

    if (!AscBrowser.isRetina)
        _ctx.rect(__x + 0.5, __y + 0.5, __w, __h);
    else
        _ctx.rect(__x, __y, __w, __h);

    _ctx.fill();
    _ctx.stroke();
    _ctx.beginPath();

    this.b_color1(68, 68, 68, 255);

    var _koef_px_to_mm = 25.4 / this.m_dDpiY;

    if (bIsHeader)
        this.t(text, x + _xPxOffset * _koef_px_to_mm, y + (__h - _yPxOffset) * _koef_px_to_mm);
    else
        this.t(text, x + _xPxOffset * _koef_px_to_mm, y - _yPxOffset * _koef_px_to_mm);
    */
}

void CGraphics::DrawStringASCII2(const std::string& name, int size, bool bold, bool italic, const v8::Local<v8::Value>& text, double x, double y, bool bIsHeader)
{
    /*
    var _textProp = {
        RFonts : { Ascii : { Name : name, Index : -1 } },
        FontSize : (((size * 2 * 96 / this.m_dDpiY) + 0.5) >> 0) / 2,
        Bold : false,
        Italic : false
    };

    this.m_oTextPr = _textProp;
    this.m_oGrFonts.Ascii.Name = this.m_oTextPr.RFonts.Ascii.Name;
    this.m_oGrFonts.Ascii.Index = -1;

    this.SetFontSlot(fontslot_ASCII, 1);

    this.m_oFontManager.LoadString2(text, 0, 0);
    var measure = this.m_oFontManager.MeasureString2();

    var _ctx = this.m_oContext;
    _ctx.beginPath();
    _ctx.fillStyle = "#E1E1E1";
    _ctx.strokeStyle = GlobalSkin.RulerOutline;
    this.m_bBrushColorInit = false;
    this.m_bPenColorInit = false;

    var _xPxOffset = 10;
    var _yPxOffset = 5;
    if (AscBrowser.isRetina)
    {
        _xPxOffset = (_xPxOffset * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
        _yPxOffset = (_yPxOffset * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
    }

    var __x = this.m_oFullTransform.TransformPointX(this.m_dWidthMM - x, y) >> 0;
    var __y = this.m_oFullTransform.TransformPointY(this.m_dWidthMM - x, y) >> 0;
    var __w = (measure.fWidth >> 0) + 2 * _xPxOffset;
    var __h = (Math.abs(measure.fY) >> 0) + 2 * _yPxOffset;
    __x -= __w;

    if (!bIsHeader)
        __y -= __h;

    if (!AscBrowser.isRetina)
        _ctx.rect(__x + 0.5, __y + 0.5, __w, __h);
    else
        _ctx.rect(__x, __y, __w, __h);

    _ctx.fill();
    _ctx.stroke();
    _ctx.beginPath();

    this.b_color1(68, 68, 68, 255);

    var _koef_px_to_mm = 25.4 / this.m_dDpiY;

    var xPos = this.m_dWidthMM - x - (__w - _xPxOffset) * _koef_px_to_mm;

    if (bIsHeader)
        this.t(text, xPos, y + (__h - _yPxOffset) * _koef_px_to_mm);
    else
        this.t(text, xPos, y - _yPxOffset * _koef_px_to_mm);
    */
}

void CGraphics::DrawHeaderEdit(double yPos, const v8::Local<v8::Value>& lock_type, int sectionNum, bool bIsRepeat, const v8::Local<v8::Value>& type)
{
    /*
    var _y = this.m_oFullTransform.TransformPointY(0,yPos);
    _y = (_y >> 0) + 0.5;
    var _x = 0;
    var _wmax = this.m_lWidthPix;

    var _w1 = 6;
    var _w2 = 3;

    var _lineWidth = 1;
    var _isRetina = AscBrowser.isRetina;
    if (_isRetina && !editor.WordControl.bIsRetinaSupport)
        _isRetina = false;

    if (_isRetina)
    {
        _y >>= 0;
        _lineWidth = 2;
    }


    var ctx = this.m_oContext;

    switch (lock_type)
    {
    case locktype_None:
    case locktype_Mine:
    {
        //this.p_color(155, 187, 277, 255);
        //ctx.lineWidth = 2;
        // GlobalSkin.RulerOutline
        this.p_color(0xBB, 0xBE, 0xC2, 255);
        ctx.lineWidth = _lineWidth;
        break;
    }
    case locktype_Other:
    case locktype_Other2:
    {
        this.p_color(238, 53, 37, 255);
        ctx.lineWidth = _lineWidth;
        _w1 = 2;
        _w2 = 1;
        break;
    }
    default:
    {
        this.p_color(155, 187, 277, 255);
        ctx.lineWidth = _lineWidth;
        _w1 = 2;
        _w2 = 1;
    }
    }

    if (_isRetina)
    {
        _w1 = (_w1 * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
        _w2 = (_w2 * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
    }

    var bIsNoIntGrid = this.m_bIntegerGrid;

    if (false == bIsNoIntGrid)
        this.SetIntegerGrid(true);

    this._s();
    while (true)
    {
        if (_x > _wmax)
            break;
        ctx.moveTo(_x,_y);
        _x+=_w1;
        ctx.lineTo(_x,_y);
        _x+=_w2;
    }
    this.ds();

    var _header_text = AscCommon.translateManager.getValue("Header");
    if (-1 != sectionNum)
        _header_text += (AscCommon.translateManager.getValue(" -Section ") + (sectionNum + 1) + "-");

    if (type)
    {
        if (type.bFirst)
            _header_text = AscCommon.translateManager.getValue("First Page ") + _header_text;
        else if (EvenAndOddHeaders)
        {
            if (type.bEven)
                _header_text = AscCommon.translateManager.getValue("Even Page ") + _header_text;
            else
                _header_text = AscCommon.translateManager.getValue("Odd Page ") + _header_text;
        }
    }

    var _fontSize = _isRetina ? ((9 * AscCommon.AscBrowser.retinaPixelRatio) >> 0) : 9;
    this.DrawStringASCII("Courier New", _fontSize, false, false, _header_text, 2, yPos, true);

    if (bIsRepeat)
        this.DrawStringASCII2("Courier New", _fontSize, false, false, AscCommon.translateManager.getValue("Same as Previous"), 2, yPos, true);

    if (false == bIsNoIntGrid)
        this.SetIntegerGrid(false);
    */
}

void CGraphics::DrawFooterEdit(double yPos, const v8::Local<v8::Value>& lock_type, int sectionNum, bool bIsRepeat, const v8::Local<v8::Value>& type)
{
    /*
    var _y = this.m_oFullTransform.TransformPointY(0,yPos);
    _y = (_y >> 0) + 0.5;
    var _x = 0;
    var _w1 = 6;
    var _w2 = 3;

    var _lineWidth = 1;
    var _isRetina = AscBrowser.isRetina;
    if (_isRetina && !editor.WordControl.bIsRetinaSupport)
        _isRetina = false;

    if (_isRetina)
    {
        _y >>= 0;
        _lineWidth = 2;
    }

    var ctx = this.m_oContext;

    switch (lock_type)
    {
    case locktype_None:
    case locktype_Mine:
    {
        //this.p_color(155, 187, 277, 255);
        //ctx.lineWidth = 2;
        // GlobalSkin.RulerOutline
        this.p_color(0xBB, 0xBE, 0xC2, 255);
        ctx.lineWidth = _lineWidth;
        break;
    }
    case locktype_Other:
    case locktype_Other2:
    {
        this.p_color(238, 53, 37, 255);
        ctx.lineWidth = _lineWidth;
        _w1 = 2;
        _w2 = 1;
        break;
    }
    default:
    {
        this.p_color(155, 187, 277, 255);
        ctx.lineWidth = _lineWidth;
        _w1 = 2;
        _w2 = 1;
    }
    }

    if (_isRetina)
    {
        _w1 = (_w1 * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
        _w2 = (_w2 * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
    }

    var _wmax = this.m_lWidthPix;

    var bIsNoIntGrid = this.m_bIntegerGrid;

    if (false == bIsNoIntGrid)
        this.SetIntegerGrid(true);

    this._s();
    while (true)
    {
        if (_x > _wmax)
            break;
        ctx.moveTo(_x,_y);
        _x+=_w1;
        ctx.lineTo(_x,_y);
        _x+=_w2;
    }
    this.ds();

    var _header_text = AscCommon.translateManager.getValue("Footer");
    if (-1 != sectionNum)
        _header_text += (AscCommon.translateManager.getValue(" -Section ") + (sectionNum + 1) + "-");

    if (type)
    {
        if (type.bFirst)
            _header_text = AscCommon.translateManager.getValue("First Page ") + _header_text;
        else if (EvenAndOddHeaders)
        {
            if (type.bEven)
                _header_text = AscCommon.translateManager.getValue("Even Page ") + _header_text;
            else
                _header_text = AscCommon.translateManager.getValue("Odd Page ") + _header_text;
        }
    }

    var _fontSize = _isRetina ? ((9 * AscCommon.AscBrowser.retinaPixelRatio) >> 0) : 9;
    this.DrawStringASCII("Courier New", _fontSize, false, false, _header_text, 2, yPos, false);

    if (bIsRepeat)
        this.DrawStringASCII2("Courier New", _fontSize, false, false, AscCommon.translateManager.getValue("Same as Previous"), 2, yPos, false);

    if (false == bIsNoIntGrid)
        this.SetIntegerGrid(false);
    */
}

void CGraphics::DrawLockParagraph(const v8::Local<v8::Value>& lock_type, double x, double y1, double y2)
{
    /*
    if (lock_type == locktype_None || editor.WordControl.m_oDrawingDocument.IsLockObjectsEnable === false || editor.isViewMode || (lock_type === locktype_Mine && true === AscCommon.CollaborativeEditing.Is_Fast()))
        return;

    if (lock_type == locktype_Mine)
    {
        this.p_color(22, 156, 0, 255);
        //this.p_color(155, 187, 277, 255);
    }
    else
        this.p_color(238, 53, 37, 255);

    var _x = this.m_oFullTransform.TransformPointX(x, y1) >> 0;
    var _xT = this.m_oFullTransform.TransformPointX(x, y2) >> 0;
    var _y1 = (this.m_oFullTransform.TransformPointY(x, y1) >> 0) + 0.5;
    var _y2 = (this.m_oFullTransform.TransformPointY(x, y2) >> 0) - 1.5;

    var ctx = this.m_oContext;
    if (_x != _xT)
    {
        // значит какой-то трансформ
        var dKoefMMToPx = 1 / Math.max(this.m_oCoordTransform.sx, 0.001);
        this.p_width(1000 * dKoefMMToPx);

        var w_dot = 2 * dKoefMMToPx;
        var w_dist = 1 * dKoefMMToPx;
        var w_len_indent = 3;

        var _interf = editor.WordControl.m_oDrawingDocument.AutoShapesTrack;

        this._s();
        _interf.AddLineDash(ctx, x, y1, x, y2, w_dot, w_dist);
        _interf.AddLineDash(ctx, x, y1, x + w_len_indent, y1, w_dot, w_dist);
        _interf.AddLineDash(ctx, x, y2, x + w_len_indent, y2, w_dot, w_dist);

        this.ds();
        return;
    }

    var bIsInt = this.m_bIntegerGrid;
    if (!bIsInt)
        this.SetIntegerGrid(true);

    ctx.lineWidth = 1;

    var w_dot = 2;
    var w_dist = 1;
    var w_len_indent = (3 * this.m_oCoordTransform.sx) >> 0;

    this._s();

    var y_mem = _y1 - 0.5;
    while (true)
    {
        if ((y_mem + w_dot) > _y2)
            break;
        ctx.moveTo(_x + 0.5,y_mem);
        y_mem+=w_dot;
        ctx.lineTo(_x + 0.5,y_mem);
        y_mem+=w_dist;
    }

    var x_max = _x + w_len_indent;

    var x_mem = _x;
    while (true)
    {
        if (x_mem > x_max)
            break;
        ctx.moveTo(x_mem,_y1);
        x_mem+=w_dot;
        ctx.lineTo(x_mem,_y1);
        x_mem+=w_dist;
    }
    x_mem = _x;
    while (true)
    {
        if (x_mem > x_max)
            break;
        ctx.moveTo(x_mem,_y2);
        x_mem+=w_dot;
        ctx.lineTo(x_mem,_y2);
        x_mem+=w_dist;
    }

    this.ds();

    if (!bIsInt)
        this.SetIntegerGrid(false);
    */
}

void CGraphics::DrawLockObjectRect(const v8::Local<v8::Value>& lock_type, double x, double y, double w, double h)
{
    /*
    if (editor.isViewMode || this.IsThumbnail || lock_type == locktype_None || this.IsDemonstrationMode || (lock_type === locktype_Mine && true === AscCommon.CollaborativeEditing.Is_Fast()))
        return;

    if (editor.WordControl.m_oDrawingDocument.IsLockObjectsEnable === false && lock_type == locktype_Mine)
        return;

    if (lock_type == locktype_Mine)
    {
        this.p_color(22, 156, 0, 255);
        //this.p_color(155, 187, 277, 255);
    }
    else
        this.p_color(238, 53, 37, 255);

    var ctx = this.m_oContext;

    var _m = this.m_oTransform;
    if (_m.sx != 1.0 || _m.shx != 0.0 || _m.shy != 0.0 || _m.sy != 1.0)
    {
        // значит какой-то трансформ
        var dKoefMMToPx = 1 / Math.max(this.m_oCoordTransform.sx, 0.001);
        this.p_width(1000 * dKoefMMToPx);

        var w_dot = 2 * dKoefMMToPx;
        var w_dist = 1 * dKoefMMToPx;

        var _interf = editor.WordControl.m_oDrawingDocument.AutoShapesTrack;

        var eps = 5 * dKoefMMToPx;
        var _x = x - eps;
        var _y = y - eps;
        var _r = x + w + eps;
        var _b = y + h + eps;

        this._s();
        _interf.AddRectDash(ctx, _x, _y, _r, _y, _x, _b, _r, _b, w_dot, w_dist, true);
        this._s();
        return;
    }

    var bIsInt = this.m_bIntegerGrid;
    if (!bIsInt)
        this.SetIntegerGrid(true);

    ctx.lineWidth = 1;

    var w_dot = 2;
    var w_dist = 2;

    var eps = 5;

    var _x = (this.m_oFullTransform.TransformPointX(x, y) >> 0) - eps + 0.5;
    var _y = (this.m_oFullTransform.TransformPointY(x, y) >> 0) - eps + 0.5;

    var _r = (this.m_oFullTransform.TransformPointX(x+w, y+h) >> 0) + eps + 0.5;
    var _b = (this.m_oFullTransform.TransformPointY(x+w, y+h) >> 0) + eps + 0.5;

    this._s();

    for (var i = _x; i < _r; i += w_dist)
    {
        ctx.moveTo(i, _y);
        i += w_dot;

        if (i > _r)
            i = _r;

        ctx.lineTo(i, _y);
    }
    for (var i = _y; i < _b; i += w_dist)
    {
        ctx.moveTo(_r, i);
        i += w_dot;

        if (i > _b)
            i = _b;

        ctx.lineTo(_r, i);
    }
    for (var i = _r; i > _x; i -= w_dist)
    {
        ctx.moveTo(i, _b);
        i -= w_dot;

        if (i < _x)
            i = _x;

        ctx.lineTo(i, _b);
    }
    for (var i = _b; i > _y; i -= w_dist)
    {
        ctx.moveTo(_x, i);
        i -= w_dot;

        if (i < _y)
            i = _y;

        ctx.lineTo(_x, i);
    }

    this.ds();

    if (!bIsInt)
        this.SetIntegerGrid(false);
    */
}

void CGraphics::DrawEmptyTableLine(double x1, double y1, double x2, double y2)
{
    /*
    if ((!editor.isShowTableEmptyLine || editor.isViewMode) && (editor.isShowTableEmptyLineAttack === false))
        return;

    var _x1 = this.m_oFullTransform.TransformPointX(x1,y1);
    var _y1 = this.m_oFullTransform.TransformPointY(x1,y1);

    var _x2 = this.m_oFullTransform.TransformPointX(x2,y2);
    var _y2 = this.m_oFullTransform.TransformPointY(x2,y2);

    _x1 = (_x1 >> 0) + 0.5;
    _y1 = (_y1 >> 0) + 0.5;
    _x2 = (_x2 >> 0) + 0.5;
    _y2 = (_y2 >> 0) + 0.5;

    this.p_color(138, 162, 191, 255);
    var ctx = this.m_oContext;

    if (_x1 != _x2 && _y1 != _y2)
    {
        // значит какой-то трансформ
        var dKoefMMToPx = 1 / Math.max(this.m_oCoordTransform.sx, 0.001);

        this.p_width(1000 * dKoefMMToPx);
        this._s();
        editor.WordControl.m_oDrawingDocument.AutoShapesTrack.AddLineDash(ctx, x1, y1, x2, y2, 2 * dKoefMMToPx, 2 * dKoefMMToPx);
        this.ds();
        return;
    }

    ctx.lineWidth = 1;
    var bIsInt = this.m_bIntegerGrid;
    if (!bIsInt)
        this.SetIntegerGrid(true);
    if (_x1 == _x2)
    {
        var _y = Math.min(_y1, _y2) + 0.5;
        var _w1 = 2;
        var _w2 = 2;
        var _wmax = Math.max(_y1, _y2) - 0.5;
        this._s();
        while (true)
        {
            if (_y > _wmax)
                break;
            ctx.moveTo(_x1,_y);
            _y+=_w1;
            if (_y > _wmax)
            {
                ctx.lineTo(_x1,_y - _w1 + 1);
            }
            else
            {
                ctx.lineTo(_x1,_y);
            }
            _y+=_w2;
        }
        this.ds();
    }
    else if (_y1 == _y2)
    {
        var _x = Math.min(_x1, _x2) + 0.5;
        var _w1 = 2;
        var _w2 = 2;
        var _wmax = Math.max(_x1, _x2) - 0.5;
        this._s();
        while (true)
        {
            if (_x > _wmax)
                break;
            ctx.moveTo(_x,_y1);
            _x+=_w1;

            if (_x > _wmax)
            {
                ctx.lineTo(_x - _w2 + 1,_y1);
            }
            else
            {
                ctx.lineTo(_x,_y1);
            }
            _x+=_w2;
        }
        this.ds();
    }
    else
    {
        // значит какой-то трансформ
        this._s();
        editor.WordControl.m_oDrawingDocument.AutoShapesTrack.AddLineDash(ctx, _x1, _y1, _x2, _y2, 2, 2);
        this.ds();
    }
    if (!bIsInt)
        this.SetIntegerGrid(false);
    */
}

void CGraphics::DrawSpellingLine(double y0, double x0, double x1, double w)
{
    /*
    if (!editor.isViewMode)
                this.drawHorLine(0, y0, x0, x1, w );
    */
}

void CGraphics::drawHorLine(int align, double y, double x, double r, int penW)
{
    /*
    var _check_transform = global_MatrixTransformer.IsIdentity2(this.m_oTransform);
    if (!this.m_bIntegerGrid || !_check_transform)
    {
        if (_check_transform)
        {
            this.SetIntegerGrid(true);
            this.drawHorLine(align, y, x, r, penW);
            this.SetIntegerGrid(false);
            return;
        }

        this.p_width(penW * 1000);
        this._s();
        this._m(x, y);
        this._l(r, y);
        this.ds();
        return;
    }

    var pen_w = ((this.m_dDpiX * penW / g_dKoef_in_to_mm) + 0.5) >> 0;
    if (0 == pen_w)
        pen_w = 1;

    var _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5 - 0.5;
    var _r = (this.m_oFullTransform.TransformPointX(r,y) >> 0) + 0.5 + 0.5;

    var ctx = this.m_oContext;

    ctx.setTransform(1,0,0,1,0,0);

    ctx.lineWidth = pen_w;

    switch (align)
    {
    case 0:
    {
        // top
        var _top = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_x, _top + pen_w / 2 - 0.5);
        ctx.lineTo(_r, _top + pen_w / 2 - 0.5);
        ctx.stroke();

        break;
    }
    case 1:
    {
        // center
        var _center = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        if (0 == (pen_w % 2))
        {
            ctx.moveTo(_x, _center - 0.5);
            ctx.lineTo(_r, _center - 0.5);
        }
        else
        {
            ctx.moveTo(_x, _center);
            ctx.lineTo(_r, _center);
        }
        ctx.stroke();

        break;
    }
    case 2:
    {
        // bottom
        var _bottom = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_x, _bottom - pen_w / 2 + 0.5);
        ctx.lineTo(_r, _bottom - pen_w / 2 + 0.5);
        ctx.stroke();

        break;
    }
    }
    */
}

void CGraphics::drawHorLine2(int align, double y, double x, double r, int penW)
{
    /*
    var _check_transform = global_MatrixTransformer.IsIdentity2(this.m_oTransform);
    if (!this.m_bIntegerGrid || !_check_transform)
    {
        if (_check_transform)
        {
            this.SetIntegerGrid(true);
            this.drawHorLine2(align, y, x, r, penW);
            this.SetIntegerGrid(false);
            return;
        }

        var _y1 = y - penW / 2;
        var _y2 = _y1 + 2 * penW;

        this.p_width(penW * 1000);
        this._s();
        this._m(x, _y1);
        this._l(r, _y1);
        this.ds();

        this._s();
        this._m(x, _y2);
        this._l(r, _y2);
        this.ds();
        return;
    }

    var pen_w = ((this.m_dDpiX * penW / g_dKoef_in_to_mm) + 0.5) >> 0;
    if (0 == pen_w)
        pen_w = 1;

    var _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5 - 0.5;
    var _r = (this.m_oFullTransform.TransformPointX(r,y) >> 0) + 0.5 + 0.5;

    var ctx = this.m_oContext;
    ctx.lineWidth = pen_w;

    switch (align)
    {
    case 0:
    {
        // top
        var _top = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        var _pos1 = _top + pen_w / 2 - 0.5 - pen_w;
        var _pos2 = _pos1 + pen_w * 2;

        ctx.beginPath();
        ctx.moveTo(_x, _pos1);
        ctx.lineTo(_r, _pos1);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(_x, _pos2);
        ctx.lineTo(_r, _pos2);
        ctx.stroke();

        break;
    }
    case 1:
    {
        // center
        // TODO:

        break;
    }
    case 2:
    {
        // bottom
        // TODO:

        break;
    }
    }
    */
}

void CGraphics::drawVerLine(int align, double x, double y, double b, int penW)
{
    /*
    var _check_transform = global_MatrixTransformer.IsIdentity2(this.m_oTransform);
    if (!this.m_bIntegerGrid || !_check_transform)
    {
        if (_check_transform)
        {
            this.SetIntegerGrid(true);
            this.drawVerLine(align, x, y, b, penW);
            this.SetIntegerGrid(false);
            return;
        }

        this.p_width(penW * 1000);
        this._s();
        this._m(x, y);
        this._l(x, b);
        this.ds();
        return;
    }

    var pen_w = ((this.m_dDpiX * penW / g_dKoef_in_to_mm) + 0.5) >> 0;
    if (0 == pen_w)
        pen_w = 1;

    var _y = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5 - 0.5;
    var _b = (this.m_oFullTransform.TransformPointY(x,b) >> 0) + 0.5 + 0.5;

    var ctx = this.m_oContext;
    ctx.lineWidth = pen_w;

    switch (align)
    {
    case 0:
    {
        // left
        var _left = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_left + pen_w / 2 - 0.5, _y);
        ctx.lineTo(_left + pen_w / 2 - 0.5, _b);
        ctx.stroke();

        break;
    }
    case 1:
    {
        // center
        var _center = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;

        ctx.beginPath();
        if (0 == (pen_w % 2))
        {
            ctx.moveTo(_center - 0.5, _y);
            ctx.lineTo(_center - 0.5, _b);
        }
        else
        {
            ctx.moveTo(_center, _y);
            ctx.lineTo(_center, _b);
        }
        ctx.stroke();

        break;
    }
    case 2:
    {
        // right
        var _right = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_right - pen_w / 2 + 0.5, _y);
        ctx.lineTo(_right - pen_w / 2 + 0.5, _b);
        ctx.stroke();

        break;
    }
    }
    */
}

void CGraphics::drawHorLineExt(int align, double y, double x, double r, int penW, double leftMW, double rightMW)
{
    /*
    var _check_transform = global_MatrixTransformer.IsIdentity2(this.m_oTransform);
    if (!this.m_bIntegerGrid || !_check_transform)
    {
        if (_check_transform)
        {
            this.SetIntegerGrid(true);
            this.drawHorLineExt(align, y, x, r, penW, leftMW, rightMW);
            this.SetIntegerGrid(false);
            return;
        }

        this.p_width(penW * 1000);
        this._s();
        this._m(x, y);
        this._l(r, y);
        this.ds();
        return;
    }

    var pen_w = Math.max(((this.m_dDpiX * penW / g_dKoef_in_to_mm) + 0.5) >> 0, 1);

    var _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;
    var _r = (this.m_oFullTransform.TransformPointX(r,y) >> 0) + 0.5;

    if (leftMW != 0)
    {
        var _center = _x;
        var pen_mw = Math.max(((this.m_dDpiX * Math.abs(leftMW) * 2 / g_dKoef_in_to_mm) + 0.5) >> 0, 1);
        if (leftMW < 0)
        {
            if ((pen_mw % 2) == 0)
            {
                _x = _center - (pen_mw / 2);
            }
            else
            {
                _x = _center - ((pen_mw / 2) >> 0);
            }
        }
        else
        {
            if ((pen_mw % 2) == 0)
            {
                _x = _center + ((pen_mw / 2) - 1.0);
            }
            else
            {
                _x = _center + ((pen_mw / 2) >> 0);
            }
        }
    }
    if (rightMW != 0)
    {
        var _center = _r;
        var pen_mw = Math.max(((this.m_dDpiX * Math.abs(rightMW) * 2 / g_dKoef_in_to_mm) + 0.5) >> 0, 1);
        if (rightMW < 0)
        {
            if ((pen_mw % 2) == 0)
            {
                _r = _center - (pen_mw / 2);
            }
            else
            {
                _r = _center - ((pen_mw / 2) >> 0);
            }
        }
        else
        {
            if ((pen_mw % 2) == 0)
            {
                _r = _center + (pen_mw / 2) - 1.0;
            }
            else
            {
                _r = _center + ((pen_mw / 2) >> 0);
            }
        }
    }

    var ctx = this.m_oContext;
    ctx.lineWidth = pen_w;

    _x -= 0.5;
    _r += 0.5;

    switch (align)
    {
    case 0:
    {
        // top
        var _top = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_x, _top + pen_w / 2 - 0.5);
        ctx.lineTo(_r, _top + pen_w / 2 - 0.5);
        ctx.stroke();

        break;
    }
    case 1:
    {
        // center
        var _center = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        if (0 == (pen_w % 2))
        {
            ctx.moveTo(_x, _center - 0.5);
            ctx.lineTo(_r, _center - 0.5);
        }
        else
        {
            ctx.moveTo(_x, _center);
            ctx.lineTo(_r, _center);
        }
        ctx.stroke();

        break;
    }
    case 2:
    {
        // bottom
        var _bottom = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_x, _bottom - pen_w / 2 + 0.5);
        ctx.lineTo(_r, _bottom - pen_w / 2 + 0.5);
        ctx.stroke();

        break;
    }
    }
    */
}

void CGraphics::rect(double x, double y, double w, double h)
{
    /*
    var ctx = this.m_oContext;
    ctx.beginPath();

    if (this.m_bIntegerGrid)
    {
        if (AscCommon.global_MatrixTransformer.IsIdentity2(this.m_oFullTransform))
        {
            var _x = (this.m_oFullTransform.TransformPointX(x, y) + 0.5) >> 0;
            var _y = (this.m_oFullTransform.TransformPointY(x, y) + 0.5) >> 0;
            var _r = (this.m_oFullTransform.TransformPointX(x + w, y) + 0.5) >> 0;
            var _b = (this.m_oFullTransform.TransformPointY(x, y + h) + 0.5) >> 0;

            ctx.rect(_x, _y, _r - _x, _b - _y);
        }
        else
        {
            var x1 = this.m_oFullTransform.TransformPointX(x, y);
            var y1 = this.m_oFullTransform.TransformPointY(x, y);
            var x2 = this.m_oFullTransform.TransformPointX(x + w, y);
            var y2 = this.m_oFullTransform.TransformPointY(x + w, y);
            var x3 = this.m_oFullTransform.TransformPointX(x + w, y + h);
            var y3 = this.m_oFullTransform.TransformPointY(x + w, y + h);
            var x4 = this.m_oFullTransform.TransformPointX(x, y + h);
            var y4 = this.m_oFullTransform.TransformPointY(x, y + h);

            ctx.moveTo(x1, y1);
            ctx.lineTo(x2, y2);
            ctx.lineTo(x3, y3);
            ctx.lineTo(x4, y4);
            ctx.closePath();
        }
    }
    else
    {
        ctx.rect(x, y, w, h);
    }
    */
}

void CGraphics::TableRect(double x, double y, double w, double h)
{
    /*
    var ctx = this.m_oContext;

    if (this.m_bIntegerGrid)
    {
        var _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;
        var _y = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;
        var _r = (this.m_oFullTransform.TransformPointX(x+w,y) >> 0) + 0.5;
        var _b = (this.m_oFullTransform.TransformPointY(x,y+h) >> 0) + 0.5;

        ctx.fillRect(_x - 0.5, _y - 0.5, _r - _x + 1, _b - _y + 1);
    }
    else
    {
        ctx.fillRect(x, y, w, h);
    }
    */
}

void CGraphics::AddClipRect(double x, double y, double w, double h)
{
    /*
    var __rect = new AscCommon._rect();
    __rect.x = x;
    __rect.y = y;
    __rect.w = w;
    __rect.h = h;
    this.GrState.AddClipRect(__rect);
    */
}

void CGraphics::SetClip(const v8::Local<v8::Value>& r)
{
    /*
    var ctx = this.m_oContext;
    ctx.save();

    ctx.beginPath();
    if (!global_MatrixTransformer.IsIdentity(this.m_oTransform))
    {
        ctx.rect(r.x, r.y, r.w, r.h);
    }
    else
    {
        var _x = (this.m_oFullTransform.TransformPointX(r.x,r.y) + 1) >> 0;
        var _y = (this.m_oFullTransform.TransformPointY(r.x,r.y) + 1) >> 0;
        var _r = (this.m_oFullTransform.TransformPointX(r.x+r.w,r.y) - 1) >> 0;
        var _b = (this.m_oFullTransform.TransformPointY(r.x,r.y+r.h) - 1) >> 0;

        ctx.rect(_x, _y, _r - _x + 1, _b - _y + 1);
    }

    this.clip();
    ctx.beginPath();
    */
}

void CGraphics::RemoveClip()
{
    /*
    this.m_oContext.restore();
    this.m_oContext.save();

    this.m_bPenColorInit = false;
    this.m_bBrushColorInit = false;

    if (this.m_oContext.globalAlpha != this.globalAlpha)
        this.m_oContext.globalAlpha = this.globalAlpha;
    */
}

void CGraphics::drawCollaborativeChanges(double x, double y, double w, double h, const v8::Local<v8::Value>& Color)
{
    /*
    this.b_color1( Color.r, Color.g, Color.b, 255 );
    this.rect( x, y, w, h );
    this.df();
    */
}

void CGraphics::drawMailMergeField(double x, double y, double w, double h)
{
    /*
    this.b_color1(206, 212, 223, 204);
    //this.b_color1(216, 221, 230, 255);
    this.rect( x, y, w, h );
    this.df();
    */
}

void CGraphics::drawSearchResult(double x, double y, double w, double h)
{
    /*
    this.b_color1( 255, 238, 128, 255 );
    this.rect( x, y, w, h );
    this.df();
    */
}

void CGraphics::drawFlowAnchor(double x, double y)
{
    /*
    if (!AscCommon.g_flow_anchor || !AscCommon.g_flow_anchor.asc_complete || (!editor || !editor.ShowParaMarks))
        return;

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(1,0,0,1,0,0);
    }

    var _x = this.m_oFullTransform.TransformPointX(x,y) >> 0;
    var _y = this.m_oFullTransform.TransformPointY(x,y) >> 0;

    this.m_oContext.drawImage(AscCommon.g_flow_anchor, _x, _y);

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CGraphics::SavePen()
{
    // this.GrState.SavePen();
}

void CGraphics::RestorePen()
{
    // this.GrState.RestorePen();
}

void CGraphics::SaveBrush()
{
    // this.GrState.SaveBrush();
}

void CGraphics::RestoreBrush()
{
    // this.GrState.RestoreBrush();
}

void CGraphics::SavePenBrush()
{
    // this.GrState.SavePenBrush();
}

void CGraphics::RestorePenBrush()
{
    // this.GrState.RestorePenBrush();
}

void CGraphics::SaveGrState()
{
    // this.GrState.SaveGrState();
}

void CGraphics::RestoreGrState()
{
    // this.GrState.RestoreGrState();
}

void CGraphics::EndClipPath()
{
    // this.m_oContext.clip();
}

void CGraphics::StartCheckTableDraw()
{
    /*
    if (!this.m_bIntegerGrid && global_MatrixTransformer.IsIdentity2(this.m_oTransform))
    {
        this.SaveGrState();
        this.SetIntegerGrid(true);
        return true;
    }
    return false;
    */
}

void CGraphics::EndCheckTableDraw(bool bIsRestore)
{
    if (bIsRestore)
        RestoreGrState();
}

void CGraphics::SetTextClipRect(double _l, double _t, double _r, double _b)
{
    /*
    this.TextClipRect = {
        l : (_l * this.m_oCoordTransform.sx) >> 0,
        t : (_t * this.m_oCoordTransform.sy) >> 0,
        r : (_r * this.m_oCoordTransform.sx) >> 0,
        b : (_b * this.m_oCoordTransform.sy) >> 0
    };
    */
}

void CGraphics::AddSmartRect(double x, double y, double w, double h, int pen_w)
{
    /*
    if (!global_MatrixTransformer.IsIdentity2(this.m_oTransform))
    {
        // проверим - может все-таки можно нарисовать как надо?
        var r = x + w;
        var b = y + h;
        var dx1 = this.m_oFullTransform.TransformPointX(x, y);
        var dy1 = this.m_oFullTransform.TransformPointY(x, y);

        var dx2 = this.m_oFullTransform.TransformPointX(r, y);
        var dy2 = this.m_oFullTransform.TransformPointY(r, y);

        var dx3 = this.m_oFullTransform.TransformPointX(x, b);
        var dy3 = this.m_oFullTransform.TransformPointY(x, b);

        var dx4 = this.m_oFullTransform.TransformPointX(r, b);
        var dy4 = this.m_oFullTransform.TransformPointY(r, b);

        var _eps = 0.001;
        var bIsClever = false;
        var _type = 1;
        if (Math.abs(dx1 - dx3) < _eps &&
                Math.abs(dx2 - dx4) < _eps &&
                Math.abs(dy1 - dy2) < _eps &&
                Math.abs(dy3 - dy4) < _eps)
        {
            bIsClever = true;
            _type = 1;
        }
        if (!bIsClever &&
                Math.abs(dx1 - dx2) < _eps &&
                Math.abs(dx3 - dx4) < _eps &&
                Math.abs(dy1 - dy3) < _eps &&
                Math.abs(dy2 - dy4) < _eps)
        {
            _type = 2;
            bIsClever = true;
        }

        if (!bIsClever)
        {
            this.ds();
            return;
        }

        var _xI = (_type == 1) ? Math.min(dx1, dx2) : Math.min(dx1, dx3);
        var _rI = (_type == 1) ? Math.max(dx1, dx2) : Math.max(dx1, dx3);
        var _yI = (_type == 1) ? Math.min(dy1, dy3) : Math.min(dy1, dy2);
        var _bI = (_type == 1) ? Math.max(dy1, dy3) : Math.max(dy1, dy2);

        var bIsSmartAttack = false;
        if (!this.m_bIntegerGrid)
        {
            this.SetIntegerGrid(true);
            bIsSmartAttack = true;

            if (this.dash_no_smart)
            {
                for (var index = 0; index < this.dash_no_smart.length; index++)
                    this.dash_no_smart[index] = (this.m_oCoordTransform.sx * this.dash_no_smart[index] + 0.5) >> 0;

                this.m_oContext.setLineDash(this.dash_no_smart);
                this.dash_no_smart = null;
            }
        }

        var _pen_w = (pen_w * this.m_oCoordTransform.sx + 0.5) >> 0;
        if (0 >= _pen_w)
            _pen_w = 1;

        this._s();

        if ((_pen_w & 0x01) == 0x01)
        {
            var _x = (_xI >> 0) + 0.5;
            var _y = (_yI >> 0) + 0.5;
            var _r = (_rI >> 0) + 0.5;
            var _b = (_bI >> 0) + 0.5;

            this.m_oContext.rect(_x, _y, _r - _x, _b - _y);
        }
        else
        {
            var _x = (_xI + 0.5) >> 0;
            var _y = (_yI + 0.5) >> 0;
            var _r = (_rI + 0.5) >> 0;
            var _b = (_bI + 0.5) >> 0;

            this.m_oContext.rect(_x, _y, _r - _x, _b - _y);
        }

        this.m_oContext.lineWidth = _pen_w;
        this.ds();

        if (bIsSmartAttack)
        {
            this.SetIntegerGrid(false);
        }
        return;
    }

    var bIsSmartAttack = false;
    if (!this.m_bIntegerGrid)
    {
        this.SetIntegerGrid(true);
        bIsSmartAttack = true;

        if (this.dash_no_smart)
        {
            for (var index = 0; index < this.dash_no_smart.length; index++)
                this.dash_no_smart[index] = (this.m_oCoordTransform.sx * this.dash_no_smart[index] + 0.5) >> 0;

            this.m_oContext.setLineDash(this.dash_no_smart);
            this.dash_no_smart = null;
        }
    }

    var _pen_w = (pen_w * this.m_oCoordTransform.sx + 0.5) >> 0;
    if (0 >= _pen_w)
        _pen_w = 1;

    this._s();

    if ((_pen_w & 0x01) == 0x01)
    {
        var _x = (this.m_oFullTransform.TransformPointX(x, y) >> 0) + 0.5;
        var _y = (this.m_oFullTransform.TransformPointY(x, y) >> 0) + 0.5;
        var _r = (this.m_oFullTransform.TransformPointX(x+w, y+h) >> 0) + 0.5;
        var _b = (this.m_oFullTransform.TransformPointY(x+w, y+h) >> 0) + 0.5;

        this.m_oContext.rect(_x, _y, _r - _x, _b - _y);
    }
    else
    {
        var _x = (this.m_oFullTransform.TransformPointX(x, y) + 0.5) >> 0;
        var _y = (this.m_oFullTransform.TransformPointY(x, y) + 0.5) >> 0;
        var _r = (this.m_oFullTransform.TransformPointX(x+w, y+h) + 0.5) >> 0;
        var _b = (this.m_oFullTransform.TransformPointY(x+w, y+h) + 0.5) >> 0;

        this.m_oContext.rect(_x, _y, _r - _x, _b - _y);
    }

    this.m_oContext.lineWidth = _pen_w;
    this.ds();

    if (bIsSmartAttack)
    {
        this.SetIntegerGrid(false);
    }
    */
}

void CGraphics::CheckUseFonts2(const v8::Local<v8::Value>& _transform)
{
    /*
    if (!global_MatrixTransformer.IsIdentity2(_transform))
    {
        if (!AscCommon.g_fontManager2)
        {
            AscCommon.g_fontManager2 = new AscFonts.CFontManager();
            AscCommon.g_fontManager2.Initialize(true);
        }

        this.m_oFontManager2 = AscCommon.g_fontManager2;

        if (null == this.m_oLastFont2)
            this.m_oLastFont2 = new AscCommon.CFontSetup();

        this.IsUseFonts2 = true;
    }
    */
}

void CGraphics::UncheckUseFonts2()
{
    // IsUseFonts2 = false;
}

void CGraphics::DrawPresentationComment(const v8::Local<v8::Value>& type, double x, double y, double w, double h)
{
    /*
    if (this.IsThumbnail || this.IsDemonstrationMode)
        return;

    if (this.m_bIntegerGrid)
    {
        if (AscCommon.g_comment_image && AscCommon.g_comment_image.asc_complete === true)
        {
            var _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0);
            var _y = (this.m_oFullTransform.TransformPointY(x,y) >> 0);

            var _index = 0;
            if ((type & 0x02) == 0x02)
                _index = 2;
            if ((type & 0x01) == 0x01)
                _index += 1;

            if (this.IsRetina)
                _index += 4;

            var _offset = AscCommon.g_comment_image_offsets[_index];
            this.m_oContext.drawImage(AscCommon.g_comment_image, _offset[0], _offset[1], _offset[2], _offset[3], _x, _y, _offset[2], _offset[3]);
        }
    }
    else
    {
        this.SetIntegerGrid(true);
        this.DrawPresentationComment(type, x, y, w, h);
        this.SetIntegerGrid(false);
    }
    */
}

void CGraphics::DrawPolygon(const v8::Local<v8::Value>& oPath, int lineWidth, double shift)
{
    /*
    this.m_oContext.lineWidth = lineWidth;
    this.m_oContext.beginPath();

    var Points    = oPath.Points;

    var nCount = Points.length;
    // берем предпоследнюю точку, т.к. последняя совпадает с первой
    var PrevX = Points[nCount - 2].X, PrevY = Points[nCount - 2].Y;
    var _x = Points[nCount - 2].X,
            _y = Points[nCount - 2].Y;
    var StartX, StartY ;

    for (var nIndex = 0; nIndex < nCount; nIndex++)
    {
        if(PrevX > Points[nIndex].X)
        {
            _y = Points[nIndex].Y - shift;
        }
        else if(PrevX < Points[nIndex].X)
        {
            _y  = Points[nIndex].Y + shift;
        }

        if(PrevY < Points[nIndex].Y)
        {
            _x = Points[nIndex].X - shift;
        }
        else if(PrevY > Points[nIndex].Y)
        {
            _x = Points[nIndex].X + shift;
        }

        PrevX = Points[nIndex].X;
        PrevY = Points[nIndex].Y;

        if(nIndex > 0)
        {
            if (1 == nIndex)
            {
                StartX = _x;
                StartY = _y;
                this._m(_x, _y);
            }
            else
            {
                this._l(_x, _y);
            }
        }
    }

    this._l(StartX, StartY);
    this.m_oContext.closePath();
    this.m_oContext.stroke();
    this.m_oContext.beginPath();
    */
}

void CGraphics::DrawFootnoteRect(double x, double y, double w, double h)
{
    /*
    var _old = this.m_bIntegerGrid;
    if (!_old)
        this.SetIntegerGrid(true);

    this.p_dash([1, 1]);

    this._s();

    var l = x;
    var t = y;
    var r = x + w;
    var b = y + h;

    this.drawHorLineExt(c_oAscLineDrawingRule.Top, t, l, r, 0, 0, 0);
    this.drawVerLine(c_oAscLineDrawingRule.Right, l, t, b, 0);
    this.drawVerLine(c_oAscLineDrawingRule.Left, r, t, b, 0);
    this.drawHorLineExt(c_oAscLineDrawingRule.Top, b, l, r, 0, 0, 0);

    this.ds();
    this._s();

    this.p_dash(null);

    if (!_old)
        this.SetIntegerGrid(false);
    */
}
}