#include <iostream>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "cmath"
#include "cstdlib"
#include "ctime"
#include "vector"
#include "Cube.h"

using namespace std;

vector<int> keypressed;
vector<int> plane;
vector<int> axe;

//Vista de camara.
double pos[3];

//Rotaciones
double x = 0;
double y = 0;

//Cambio de eje y fila
int Axis = 0;
int Row = 0;

//Timer
double TIME = 0.0;

//Pausa
bool playing = false;

//No resuelto
bool mess = false;
bool solving = false;

//Valores para mover cubo aleatoriamente
int move_left = 0;
int move_down = 0;
int moveCube = 0;
int Current_dir = 0;

void reshape(int w,int h);
void init();
void key(unsigned char key,int x ,int y);
void set_position();
void specialkey(int key,int x ,int y);
void display();
void idle();
void random(int val);
void SolveCubeTimer(int value);
void Timer(int value);
void SolveCube();
void printLog();

class Rubix{
public:
	Cube c[27]; //27 objetos cubos que componen el Rubik
	double len; //Largo de cubo
    double theta; //Angulo de rotacion de cada fila de cubos
	int dir; //Direccion de rotacion
    int cubepos[3][3][3];  //Referencia a cada cubo.
	bool rotating;  //Esta rotando?
	std::vector<int> axis; //Eje
    std::vector<int> plane;//
    std::vector<int> keypressed; //Record de todo los movimientos realizados.
public:
	Rubix(double length);
	void fix_cube();
	void display();
    bool check_solved();
	void rotate_cube_plane();
	void display_rotation();
	void rotation_idle_func();
    void change_pos(int dir,int & a0,int & a1,int & a2,int & b0,int & b1,int & b2,int & c0,int & c1,int & c2);
};

//Crea cubo grande
Rubix::Rubix(double length) {
    
	len = length;
	plane.push_back(2); //Plano de el cubo
	axis.push_back(1); //Empieza en eje
    rotating = false;
    //Direcciones de rotacion no set.
	theta = 0;
	dir = 0;
    
	for(int i = 0 ; i < 9 ; i++) {
        c[i].color[2] = 3; //Cara roja
        c[i + 18].color[5] = 6; // Cara blanca
    }
	
    for(int i = 0 ; i < 27 ; i += 9) {
        //Cara amarilla
		c[i + 5].color[0] = 1;
        c[i + 8].color[0] = 1;
        c[i + 2].color[0] = 1;
        //Cara violeta
		c[i + 2].color[1] = 2;
        c[i + 1].color[1] = 2;
        c[i].color[1] = 2;
        //Cara verde
		c[i].color[3] = 4;
        c[i + 3].color[3] = 4;
        c[i + 6].color[3] = 4;
		//Cara azul
        c[i + 6].color[4] = 5;
        c[i + 7].color[4] = 5;
        c[i + 8].color[4] = 5;
	}
    
    //Forma de referenciar cada cubo.
	int p = 0;
	for(int i = 0 ; i < 3 ; i++)
		for(int j = 0 ; j < 3 ; j++)
			for(int k = 0 ; k < 3 ; k++)
				cubepos[i][j][k]=p++;
    
    fix_cube(); //Posiciona cubos.
}

//Verifica si todo los lados tienen el mismo color
bool Rubix::check_solved() {

	int lado1 = c[cubepos[0][0][0]].color[2];
    int lado2 = c[cubepos[2][0][0]].color[5];
    
	for(int i = 0 ; i < 3 ; i++)
		for(int j = 0 ; j < 3 ; j++)
			if( c[cubepos[0][i][j]].color[2] != lado1 || c[cubepos[2][i][j]].color[5] != lado2)
				return false;
    
	lado1 = c[cubepos[0][0][0]].color[0];
    lado2 = c[cubepos[0][0][2]].color[3];
    
	for(int i = 0 ; i < 3 ; i++)
		for(int j = 0 ; j < 3 ; j++)
			if(c[cubepos[i][j][0]].color[0] != lado1 || c[cubepos[i][j][2]].color[3] != lado2)
				return false;
	
	lado1 = c[cubepos[0][0][0]].color[1];
    lado2 = c[cubepos[0][2][0]].color[4];
    
	for(int i = 0 ; i < 3 ; i++)
		for(int j = 0 ; j < 3 ; j++)
			if(c[cubepos[i][0][j]].color[1] != lado1 || c[cubepos[i][2][j]].color[4] != lado2)
				return false;
    
	return true;
}


