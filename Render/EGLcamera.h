#pragma once

#include "../Math/EGLmath.h"
#include "../Math/EGLmatrix.h"

class EGLcamera;

namespace EGL_CAM
{
    EGLcamera* egl_current_cam = NULL;

    EGLmatrix GetViewMatrix();
}

class EGLcamera
{
public:
    EGLcamera();
    EGLcamera(const EGLvector& eye_v_set, 
              const EGLvector& dir_v_set, 
              const EGLvector&  up_v_set, bool is_current = false);

    ~EGLcamera();

    EGLmatrix getViewMatrix() const;
    void      setCurrent   (bool is_current);
    
private:
    EGLvector eye_v_;
    EGLvector dir_v_, up_v_;
};

namespace EGL_CAM
{
    EGLmatrix GetViewMatrix()
    {
        if (!egl_current_cam) return EGLmatrix();

        return egl_current_cam->getViewMatrix();
    }
}


EGLcamera::EGLcamera(): eye_v_(), dir_v_(), up_v_() {}

EGLcamera::EGLcamera(const EGLvector& eye_v_set, 
                     const EGLvector& dir_v_set, 
                     const EGLvector&  up_v_set, bool is_current/* = false*/): 
    eye_v_(eye_v_set), dir_v_(dir_v_set), up_v_(up_v_set)
{
    eye_v_.Simple(); 

    if (is_current) 
        EGL_CAM::egl_current_cam = this;
};

EGLcamera::~EGLcamera()
{
    if (EGL_CAM::egl_current_cam == this)
        EGL_CAM::egl_current_cam = NULL;
}

EGLmatrix EGLcamera::getViewMatrix() const
{
    EGLvector x_nv = !(up_v_*dir_v_);
    EGLvector z_nv =        !dir_v_;
    EGLvector y_nv = !(z_nv*x_nv);
    
    x_nv.Simple(); y_nv.Simple(); z_nv.Simple();

    EGLmatrix view_m = EGLmatrix(x_nv.x, x_nv.y, x_nv.z, -eye_v_.x,
                                 y_nv.x, y_nv.y, y_nv.z, -eye_v_.y,
                                 z_nv.x, z_nv.y, z_nv.z, -eye_v_.z,
                                    0.0,    0.0,    0.0,       1.0);
    
    return view_m;
}

void EGLcamera::setCurrent(bool is_current)
{
    if (is_current) 
        EGL_CAM::egl_current_cam = this;
    else if (EGL_CAM::egl_current_cam == this) 
        EGL_CAM::egl_current_cam = NULL;
}
