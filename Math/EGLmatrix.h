#pragma once
#include "stdafx.h"
#include "math//EGLmath.h"

using std::stack;

void eglPushMatrix();
void eglPopMatrix();
void eglMulMatrix(const EGLmatrix& mult_m);
void eglLoadIdentity();

namespace EGL_MAT {
    stack<EGLmatrix> egl_mat_stack = stack<EGLmatrix>({{EGLmatrix()}});
    
    const EGLmatrix& GetCurrentMatrix() { return egl_mat_stack.top(); }
}

void eglPushMatrix()
{
	EGL_MAT::egl_mat_stack.push(EGL_MAT::egl_mat_stack.top());
}

void eglPopMatrix()
{
    if (EGL_MAT::egl_mat_stack.size() > 1)
	    EGL_MAT::egl_mat_stack.pop();
}
    
void eglMulMatrix(const EGLmatrix& mult_m)
{
    EGL_MAT::egl_mat_stack.top() = mult_m * EGL_MAT::egl_mat_stack.top();
}
    
void eglLoadIdentity()
{
    eglPopMatrix();
    eglPushMatrix();
}
