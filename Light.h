#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "stdafx.h"
#include "Math3D.h"
#include "EGLcolor.h"

struct EGLlight
{
	Vect     place;
    EGLcolor color;
	bool     enabled;

	EGLlight();
	EGLlight(const Vect& place_set, const EGLcolor& color_set, bool enabled_set = true);
   ~EGLlight();

    void Enable()  { enabled =  true; }
    void Disable() { enabled = false; }

    void Transform(const Matrix& transf_m);
};

EGLlight::EGLlight() : 
	place(Vect()), color(RGB(255, 255, 255)), enabled(false) {}

EGLlight::EGLlight(const Vect& place_set, const EGLcolor& color_set, bool enabled_set/*= true*/) :
	place(place_set), color(color_set),	enabled(enabled_set) {}

EGLlight::~EGLlight()
{}

void EGLlight::Transform(const Matrix& transf_m)
{
	place *= transf_m;
}

namespace EGL_LIGHT {
	vector<EGLlight> egl_lights_v = vector<EGLlight>();

	EGLcolor GetPointColor(const EGLcolor& color, const Vect& place, const Vect& normal);
	size_t   SetLight	  (const EGLcolor& color, const Vect& place);

	EGLcolor GetPointColor(const EGLcolor& color, const Vect& place, const Vect& normal)
	{
		if (egl_lights_v.empty()) return color;

		UINT   sum_R = 0, sum_G = 0, sum_B = 0;
		float  factor = 0.0f;
		
		for (EGLlight light : egl_lights_v)
		{
			if (!light.enabled) continue;

			factor = (!(light.place - place) % !normal);//TODO: normalizing pointNormal
			if (factor <= 0.0f) continue;

			sum_R += light.color.r*factor;
			sum_G += light.color.g*factor;
			sum_B += light.color.b*factor;
		}

		if (sum_R > 255) sum_R = 255;
		if (sum_G > 255) sum_G = 255;
		if (sum_B > 255) sum_B = 255;

		return EGLcolor(sum_R, sum_G, sum_B)*color;
	}

	size_t SetLight(const EGLcolor& color, const Vect& place)
	{
		egl_lights_v.push_back(EGLlight (place, color));
		return egl_lights_v.size() - 1;
	}
}

#endif
