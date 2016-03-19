#ifndef FILL_FUNCTIONS_H_INCLUDED
#define FILL_FUNCTIONS_H_INCLUDED

#include "stdafx.h"
#include "EGLwindow.h"

struct EGLcolor
    {
    unsigned char r, g, b;

    EGLcolor (): r (0), g (0), b (0) {}

    EGLcolor (unsigned char r_set,
              unsigned char g_set,
              unsigned char b_set): r (b_set), g (g_set), b (r_set) {}

    EGLcolor (COLORREF color_set):
        r (GetBValue (color_set)),
        g (GetGValue (color_set)),
        b (GetRValue (color_set))
        {}

    operator COLORREF() const
        {
        return RGB(r, g, b);
        }
    };

inline EGLcolor operator *  (const EGLcolor&  color,           float factor);
inline EGLcolor operator *  (          float factor, const EGLcolor&  color);
inline EGLcolor operator *  (const EGLcolor&     c1, const EGLcolor&     c2);
inline     void operator *= (      EGLcolor&  color,           float factor);
inline     void operator *= (const EGLcolor&     c1, const EGLcolor&     c2);

inline EGLcolor eglColorMix (const EGLcolor& c1, float factor, const EGLcolor& c2);

EGLcolor operator * (const EGLcolor& color, float factor)
    {
    if (factor > 1.0f) factor = 1.0f;

    return EGLcolor(color.r*factor, color.g*factor, color.b*factor);
    }

EGLcolor operator * (float factor, const EGLcolor& color)
    {
    if (factor > 1.0f) factor = 1.0f;

    return EGLcolor(color.r*factor, color.g*factor, color.b*factor);
    }

EGLcolor operator * (const EGLcolor& c1, const EGLcolor& c2)
    {
    return EGLcolor((c1.r * c2.r)/255,
                    (c1.g * c2.g)/255,
                    (c1.b * c2.b)/255);
    }

void operator *= (EGLcolor& color, float factor)
    {
    if (factor > 1.0f) factor = 1.0f;

    color.r *= factor;
    color.g *= factor;
    color.b *= factor;
    }

void operator *= (EGLcolor& c1, const EGLcolor& c2)
    {
    c1.r = (c1.r * c2.r)/255;
    c1.g = (c1.g * c2.g)/255;
    c1.b = (c1.b * c2.b)/255;
    }

EGLcolor eglColorMix (const EGLcolor& c1, float factor, const EGLcolor& c2)
    {
    if (factor > 1.0f) factor = 1.0f;

    return EGLcolor((c1.r-c2.r)*factor + c2.r,
                    (c1.g-c2.g)*factor + c2.g,
                    (c1.b-c2.b)*factor + c2.b);
    }

inline bool eglInBorders(short x, short y);

inline void eglSetPixel (short x, short y, float z, const EGLcolor& color);
//without LERP:
inline void eglPixelSequence (short x0,          float z0,
                              short x1, short y, float z1, const EGLcolor& color);

bool eglLine (short x0, short y0, float z0,
              short x1, short y1, float z1, const EGLcolor& color);

bool eglFillPart (short x0, short y0, float z0,
                  short x1          , float z1,
                  short x2, short y1, float z2, const EGLcolor& color);

bool eglFillTriangle (short x0, short y0, float z0,
                      short x1, short y1, float z1,
                      short x2, short y2, float z2, const EGLcolor& color);
//with LERP:
inline void eglPixelSequence (EGLcolor c0, short x0,          float z0,
                              EGLcolor c1, short x1, short y, float z1);

bool eglLine (const EGLcolor& c0, short x0, short y0, float z0,
              const EGLcolor& c1, short x1, short y1, float z1);

bool eglCircle (const EGLcolor& color, short x, short y, float z, unsigned short r);

bool eglFillPart (EGLcolor c0, short x0, short y0, float z0,
                  EGLcolor c1, short x1          , float z1,
                  EGLcolor c2, short x2, short y1, float z2);

