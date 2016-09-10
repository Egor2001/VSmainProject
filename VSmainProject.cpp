// VSmainProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Parser.h"
#include "Graph.h"
#include "Object.h"
#include "Render/EGLpipeline.h"

int _tmain(int argc, _TCHAR* argv[])
{
	float ScreenX = 800;
    float ScreenY = 800;
    
    eglCreateWindow(ScreenX, ScreenY);
/*    
    EGLlight light1(EGLvector(0      ,         0,   -2048), 0xFFFFAA);
	EGLlight light2(EGLvector(ScreenX,   ScreenY,   -2048), 0xAAFFFF);
	EGLlight light3(EGLvector(ScreenX/2, ScreenY/2, -2048), 0xFFAAFF);
    EGLcamera cam = EGLcamera(EGLvector(0, 0, 1500), EGLvector(0, 0, 1), EGLvector(0, 1, 0), true);

    //eglSetDiffuseColor(EGLcolor(255, 255, 255));
    eglFrustum(600, -600, 600, -600, 1000, 3000);
    
    EGLvertexbuf vbuf_tetr = { EGLvertex(EGLvector(   0,  450, -250), !EGLvector( 0,  9, -5), 0xFFFF00),
                               EGLvertex(EGLvector(-400, -250, -250), !EGLvector(-8, -5, -5), 0xFF00FF),
                               EGLvertex(EGLvector( 400, -250, -250), !EGLvector( 8, -5, -5), 0x00FFFF),
                               EGLvertex(EGLvector(   0,    0,  450), !EGLvector( 0,  0,  9), 0xFFFFFF)};

    EGLindexbuf ibuf_tetr = { EGLindex3(0, 1, 2), 
                              EGLindex3(0, 1, 3), 
                              EGLindex3(0, 2, 3), 
                              EGLindex3(1, 2, 3) };
    
    EGLobject tetr_obj(ibuf_tetr, vbuf_tetr); 

    EGLvertexbuf vbuf_cube = { EGLvertex(EGLvector( 400,  400,  400), EGLvector( 1,  1,  1), RGB( 32, 128, 255)),
                               EGLvertex(EGLvector( 400,  400, -400), EGLvector( 1,  1, -1), RGB( 64, 128, 234)),
                               EGLvertex(EGLvector( 400, -400,  400), EGLvector( 1, -1,  1), RGB( 96, 128, 202)),
                               EGLvertex(EGLvector( 400, -400, -400), EGLvector( 1, -1, -1), RGB(128, 128, 170)),
                               EGLvertex(EGLvector(-400,  400,  400), EGLvector(-1,  1,  1), RGB(170, 128, 128)),
                               EGLvertex(EGLvector(-400,  400, -400), EGLvector(-1,  1, -1), RGB(202, 128,  92)),
                               EGLvertex(EGLvector(-400, -400,  400), EGLvector(-1, -1,  1), RGB(234, 128,  64)),
                               EGLvertex(EGLvector(-400, -400, -400), EGLvector(-1, -1, -1), RGB(255, 128,  32)) };

    EGLindexbuf ibuf_cube = { EGLindex3(4, 5, 6), EGLindex3(7, 5, 6),
                              EGLindex3(0, 1, 2), EGLindex3(3, 1, 2),
                              EGLindex3(0, 1, 4), EGLindex3(5, 1, 4),
                              EGLindex3(7, 6, 3), EGLindex3(2, 6, 3),
                              EGLindex3(4, 0, 6), EGLindex3(2, 0, 6),
                              EGLindex3(3, 1, 7), EGLindex3(5, 1, 7) };

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        eglClear(8096);

        eglMulMatrix(eglGetRotateMatrix(EGLvector(1, 2, 3), M_PI/180));
        
        //eglSetIndexBuf(ibuf_tetr);
        //eglSetVertexBuf(vbuf_tetr);
        //eglRender(EGLrenderType::RT_SOLID_LERP);
        tetr_obj.render(EGLrenderType::RT_SOLID_LERP);
        //eglSetIndexBuf(ibuf_cube);
        //eglSetVertexBuf(vbuf_cube);
        //eglRender(EGLrenderType::RT_SOLID_LERP);
        
        eglFlush();
    }
    

    */
	EGLlight light1(EGLvector(0      ,         0,   -4096), 0x00FFFF);
	EGLlight light2(EGLvector(ScreenX,   ScreenY,   -4096), 0xFFFF00);
	EGLlight light3(EGLvector(ScreenX/2, ScreenY/2, -4096), 0xFFAAFF);
    EGLcamera cam = EGLcamera(EGLvector(0, 0, 1200), EGLvector(0, 0, 1), EGLvector(0, 1, 0), true);
    eglFrustum(600, -600, 600, -600, 500, 2000);
    
    char exp [256] = "";

    fgets (exp, 256, stdin);
    exp[strlen(exp)-1] = '\0';

    if (!strcmp (exp, "")) return 0;

    EGLobject* func_graph = new Function(ScreenY, 100, 0.5, 0x0000FF, 0xFF0000, exp);

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
		eglClear(16192);

        eglMulMatrix(eglGetRotateMatrix(EGLvector(1, 2, 3), M_PI/180));
        
        func_graph->render(EGLrenderType::RT_SOLID_LERP);

        eglFlush();
    }
    
    FILE* model_f = NULL;
    fopen_s(&model_f, "kek.stl", "w");

    func_graph->intoFile(model_f);
    
    delete func_graph;

    return 0;
}