//Muestra rotacion de filas de cubos
void Rubix::display_rotation() {
    
	double angle;
    
    //Dependiendo direccion
	if(dir) {
        angle = -1 * theta;
    }else {
        angle = theta;
    }
    
	glPushMatrix();
	
    switch(axis.back()) {
        case 0:
            //Dibuja todo el cubo otra vez
            for(int i = 0 ; i < 3 ; i++) {
                if(i != plane.back()) {
                    for(int j = 0 ; j < 3 ; j++)
                        for(int k = 0 ; k < 3 ; k++)
                            c[cubepos[i][j][k]].display();
                }
            }
            glRotated(angle,1,0,0); //Rota rota todo el cubo
            for(int j = 0 ; j < 3 ; j++)
                for(int k = 0 ; k < 3 ; k++)
                    c[cubepos[plane.back()][j][k]].display(); // Redibuja solo la parte que quiere enseñas rotando
            break;
        case 1:
            for(int j = 0 ; j < 3 ; j++) {
                if(j != plane.back()) {
                    for(int i = 0 ; i < 3 ; i++)
                        for(int k = 0 ; k < 3 ; k++)
                            c[cubepos[i][j][k]].display();
                }
            }
            glRotated(angle,0,1,0);
            for(int i=0;i<3;i++)
                for(int k=0;k<3;k++)
                    c[cubepos[i][plane.back()][k]].display();
            break;
        case 2:
            for(int k = 0 ; k < 3 ; k++) {
                if(k!=plane.back()) {
                    for(int i = 0 ; i < 3 ; i++)
                        for(int j = 0 ; j < 3 ; j++)
                            c[cubepos[i][j][k]].display();
                }
            }
            glRotated(angle,0,0,1);
            for(int i = 0 ; i < 3 ; i++)
                for(int j = 0 ; j < 3 ; j++)
                    c[cubepos[i][j][plane.back()]].display();
            break;
	}
	glPopMatrix();
}

//Se mantiene mostrando rotaciones
void Rubix::rotation_idle_func() {
	if(rotating) {
		if(theta >= (double)90) {
			rotating = false;
			theta = 0.0;
			rotate_cube_plane();
		}
		else
			theta += 1.8;
		glutPostRedisplay();
	}
}

//Acomoda cada cubo en su lugar
void Rubix::fix_cube() {
	double dist = len + 0.13; //Espacio entre cubos
	for(int j = 0 ; j < 3 ; j++) {
		for(int k = 0 ; k < 3 ; k++) {
			c[cubepos[0][j][k]].cx = -1 * dist;
			c[cubepos[1][j][k]].cx = 0;
			c[cubepos[2][j][k]].cx = dist;
		}
	}
    
	for(int i = 0 ; i < 3 ; i++) {
		for(int j = 0 ; j < 3 ; j++) {
			c[cubepos[i][j][0]].cz = dist;
			c[cubepos[i][j][1]].cz = 0;
			c[cubepos[i][j][2]].cz = -1 * dist;
		}
	}
    
	for(int i = 0 ; i < 3 ; i++) {
		for(int k = 0 ; k < 3 ; k++) {
			c[cubepos[i][0][k]].cy = -1 * dist;
			c[cubepos[i][1][k]].cy = 0;
			c[cubepos[i][2][k]].cy = dist;
		}
	}
}

