/*
 * DrawingTools.cpp
 *
 *  Created on: Nov 17, 2017
 *      Author: Talon
 */

#ifndef DRAWINGTOOLS_H_
#define DRAWINGTOOLS_H_

#include "HexBoard.h"
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

//will draw a circle piece centered at x, y of the specified color
void drawPiece(int xVar, int yVar, tileStatus color) {
	if(color != OPEN) {
		//resetting radius so the piece will be smaller than the hexagon
		float cirR = R - 0.025;

		float x = MASTER_X;
		x += 2 * xVar * X_OFFSET;

		for(int i = 0; i < yVar; i++) {
			x += X_OFFSET;
		}

		float y = MASTER_Y;
		y -= yVar * Y_OFFSET;

		//off setting x, y so shadow will be at bottom left
		x -= 0.005;
		y -= 0.005;

		//drawing a CIRCLE to the screen
		glBegin(GL_POLYGON);					//starting drawing boarder
		//piece shadow same gray as the board shadow
		glColor3f(0.794f, 0.766f, 0.790f);
			//N
			glVertex2f(x + cirR * 0.0,           y + cirR * 1.0);
			//Quadrant II
			glVertex2f(x - cirR * 0.5,           y + cirR * (sqrt(3) / 2));
			glVertex2f(x - cirR * (sqrt(2) / 2), y + cirR * (sqrt(2) / 2));
			glVertex2f(x - cirR * (sqrt(3) / 2), y + cirR * 0.5);
			//W
			glVertex2f(x - cirR * 1.0,           y + cirR * 0.0);
			//Quadrant III
			glVertex2f(x - cirR * (sqrt(3) / 2), y - cirR * 0.5);
			glVertex2f(x - cirR * (sqrt(2) / 2), y - cirR * (sqrt(2) / 2));
			glVertex2f(x - cirR * 0.5,           y - cirR * (sqrt(3) / 2));
			//S
			glVertex2f(x + cirR * 0.0,           y - cirR * 1.0);
			//Quadrant IV
			glVertex2f(x + cirR * 0.5,           y - cirR * (sqrt(3) / 2));
			glVertex2f(x + cirR * (sqrt(2) / 2), y - cirR * (sqrt(2) / 2));
			glVertex2f(x + cirR * (sqrt(3) / 2), y - cirR * 0.5);
			//E
			glVertex2f(x + cirR * 1.0,           y + cirR * 0.0);
			//Quadrant I
			glVertex2f(x + cirR * (sqrt(3) / 2), y + cirR * 0.5);
			glVertex2f(x + cirR * (sqrt(2) / 2), y + cirR * (sqrt(2) / 2));
			glVertex2f(x + cirR * 0.5,           y + cirR * (sqrt(3) / 2));

		glEnd();							//ending drawing boarder

		//resetting radius so the piece will be smaller than the shadow
		cirR = R - 0.03;

		//resetting x, y so piece will be at center of hexagon
		x += 0.005;
		y += 0.005;

		//drawing a CIRCLE to the screen
		glBegin(GL_POLYGON);					//starting drawing boarder

			if(color == RED) {
				//piece output RED
				glColor3f(1.0f, 0.0, 0.0f);
			}
			else {
				//otherwise BLUE
				glColor3f(0.0f, 0.0, 1.0f);
			}
			//N
			glVertex2f(x + cirR * 0.0,           y + cirR * 1.0);
			//Quadrant II
			glVertex2f(x - cirR * 0.5,           y + cirR * (sqrt(3) / 2));
			glVertex2f(x - cirR * (sqrt(2) / 2), y + cirR * (sqrt(2) / 2));
			glVertex2f(x - cirR * (sqrt(3) / 2), y + cirR * 0.5);
			//W
			glVertex2f(x - cirR * 1.0,           y + cirR * 0.0);
			//Quadrant III
			glVertex2f(x - cirR * (sqrt(3) / 2), y - cirR * 0.5);
			glVertex2f(x - cirR * (sqrt(2) / 2), y - cirR * (sqrt(2) / 2));
			glVertex2f(x - cirR * 0.5,           y - cirR * (sqrt(3) / 2));
			//S
			glVertex2f(x + cirR * 0.0,           y - cirR * 1.0);
			//Quadrant IV
			glVertex2f(x + cirR * 0.5,           y - cirR * (sqrt(3) / 2));
			glVertex2f(x + cirR * (sqrt(2) / 2), y - cirR * (sqrt(2) / 2));
			glVertex2f(x + cirR * (sqrt(3) / 2), y - cirR * 0.5);
			//E
			glVertex2f(x + cirR * 1.0,           y + cirR * 0.0);
			//Quadrant I
			glVertex2f(x + cirR * (sqrt(3) / 2), y + cirR * 0.5);
			glVertex2f(x + cirR * (sqrt(2) / 2), y + cirR * (sqrt(2) / 2));
			glVertex2f(x + cirR * 0.5,           y + cirR * (sqrt(3) / 2));

		glEnd();							//ending drawing boarder
	}
}//end - drawPiece

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void drawHexBoard(){
	float currX = MASTER_X - 0.02;
	float y     = MASTER_Y - 0.02;
	float x     = currX;	//currX for line position when new line is started

	//background color is opaque white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//setting initial line width
	glLineWidth(1.0f);

	//creating the drop shadow below board
    for(int i = 0; i < BOARD_SIZE; i++) {
		glBegin(GL_POLYGON);				//starting drawing polygon

			//light gray
			glColor3f(0.794f, 0.766f, 0.790f);
			//adding all points to hexagon starting with north-most point
			glVertex2f(x + R * 0.0,           y + R * 1.0);
			glVertex2f(x - R * (sqrt(3) / 2), y + R * 0.5);
			glVertex2f(x - R * (sqrt(3) / 2), y - R * 0.5);
			glVertex2f(x + R * 0.0,           y - R * 1.0);
			glVertex2f(x + R * (sqrt(3) / 2), y - R * 0.5);
			glVertex2f(x + R * (sqrt(3) / 2), y + R * 0.5);

		glEnd();							//ending drawing polygon
		//incrementing x by X_OFFSET
		x += X_OFFSET;
		y -= Y_OFFSET;
	}//end - inner for

	x = MASTER_X + 10  * X_OFFSET - 0.02;
	y = MASTER_Y - 10 * Y_OFFSET - 0.02;

	//creating the drop shadow below board
    for(int i = 0; i < BOARD_SIZE; i++) {
		glBegin(GL_POLYGON);				//starting drawing polygon

			//line RGB light purple-gray
			glColor3f(0.794f, 0.766f, 0.790f);
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

    currX = MASTER_X;
    	y = MASTER_Y;
    	x = currX;		//currX for line position when new line is started

    	//creating the background of the board
        for(int i = 0; i < BOARD_SIZE; i++) {
        	for(int j = 0; j < BOARD_SIZE; j++) {
    			glBegin(GL_POLYGON);				//starting drawing polygon

    				//line RGB light gray
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

				//slightly darker shadow for piece outline
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

	x = MASTER_X;
	y = MASTER_Y;

	x += 10*X_OFFSET;
	y -= 10*Y_OFFSET;

	//drawing a line in half RED/BLUE in the SW and NE edges
	glBegin(GL_LINES);					//starting drawing boarder
	//RED for lower line half
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(x - R * sqrt(3)/4, y - R * 3/4);	//SWL
	glVertex2f(x + R * 0.0,       y - R * 1.0);	//S
	glEnd();							//ending drawing boarder

	x = MASTER_X;
	y = MASTER_Y;

	x += 20*X_OFFSET;

	//drawing a line in half RED/BLUE in the SW and NE edges
	glBegin(GL_LINES);					//starting drawing boarder
	//RED for lower line half
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(x + R * 0.0,       y + R * 1.0);	//N
	glVertex2f(x + R * sqrt(3)/4, y + R * 3/4);	//NEU
	glEnd();							//ending drawing boarder

	for(int i = 0; i < 11; i++) {
		for(int j = 0; j < 11; j++) {
			drawPiece(i, j, masterList.at(i * 11 + j).second);
		}
	}

//	drawPiece(0, 0, BLUE);


	//rendering all data to the screen
	glFlush();
}

#endif /* DRAWINGTOOLS_H_ */
