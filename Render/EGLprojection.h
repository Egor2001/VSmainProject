#pragma once
#include "../stdafx.h"
#include "../Render/EGLwindow.h"
#include "../Math/EGLmath.h"

void eglDisableProjection();
void eglOrtho  (float right_set, float left_set, float top_set, float bottom_set, float near_set, float far_set);
void eglFrustum(float right_set, float left_set, float top_set, float bottom_set, float near_set, float far_set);

namespace EGL_PROJ
{
    bool      egl_is_proj = false;
    EGLmatrix egl_proj_m = EGLmatrix();

    bool World_to_CVV     (EGLvector& proc_v);
    bool World_to_Viewport(EGLvector& proc_v, const EGLvector& viewport_v);
    
    bool World_to_CVV(EGLvector& proc_v)
    {
        proc_v *= egl_proj_m;

        if (proc_v.p == 0.0f) return false;
        
        proc_v.Simple();
        
        if (proc_v.x > 1.0f || -1.0f > proc_v.x ||
            proc_v.y > 1.0f || -1.0f > proc_v.y ||
            proc_v.z > 1.0f || -1.0f > proc_v.z) return false;

        return true;
    }

    bool World_to_Viewport(EGLvector& proc_v, const EGLvector& viewport_v)
    {
        bool res = true;

        if (egl_is_proj)
        {
            if (!World_to_CVV(proc_v)) res = false;

            proc_v.x = (viewport_v.x / 2.0f)*(proc_v.x + 1.0f);
            proc_v.y = (viewport_v.y / 2.0f)*(proc_v.y + 1.0f);
            proc_v.z = (viewport_v.z / 2.0f)*(proc_v.z + 1.0f);
            proc_v.p = viewport_v.p;

            proc_v.Simple();
        }
        else 
        {
            proc_v += viewport_v / 2.0f; proc_v.Simple();

            if (proc_v.x < 0.0f || proc_v.x > viewport_v.x ||
                proc_v.y < 0.0f || proc_v.y > viewport_v.y ||
                proc_v.z < 0.0f || proc_v.z > viewport_v.z) res = false;
        }
        
        return res;
    }
}

void eglOrtho(float right_set, float left_set, float top_set, float bottom_set, float near_set, float far_set)
{
    EGLmatrix result_m;
    
    float ort00 =  2/(right_set-  left_set), ort03 = -(right_set+  left_set)/(right_set-  left_set),
          ort11 =  2/  (top_set-bottom_set), ort13 =   -(top_set+bottom_set)/  (top_set-bottom_set),
          ort22 = -2/  (far_set-  near_set), ort23 =   -(far_set+  near_set)/  (far_set-  near_set);
    //world to view different systems('-')?  
    result_m = EGLmatrix(ort00,   0.0,   0.0, ort03,
                           0.0, ort11,   0.0, ort13,
                           0.0,   0.0, ort22, ort23,
                           0.0,   0.0,   0.0,   1.0);
    
    EGL_PROJ::egl_proj_m = result_m;
    EGL_PROJ::egl_is_proj = true;
}

void eglFrustum(float right_set, float left_set, float top_set, float bottom_set, float near_set, float far_set)
{
    EGLmatrix result_m;
    
    float fru00 =         2*near_set /(right_set-  left_set), fru02 =  (right_set+  left_set)/(right_set-  left_set),
          fru11 =         2*near_set /  (top_set-bottom_set), fru12 =    (top_set+bottom_set)/  (top_set-bottom_set),
          fru22 = -(far_set+near_set)/  (far_set-  near_set), fru23 = -2*(far_set*  near_set)/  (far_set-  near_set);

    result_m = EGLmatrix(fru00,   0.0, fru02,   0.0,
                           0.0, fru11, fru12,   0.0,
                           0.0,   0.0, fru22, fru23,
                           0.0,   0.0,  -1.0,   0.0);

    EGL_PROJ::egl_proj_m = result_m;
    EGL_PROJ::egl_is_proj = true;
}

void eglDisableProjection() { EGL_PROJ::egl_is_proj = false; }
