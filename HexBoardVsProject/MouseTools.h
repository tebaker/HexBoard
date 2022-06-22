#ifndef MOUSETOOLS_H_
#define MOUSETOOLS_H_
#pragma once

#include <windows.h>  // For MS Windows
#include <C:/OpenGL/glut-3.7/include/GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <math.h>
#include <iostream>

namespace MouseTools {

	// For mouse controls
	void glutMouseFunc(void (*func)(int button, int state, int x, int y));

	void glutMouseFunc(void (*func)(int button, int state, int x, int y)) {

	}

}
#endif /* MOUSETOOLS_H_ */