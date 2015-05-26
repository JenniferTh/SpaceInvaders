#define GLFW_DLL
#include <c:/MinGW/include/GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <iostream>
#include "vec3.hpp"
#include <iostream>
#include <time.h>
using namespace std;
static double window_width_ = 1024;
static double window_height_ = 768;
static double xpos;
static double ypos = -10; //Maus Position//Rotation
double w1RSpeed = 4;
static double alpha_1 = 90;
static double alpha_2 = 0;
double length = 2;
//Kugel1
double radius = .35;
Vec3 kugel1(0,radius,10);
Vec3 speedKugel (0.01, 0, 0.01);




void DrawSphere(const Vec3& ctr, double r){
  int     i, j,
          n1 = 6, n2 = 12;
  Vec3    normal, v1;
  double  a1, a1d = M_PI / n1,
          a2, a2d = M_PI / n2,
          s1, s2,
          c1, c2;

  glShadeModel(GL_SMOOTH);
  for(i = 0; i < n1; i++){
    a1 = i * a1d;

    glBegin(GL_TRIANGLE_STRIP);
    for(j = 0; j <= n2; j++){
      a2 = (j + .5 * (i % 2)) * 2 * a2d;

      s1 = sin(a1);
      c1 = cos(a1);
      s2 = sin(a2);
      c2 = cos(a2);
      normal = c1 * XVec3 + s1 * (c2 * YVec3 + s2 * ZVec3);
      v1 = ctr + r * normal;
      glNormal3dv(normal.p);
      glVertex3dv(v1.p);

      s1 = sin(a1 + a1d);
      c1 = cos(a1 + a1d);
      s2 = sin(a2 + a2d);
      c2 = cos(a2 + a2d);
      normal = c1 * XVec3 + s1 * (c2 * YVec3 + s2 * ZVec3);
      v1 = ctr + r * normal;
      glNormal3dv(normal.p);
      glVertex3dv(v1.p);
    }
    glEnd();
  }
}
void InitLighting() {
  GLfloat lp1[4]  = { 10,  5,  10,  0};
  GLfloat lp2[4]  = { -5,  5, -10,  0};
  GLfloat red[4]  = {1.0, .8,  .8,  1};
  GLfloat blue[4] = { .8, .8, 1.0,  1};

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glShadeModel(GL_SMOOTH);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
  glEnable(GL_LIGHTING);

  glLightfv(GL_LIGHT1, GL_POSITION, lp1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,  red);
  glLightfv(GL_LIGHT1, GL_SPECULAR, red);
  glEnable(GL_LIGHT1);

  glLightfv(GL_LIGHT2, GL_POSITION, lp2);
  glLightfv(GL_LIGHT2, GL_DIFFUSE,  blue);
  glLightfv(GL_LIGHT2, GL_SPECULAR, blue);
  glEnable(GL_LIGHT2);

  glClearColor(1, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // init viewport to canvassize
  glViewport(0, 0, window_width_, window_height_);

  // init coordinate system
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-15, 15, -10, 10, -20, 20);


  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void SetMaterialColor(int side, double r, double g, double b) {
  float	amb[4], dif[4], spe[4];
  int mat;

  dif[0] = r;
  dif[1] = g;
  dif[2] = b;

  for(int i = 0; i < 3; i++) {
    amb[i] = .1 * dif[i];
    spe[i] = .5;
  }
  amb[3] = dif[3] = spe[3] = 1.0;

  switch(side){
    case 1:	mat = GL_FRONT;
      break;
    case 2:	mat = GL_BACK;
      break;
    default: mat = GL_FRONT_AND_BACK;
  }

  glMaterialfv(mat, GL_AMBIENT, amb);
  glMaterialfv(mat, GL_DIFFUSE, dif);
  glMaterialfv(mat, GL_SPECULAR, spe);
  glMaterialf( mat, GL_SHININESS, 20);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

	//Rotation
    if (key == GLFW_KEY_W) alpha_1 -= w1RSpeed;	//Hinten drehen
    if (key == GLFW_KEY_A) alpha_2 -= w1RSpeed;	//Vorne drehen
    if (key == GLFW_KEY_S) alpha_1 += w1RSpeed;	//Links drehen
    if (key == GLFW_KEY_D) alpha_2 += w1RSpeed;	//Rechts drehen
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS){
		speedKugel.p[0] = (kugel1.p[0]-(xpos*30/window_width_));
		speedKugel.p[2] = (kugel1.p[2]-(-ypos*20/window_height_))+10;
	}
}
void drawSquare( Vec3 seite1, Vec3 seite2, Vec3 seite3, Vec3 seite4){
	glBegin(GL_QUADS);
	//glNormal3d( 0, 0, 1);
	glVertex3dv( seite1.p);
	glVertex3dv( seite2.p);
	glVertex3dv( seite3.p);
	glVertex3dv( seite4.p);
	glEnd();
}
void kollision(){
	if((kugel1.p[0]+radius)>=7){
		speedKugel.p[0] *= -1;
	}else if((kugel1.p[0]-radius)<=-7){
		speedKugel.p[0] *= -1;
	}else if((kugel1.p[2]-radius)<=5){
		speedKugel.p[2] *= -1;
	}else if((kugel1.p[2]+radius)>=15){
		speedKugel.p[2] *= -1;
	}
}
void moveKugel(){
	kollision();
	kugel1.p[0] += speedKugel.p[0];
	kugel1.p[2] += speedKugel.p[2];
}
void Preview() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
		glTranslated(0,0,-10);
		//Rotation
		glRotated(alpha_1, 1, 0, 0);
		glRotated(alpha_2, 0, 1, 0);
		glPushMatrix();
			//Boden
			SetMaterialColor(2, 0, .6, 0);
			SetMaterialColor(1, 1, 1, 1);
			drawSquare(Vec3(-7,0,-5), Vec3(7,0,-5), Vec3(7,0,5), Vec3(-7,0,5));
			//Seiten .innen
			SetMaterialColor(2, .6, .4, 0);
			SetMaterialColor(1, 0, 0, 1);
			drawSquare(Vec3(-7,1,-5), Vec3(7,1,-5), Vec3(7,0,-5), Vec3(-7,0,-5));
			SetMaterialColor(2, 0, 0, 1);
			SetMaterialColor(1, .6, .4, 0);
			drawSquare(Vec3(-7,1,-5), Vec3(-7,1,5), Vec3(-7,0,5), Vec3(-7,0,-5));
			SetMaterialColor(1, 0, 0, 1);
			SetMaterialColor(2, .6, .4, 0);
			drawSquare(Vec3(7,1,-5), Vec3(7,1,5), Vec3(7,0,5), Vec3(7,0,-5));
			SetMaterialColor(2, 0, 0, 1);
			SetMaterialColor(1, .6, .4, 0);
			drawSquare(Vec3(-7,1,5), Vec3(7,1,5), Vec3(7,0,5), Vec3(-7,0,5));
			//Seiten .aussen
			SetMaterialColor(2, 1, 1, 1);
			SetMaterialColor(1, .6, .4, 0);
			drawSquare(Vec3(-7.5,1,-5.5), Vec3(7.5,1,-5.5), Vec3(7.5,0,-5.5), Vec3(-7.5,0,-5.5));
			SetMaterialColor(2, .6, .4, 0);
			SetMaterialColor(1, 1, 1, 1);
			drawSquare(Vec3(-7.5,1,-5.5), Vec3(-7.5,1,5.5), Vec3(-7.5,0,5.5), Vec3(-7.5,0,-5.5));
			SetMaterialColor(1, .6, .4, 0);
			SetMaterialColor(2, 1, 1, 1);
			drawSquare(Vec3(7.5,1,-5.5), Vec3(7.5,1,5.5), Vec3(7.5,0,5.5), Vec3(7.5,0,-5.5));
			SetMaterialColor(2, .6, .4, 0);
			SetMaterialColor(1, 1, 1, 1);
			drawSquare(Vec3(-7.5,1,5.5), Vec3(7.5,1,5.5), Vec3(7.5,0,5.5), Vec3(-7.5,0,5.5));
			//Seiten .verkleidung
			SetMaterialColor(2, 1, 1, 1);
			SetMaterialColor(1, .6, .4, 0);
			drawSquare(Vec3(-7.5,0,-5.5), Vec3(-7.5,0,-5), Vec3(7.5,0,-5), Vec3(7.5,0,-5.5));
			SetMaterialColor(2, 1, 1, 1);
			SetMaterialColor(1, .6, .4, 0);
			drawSquare(Vec3(-7.5,1,-5.5), Vec3(-7.5,1,-5), Vec3(7.5,1,-5), Vec3(7.5,1,-5.5));

			SetMaterialColor(1, 1, 1, 1);
			SetMaterialColor(2, .6, .4, 0);
			drawSquare(Vec3(-7.5,0,5.5), Vec3(-7.5,0,5), Vec3(7.5,0,5), Vec3(7.5,0,5.5));
			SetMaterialColor(1, 1, 1, 1);
			SetMaterialColor(2, .6, .4, 0);
			drawSquare(Vec3(-7.5,1,5.5), Vec3(-7.5,1,5), Vec3(7.5,1,5), Vec3(7.5,1,5.5));

			SetMaterialColor(1, 1, 1, 1);
			SetMaterialColor(2, .6, .4, 0);
			drawSquare(Vec3(7.5,0,5), Vec3(7,0,5), Vec3(7,0,-5), Vec3(7.5,0,-5));
			SetMaterialColor(1, 1, 1, 1);
			SetMaterialColor(2, .6, .4, 0);
			drawSquare(Vec3(7.5,1,5), Vec3(7,1,5), Vec3(7,1,-5), Vec3(7.5,1,-5));

			SetMaterialColor(2, 1, 1, 1);
			SetMaterialColor(1, .6, .4, 0);
			drawSquare(Vec3(-7.5,0,5), Vec3(-7,0,5), Vec3(-7,0,-5), Vec3(-7.5,0,-5));
			SetMaterialColor(2, 1, 1, 1);
			SetMaterialColor(1, .6, .4, 0);
			drawSquare(Vec3(-7.5,1,5), Vec3(-7,1,5), Vec3(-7,1,-5), Vec3(-7.5,1,-5));

			glPushMatrix();
				//Kugel
				glTranslated(0, 0, -10);
				SetMaterialColor(3, .99, 1, 1);
				DrawSphere(kugel1, radius);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	moveKugel();
}
int main() {
	  GLFWwindow* window = NULL;

	  printf("Here we go!\n");

	  if(!glfwInit()){
	    return -1;
	  }

	  window = glfwCreateWindow(window_width_, window_height_,
	                            "Ueberragende 3D Animation", NULL, NULL);
	  if(!window) {
	    glfwTerminate();
	    return -1;
	  }

	  glfwMakeContextCurrent(window);

	  while(!glfwWindowShouldClose(window)) {
	    // switch on lighting (or you don't see anything)
	    InitLighting();

	    // set background color
	    glClearColor(0.8, 0.8, 0.8, 1.0);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    // draw the scene
	    Preview();

	    //Listener
	    glfwSetKeyCallback(window, key_callback);
	    glfwGetCursorPos(window, &xpos, &ypos);
	    glfwSetMouseButtonCallback(window, mouse_button_callback);
	    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	    // make it appear (before this, it's hidden in the rear buffer)
	    glfwSwapBuffers(window);

	    glfwPollEvents();
	  }
	  glPopMatrix();

	  glfwTerminate();

	  printf("Goodbye!\n");

	  return 0;
}
