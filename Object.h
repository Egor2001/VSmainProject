#pragma once

#include "stdafx.h"
#include "Math3D.h"
#include "FillFunctions.h"
#include "Light.h"

//For projection matrix we need z in range [0; 1]!

struct EGLobject
    {
    private:

    FILE* obj_model_f;

    int obj_point_buf_sz;
    int obj_index_buf_sz;

    Vect* obj_point_buf;
     int* obj_index_buf;

    public:

    EGLobject ();

    EGLobject (const EGLobject& setObject);

    EGLobject (int pbuf_sz_set, int ibuf_sz_set);

    EGLobject (const Vect* point_buf_set, int pbuf_sz_set,
               const  int* index_buf_set, int ibuf_sz_set);

   ~EGLobject ();

    FILE* Get_model_f     () const { return obj_model_f;      }
     int  Get_point_buf_sz() const { return obj_point_buf_sz; }
     int  Get_index_buf_sz() const { return obj_index_buf_sz; }
    Vect* Get_point_buf   () const { return obj_point_buf;    }
     int* Get_index_buf   () const { return obj_index_buf;    }

    void obj_FromFile (LPCSTR model_f_set = NULL);
    void obj_IntoFile (LPCSTR model_f_set = NULL);

    void obj_Point_buf_set (const Vect* point_buf_set, int buf_sz_set);
    void obj_Index_buf_set (const  int* index_buf_set, int buf_sz_set);

    void obj_Draw      (const EGLcolor& color, bool filled, const EGLlight& light) const;
    void obj_Transform (const Matrix& mat);
    };

EGLobject::EGLobject ():
    obj_model_f      (NULL),
    obj_point_buf_sz (0),
    obj_index_buf_sz (0),
    obj_point_buf    (new Vect [obj_point_buf_sz]),
    obj_index_buf    (new  int [obj_index_buf_sz])
    {
    for (int i = 0; i < obj_point_buf_sz; i++)
        obj_point_buf[i] = Vect ();

    for (int i = 0; i < obj_index_buf_sz; i++)
        obj_index_buf[i] = 0;
    }

EGLobject::EGLobject (const EGLobject& setObject):
    obj_model_f      (NULL),
    obj_point_buf_sz (setObject.obj_point_buf_sz),
    obj_index_buf_sz (setObject.obj_index_buf_sz),
    obj_point_buf    (new Vect [setObject.obj_point_buf_sz]),
    obj_index_buf    (new  int [setObject.obj_index_buf_sz])
    {
    for (int i = 0; i < obj_point_buf_sz; i++)
        obj_point_buf[i] = setObject.obj_point_buf[i];

    for (int i = 0; i < obj_index_buf_sz; i++)
        obj_index_buf[i] = setObject.obj_index_buf[i];
    }

EGLobject::EGLobject (int pbuf_sz_set, int ibuf_sz_set):
    obj_model_f      (NULL),
    obj_point_buf_sz (pbuf_sz_set),
    obj_index_buf_sz (ibuf_sz_set),
    obj_point_buf    (new Vect [obj_point_buf_sz]),
    obj_index_buf    (new  int [obj_index_buf_sz])
    {
    for (int i = 0; i < obj_point_buf_sz; i++)
        obj_point_buf[i] = Vect();

    for (int i = 0; i < obj_index_buf_sz; i++)
        obj_index_buf[i] = 0;
    }

EGLobject::EGLobject (const Vect* point_buf_set, int pbuf_sz_set,
                      const  int* index_buf_set, int ibuf_sz_set):
    obj_model_f      (NULL),
    obj_point_buf_sz (pbuf_sz_set),
    obj_index_buf_sz (ibuf_sz_set),
    obj_point_buf    (new Vect [obj_point_buf_sz]),
    obj_index_buf    (new  int [obj_index_buf_sz])
    {
    for (int i = 0; i < obj_point_buf_sz; i++)
        obj_point_buf[i] = point_buf_set[i].GetSimple();

    for (int i = 0; i < obj_index_buf_sz; i++)
        obj_index_buf[i] = index_buf_set[i];
    }

EGLobject::~EGLobject ()
    {
    delete [] obj_index_buf;
              obj_index_buf_sz = 0;
              obj_index_buf    = NULL;

    delete [] obj_point_buf;
              obj_point_buf_sz = 0;
              obj_point_buf    = NULL;

	if (obj_model_f) fclose (obj_model_f);
    }

