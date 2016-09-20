/*
An experiment with writing an .obj importer for a school project. It will read in polygons, normals, and material properties.
*/

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "glut.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <glm/glm.hpp>

using namespace std;

#define PERSPECTIVE_PROJECTION 1
#define PARALLEL_PROJECTION 2


const int X_MAX = 1000, Y_MAX = 1000;
const float AXIS_LENGTH = .5;
int projection = PERSPECTIVE_PROJECTION;
float nearClip = .2, farClip = 100.0;
bool axisOn = true, gridOn = true, singlePoly = false;

float light0ambient[] = {.2, .2, .2, 1};
float light0spec[] = {0, 0, 0, 1};
float light0diffuse[] = {0, 0, 0, 1};
float light0pos[] = {0, 0, 0, 1};
float light1ambient[] = {0, 0, 0, 1};
float light1spec[] = {1, 1, 1, 1};
float light1diffuse[] = {1, 1, 1, 1};
float light1pos[] = {10, 15, 5, 1};
float light2ambient[] = {0, 0, 0, 1};
float light2spec[] = {1, 1, 1, 1};
float light2diffuse[] = {.5, .5, .5, 1};
float light2pos[] = {-10, 15, -10, 1};
float light3ambient[] = {0, 0, 0, 1};
float light3spec[] = {1, .9, .9, 1};
float light3diffuse[] = {1, .5, .5, 1};
float light3pos[] = {0, 15, 5, 0};
bool light0on = true;
bool light1on = true;
bool light2on = false;
bool light3on = false;

class spherical{
public:
	double ro;
	double theta;
	double phi;

	double getX()
	{
		return ro * sin(theta) * sin(phi);
	}
	double getY()
	{
		return ro * cos(phi);
	}
	double getZ()
	{
		return ro * cos(theta) * sin(phi);
	}
};
vector<vector<spherical>> spherelist;

spherical cameraPos;

struct mat{
float amb[4];
float spec[4];
float diff[4];
float ns;
int illum;
};

struct face{
	int v1, v2, v3;
	int n1, n2, n3;
	mat material;
};

bool loadOBJ(const char* path, vector<glm::vec3> & outvertices, vector<face> & outfaces, vector<glm::vec3> & outnormals)
{
	ifstream infile;
	ifstream inmat;
	infile.open(path);
	inmat.open("stewart.mtl");
	mat currentmat;
	currentmat.amb[3] = 1; currentmat.diff[3] = 1; currentmat.spec[3] = 1;
	string matname;
	string tempstr;

	while ( 1 ){
		string lineHeader;
		infile >> lineHeader;
		if (infile.eof())
		{
			break;
		}

		if (lineHeader == "v"){
			glm::vec3 vertex;
			infile >> vertex.x >> vertex.y >> vertex.z;
			outvertices.push_back(vertex);
		}
		else if (lineHeader == "vn"){
			glm::vec3 normal;
			infile >> normal.x >> normal.y >> normal.z;
			outnormals.push_back(normal);
		}
		else if (lineHeader == "usemtl"){
			infile >> matname;
			inmat.clear();
			inmat.seekg(0);
			while(!inmat.eof())
			{
				inmat >> tempstr;
				if (tempstr == matname)
				{
					break;
				}
			}
			inmat >> tempstr >> currentmat.ns;
			inmat >> tempstr >> currentmat.amb[0] >> currentmat.amb[1] >> currentmat.amb[2];
			inmat >> tempstr >> currentmat.diff[0] >> currentmat.diff[1] >> currentmat.diff[2];
			inmat >> tempstr >> currentmat.spec[0] >> currentmat.spec[1] >> currentmat.spec[2];
			inmat >> tempstr >> tempstr >> tempstr >> tempstr;
			inmat >> tempstr >> tempstr;
			inmat >> tempstr >> tempstr;
			inmat >> tempstr >> currentmat.illum;
		}
		else if (lineHeader == "f"){
			face daface;
			infile >> lineHeader;
			stringstream ss(lineHeader);
			char tempchar;
			ss >> daface.v1 >> tempchar >> tempchar >> daface.n1;

			ss.clear();
			infile >> lineHeader;
			ss.str(lineHeader);
			ss >> daface.v2 >> tempchar >> tempchar >> daface.n2;

			ss.clear();
			infile >> lineHeader;
			ss.str(lineHeader);
			ss >> daface.v3 >> tempchar >> tempchar >> daface.n3;
			daface.material = currentmat;
			outfaces.push_back(daface);
		}

	}
	return true;
}

vector<glm::vec3> vertices;
vector<face> faces;
vector<glm::vec3> normals;
bool res = loadOBJ("stewart.obj", vertices, faces, normals);




void myInit()
{
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	cameraPos.ro = 3;
	cameraPos.theta = 0;
	cameraPos.phi = M_PI/2.0;
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

void drawAxis()
{
	
	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(AXIS_LENGTH, 0, 0);
	glEnd();
	glColor3ub(0, 255, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, AXIS_LENGTH, 0);
	glEnd();
	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, AXIS_LENGTH);
	glEnd();
	
}

