#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "stdafx.h"
#include "Object.h"
#include "Parser.h"

class Function
    {
    public:

    Function ();
    Function (float screenX_set, float screenY_set,
              int      side_set, float    step_set, const string& equation_set);

   ~Function ();

    void SetFunc (const string& equation_set)
        { equation_.SetExp (equation_set); PointsFill(); }

    void Draw       (COLORREF color);
    void Transform  ();
    void PrintMatrix();
    void PointsFill (float step_set = 0.0f);
    void IntoFile   (LPCSTR fname);

    private:
    float     screenX_;
    float     screenY_;
    int       side_;
    float     step_;
    Matrix    current_m_;
    EGLobject object_;
    Parser    equation_;
    };

Function::Function():
    screenX_   (          0),
    screenY_   (          0),
    side_      (          0),
    step_      (          0),
    current_m_ (   Matrix()),
    object_    (EGLobject()),
    equation_  (   Parser())
    {
    equation_.SetVar ("x", 0);
    equation_.SetVar ("y", 0);
    }

Function::Function (float screenX_set, float screenY_set,
                    int      side_set, float    step_set, const string& equation_set):
    screenX_   (screenX_set),
    screenY_   (screenY_set),
    side_      (   side_set),
    step_      (   step_set),
    current_m_ (   Matrix()),
    object_    (EGLobject (side_*side_, 2*(side_-1)*(side_-1)*3)),
    equation_  (equation_set)
    {
    equation_.SetVar ("x", 0);
    equation_.SetVar ("y", 0);

    if (side_ == 0) return;

    for (int i = 0;  i < (side_-1); i++)
    for (int j = 0;  j < (side_-1); j++)
        {
        object_.Get_index_buf()[(j*(side_-1) + i)*6    ] =  j   *(side_) + i+1;
        object_.Get_index_buf()[(j*(side_-1) + i)*6 + 1] =  j   *(side_) + i;
        object_.Get_index_buf()[(j*(side_-1) + i)*6 + 2] = (j+1)*(side_) + i;

        object_.Get_index_buf()[(j*(side_-1) + i)*6 + 3] = (j+1)*(side_) + i;
        object_.Get_index_buf()[(j*(side_-1) + i)*6 + 4] = (j+1)*(side_) + i+1;
        object_.Get_index_buf()[(j*(side_-1) + i)*6 + 5] =  j   *(side_) + i+1;
        }

    PointsFill();
    }

Function::~Function()
    {}

void Function::Transform ()
    {
    Matrix transf_m = Matrix();
    float scale = current_m_[3][3];

    if      (GetAsyncKeyState('Z'     )) PointsFill (step_ * 1.025f);
    else if (GetAsyncKeyState('X'     )) PointsFill (step_ / 1.025f);

    if      (GetAsyncKeyState(VK_UP   )) transf_m *= GetRotateMatrix (Vect (1, 0, 0), (float) -M_PI/72.0f);
    else if (GetAsyncKeyState(VK_DOWN )) transf_m *= GetRotateMatrix (Vect (1, 0, 0), (float)  M_PI/72.0f);

    if      (GetAsyncKeyState(VK_RIGHT)) transf_m *= GetRotateMatrix (Vect (0, 1, 0), (float) -M_PI/72.0f);
    else if (GetAsyncKeyState(VK_LEFT )) transf_m *= GetRotateMatrix (Vect (0, 1, 0), (float)  M_PI/72.0f);

    if      (GetAsyncKeyState('D'     )) transf_m *= GetRotateMatrix (Vect (0, 0, 1), (float) -M_PI/72.0f);
    else if (GetAsyncKeyState('A'     )) transf_m *= GetRotateMatrix (Vect (0, 0, 1), (float)  M_PI/72.0f);

    if      (GetAsyncKeyState('W'     ) && scale <= 200.0f)
        { transf_m *= GetScaleMatrix (     1.05f); }

    else if (GetAsyncKeyState('S'     ) && scale >= 1.0f/200.0f)
        { transf_m *= GetScaleMatrix (1.0f/1.05f); }

    current_m_ = transf_m * current_m_;
    }

void Function::PrintMatrix()
    {
	printf ("Matrix\n");
    for (int i = 0; i < 4; i++)
        printf ("%.2f %.2f %.2f %.2f \n", current_m_[i][0], current_m_[i][1], current_m_[i][2], current_m_[i][3]);

    printf ("step \n %f \n\n", step_);
    }

void Function::PointsFill (float step_set/*= 0.0f*/)
    {
    if (step_set != 0.0f) step_ = step_set;

    for (int i = -side_/2; i < side_/2; i++)
    for (int j = -side_/2; j < side_/2; j++)
        {
        equation_["x"] = i*step_;
        equation_["y"] = j*step_;

        object_.Get_point_buf()[int(i+side_/2 + (j+side_/2)*side_)] =
            Vect (i*step_, j*step_, equation_.GetResult());
        }

    object_.obj_Transform (GetScaleMatrix (screenY_ / ((side_ - 1) * step_)));
    }

void Function::Draw (COLORREF color)
    {
    Matrix result_m     = GetTranslateMatrix(Vect (screenX_ / 2, screenY_ / 2, screenY_ / 2)) * current_m_;
    Matrix result_m_inv = ~result_m;

    object_.obj_Transform (result_m);
    object_.obj_Draw      (color, true);
    object_.obj_Transform (result_m_inv);
    }

void Function::IntoFile (LPCSTR fname)
    {
    object_.obj_IntoFile (fname);
    }

#endif
