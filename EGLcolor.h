#pragma once
#include "stdafx.h"

struct EGLcolor
{
	BYTE r, g, b;

	EGLcolor() : r(0), g(0), b(0) {}

	EGLcolor(BYTE r_set, BYTE g_set, BYTE b_set) : r(b_set), g(g_set), b(r_set) {}

	EGLcolor(COLORREF color_set) :
		r(GetBValue(color_set)),
		g(GetGValue(color_set)),
		b(GetRValue(color_set))
	{}

	operator COLORREF() const
	{
		return RGB(r, g, b);
	}
};

inline EGLcolor operator *  (const EGLcolor&  color,		   float factor);
inline EGLcolor operator *  (          float factor, const EGLcolor&  color);
inline EGLcolor operator *  (const EGLcolor&     c1, const EGLcolor&     c2);
inline     void operator *= (	   EGLcolor&  color,           float factor);
inline     void operator *= (const EGLcolor&     c1, const EGLcolor&     c2);

inline EGLcolor eglColorMix(const EGLcolor& c1, float factor, const EGLcolor& c2);

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
	return EGLcolor((c1.r * c2.r) / 255,
					(c1.g * c2.g) / 255,
					(c1.b * c2.b) / 255);
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
	c1.r = (c1.r * c2.r) / 255;
	c1.g = (c1.g * c2.g) / 255;
	c1.b = (c1.b * c2.b) / 255;
}

EGLcolor eglColorMix(const EGLcolor& c1, float factor, const EGLcolor& c2)
{
	if (factor > 1.0f) factor = 1.0f;

	return EGLcolor((c1.r - c2.r)*factor + c2.r,
					(c1.g - c2.g)*factor + c2.g,
					(c1.b - c2.b)*factor + c2.b);
}