void EGLobject::obj_FromFile (LPCSTR model_f_set/* = NULL */)
    {
    if (model_f_set)
        {
		if (obj_model_f) fclose (obj_model_f);
        fopen_s (&obj_model_f, model_f_set, "r");

        if (!obj_model_f) return;
        }

    char read [256] = "";

    delete [] obj_index_buf;
              obj_index_buf_sz = 0;

    for (int res = 0; res != EOF; res = fscanf_s (obj_model_f, "%s\n", read))
        if (!strcmp (read, "facet")) obj_index_buf_sz += 3;

    rewind (obj_model_f);

    Vect* vertex_buf = new Vect [obj_index_buf_sz];
    Vect    temp_v;

    obj_index_buf = new int [obj_index_buf_sz];
    for (int i = 0; i < obj_index_buf_sz; i++) obj_index_buf[i] = -1;

    fscanf_s (obj_model_f, "solid %s \n", read);
    for (int i = 0; (i+2) < obj_index_buf_sz; i += 3)
        {
        fscanf_s (obj_model_f, "facet normal %f %f %f \n", &temp_v.x, &temp_v.y, &temp_v.z);
        fscanf_s (obj_model_f, "outer loop \n");

        for (int j = 0; j < 3; j++)
            {
            fscanf_s (obj_model_f, "vertex %f %f %f \n", &temp_v.x, &temp_v.y, &temp_v.z);
                                    vertex_buf[i+j] = temp_v;
            }

        fscanf_s (obj_model_f, "endloop \n");
        fscanf_s (obj_model_f, "endfacet \n");
        }
    fscanf_s (obj_model_f, "endsolid \n");

    delete [] obj_point_buf;
              obj_point_buf_sz = 0;
    for (int i = 0; i < obj_index_buf_sz; i++)
        {
        if (obj_index_buf[i] == -1)
            obj_index_buf[i] = obj_point_buf_sz++;
        else continue;

        for (int j = i+1; j < obj_index_buf_sz; j++)
            if (   vertex_buf[j] ==   vertex_buf[i])
                obj_index_buf[j] = obj_index_buf[i];
        }

    obj_point_buf = new Vect [obj_point_buf_sz];

    for (int i = 0; i < obj_index_buf_sz; i++)
        obj_point_buf[obj_index_buf[i]] = vertex_buf[i];

    delete [] vertex_buf;
    }

void EGLobject::obj_IntoFile (LPCSTR model_f_set/* = NULL */)
    {
    if (model_f_set)
        {
		if (obj_model_f) fclose (obj_model_f);
        fopen_s (&obj_model_f, model_f_set, "w");

        if (!obj_model_f) return;
        }

    Vect normal_cur = Vect ();
    Vect v1 = Vect (),
         v2 = Vect (),
         v3 = Vect ();

    fprintf_s (obj_model_f, "solid %s\n", model_f_set);

    for (int i = 0; (i+2) < obj_index_buf_sz; i+=3)
        {
        v1 = obj_point_buf[obj_index_buf[i+2]];
        v2 = obj_point_buf[obj_index_buf[i+1]];
        v3 = obj_point_buf[obj_index_buf[i  ]];

        fprintf_s (obj_model_f, "facet normal %f %f %f\n", 0.0f, 0.0f, 0.0f);
        fprintf_s (obj_model_f, "  outer loop\n");
        fprintf_s (obj_model_f, "    vertex %f %f %f\n", v1.x, v1.y, v1.z);
        fprintf_s (obj_model_f, "    vertex %f %f %f\n", v2.x, v2.y, v2.z);
        fprintf_s (obj_model_f, "    vertex %f %f %f\n", v3.x, v3.y, v3.z);
        fprintf_s (obj_model_f, "  endloop\n");
        fprintf_s (obj_model_f, "endfacet\n");
        }

    fprintf_s (obj_model_f, "endsolid\n");
    }

void EGLobject::obj_Point_buf_set (const Vect* point_buf_set, int buf_sz_set)
    {
    delete [] obj_point_buf;
              obj_point_buf_sz = buf_sz_set;
              obj_point_buf    = new Vect [obj_point_buf_sz];

    for (int i = 0; i < obj_point_buf_sz; i++)
        obj_point_buf[i] = point_buf_set[i].GetSimple();
    }

void EGLobject::obj_Index_buf_set (const int* index_buf_set, int buf_sz_set)
    {
    delete [] obj_index_buf;
              obj_index_buf_sz = buf_sz_set;
              obj_index_buf    = new int [obj_index_buf_sz];

    for (int i = 0; i < obj_index_buf_sz; i++)
        obj_index_buf[i] = index_buf_set[i];
    }

void EGLobject::obj_Draw (const EGLcolor& color, bool filled, const EGLlight& light) const
    {
    Vect v1 = Vect (),
         v2 = Vect (),
         v3 = Vect ();

    Vect place_cur = Vect (),
        normal_cur = Vect ();

    for (int i = 0; (i+2) < obj_index_buf_sz; i+=3)
        {
        v1 = obj_point_buf[obj_index_buf[i]  ];
        v2 = obj_point_buf[obj_index_buf[i+1]];
        v3 = obj_point_buf[obj_index_buf[i+2]];

        if (filled)
            {
            eglFillTriangle (v1.x, v1.y, v1.z,
                             v2.x, v2.y, v2.z,
                             v3.x, v3.y, v3.z,
                             light.GetPointColor (color, (v1+v3)/2, (v3-v2)*(v1-v2)));
            }
        else
            {
            eglLine (v1.x, v1.y, v1.z,
                     v2.x, v2.y, v2.z, color);
            eglLine (v1.x, v1.y, v1.z,
                     v3.x, v3.y, v3.z, color);
            eglLine (v3.x, v3.y, v3.z,
                     v2.x, v2.y, v2.z, color);
            }
        }
    }

void EGLobject::obj_Transform (const Matrix& mat)
    {
    for (int i = 0; i < obj_point_buf_sz; i++)
        obj_point_buf[i] *= mat;
    }
