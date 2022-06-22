/* PROGRAMMER  : TALON BAKER
 * CLASS       : CMPS 109
 * HW_06       : MONTE CARLO
 * DUE DATE    : 12/6/17
 */
#include "HexBoard.h"
#include "DrawingTools.h"
#include <iostream>
#include <random>
#include <vector>
#include <algorithm> //needed for random_shuffle
#include <chrono>
#include <windows.h> //for MS Windows
#include <C:/OpenGL/glut-3.7/include/GL/glut.h> //GLUT, includes glu.h and gl.h

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);         // Initialize GLUT
	glutCreateWindow("Hex Board"); // Create a window with the given title
	glutReshapeWindow(800, 500);

	HexBoard board(DrawingTools::BOARD_SIZE);
	
	glutDisplayFunc(DrawingTools::drawHexBoard);
	glutMouseFunc(DrawingTools::MyMouse);

	glutMainLoop();

	return 0;
}//end - main
