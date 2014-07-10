#include <iostream>
#include <vector>
#include "Cube.cpp"

class RubixCube {
	Cube c[27];
	double len,theta;
	int dir, cubepos[3][3][3];
	bool rotating;
	std::vector<int> axis,plane,
    keypressed/*0=axis change, 1=plane change, 2=clockwise rot, 3=anticlockwise rot*/;
public:
	RubixCube(double length);
    
	void set_centre();
	void display();
    bool check_solved();
	void rotate_cube_plane();
	void display_rotation();
	void rotation_idle_func();
	void keyboard(unsigned char key);
    void change_pos(int dir,int & a0,int & a1,int & a2,int & b0,int & b1,int & b2,int & c0,int & c1,int & c2);
	void randomize();
};

int Axis = 0;
int Row = 0;

RubixCube::RubixCube(double length) {
	len=length;
	plane.push_back(2);
	axis.push_back(1);
    rotating=false;
	theta=0;
	dir=0;
	int p=0,i,j,k;
    
	for(i=0;i<9;i++)
	{ c[i].color[2]=3; c[i+18].color[5]=6; }
	for(i=0;i<27;i+=9) {
		c[i+5].color[0]=1; c[i+8].color[0]=1; c[i+2].color[0]=1;
		c[i+2].color[1]=2; c[i+1].color[1]=2; c[i].color[1]=2;
		c[i].color[3]=4; c[i+3].color[3]=4; c[i+6].color[3]=4;
		c[i+6].color[4]=5; c[i+7].color[4]=5; c[i+8].color[4]=5;
	}
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
				cubepos[i][j][k]=p++;
	set_centre();
}

bool RubixCube::check_solved()
{
	int v1, v2, i, j;
	v1=c[cubepos[0][0][0]].color[2]; v2=c[cubepos[2][0][0]].color[5];
    
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			if( c[cubepos[0][i][j]].color[2]!=v1 || c[cubepos[2][i][j]].color[5]!=v2)
				return false;
    
	v1=c[cubepos[0][0][0]].color[0]; v2=c[cubepos[0][0][2]].color[3];
    
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			if( c[cubepos[i][j][0]].color[0]!=v1 || c[cubepos[i][j][2]].color[3]!=v2)
				return false;
	
	v1=c[cubepos[0][0][0]].color[1]; v2=c[cubepos[0][2][0]].color[4];
    
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			if( c[cubepos[i][0][j]].color[1]!=v1 || c[cubepos[i][2][j]].color[4]!=v2)
				return false;
    
	return true;
}

void RubixCube::randomize() {
	time_t t; time(&t);
	tm * a; a=localtime(&t);
    
	int i,j,k,till;
	till=20+(a->tm_sec + a->tm_min + a->tm_hour + a->tm_mday + a->tm_mon + a->tm_year)%30;
	j=(a->tm_sec + a->tm_min + a->tm_hour + a->tm_mday + a->tm_mon + a->tm_year)%4;
	for(i=0;i<till;i++) {
		k=j;
		while(j==k  || (j==2 && k==3) || (j==3 && k==2))
			j=rand()%4;
		keypressed.push_back(k);
		switch(k) {
            case 0:
                axis.push_back(rand()%3);
                break;
            case 1:
                plane.push_back(rand()%3);
                break;
            case 2:
                dir=0;
                rotating=false;
                rotate_cube_plane();
                break;
            case 3:
                dir=1;
                rotating=false;
                rotate_cube_plane();
                break;
		}
	}
}

void RubixCube::display_rotation() {
	int i,j,k;
	double angle, disp=len+0.2;
    
	if(dir) angle=-theta;
	else angle=theta;
    
	glPushMatrix();
	switch(axis.back())
	{
        case 0:
            for(i=0;i<3;i++)
            {
                if(i!=plane.back())
                {
                    for(j=0;j<3;j++)
                        for(k=0;k<3;k++)
                            c[cubepos[i][j][k]].display();
                }
            }
            glRotated(angle,1,0,0);
            for(j=0;j<3;j++)
                for(k=0;k<3;k++)
                    c[cubepos[plane.back()][j][k]].display();
            break;
        case 1:
            for(j=0;j<3;j++)
            {
                if(j!=plane.back())
                {
                    for(i=0;i<3;i++)
                        for(k=0;k<3;k++)
                            c[cubepos[i][j][k]].display();
                }
            }
            glRotated(angle,0,1,0);
            for(i=0;i<3;i++)
                for(k=0;k<3;k++)
                    c[cubepos[i][plane.back()][k]].display();
            break;
        case 2:
            for(k=0;k<3;k++)
            {
                if(k!=plane.back())
                {
                    for(i=0;i<3;i++)
                        for(j=0;j<3;j++)
                            c[cubepos[i][j][k]].display();
                }
            }
            glRotated(angle,0,0,1);
            for(i=0;i<3;i++)
                for(j=0;j<3;j++)
                    c[cubepos[i][j][plane.back()]].display();
            break;
	}
	if(axis.back()==0)
	{
		if(plane.back()==0) glTranslated(-disp,0,0);
		else if(plane.back()==2) glTranslated(disp,0,0);
		glRotatef(90,0,1,0);
	}
	else if(axis.back()==1)
	{
		if(plane.back()==0) glTranslated(0,-disp,0);
		else if(plane.back()==2) glTranslated(0,disp,0);
		glRotatef(90,1,0,0);
	}
	else
	{
		if(plane.back()==0) glTranslated(0,0,disp);
		else if(plane.back()==2) glTranslated(0,0,-disp);
	}
	glColor4f(1,0,0,0.3);
	glRectd(-(len*2.25),-(len*2.25),(len*2.25),(len*2.25));
	glPopMatrix();
}

