#pragma once
#include "stdafx.h"
#include "Math3D.h"

using std::stack;

namespace EGL_MAT {
	stack<Matrix> mat_stack = stack<Matrix>();
		  Matrix  mat_cur   =		Matrix ();
	float		  mouseRotateRadius = 0.0f;
	float         mouseScaleFactor  = 0.0f;
}

void eglPushMatrix();
void eglPopMatrix();
void eglLoadIdentity();

void eglPushMatrix()
{
	if (!EGL_MAT::mat_stack.empty()) EGL_MAT::mat_cur *= EGL_MAT::mat_stack.top();
	EGL_MAT::mat_stack.push(Matrix());
}

void eglPopMatrix()
{
	EGL_MAT::mat_stack.pop();
	if (!EGL_MAT::mat_stack.empty()) EGL_MAT::mat_cur /= EGL_MAT::mat_stack.top();
}

void eglLoadIdentity()
{
	EGL_MAT::mat_stack.top() = Matrix();
}
