// VSmainProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Parser.h"
#include "Graph.h"

int _tmain(int argc, _TCHAR* argv[])
    {
	float ScreenX = 800;
    float ScreenY = 800;

    char exp [256] = "";

    eglCreateWindow(ScreenX, ScreenY);

    fgets (exp, 256, stdin);
    exp[strlen(exp)-1] = '\0';

    if (!strcmp (exp, "")) return 1;

    Function func_graph (ScreenX, ScreenY, 100, 0.5, exp);

    while (!GetAsyncKeyState(VK_ESCAPE))
        {
        func_graph.Transform ();

		eglClear(1024);

        func_graph.Draw (RGB(170, 255, 85));

        eglFlush ();
        }
    
    //func_graph.IntoFile ("kek.stl");

    return 0;
    }

