// VSmainProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Parser.h"
#include "Graph.h"

int _tmain(int argc, _TCHAR* argv[])
    {
	float ScreenX = 800;
    float ScreenY = 800;

	EGL_LIGHT::SetLight(0xFFFFFF, Vect(0      ,       0, 0.0f));
	EGL_LIGHT::SetLight(0xFFFF00, Vect(0      , ScreenY, 0.0f));
	EGL_LIGHT::SetLight(0xFFFF00, Vect(ScreenX,       0, 0.0f));
	EGL_LIGHT::SetLight(0x00FFFF, Vect(ScreenX, ScreenY, 0.0f));

    char exp [256] = "";

    eglCreateWindow(ScreenX, ScreenY);

    fgets (exp, 256, stdin);
    exp[strlen(exp)-1] = '\0';

    if (!strcmp (exp, "")) return 0;

    Function func_graph(ScreenX, ScreenY, 100, 0.5, exp);

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
		func_graph.Transform();

		eglClear(1024);

        func_graph.Draw(RGB(170, 255, 85));

        eglFlush();
    }
    
    //func_graph.IntoFile ("kek.stl");

    return 0;
    }