//Es llamado constantemente
void Rubix::display() {
	if(!rotating) {
		double disp = len + 0.13;
		glPushMatrix();
        
        //Dibija cada cubo.
		for(int i = 0 ; i < 27 ; i++)
			c[i].display();
        
        //Posiciona el plano
		if(axis.back() == 0) {
			if(plane.back() == 0) {
                glTranslated(-disp, 0, 0);
            } else if(plane.back() == 2) {
                glTranslated(disp, 0, 0);
            }
            glRotatef(90,0,1,0);
		} else if(axis.back() == 1) {
			if(plane.back() == 0) {
                glTranslated(0, -disp, 0);
            } else if(plane.back() == 2) {
                glTranslated(0, disp, 0);
            }
			glRotatef(90, 1, 0, 0);
        } else {
			if(plane.back() == 0) {
                glTranslated(0, 0, len);
            }else if(plane.back() == 2) {
                glTranslated(0, 0, -len);
            }
        }
        
        //Dibuja el plano
		glColor4f(1, 1, 1, 0.5);
		glRectd(-(len * 2), -( len * 2), (len * 2), (len * 2));
		glPopMatrix();
	}
	else
		display_rotation();
}

//Intercambia posisiones de cubo dependiendo la direccion
void Rubix::change_pos(int dir,
                       int & a0,int & a1,int & a2,
                       int & b0,int & b1,int & b2,
                       int & c0,int & c1,int & c2) {
	int temp;
	if(dir) {
		temp = a0;
        a0 = a2;
        a2 = c2;
        c2 = c0;
        c0 = temp;
		temp = b0;
        b0 = a1;
        a1 = b2;
        b2 = c1;
        c1 = temp;
	} else {
		temp = a0;
        a0 = c0;
        c0 = c2;
        c2 = a2;
        a2 = temp;
		temp = b0;
        b0 = c1;
        c1 = b2;
        b2 = a1;
        a1 = temp;
	}
}

//Rota fila de cubos.
void Rubix::rotate_cube_plane() {
	if(!rotating) {
        
        //Si el plano no esta en algun lugar del cubo.
		if(plane.back()!=0 && plane.back()!=1 && plane.back()!=2)
			exit(2);
        
        //Gira los cubos usando referencia cubepos[3][3][3]
		switch(axis.back()) {
            case 0:
                change_pos(dir,cubepos[plane.back()][0][0],cubepos[plane.back()][0][1],cubepos[plane.back()][0][2],cubepos[plane.back()][1][0],cubepos[plane.back()][1][1],cubepos[plane.back()][1][2],cubepos[plane.back()][2][0],cubepos[plane.back()][2][1],cubepos[plane.back()][2][2]);
                break;
            case 1:
                change_pos(dir,cubepos[2][plane.back()][0],cubepos[2][plane.back()][1],cubepos[2][plane.back()][2],cubepos[1][plane.back()][0],cubepos[1][plane.back()][1],cubepos[1][plane.back()][2],cubepos[0][plane.back()][0],cubepos[0][plane.back()][1],cubepos[0][plane.back()][2]);
                break;
            case 2:
                change_pos(dir,cubepos[0][0][plane.back()],cubepos[1][0][plane.back()],cubepos[2][0][plane.back()],cubepos[0][1][plane.back()],cubepos[1][1][plane.back()],cubepos[2][1][plane.back()],cubepos[0][2][plane.back()],cubepos[1][2][plane.back()],cubepos[2][2][plane.back()]);
                break;
            default:
                exit(2);
		}
		fix_cube();
        
        //Rota los cubos
		switch(axis.back()) {
            case 0:
                for(int j = 0 ; j < 3 ; j++)
                    for(int k = 0 ; k < 3 ; k++)
                        c[cubepos[plane.back()][j][k]].rotate(0,dir);
                break;
            case 1:
                for(int i = 0 ; i < 3 ; i++)
                    for(int k = 0 ; k < 3 ; k++)
                        c[cubepos[i][plane.back()][k]].rotate(1,dir);
                break;
            case 2:
                for(int i = 0 ; i < 3 ; i++)
                    for(int j = 0 ; j < 3 ; j++)
                        c[cubepos[i][j][plane.back()]].rotate(2,dir);
                break;
		}
    }
	else
		glutIdleFunc(idle);
}

Rubix r(2);

