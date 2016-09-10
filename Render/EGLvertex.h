#pragma once
#include "../Raster/EGLcolor.h"
#include "../Math/EGLmath.h"

struct EGLvertex
{
    EGLvertex(): vector(), normal(), color() {}
    EGLvertex(const EGLvector& vector_set, const EGLvector& normal_set, const EGLcolor& color_set): 
        vector(vector_set), normal(normal_set), color(color_set) {}

    EGLvector vector;
    EGLvector normal;
    EGLcolor  color;
};

struct EGLindex3
{
    EGLindex3(): ind1(-1), ind2(-1), ind3(-1) {}
    EGLindex3(USHORT ind1_set, USHORT ind2_set, USHORT ind3_set): 
        ind1(ind1_set), ind2(ind2_set), ind3(ind3_set) {}

    USHORT ind1, ind2, ind3;
};

typedef vector<EGLvertex> EGLvertexbuf;
typedef vector<EGLindex3>  EGLindexbuf;
