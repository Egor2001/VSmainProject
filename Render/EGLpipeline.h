#pragma once

#include "../stdafx.h"
#include "../Math/EGLmatrix.h"
#include "../Render/EGLcamera.h"
#include "../Render/EGLlight.h"
#include "../Render/EGLprojection.h"
#include "../Render/EGLwindow.h"
#include "../Raster/FillFunctions.h"
#include "../Render/EGLvertex.h"

enum class EGLrenderType { RT_WIRE, RT_SOLID_FILL, RT_SOLID_LERP };

void eglSetVertex   (const EGLvector& vector_set, const EGLvector& normal_set, const EGLcolor& color_set);
void eglSetVertexBuf(const EGLvertexbuf& vertex_buf_set);
void eglSetIndexBuf (const EGLindexbuf& vertex_buf_set);
void eglRender      (EGLrenderType render_type);

namespace EGL_PPL
{
    void RenderWire();
    void RenderSolidFill();
    void RenderSolidLerp();

    EGLvertexbuf egl_vertexbuf;
    EGLindexbuf  egl_indexbuf;
    EGLvector viewport_v;
    
    void RenderWire()
    {    
        EGLvertex vert1, vert2, vert3;
        
        for (const EGLindex3& ind_triang : egl_indexbuf)
        {
            vert1 = egl_vertexbuf[ind_triang.ind1]; 
            vert2 = egl_vertexbuf[ind_triang.ind2]; 
            vert3 = egl_vertexbuf[ind_triang.ind3]; 

            if (vert1.vector.p == 0.0f || 
                vert2.vector.p == 0.0f || 
                vert3.vector.p == 0.0f) continue; 

            eglLine(vert1.color, vert1.vector.x, vert1.vector.y, vert1.vector.z, 
                    vert2.color, vert2.vector.x, vert2.vector.y, vert2.vector.z);
            eglLine(vert2.color, vert2.vector.x, vert2.vector.y, vert2.vector.z, 
                    vert3.color, vert3.vector.x, vert3.vector.y, vert3.vector.z);
            eglLine(vert3.color, vert3.vector.x, vert3.vector.y, vert3.vector.z, 
                    vert1.color, vert1.vector.x, vert1.vector.y, vert1.vector.z);
        }
    }

    void RenderSolidFill()
    {    
        EGLvertex vert1, vert2, vert3;
        EGLcolor color;

        for (const EGLindex3& ind_triang : egl_indexbuf)
        {
            vert1 = egl_vertexbuf[ind_triang.ind1]; 
            vert2 = egl_vertexbuf[ind_triang.ind2]; 
            vert3 = egl_vertexbuf[ind_triang.ind3]; 

            if (vert1.vector.p == 0.0f || 
                vert2.vector.p == 0.0f || 
                vert3.vector.p == 0.0f) continue; 

            color = EGLcolor((vert1.color.r + vert2.color.r + vert3.color.r)/3,
                             (vert1.color.g + vert2.color.g + vert3.color.g)/3,
                             (vert1.color.b + vert2.color.b + vert3.color.b)/3);

            eglFillTriangle(vert1.vector.x, vert1.vector.y, vert1.vector.z,
                            vert2.vector.x, vert2.vector.y, vert2.vector.z,
                            vert3.vector.x, vert3.vector.y, vert3.vector.z, color);
        }
    }

    void RenderSolidLerp()
    {    
        EGLvertex vert1, vert2, vert3;

        for (const EGLindex3& ind_triang : egl_indexbuf)
        {
            vert1 = egl_vertexbuf[ind_triang.ind1]; 
            vert2 = egl_vertexbuf[ind_triang.ind2]; 
            vert3 = egl_vertexbuf[ind_triang.ind3]; 

            if (vert1.vector.p == 0.0f || 
                vert2.vector.p == 0.0f || 
                vert3.vector.p == 0.0f) continue; 

            eglFillTriangle(vert1.color, vert1.vector.x, vert1.vector.y, vert1.vector.z,
                            vert2.color, vert2.vector.x, vert2.vector.y, vert2.vector.z,
                            vert3.color, vert3.vector.x, vert3.vector.y, vert3.vector.z);
        }
    }
}

void eglSetVertex(const EGLvector& vector_set, const EGLvector& normal_set, const EGLcolor& color_set)
{
    EGLvertex vert = EGLvertex(vector_set, normal_set, color_set);
    
    vert.vector *= EGL_MAT  ::GetCurrentMatrix();
    vert.normal *= EGL_MAT  ::GetCurrentMatrix(); vert.normal = !vert.normal;
    vert.color   = EGL_LIGHT::GetVertexColor(vert);
        
    if (!EGL_PROJ::World_to_Viewport(vert.vector, EGL_PPL::viewport_v))
        vert.vector.p = 0.0f;
    else vert.vector.Simple();

    EGL_PPL::egl_vertexbuf.push_back(vert);
}

void eglSetVertexBuf(const EGLvertexbuf& vertex_buf_set)
{
    if (vertex_buf_set.empty()) return;

    EGL_PPL::egl_vertexbuf.resize(vertex_buf_set.size());
    std::copy(vertex_buf_set.begin(), vertex_buf_set.end(), EGL_PPL::egl_vertexbuf.begin());

    eglPushMatrix();
    eglMulMatrix(EGL_CAM::GetViewMatrix());

    for (EGLvertex& vert : EGL_PPL::egl_vertexbuf)
    {
        vert.vector *= EGL_MAT  ::GetCurrentMatrix();
        vert.normal *= EGL_MAT  ::GetCurrentMatrix(); vert.normal = !vert.normal;
        vert.color   = EGL_LIGHT::GetVertexColor(vert);
            
    if (!EGL_PROJ::World_to_Viewport(vert.vector, EGL_PPL::viewport_v))
        vert.vector.p = 0.0f;
    else vert.vector.Simple();
    }

    eglPopMatrix();
}

void eglSetIndexBuf(const EGLindexbuf& index_buf_set)
{
    if (index_buf_set.empty()) return;

    EGL_PPL::egl_indexbuf.resize(index_buf_set.size());
    std::copy(index_buf_set.begin(), index_buf_set.end(), EGL_PPL::egl_indexbuf.begin());
}
    
void eglRender(EGLrenderType render_type)
{
    EGL_PPL::viewport_v = EGLvector(EGL_WND::GetWndSize().cx, EGL_WND::GetWndSize().cy, 1.0f);

    switch(render_type)
    {
        case EGLrenderType::RT_WIRE:       EGL_PPL::RenderWire();      break;
        case EGLrenderType::RT_SOLID_FILL: EGL_PPL::RenderSolidFill(); break;
        case EGLrenderType::RT_SOLID_LERP: EGL_PPL::RenderSolidLerp(); break;
        default: throw exception("invalid render type"); break;
    }
}