int main(int argc,char * argv[]) {
	glutInit(&argc,argv);   //Inizialize GLUT Lib
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800,600);    //Set windows size
	glutCreateWindow("Rubix's Cube");   //Window tittle
	glutDisplayFunc(display);
	glutReshapeFunc(reshape); //Gives camera perspectives
	glutKeyboardFunc(key);
	glutSpecialFunc(specialkey);
    Timer(0);
	init();
	glutMainLoop();
}

void idle() {
    //Se mantiene rotando el cubo de ser necesario.
	r.rotation_idle_func();
}

void reshape(int w,int h) {
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50,(GLfloat)w/(GLfloat)h,1,200);
	glMatrixMode(GL_MODELVIEW);
}

//Rotaciones de filas de cubos.
void key(unsigned char key,int x ,int y) {
    if(r.rotating) {
        return;
    }
	switch(key) {
        case 'q':
            SolveCubeTimer(0);
            break;
        case 'i':
            printLog();
            break;
        case 'r':
            random(0);
            break;
        case 32:
            if(!mess) { //Desordena cubo
                random(0);
                glutPostRedisplay();
            }
            playing = (playing) ? false : true;
            break;
        case ',':
        case '<':
            if(!playing){
                return;
            }
            r.dir=1;
            r.keypressed.push_back(3);
            r.rotating = true;
            r.rotate_cube_plane();
            break;
        case '.':
        case '>':
            if(!playing){
                return;
            }
            r.dir=0;
            r.keypressed.push_back(2);
            r.rotating = true;
            r.rotate_cube_plane();
            break;
        case 'a':
        case 'A':
            if(!playing){
                return;
            }
            if(Axis == 0) {
                r.axis.push_back(0);
                r.keypressed.push_back(0);
                Axis = 1;
            } else if(Axis == 1){
                r.axis.push_back(1);
                r.keypressed.push_back(0);
                Axis = 2;
            } else {
                r.axis.push_back(2);
                r.keypressed.push_back(0);
                Axis = 0;
            }
            break;
        case 's':
        case 'S':
            if(!playing){
                return;
            }
            if(Row == 0) {
                r.plane.push_back(0);
                r.keypressed.push_back(1);
                Row = 1;
            } else if(Row == 1) {
                r.plane.push_back(1);
                r.keypressed.push_back(1);
                Row = 2;
            } else {
                r.plane.push_back(2);
                r.keypressed.push_back(1);
                Row = 0;
            }
            break;
    }
}

void Solve(int key) {
    if(r.rotating) {
        return;
    }
    switch (key) {
        case 0: {
            int axis_val = axe.back();
            if(axis_val == 0){
                r.axis.push_back(0);
                r.keypressed.push_back(0);
            } else if(axis_val == 1) {
                r.axis.push_back(1);
                r.keypressed.push_back(0);
            } else {
                r.axis.push_back(2);
                r.keypressed.push_back(0);
            }
            
            axe.pop_back();
            break;
        }
        case 1: {
            int plane_val = plane.back();
            if (plane_val == 0) {
                r.plane.push_back(0);
                r.keypressed.push_back(1);
            } else if(plane_val == 1) {
                r.plane.push_back(1);
                r.keypressed.push_back(1);
            } else {
                r.plane.push_back(2);
                r.keypressed.push_back(1);
            }
            plane.pop_back();
            break;
        }
        case 2: {
            r.dir=1;
            r.keypressed.push_back(3);
            r.rotating = true;
            r.rotate_cube_plane();
            break;
        }
        case 3: {
            r.dir=0;
            r.keypressed.push_back(2);
            r.rotating = true;
            r.rotate_cube_plane();
            break;
        }
        default:
            break;
    }

}