void RubixCube::rotation_idle_func() {
	if(rotating) {
		if(theta>=(double)90) {
			glutIdleFunc(NULL);
			rotating=false;
			theta=0.0;
			rotate_cube_plane();
		}
		else
			theta+=+0.5;
		glutPostRedisplay();
	}
}

void RubixCube::set_centre() {
	int i,j,k;
	double dist=len+0.2;
	for(j=0;j<3;j++)
	{
		for(k=0;k<3;k++)
		{
			c[cubepos[0][j][k]].cx=-dist;
			c[cubepos[1][j][k]].cx=0;
			c[cubepos[2][j][k]].cx=dist;
		}
	}
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			c[cubepos[i][j][0]].cz=dist;
			c[cubepos[i][j][1]].cz=0;
			c[cubepos[i][j][2]].cz=-dist;
		}
	}
	for(i=0;i<3;i++)
	{
		for(k=0;k<3;k++)
		{
			c[cubepos[i][0][k]].cy=-dist;
			c[cubepos[i][1][k]].cy=0;
			c[cubepos[i][2][k]].cy=dist;
		}
	}
}
void RubixCube::display()
{
	if(!rotating)
	{
		double disp=len+0.2;
		glPushMatrix();
		for(int i=0;i<27;i++)
			c[i].display();
        
		if(axis.back()==0)
		{
			if(plane.back()==0) glTranslated(-disp,0,0);
			else if(plane.back()==2) glTranslated(disp,0,0);
			glRotatef(90,0,1,0);
		}
		else if(axis.back()==1)
		{
			if(plane.back()==0) glTranslated(0,-disp,0);
			else if(plane.back()==2) glTranslated(0,disp,0);
			glRotatef(90,1,0,0);
		}
		else
		{
			if(plane.back()==0) glTranslated(0,0,len);
			else if(plane.back()==2) glTranslated(0,0,-len);
		}
		glColor4f(1,1,1,0.3);
		glRectd(-(len*2.25),-(len*2.25),(len*2.25),(len*2.25));
		glPopMatrix();
	}
	else
		display_rotation();
}
void RubixCube::change_pos(int dir,int & a0,int & a1,int & a2,int & b0,int & b1,int & b2,int & c0,int & c1,int & c2)
{
	int temp;
	if(dir) {
		temp=a0; a0=a2; a2=c2; c2=c0; c0=temp;
		temp=b0; b0=a1; a1=b2; b2=c1; c1=temp;
	}
	else {
		temp=a0; a0=c0; c0=c2; c2=a2; a2=temp;
		temp=b0; b0=c1; c1=b2; b2=a1; a1=temp;
	}
}
void RubixCube::rotate_cube_plane()
{
	if(!rotating)
	{
		int i,j,k;
		if(plane.back()!=0 && plane.back()!=1 && plane.back()!=2)
			exit(2);
        
		switch(axis.back())
		{
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
		set_centre();
        
		switch(axis.back())
		{
            case 0:
                for(j=0;j<3;j++)
                    for(k=0;k<3;k++)
                        c[cubepos[plane.back()][j][k]].rotate(0,dir);
                break;
            case 1:
                for(i=0;i<3;i++)
                    for(k=0;k<3;k++)
                        c[cubepos[i][plane.back()][k]].rotate(1,dir);
                break;
            case 2:
                for(i=0;i<3;i++)
                    for(j=0;j<3;j++)
                        c[cubepos[i][j][plane.back()]].rotate(2,dir);
                break;
		}
    }
	else
		glutIdleFunc(NULL);
}
void RubixCube::keyboard(unsigned char key) {
    switch (key) {
        case 'r':
        case 'R':
            //randomize();
            glutPostRedisplay();
            break;
        case ',':
        case '<':
            dir=1;
            keypressed.push_back(3);
            rotating=true;
            rotate_cube_plane();
            break;
        case '.':
        case '>':
            dir=0;
            keypressed.push_back(2);
            rotating=true;
            rotate_cube_plane();
            break;
        case 'a':
        case 'A':
            if(Axis == 0) {
                axis.push_back(0);
                keypressed.push_back(0);
                Axis = 2;
            } else if(Axis == 2) {
                axis.push_back(2);
                keypressed.push_back(0);
                Axis = 1;
            } else {
                axis.push_back(1);
                keypressed.push_back(0);
                Axis = 0;
            }
            break;
        case 's':
        case 'S':
            if(Row == 0) {
                plane.push_back(0);
                keypressed.push_back(1);
                Row = 1;
            } else if(Row == 1) {
                plane.push_back(1);
                keypressed.push_back(1);
                Row = 2;
            } else {
                plane.push_back(2);
                keypressed.push_back(1);
                Row = 0;
            }
            break;
    }
}
