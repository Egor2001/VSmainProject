#pragma once

#include "stdafx.h"
#include "math//EGLmath.h"
#include <functional>

class EGLslerper
{
public:
    EGLslerper(): from_q_(), to_q_(), angle_(0.0f) {}
    EGLslerper(const EGLquaternion& from_q_set, 
               const EGLquaternion&   to_q_set);

    ~EGLslerper() {}

    EGLquaternion operator()(float t)
    { 
        return from_q_*sinf(      t *angle_) + 
                 to_q_*sinf((1.0f-t)*angle_); 
    }

private:
    EGLquaternion from_q_, to_q_;
    float angle_;
};

EGLslerper::EGLslerper(const EGLquaternion& from_q_set, 
                       const EGLquaternion&   to_q_set): 
    from_q_(from_q_set), to_q_(to_q_set), angle_(0.0f) 
{
    from_q_.Simple(); 
        to_q_.Simple();

    angle_ = acosf(from_q_ % to_q_);

    float inv_sin_ang = 1.0f / sinf(angle_);
    
    from_q_ *= inv_sin_ang;
      to_q_ *= inv_sin_ang;
}