//Cambia perspectiva de camara "Rotaciones"
void set_position() {
   
    if(x >= 360)
		x -= 360;
	else if(x < 0)
		x += 360;
	if(y >= 360)
		y -= 360;
	else if(y < 0)
		y += 360;
    
    pos[0] = (15 * cos((3.14/180.0) * y)) * cos((3.14/180.0) * x);
	pos[1] = 15 * sin((3.14/180.0) * y);
	pos[2] = (15 * cos((3.14/180.0) * y)) * sin((3.14/180.0) * x);
    
    if(y > 90 && y < 270)
		gluLookAt(pos[0],pos[1],pos[2],0,0,0,0,-1,0);
	else
		gluLookAt(pos[0],pos[1],pos[2],0,0,0,0,1,0);
}

//Funcion para teclas especiales.
void specialkey(int key,int a ,int b) {
    if(!playing) { //If game is paused.
        return;
    }
	switch(key) {
        case GLUT_KEY_LEFT:
            x-=15;
            set_position(); //Change camera perspective.
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            x+=15;
            set_position();
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            y+=15;
            set_position();
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            y-=15;
            set_position();
            glutPostRedisplay();
            break;
    }
}

//Escribe instrucciones en pantalla.
void drawText(const string text, unsigned long length, int x, int y){
    glMatrixMode(GL_PROJECTION);
    double matrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    for(int i=0; i<length; i++){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
	glClearColor(0, 0, 0, 1.0);
	glEnable(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Funcion que mantiene tiempo correindo, se llama cada milisegundo.
void Timer(int value){
    if(playing && mess) {
        TIME += 0.01;
    }
    glutPostRedisplay();
    glutTimerFunc(10, Timer, 0);
}


void SolveCubeTimer(int value) {
    if(!solving) {
        keypressed = r.keypressed;
        axe = r.axis;
        plane = r.plane;
        solving = true;
        cout << "Solving" << endl << "Key: ";
    }
    
    if(!keypressed.empty()) {
        int key = keypressed.back();
        cout << key << " ";
        keypressed.pop_back();
        Solve(key);
        glutPostRedisplay();
        glutTimerFunc(100, SolveCubeTimer, 0);
    } else {
        cout << endl;
        solving = false;
        r.check_solved();
    }
}

//Mueve el cubo aleatoriamente
void random(int value){
    if(move_left != 3) {
        y += 15;
        set_position(); 
        move_left++;
        glutPostRedisplay();
        glutTimerFunc(100, random, 0);
    }else if(move_down != 3) {
        x-=15;
        set_position();
        move_down++;
        glutPostRedisplay();
        glutTimerFunc(100, random, 0);
    }else if(moveCube != 50) {
        int limit = rand() % 8;
        for(int i = 0 ; i < limit ; i++) {
            key('a', 0 , 0);
        }
        limit = rand() % 8;
        for(int i = 0 ; i < limit ; i++) {
            key('s', 0, 0);
        }
        if(Current_dir == 0) {
            key('.', 0, 0);
            Current_dir = rand() % 2;
        } else {
            key(',', 0, 0);
            Current_dir = rand() % 2;
        }
        moveCube++;
        glutPostRedisplay();
        glutTimerFunc(100, random, 0);
    } else {
        printLog();
        mess = true;
    }
}

void printLog() {
    cout << "Randomly" << endl << "Key: ";
    for(int i = 0 ; i < r.keypressed.size() ; i++) {
        cout << r.keypressed[i] << " ";
    }
    cout << endl;
}

//Dibuja pantalla
void display() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
    set_position();
    r.display(); //Dibuja cubos
    
    glColor3f(1, 0, 0);
    drawText("Instructions:", 13, 10, 580);
    glColor3f(1, 1, 1);
    drawText("Rotate Cube: [->], [v], [^] or [<-]", 35, 10, 560);
    drawText("Change Axis: [A]", 16 , 10, 540);
    drawText("Change Row: [S]", 15 , 10, 520);
    drawText("Rotate Axis: [<] or [>]", 23 , 10, 500);
    drawText("Start/Pause: [Space]", 20, 10, 480);
    glColor3f(1, 1, 1);
    drawText("Time:", 5, 620, 580);
    glColor3f(1, 0, 0);
    
    string time =  to_string(TIME);
    drawText(time, time.length() - 4, 670, 580);
    
    glutSwapBuffers();
}