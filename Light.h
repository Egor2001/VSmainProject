#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "stdafx.h"
#include "Math3D.h"
#include "FillFunctions.h"

struct EGLlight
    {
    Vect     place_v;
    EGLcolor color_c;

    EGLlight ();
    EGLlight (const Vect& place_set, const EGLcolor& color_set);
   ~EGLlight ();

    void     Transform     (const Matrix& transf_m);
    EGLcolor GetPointColor (const EGLcolor& pointColor, const Vect& pointPlace, const Vect& pointNormal) const;
    };

EGLlight::EGLlight ():
    place_v (Vect ()),
    color_c (RGB (255, 255, 255))
    {}

EGLlight::EGLlight (const Vect& place_set, const EGLcolor& color_set):
    place_v (place_set),
    color_c (color_set)
    {}

EGLlight::~EGLlight ()
    {}

void EGLlight::Transform (const Matrix& transf_m)
    {
    place_v *= transf_m;
    }

EGLcolor EGLlight::GetPointColor (const EGLcolor& pointColor, const Vect& pointPlace, const Vect& pointNormal) const
    {
    float factor = (!(place_v - pointPlace) % !pointNormal);

    if (factor <= 0.0f) return RGB(0, 0, 0);

    return (color_c * pointColor) * factor;
    }

#endif