bool eglFillTriangle (EGLcolor c0, short x0, short y0, float z0,
                      EGLcolor c1, short x1, short y1, float z1,
                      EGLcolor c2, short x2, short y2, float z2);

inline bool eglInBorders(short x, short y)
{
	return x >= 0 && x < EGL_RES::egl_depthBufSz.cx &&
		   y >= 0 && y < EGL_RES::egl_depthBufSz.cy;
}

inline void eglSetPixel (short x, short y, float z, const EGLcolor& color)
{
	if (!eglInBorders(x, y)) return;

	if (EGL_RES::egl_depthBuf[EGL_RES::egl_depthBufSz.cx*y + x] > z)
	{
		ULONG index = EGL_RES::egl_colorBufSz.cx*(EGL_RES::egl_colorBufSz.cy-1 - y) + x*3;

		EGL_RES::egl_colorBuf[index + 0] = color.b;
		EGL_RES::egl_colorBuf[index + 1] = color.g;
		EGL_RES::egl_colorBuf[index + 2] = color.r;

		EGL_RES::egl_depthBuf[EGL_RES::egl_depthBufSz.cx*y + x] = z;
	}
}
//without Lerp
inline void eglPixelSequence (short x0,          float z0,
                              short x1, short y, float z1, const EGLcolor& color)
{
	if (!(eglInBorders(x0, y) || eglInBorders(x1, y))) return;

	if (x0 == x1)
    {
        eglSetPixel (x1, y, (z0+z1)/2.0f, color);

        return;
    }

    if (x0 > x1)
    {
        std::swap (x0, x1);
        std::swap (z0, z1);
    }

    float fact     = 0.0f;
    float factStep = 1.0f / float (abs(x1 - x0)+1);

    for (short xCur = x0; xCur <= x1; xCur++, fact += factStep)
    {
        eglSetPixel (xCur, y, (z1-z0)*fact + z0, color);
    }
}

bool eglLine (short x0, short y0, float z0,
              short x1, short y1, float z1, const EGLcolor& color)
{
	if (!(eglInBorders(x0, y0) || eglInBorders(x1, y1))) return false;
	
	bool inv = false;

    short dx = abs(x1 - x0),
          dy = abs(y1 - y0);

    if (dx < dy)
    {
        inv = true;

        std::swap (x0, y0);
        std::swap (x1, y1);
        std::swap (dx, dy);
    }

    if (dx == 0) return false;

    if (x0 > x1)
    {
        std::swap (x0, x1);
        std::swap (y0, y1);
        std::swap (z0, z1);
    }

    short yStep = y0 < y1 ? 1 : -1;
    short err   = dx / 2;

    float fact     = 0.0f;
    float factStep = 1.0f / float (dx+1);

    for (short xCur = x0, yCur = y0; xCur <= x1; xCur++, fact += factStep)
    {
        eglSetPixel(inv ? yCur : xCur,
                    inv ? xCur : yCur, (z0-z1)*fact + z1, color);

        err -= dy;

        if (err < 0)
        {
            yCur += yStep;
            err  += dx;
        }
    }

    return true;
}

bool eglFillPart (short x0, short y0, float z0,
                  short x1          , float z1,
                  short x2, short y1, float z2, const EGLcolor& color)
{
	if (!(eglInBorders(x0, y0) || eglInBorders(x1, y0) || eglInBorders(x2, y1))) return false;
	
	if (x1 > x2)
    {
        std::swap (x1, x2);
        std::swap (z1, z2);
    }

    short dxR = (short) abs (x2 - x0),
          dxL = (short) abs (x1 - x0),
          dy  = (short) abs (y1 - y0);

    if (dy == 0) return false;

    short errR = dy/2, errL = dy/2;

    short xStepR = x0 < x2 ? 1 : -1,
          xStepL = x0 < x1 ? 1 : -1,
          yStep  = y0 < y1 ? 1 : -1;

    short xShiftR = xStepR*floor (dxR/dy),
          xShiftL = xStepL*floor (dxL/dy);

    short derrR = dxR%dy,
          derrL = dxL%dy;

    if (dy+yStep+1 == 0) return false;

    float fact     = 0.0f;
    float factStep = 1.0f / float (dy+yStep+1);

    for (short yCur  = y0   , xCurR  = x0     , xCurL  = x0; yCur != (y1+yStep);
               yCur += yStep, xCurR += xShiftR, xCurL += xShiftL, fact += factStep)
    {
        errR -= derrR;
        errL -= derrL;

        if (errL < 0)
        {
            xCurL += xStepL;
             errL += dy;
        }

        if (errR < 0)
        {
            xCurR += xStepR;
             errR += dy;
        }

        eglPixelSequence (xCurL,       (z1-z0)*fact + z0,
                          xCurR, yCur, (z2-z0)*fact + z0, color);
    }

    return true;
}

