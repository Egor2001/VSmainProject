#pragma once

#include "stdafx.h"
#include "Render/EGLpipeline.h"

//For projection matrix we need z in range [0; 1]!

struct EGLobject
{
public:
    EGLobject();
    EGLobject(const EGLindexbuf& index_buf_set, const EGLvertexbuf& point_buf_set);
    EGLobject(const EGLobject&  set);
    EGLobject      (EGLobject&& set);

    EGLobject& operator= (const EGLobject&  set);
    EGLobject& operator=       (EGLobject&& set);

    virtual ~EGLobject();

    const EGLvertexbuf& getPointBuf() const { return point_buf_; }
    const EGLindexbuf&  getIndexBuf() const { return index_buf_; }

    virtual void fromFile (FILE* model_f = NULL);
    virtual void intoFile (FILE* model_f = NULL);

    void setPointBuf(const EGLvertexbuf&  point_buf_set) { point_buf_ = point_buf_set; point_buf_.shrink_to_fit(); }
    void setPointBuf      (EGLvertexbuf&& point_buf_set) { point_buf_ = point_buf_set; point_buf_.shrink_to_fit(); }
    void setIndexBuf(const EGLindexbuf&   index_buf_set) { index_buf_ = index_buf_set; index_buf_.shrink_to_fit(); }
    void setIndexBuf      (EGLindexbuf&&  index_buf_set) { index_buf_ = index_buf_set; index_buf_.shrink_to_fit(); }
    
    void render(EGLrenderType renderType) const;
    
protected:
    EGLvertexbuf point_buf_;
    EGLindexbuf  index_buf_;
};

EGLobject::EGLobject (): 
    point_buf_(), index_buf_() {}

EGLobject::EGLobject(const EGLindexbuf& index_buf_set, const EGLvertexbuf& point_buf_set):
    point_buf_(point_buf_set), index_buf_(index_buf_set) {}

EGLobject::EGLobject(const EGLobject& set):
    point_buf_(set.point_buf_), index_buf_(set.index_buf_) 
{
    point_buf_.shrink_to_fit();
    index_buf_.shrink_to_fit();
}

EGLobject::EGLobject(EGLobject&& set):
    point_buf_(std::move(set.point_buf_)), index_buf_(std::move(set.index_buf_)) 
{
    point_buf_.shrink_to_fit();
    index_buf_.shrink_to_fit();
}

EGLobject& EGLobject::operator=(const EGLobject& set)
{
    point_buf_ = set.point_buf_; point_buf_.shrink_to_fit();
    index_buf_ = set.index_buf_; index_buf_.shrink_to_fit();
}

EGLobject& EGLobject::operator=(EGLobject&& set)
{
    point_buf_ = std::move(set.point_buf_); point_buf_.shrink_to_fit();
    index_buf_ = std::move(set.index_buf_); index_buf_.shrink_to_fit();
}

EGLobject::~EGLobject() { index_buf_.clear(); point_buf_.clear(); }

void EGLobject::fromFile(FILE* model_f/* = NULL */)
{
    if (!model_f) return;

    char token[256] = "";
    EGLvector temp_v;
    vector<EGLvector> temp_point_buf;
    vector<USHORT>    temp_index_buf;
    
    index_buf_.clear();
    point_buf_.clear();
    
    for (size_t res = 0; res != EOF; res = fscanf_s (model_f, "%s\n", token))
		if (!strcmp(token, "facet")) index_buf_.push_back(EGLindex3());

    rewind(model_f);

    temp_index_buf.resize(index_buf_.size()*3);
    temp_point_buf.resize(index_buf_.size()*3);
    
    fscanf_s(model_f, "solid %s \n", token);
    for (size_t i = 0; i < temp_point_buf.size(); i++)
    {
        fscanf_s(model_f, "facet normal %f %f %f \n", &temp_v.x, &temp_v.y, &temp_v.z);
        fscanf_s(model_f, "outer loop \n");

            fscanf_s(model_f, "vertex %f %f %f \n", &temp_v.x, &temp_v.y, &temp_v.z);
            temp_point_buf[3*i + 0] = temp_v;
            
            fscanf_s(model_f, "vertex %f %f %f \n", &temp_v.x, &temp_v.y, &temp_v.z);
            temp_point_buf[3*i + 1] = temp_v;
            
            fscanf_s(model_f, "vertex %f %f %f \n", &temp_v.x, &temp_v.y, &temp_v.z);
            temp_point_buf[3*i + 2] = temp_v;
        
        fscanf_s(model_f, "endloop \n");
        fscanf_s(model_f, "endfacet \n");
    }
    fscanf_s(model_f, "endsolid \n");
    
    USHORT obj_point_buf_sz = 0;
	for (size_t i = 0; i < temp_index_buf.size(); i++)
    {
        if (temp_index_buf[i] != -1) continue;

        temp_index_buf[i] = obj_point_buf_sz++; 
        
        for (size_t j = i+1; j < temp_index_buf.size(); j++) 
			if (temp_point_buf[j] == temp_point_buf[i]) 
                temp_index_buf[j] =  temp_index_buf[i];
    }

    point_buf_.resize(obj_point_buf_sz);

    for (size_t i = 0; i < temp_index_buf.size(); i++)
        point_buf_[temp_index_buf[i]].vector = temp_point_buf[i];

    for (size_t i = 0; i < index_buf_.size(); i++)
    {
        index_buf_[i].ind1 = temp_index_buf[3*i + 0];
        index_buf_[i].ind2 = temp_index_buf[3*i + 1];
        index_buf_[i].ind3 = temp_index_buf[3*i + 2];
    }

    index_buf_.shrink_to_fit();
    point_buf_.shrink_to_fit();
    
    rewind(model_f);
}

void EGLobject::intoFile (FILE* model_f/* = NULL */)
{
    if (!model_f) return;
    
    EGLvector normal_cur;
    EGLvector v1, v2, v3;

    fprintf_s(model_f, "solid %s\n", "");

    for (size_t i = 0; i < index_buf_.size(); i++)
    {
        v1 = point_buf_[index_buf_[i].ind1].vector;
        v2 = point_buf_[index_buf_[i].ind2].vector;
        v3 = point_buf_[index_buf_[i].ind3].vector;

        fprintf_s(model_f, "facet normal %f %f %f\n", 0.0f, 0.0f, 0.0f);
        fprintf_s(model_f, "  outer loop\n");
        fprintf_s(model_f, "    vertex %f %f %f\n", v1.x, v1.y, v1.z);
        fprintf_s(model_f, "    vertex %f %f %f\n", v2.x, v2.y, v2.z);
        fprintf_s(model_f, "    vertex %f %f %f\n", v3.x, v3.y, v3.z);
        fprintf_s(model_f, "  endloop\n");
        fprintf_s(model_f, "endfacet\n");
    }

    fprintf_s(model_f, "endsolid\n");
}

void EGLobject::render(EGLrenderType renderType) const
{
    eglSetIndexBuf (index_buf_);
    eglSetVertexBuf(point_buf_);
    eglRender(renderType);
}
