#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "stdafx.h"
#include "Object.h"
#include "parser//Parser.h"

class Function : public EGLobject
{
public:
    Function();
    Function(float size_set, size_t side_set, float step_set,
             const EGLcolor& min_color_set, const EGLcolor& max_color_set, const string& equation_set);

   ~Function();

    void setFunction(const string& equation_set);

//    void Transform  ();
    void recalculate(float step_set = 0.0f);

private:
    float     size_;
    size_t    side_;
    float     step_;
    EGLcolor  min_color_;
    EGLcolor  max_color_;
    EGLnode   equation_;
    double    parser_x_, parser_y_;// Œ—“€€€À‹!!! €€€!!!
};

Function::Function(): 
    EGLobject(), size_(), side_(), step_(), min_color_(), max_color_(), 
    equation_(), parser_x_(0), parser_y_(0) {}

Function::Function(float size_set, size_t side_set, float step_set, 
                   const EGLcolor& min_color_set, const EGLcolor& max_color_set, const string& equation_set):
    EGLobject   (EGLobject()),
    size_          (size_set), 
    side_          (side_set),
    step_          (step_set),
    min_color_(min_color_set),
    max_color_(max_color_set),
    equation_              (),
    parser_x_(0), parser_y_(0)
{
    setFunction(equation_set);

    if (side_ == 0) return;

    index_buf_.clear(); index_buf_.shrink_to_fit(); index_buf_.reserve(2*(side_-1)*(side_-1)); 
    point_buf_.clear(); point_buf_.shrink_to_fit(); point_buf_.reserve(side_*side_);

    for (size_t i = 0; i < (side_-1); i++)
    for (size_t j = 0; j < (side_-1); j++)
    {
        index_buf_.push_back(EGLindex3(j*(side_) + i+1,  j   *(side_) + i,   (j+1)*(side_) + i));
        index_buf_.push_back(EGLindex3(j*(side_) + i+1, (j+1)*(side_) + i+1, (j+1)*(side_) + i));
    }

    point_buf_.resize(side_*side_);
    
    recalculate();
}

Function::~Function() {}

void Function::setFunction(const string& equation_set)
{
    EGLtokenizer tokenizer;
    EGLanalyzer  analyzer;
    EGLoptimizer optimizer;
    
    tokenizer.set_variable("x", &parser_x_);
    tokenizer.set_variable("y", &parser_y_);

    vector<EGLtoken> out_vec;
    tokenizer(equation_set, out_vec);
    analyzer (out_vec, equation_);
    optimizer(equation_);
}
/*
void Function::Transform ()
{
    EGLmatrix transf_m = EGLmatrix();
    float scale = current_m_[3][3];

    if      (GetAsyncKeyState('Z'     )) recalculate (step_ * 1.025f);
    else if (GetAsyncKeyState('X'     )) recalculate (step_ / 1.025f);

    if      (GetAsyncKeyState(VK_UP   )) transf_m *= eglGetRotateMatrix (EGLvector (1, 0, 0), (float) -M_PI/72.0f);
    else if (GetAsyncKeyState(VK_DOWN )) transf_m *= eglGetRotateMatrix (EGLvector (1, 0, 0), (float)  M_PI/72.0f);

    if      (GetAsyncKeyState(VK_RIGHT)) transf_m *= eglGetRotateMatrix (EGLvector (0, 1, 0), (float) -M_PI/72.0f);
    else if (GetAsyncKeyState(VK_LEFT )) transf_m *= eglGetRotateMatrix (EGLvector (0, 1, 0), (float)  M_PI/72.0f);

    if      (GetAsyncKeyState('D'     )) transf_m *= eglGetRotateMatrix (EGLvector (0, 0, 1), (float) -M_PI/72.0f);
    else if (GetAsyncKeyState('A'     )) transf_m *= eglGetRotateMatrix (EGLvector (0, 0, 1), (float)  M_PI/72.0f);

    if      (GetAsyncKeyState('W'     ) && scale <= 200.0f)
        { transf_m *= eglGetScaleMatrix (     1.05f); }

    else if (GetAsyncKeyState('S'     ) && scale >= 1.0f/200.0f)
        { transf_m *= eglGetScaleMatrix (1.0f/1.05f); }

    current_m_ = transf_m * current_m_;
}
*/
void Function::recalculate(float step_set/*= 0.0f*/)
{
    float max_z = 0.0f, min_z = 0.0f;
    EGLvector normal_v;
    EGLmatrix scale_m = eglGetScaleMatrix(size_ / ((side_-1)*step_));
    
    if (step_set != 0.0f) step_ = step_set;

    for (int i = 0; i < side_; i++)
    for (int j = 0; j < side_; j++)
    {
        parser_x_ = float(i - int(side_/2))*step_;
        parser_y_ = float(j - int(side_/2))*step_;

        point_buf_[i + j*side_].vector = EGLvector(parser_x_, parser_y_, eglCalculate(equation_));
        
        max_z = max(max_z, point_buf_[int(i + j*side_)].vector.z);
        min_z = min(min_z, point_buf_[int(i + j*side_)].vector.z);
    }

    for (const EGLindex3& triang : index_buf_)
    {
        normal_v = !((point_buf_[triang.ind1].vector - point_buf_[triang.ind2].vector) *
                     (point_buf_[triang.ind3].vector - point_buf_[triang.ind2].vector));

        point_buf_[triang.ind1].normal += normal_v;
        point_buf_[triang.ind2].normal += normal_v;
        point_buf_[triang.ind3].normal += normal_v;
    }
    
    bool is_mix = (max_z - min_z) > 0.0001f;

    for (EGLvertex& vert : point_buf_)
    {
        vert.color = eglColorMix(max_color_, (is_mix ? (vert.vector.z - min_z)/(max_z - min_z) : 0.5f), min_color_);
        vert.vector *= scale_m;
        vert.normal = !vert.normal;
    }
}

#endif
