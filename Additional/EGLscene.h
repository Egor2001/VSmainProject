#pragma once

#include "../stdafx.h"
#include "EGLsingleton.h"

#include "../Math/EGLmatrix.h"
#include "../Render/EGLcamera.h"
#include "../Render/EGLlight.h"
#include "../Render/EGLprojection.h"
#include "../Render/EGLwindow.h"
#include "../Raster/FillFunctions.h"
#include "../Render/EGLvertex.h"

class EGLscene : public EGLsingleton<EGLscene>
{
    friend class EGLsingleton<EGLscene>;

public:
    ~EGLscene();

    template<typename ...Types>
    void             setCamera(Types&& ...args);
    const EGLcamera* getCamera() const { return camera_ptr_; } 

    const EGLvector& getViewport() const { return viewport_; }

    const EGLmatrix& getMatrix() const { return matrix_stack_.top(); }

private:
    EGLscene() = default;
    
    EGLcamera*            camera_ptr_;
    std::vector<EGLlight> light_vec_;             
    EGLvector             viewport_;
    std::stack<EGLmatrix> matrix_stack_;
    
};

EGLscene::~EGLscene()
{
    if (camera_ptr_) delete camera_ptr_;
}

template<typename ...Types>
void EGLscene::setCamera(Types&& ...args)
{
    if (camera_ptr_) delete camera_ptr_;

    camera_ptr_ = new EGLcamera(std::forward<Types>(args)...);
}