bool eglFillTriangle (short x0, short y0, float z0,
                      short x1, short y1, float z1,
                      short x2, short y2, float z2, const EGLcolor& color)
{
	if (!(eglInBorders(x0, y0) || eglInBorders(x1, y1) || eglInBorders(x2, y2))) return false;

	if (y0 > y1)
    {
        std::swap (x0, x1);
        std::swap (y0, y1);
        std::swap (z0, z1);
    }

    if (y1 > y2)
    {
        std::swap (x1, x2);
        std::swap (y1, y2);
        std::swap (z1, z2);
    }

    if (y0 > y1)
    {
        std::swap (x0, x1);
        std::swap (y0, y1);
        std::swap (z0, z1);
    }

    if (abs (y2 - y0) == 0) return false;

    short    xMid = x0 + (x2 - x0) * (y1-y0) / (y2-y0);
    float    zMid = z0 + (z2 - z0) * (y1-y0) / (y2-y0);

    eglFillPart (x0  , y0, z0,
                 xMid,     zMid,
                 x1  , y1, z1, color);

    eglFillPart (x2  , y2, z2,
                 xMid,     zMid,
                 x1  , y1, z1, color);

    return true;
}
//with Lerp:
inline void eglPixelSequence (EGLcolor c0, short x0,          float z0,
                              EGLcolor c1, short x1, short y, float z1)
{
    if (x0 == x1)
    {
        eglSetPixel (x1, y, (z0+z1)/2.0f, eglColorMix (c1, 0.5f, c0));

        return;
    }

    if (x0 > x1)
    {
        std::swap (x0, x1);
        std::swap (z0, z1);
        std::swap (c0, c1);
    }

    float fact     = 0.0f;
    float factStep = 1.0f / float (abs(x1 - x0)+1);

    for (short xCur = x0; xCur <= x1; xCur++, fact += factStep)
    {
        eglSetPixel (xCur, y, (z1-z0)*fact + z0, eglColorMix (c1, fact, c0));
    }
}

bool eglLine (const EGLcolor& c0, short x0, short y0, float z0,
              const EGLcolor& c1, short x1, short y1, float z1)
{
    if (x0 < 0 || y0 < 0 ||
        x1 < 0 || y1 < 0) return false;

    bool inv = false;

    short dx = abs(x1 - x0),
          dy = abs(y1 - y0);

    if (dx < dy)
    {
        inv = true;

        std::swap (x0, y0);
        std::swap (x1, y1);
        std::swap (dx, dy);
    }

    if (dx == 0) return false;

    if (x0 > x1)
    {
        std::swap (x0, x1);
        std::swap (y0, y1);
        std::swap (z0, z1);
    }

    short yStep = y0 < y1 ? 1 : -1;
    short err   = dx / 2;

    float fact     = 0.0f;
    float factStep = 1.0f / float (dx+1);

    for (short xCur = x0, yCur = y0; xCur <= x1; xCur++, fact += factStep)
    {
        eglSetPixel(inv ? yCur : xCur,
                    inv ? xCur : yCur, (z0-z1)*fact + z1, eglColorMix (c0, fact, c1));

        err -= dy;

        if (err < 0)
        {
            yCur += yStep;
            err  += dx;
        }
    }

    return true;
}

