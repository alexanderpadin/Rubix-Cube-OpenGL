#include <iostream>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Cube.h"

Cube::Cube() {
	cx = 0;
    cy = 0;
    cz = 0;
}

void Cube::set_color(int color) {
	switch(color) {
        case 0:
            glColor3f(0,0,0);
            break;
        case 1:
            glColor3f(1,1,0);
            break;
        case 2:
            glColor3f(1,0,1); 
            break;
        case 3:
            glColor3f(1,0,0);
            break;
        case 4:
            glColor3f(0,0.8,0);
            break;
        case 5:
            glColor3f(0,0,1);
            break;
        case 6:
            glColor3f(1,1,1);
            break;
        default:
            glColor3f(0,0,1);
	}
}


void Cube::display() {
	glPushMatrix();
	glTranslatef(cx,cy,cz);
    
	set_color(color[0]);
	glBegin(GL_POLYGON);
	glVertex3d(-1,-1,-1);
    glVertex3d(-1,1,-1);
    glVertex3d(1,1,-1);
    glVertex3d(1,-1,-1);
	glEnd();
    
	set_color(color[1]);
	glBegin(GL_POLYGON);
	glVertex3d(-1,-1,-1);
    glVertex3d(1,-1,-1);
    glVertex3d(1,-1,1);
    glVertex3d(-1,-1,1);
	glEnd();
    
	set_color(color[2]);
	glBegin(GL_POLYGON);
	glVertex3d(-1,-1,1);
    glVertex3d(-1,1,1);
    glVertex3d(-1,1,-1);
    glVertex3d(-1,-1,-1);
	glEnd();
    
	set_color(color[3]);
	glBegin(GL_POLYGON);
	glVertex3d(1,-1,1);
    glVertex3d(1,1,1);
    glVertex3d(-1,1,1);
    glVertex3d(-1,-1,1);
	glEnd();
    
	set_color(color[4]);
	glBegin(GL_POLYGON);
	glVertex3d(-1,1,1);
    glVertex3d(1,1,1);
    glVertex3d(1,1,-1);
    glVertex3d(-1,1,-1);
	glEnd();
    
	set_color(color[5]);
	glBegin(GL_POLYGON);
	glVertex3d(1,-1,1);
    glVertex3d(1,1,1);
    glVertex3d(1,1,-1);
    glVertex3d(1,-1,-1);
	glEnd();
    
	glPopMatrix();
}

void Cube::rotate(int axis, int dir) {
	int temp;
	switch(axis) {
        case 0: //Eje X
            if(dir) {
                temp = color[0];
                color[0] = color[4];
                color[4] = color[3];
                color[3] = color[1];
                color[1] = temp;
            } else {
                temp = color[0];
                color[0] = color[1];
                color[1] = color[3];
                color[3] = color[4];
                color[4] = temp;
            }
            break;
        case 1: //Eje Y
            if(dir) {
                temp = color[0];
                color[0] = color[2];
                color[2] = color[3];
                color[3] = color[5];
                color[5] = temp;
            } else {
                temp = color[0];
                color[0] = color[5];
                color[5] = color[3];
                color[3] = color[2];
                color[2] = temp;
            }
            break;
        case 2: //Eje Z
            if(dir) {
                temp = color[1];
                color[1] = color[5];
                color[5] = color[4];
                color[4] = color[2];
                color[2] = temp;
            } else {
                temp = color[1];
                color[1] = color[2];
                color[2] = color[4];
                color[4] = color[5];
                color[5] = temp;
            }
            break;
	}
}
