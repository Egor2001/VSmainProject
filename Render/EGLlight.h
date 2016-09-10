#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "../stdafx.h"
#include "../Math/EGLmath.h"
#include "../Raster/EGLcolor.h"
#include "EGLvertex.h"
#include <stdexcept>

using std::exception;

class EGLlight;

void eglSetDiffuseColor(const EGLcolor& diff_set);

namespace EGL_LIGHT
{
    EGLcolor GetVertexColor(const EGLvertex& vertex);
    
    vector<const EGLlight*> egl_lights_v;
    EGLcolor diffuse;

    EGLcolor GetVertexColor(const EGLvertex& vertex);
}

void eglSetDiffuseColor(const EGLcolor& diff_set) { EGL_LIGHT::diffuse = diff_set; }

class EGLlight
{
public:
	EGLlight();
	EGLlight(const EGLvector& place_set, const EGLcolor& color_set, bool enabled_set = true);
    
    ~EGLlight();

    EGLvector& place()   { return place_; }
    EGLcolor&  color()   { return color_; }
    bool&      enabled() { return enabled_; }
    
    const EGLvector& place()   const { return place_; }
    const EGLcolor&  color()   const { return color_; }
    const bool&      enabled() const { return enabled_; }
    
    void     transform     (const EGLmatrix& transf_m);
    EGLcolor getVertexColor(const EGLvertex& vertex);

private:
    EGLvector place_;
    EGLcolor  color_;
	bool      enabled_;
    size_t    index_;
};

namespace EGL_LIGHT
{
    EGLcolor GetVertexColor(const EGLvertex& vertex)
    {
        if (vertex.normal.Length2() - 1.0f > 0.0001f) 
            throw exception("normal of vertex must be normalized");

	    if (egl_lights_v.empty()) return vertex.color;

	    UINT sum_R = diffuse.r, sum_G = diffuse.g, sum_B = diffuse.b;
	    float factor = 0.0f;
		
	    for (const EGLlight* light : egl_lights_v)
	    {
		    if (!light->enabled()) continue;

		    factor = (!(light->place() - vertex.vector) % vertex.normal);//TODO: normalizing pointNormal
		    if (factor <= 0.0f) continue;

		    sum_R += light->color().r*factor;
		    sum_G += light->color().g*factor;
		    sum_B += light->color().b*factor;
	    }

	    sum_R = min(sum_R, 255);
	    sum_G = min(sum_G, 255);
	    sum_B = min(sum_B, 255);
	    
	    return EGLcolor(sum_R, sum_G, sum_B)*vertex.color;
    }
}

EGLlight::EGLlight(): place_(EGLvector()), color_(RGB(255, 255, 255)), enabled_(false), index_(-1)
{
    EGL_LIGHT::egl_lights_v.push_back(this);
    index_ = EGL_LIGHT::egl_lights_v.size() - 1;
}

EGLlight::EGLlight(const EGLvector& place_set, const EGLcolor& color_set, bool enabled_set/*= true*/):
    place_(place_set), color_(color_set), enabled_(enabled_set), index_(-1) 
{
    EGL_LIGHT::egl_lights_v.push_back(this);
    index_ = EGL_LIGHT::egl_lights_v.size() - 1;
}

EGLlight::~EGLlight()
{
    EGL_LIGHT::egl_lights_v.erase(EGL_LIGHT::egl_lights_v.begin() + index_);
}

void EGLlight::transform(const EGLmatrix& transf_m)
{
	place_ *= transf_m;
}

EGLcolor EGLlight::getVertexColor(const EGLvertex& vertex)
{
    if (vertex.normal.Length() != 1.0f) 
            throw exception("EGLvector as argument 2 must be normalized");

    if (!enabled_) return vertex.color;

    float factor = (!(place_ - vertex.vector) % vertex.normal);
    
    if (factor <= 0.0f) return EGLcolor();

    return vertex.color*factor;
}

#endif
