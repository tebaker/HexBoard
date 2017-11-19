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

const float R          = 0.065;
const float PI         = 3.141592653589;
const float X_OFFSET   = R * cos(PI / 6);
const float Y_OFFSET   = R + R / 2;
const int   BOARD_SIZE = 11;
const float MASTER_X   = -0.85;
const float MASTER_Y   =  0.85;

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void drawHexBoard(){
	float currX = MASTER_X;
	float y     = MASTER_Y;
	float x     = currX;	//currX for line position when new line is started

	//background color is opaque white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//setting initial line width
	glLineWidth(1.0f);

	//creating the background of the board
    for(int i = 0; i < BOARD_SIZE; i++) {
    	for(int j = 0; j < BOARD_SIZE; j++) {
			glBegin(GL_POLYGON);				//starting drawing polygon

				//background color set to light gray
				glColor3f(0.937f, 0.921f, 0.937f);
				//adding all points to hexagon starting with north-most point
				glVertex2f(x + R * 0.0,           y + R * 1.0);
				glVertex2f(x - R * (sqrt(3) / 2), y + R * 0.5);
				glVertex2f(x - R * (sqrt(3) / 2), y - R * 0.5);
				glVertex2f(x + R * 0.0,           y - R * 1.0);
				glVertex2f(x + R * (sqrt(3) / 2), y - R * 0.5);
				glVertex2f(x + R * (sqrt(3) / 2), y + R * 0.5);

    		glEnd();							//ending drawing polygon
    		//incrementing x by X_OFFSET
    		x += 2 * X_OFFSET;
    	}//end - inner for
    	//resetting x, y for new row
    	currX += X_OFFSET;
    	x = currX;
    	y -= Y_OFFSET;
    }//end - outer for

	currX = MASTER_X;
	y     = MASTER_Y;
	x 	  = currX;		//currX for line position when new line is started

	//creating the shadow for the pieces
    for(int i = 0; i < BOARD_SIZE; i++) {
    	for(int j = 0; j < BOARD_SIZE; j++) {
			glBegin(GL_LINE_LOOP);				//starting drawing polygon

				//line RGB light purple-gray
				glColor3f(0.894f, 0.866f, 0.890f);
				//adding all points to hexagon starting with north-most point
				glVertex2f(x + R * 0.0,           y + R * 1.0);	//N
				glVertex2f(x - R * (sqrt(3) / 2), y + R * 0.5);	//NW
				glVertex2f(x - R * (sqrt(3) / 2), y - R * 0.5);	//SW
				glVertex2f(x + R * 0.0,           y - R * 1.0);	//S
				glVertex2f(x + R * (sqrt(3) / 2), y - R * 0.5);	//SE
				glVertex2f(x + R * (sqrt(3) / 2), y + R * 0.5); //NE

    		glEnd();							//ending drawing polygon
    		//incrementing x by X_OFFSET
    		x += 2 * X_OFFSET;
    	}//end - inner for
    	//resetting x, y for new row
    	currX += X_OFFSET;
    	x = currX;
    	y -= Y_OFFSET;
    }//end - outer for

	//resetting the current x, x, and y
	x = MASTER_X;
	y = MASTER_Y;

	//setting thicker line width for red/blue output
	glLineWidth(2.0f);

	//creating the RED TOP boarder
	for(int i = 0; i < BOARD_SIZE; i++) {
		glBegin(GL_LINES);					//starting drawing boarder

			//RED for TOP boarder
			glColor3f(1.0f, 0.0f, 0.0f);
			//adding all points top points to hexagon starting with northwest-most point
			glVertex2f(x - R * (sqrt(3) / 2), y + R * 0.5);	//NW
			glVertex2f(x + R * 0.0,           y + R * 1.0);	//N
			glVertex2f(x + R * 0.0,           y + R * 1.0);	//N
			glVertex2f(x + R * (sqrt(3) / 2), y + R * 0.5);	//NE

		glEnd();							//ending drawing boarder
		//incrementing x by X_OFFSET
		x += 2 * X_OFFSET;
	}//end - inner for

	//resetting the current x, x, and y
	x = MASTER_X;
	y = MASTER_Y;

	//manually setting x, y values: moving x five places to the right and y eleven places down
	x += 10 * X_OFFSET;
	y -= 10 * Y_OFFSET;

	//creating the RED BOTTOM boarder
	for(int i = 0; i < BOARD_SIZE; i++) {
		glBegin(GL_LINES);					//starting drawing boarder

			//RED for BOTTOM boarder
			glColor3f(1.0f, 0.0f, 0.0f);
			//adding all points top points to hexagon starting with northwest-most point
			glVertex2f(x - R * (sqrt(3) / 2), y - R * 0.5);	//SW
			glVertex2f(x + R * 0.0,           y - R * 1.0);	//S
			glVertex2f(x + R * 0.0,           y - R * 1.0);	//S
			glVertex2f(x + R * (sqrt(3) / 2), y - R * 0.5);	//SE

		glEnd();							//ending drawing boarder
		//incrementing x by X_OFFSET
		x += 2 * X_OFFSET;
	}//end - inner for

	//resetting the current x, x, and y
	x = MASTER_X;
	y = MASTER_Y;

	//creating the BLUE LEFT boarder
	for(int i = 0; i < BOARD_SIZE; i++) {
		glBegin(GL_LINES);					//starting drawing boarder

			//BLUE for LEFT boarder color
			glColor3f(0.0f, 0.0f, 1.0f);
			//adding all points top points to hexagon starting with northwest-most point
			glVertex2f(x - R * (sqrt(3) / 2), y + R * 0.5);	//NW
			glVertex2f(x - R * (sqrt(3) / 2), y - R * 0.5);	//SW
			glVertex2f(x - R * (sqrt(3) / 2), y - R * 0.5);	//SW
			glVertex2f(x + R * 0.0,           y - R * 1.0);	//S

		glEnd();							//ending drawing boarder
		//incrementing x, y moving down on the left hand side
		x += X_OFFSET;
		y -= Y_OFFSET;
	}//end - inner for

	//resetting the current x, x, and y
	x = MASTER_X;
	y = MASTER_Y;

	//manually setting x, y values: moving x 11 places to the right keeping y where it is
	x += 20 * X_OFFSET;

	//creating the BLUE RIGHT boarder
	for(int i = 0; i < BOARD_SIZE; i++) {
		glBegin(GL_LINES);					//starting drawing boarder

			//BLUE for RIGHT boarder
			glColor3f(0.0f, 0.0f, 1.0f);
			//adding all points top points to hexagon starting with northwest-most point
			glVertex2f(x + R * 0.0,           y + R * 1.0);	//N
			glVertex2f(x + R * (sqrt(3) / 2), y + R * 0.5); //NE
			glVertex2f(x + R * (sqrt(3) / 2), y + R * 0.5); //NE
			glVertex2f(x + R * (sqrt(3) / 2), y - R * 0.5);	//SE

		glEnd();							//ending drawing boarder
		//incrementing x, y moving down on the left hand side
		x += X_OFFSET;
		y -= Y_OFFSET;
	}//end - inner for


	//resetting the current x, x, and y
	x = MASTER_X;
	y = MASTER_Y;

	//manually setting x, y values: moving x five places to the right and y eleven places down
	x += 10 * X_OFFSET;
	y -= 10 * Y_OFFSET;

	//accounting for the half-color (blue/red) at bottom left
	glBegin(GL_LINES);					//starting drawing boarder
		//PURPLE for SW and NE corners
		glColor3f(0.4f, 0.1f, 0.7f);
		//coloring the SW corner purple
		glVertex2f(x - R * (sqrt(3) / 2), y - R * 0.5);	//SW
		glVertex2f(x + R * 0.0,           y - R * 1.0);	//S
	glEnd();							//ending drawing boarder

	//resetting the current x, x, and y
	x = MASTER_X;
	y = MASTER_Y;

	//manually setting x, y values: moving x five places to the right an0.396f, 0.070f, 0.709fd y eleven places down
	x += 20 * X_OFFSET;

	//accounting for the half-color (blue/red) at bottom left
	glBegin(GL_LINES);					//starting drawing boarder
		//PURPLE for SW and NE corners
		glColor3f(0.4f, 0.1f, 0.7f);
		//coloring the SW corner purple
		glVertex2f(x + R * 0.0,           y + R * 1.0);	//N
		glVertex2f(x + R * (sqrt(3) / 2), y + R * 0.5); //NE
	glEnd();							//ending drawing boarder

	//rendering all data to the screen
	glFlush();
}

void drawHexAtPoint(float xVal, float yVal) {

}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
   glutInit(&argc, argv);                 // Initialize GLUT
   glutCreateWindow("Hex Board"); // Create a window with the given title
   glutReshapeWindow(500, 500);

   glutDisplayFunc(drawHexBoard); // Register display callback handler for window re-paint
   glutMainLoop();           // Enter the infinitely event-processing loop
   return 0;
}