void drawPoly()
{
	glColor3ub(255, 255, 255);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < faces.size(); i++)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, faces[i].material.amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, faces[i].material.diff);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, faces[i].material.spec);
		glNormal3f(normals[faces[i].n1 - 1].x, normals[faces[i].n1 - 1].y, normals[faces[i].n1 - 1].z);
		glVertex3f(vertices[faces[i].v1 - 1].x, vertices[faces[i].v1 - 1].y, vertices[faces[i].v1 - 1].z);
		glNormal3f(normals[faces[i].n2 - 1].x, normals[faces[i].n2 - 1].y, normals[faces[i].n2 - 1].z);
		glVertex3f(vertices[faces[i].v2 - 1].x, vertices[faces[i].v2 - 1].y, vertices[faces[i].v2 - 1].z);
		glNormal3f(normals[faces[i].n3 - 1].x, normals[faces[i].n3 - 1].y, normals[faces[i].n3 - 1].z);
		glVertex3f(vertices[faces[i].v3 - 1].x, vertices[faces[i].v3 - 1].y, vertices[faces[i].v3 - 1].z);
	}
	glEnd();
}

void setCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (projection == PERSPECTIVE_PROJECTION)
	{
		gluPerspective(45, 1, nearClip, farClip);
	}
	else
	{
		glOrtho(-4.0, 4.0, -4.0, 4.0, nearClip, farClip);
	}
	glMatrixMode(GL_MODELVIEW);
	if(cameraPos.phi != 0)
	{
		gluLookAt(cameraPos.getX(), cameraPos.getY(), cameraPos.getZ(), 0, 0, 0, 0, 1, 0);
	}
	else
	{
		gluLookAt(cameraPos.getX(), cameraPos.getY(), cameraPos.getZ(), 0, 0, 0, -sin(cameraPos.theta), 0, -cos(cameraPos.theta));
	}
}

void setLights()
{
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0spec);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1ambient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1spec);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
	
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2ambient);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2spec);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2diffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, light2pos);
	
	glLightfv(GL_LIGHT3, GL_AMBIENT, light3ambient);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3spec);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3diffuse);
	glLightfv(GL_LIGHT3, GL_POSITION, light3pos);
	
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	setCamera();
	setLights();
	if (axisOn)
	{
		drawAxis();
	}
	drawPoly();

	glutSwapBuffers();
}

void myKey(unsigned char key, int x, int y)
{
	switch(key)
	{
	case '2': cameraPos.theta += M_PI/180.0; break;
	case '1': cameraPos.theta -= M_PI/180.0; break;
	case 'a': axisOn = !axisOn; break;
	case 'z': if (cameraPos.ro > .3) cameraPos.ro -= .2; break;
	case 'x': if (cameraPos.ro < 30) cameraPos.ro += .2; break;
	case '7': cameraPos.phi += .05; if (cameraPos.phi > M_PI) {cameraPos.phi = M_PI;} break;
	case '8': cameraPos.phi -= .05; if (cameraPos.phi < 0) {cameraPos.phi = 0;} break;
	case 'p': projection++; projection = projection % 2; break;
	case '.': axisOn = true; projection = 1; cameraPos.ro = 3; cameraPos.phi = M_PI/2; cameraPos.theta = 0; break;
	}
	glutPostRedisplay();
}

void menumanage(int value)
{
	switch (value)
	{
	case 0: if (light0on) {glDisable(GL_LIGHT0); light0on = false;} else {glEnable(GL_LIGHT0); light0on = true;} break;
	case 1: if (light1on) {glDisable(GL_LIGHT1); light1on = false;} else {glEnable(GL_LIGHT1); light1on = true;} break;
	case 2: if (light2on) {glDisable(GL_LIGHT2); light2on = false;} else {glEnable(GL_LIGHT2); light2on = true;} break;
	case 3: if (light3on) {glDisable(GL_LIGHT3); light3on = false;} else {glEnable(GL_LIGHT3); light3on = true;} break;
	}
	glutPostRedisplay();
}

void othmenu(int value)
{
	switch (value)
	{
	case 0: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
	case 1: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glShadeModel(GL_SMOOTH); break;
	case 2: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glShadeModel(GL_FLAT); break;
	}
	glutPostRedisplay();
}

int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	glutInitWindowSize(X_MAX, Y_MAX);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Model Viewer");
	
	glutCreateMenu(menumanage);
	glutAddMenuEntry("Ambient Light", 0);
	glutAddMenuEntry("Positional Light 1", 1);
	glutAddMenuEntry("Positional Light 2", 2);
	glutAddMenuEntry("Directional Light", 3);
	glutAttachMenu(GLUT_LEFT_BUTTON);
	
	glutCreateMenu(othmenu);
	glutAddMenuEntry("Wireframe", 0);
	glutAddMenuEntry("Smooth Filled", 1);
	glutAddMenuEntry("Flat Filled", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKey);
	
	myInit();
	glutMainLoop();
}