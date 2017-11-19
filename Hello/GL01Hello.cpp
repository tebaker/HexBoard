/*
 * GL01Hello.cpp
 *
 *  Created on: Nov 17, 2017
 *      Author: Talon
 */

#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <math.h>
#include <iostream>

using namespace std;

const float R          = 0.05;
const float PI         = 3.141592653589;
const float X_OFFSET   = R * cos(PI / 6);
const float Y_OFFSET   = R + R / 2;
const int   BOARD_SIZE = 11;


/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

    glBegin(GL_POLYGON);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f); 		// Red

	float currX = -0.7;
	float y     =  0.7;

	float x = currX;		//currX for line position when new line is started

    for(int i = 0; i < BOARD_SIZE; i++) {
    	for(int j = 0; j < BOARD_SIZE; j++) {
    		//adding all points to hexagon
    		glVertex2f(x + R * 0.0,           y + R * 1.0);
    		glVertex2f(x - R * (sqrt(3) / 2), y + R * 0.5);
    		glVertex2f(x - R * (sqrt(3) / 2), y - R * 0.5);
    		glVertex2f(x + R * 0.0,           y - R * 1.0);
    		glVertex2f(x + R * (sqrt(3) / 2), y - R * 0.5);
    		glVertex2f(x + R * (sqrt(3) / 2), y + R * 0.5);
    		//incrementing x by X_OFFSET
    		x += 2 * X_OFFSET;
    	}//end - inner for
    	//resetting x, y for new row
    	currX += X_OFFSET;
    	x = currX;
    	y -= Y_OFFSET;
    }//end - outer for



	x = 0.6;
	y = 0.6 - R*2;

   glEnd();

   glFlush();  // Render now
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
   glutInit(&argc, argv);                 // Initialize GLUT
   glutCreateWindow("Hex Board"); // Create a window with the given title
   glutInitWindowSize(500, 500);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutDisplayFunc(display); // Register display callback handler for window re-paint
   glutMainLoop();           // Enter the infinitely event-processing loop
   return 0;
}