bool eglCircle (const EGLcolor& color, short x, short y, float z, unsigned short r)
{
    if (r == 0) return false;

    short err = r - 1;

    for (short xCur = 0, yCur = r; xCur <= yCur; xCur++)
    {
        eglSetPixel (x + xCur, y + yCur, z, color);
        eglSetPixel (x + xCur, y - yCur, z, color);
        eglSetPixel (x + yCur, y + xCur, z, color);
        eglSetPixel (x + yCur, y - xCur, z, color);
        eglSetPixel (x - xCur, y + yCur, z, color);
        eglSetPixel (x - xCur, y - yCur, z, color);
        eglSetPixel (x - yCur, y + xCur, z, color);
        eglSetPixel (x - yCur, y - xCur, z, color);

        if (err < 0)
        {
            yCur--;
            err -= (xCur - yCur + 1)*2;
        }

        else err -= xCur*2 + 1;
    }

    return true;
}

bool eglFillPart (EGLcolor c0, short x0, short y0, float z0,
                  EGLcolor c1, short x1,           float z1,
                  EGLcolor c2, short x2, short y1, float z2)
{
    if (x1 > x2)
    {
        std::swap (x1, x2);
        std::swap (z1, z2);
        std::swap (c1, c2);
    }

    short dxR = (short) abs (x2 - x0),
          dxL = (short) abs (x1 - x0),
          dy  = (short) abs (y1 - y0);

    if (dy == 0) return false;

    short errR = dy/2, errL = dy/2;

    short xStepR = x0 < x2 ? 1 : -1,
          xStepL = x0 < x1 ? 1 : -1,
          yStep  = y0 < y1 ? 1 : -1;

    short xShiftR = xStepR*floor (dxR/dy),
          xShiftL = xStepL*floor (dxL/dy);

    short derrR = dxR%dy,
          derrL = dxL%dy;

    if (dy+yStep+1 == 0) return false;

    float fact     = 0.0f;
    float factStep = 1.0f / float (dy+yStep+1);

    for (short yCur  = y0   , xCurR  = x0     , xCurL  = x0; yCur != (y1+yStep);
               yCur += yStep, xCurR += xShiftR, xCurL += xShiftL, fact += factStep)
    {
        errR -= derrR;
        errL -= derrL;

        if (errL < 0)
        {
            xCurL += xStepL;
             errL += dy;
        }

        if (errR < 0)
        {
            xCurR += xStepR;
             errR += dy;
        }

        eglPixelSequence (eglColorMix (c1, fact, c0), xCurL,       (z1-z0)*fact + z0,
                          eglColorMix (c2, fact, c0), xCurR, yCur, (z2-z0)*fact + z0);
    }

    return true;
}

bool eglFillTriangle (EGLcolor c0, short x0, short y0, float z0,
                      EGLcolor c1, short x1, short y1, float z1,
                      EGLcolor c2, short x2, short y2, float z2)
{
    if (y0 > y1)
    {
        std::swap (x0, x1);
        std::swap (y0, y1);
        std::swap (z0, z1);
        std::swap (c0, c1);
    }

    if (y1 > y2)
    {
        std::swap (x1, x2);
        std::swap (y1, y2);
        std::swap (z1, z2);
        std::swap (c1, c2);
    }

    if (y0 > y1)
    {
        std::swap (x0, x1);
        std::swap (y0, y1);
        std::swap (z0, z1);
        std::swap (c0, c1);
    }

    if (abs (y2 - y0) == 0) return false;

    short    xMid = x0 + (x2 - x0) * (y1-y0) / (y2-y0);
    float    zMid = z0 + (z2 - z0) * (y1-y0) / (y2-y0);
    EGLcolor cMid = eglColorMix (c2, float(y1-y0) / float(y2-y0), c0);

    eglFillPart (c0  , x0  , y0, z0,
                 cMid, xMid,     zMid,
                 c1  , x1  , y1, z1);

    eglFillPart (c2  , x2  , y2, z2,
                 cMid, xMid,     zMid,
                 c1  , x1  , y1, z1);

    return true;
}

#endif
