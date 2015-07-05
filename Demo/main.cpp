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
#include <vector>

using namespace std;
static double window_width_ = 1024;
static double window_height_ = 768;
static double xpos;
static double ypos = -10; //Maus Position//Rotation
double w1RSpeed = 4;
static double alpha_1 = 0;
static double alpha_2 = 0;
int ro = 0;

//Asteriods
vector<Vec3> asteroids;
vector<Vec3> asteroidsSpeed;
vector<double> radius;
double speed = 1;

//Shuttle
Vec3 shuttle(0,0,10);
Vec3 shuttleSpeed(0.05,0.05,0);
vector<Vec3> projectiles;
vector<Vec3> projectilesDirection;

double shuttleR = 1;

	void DrawSphere(const Vec3& ctr, double r){
	int i, j,
	n1 = 6, n2 = 12;
	Vec3 normal, v1;
	double a1, a1d = M_PI / n1,
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
		GLfloat lp1[4] = { 10, 5, 10, 0};
		GLfloat lp2[4] = { -5, 5, -10, 0};
		GLfloat red[4] = {1.0, .8, .8, 1};
		GLfloat blue[4] = { .8, .8, 1.0, 1};
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glShadeModel(GL_SMOOTH);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT1, GL_POSITION, lp1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
		glLightfv(GL_LIGHT1, GL_SPECULAR, red);
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT2, GL_POSITION, lp2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, blue);
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
			case 1: mat = GL_FRONT;
			break;
			case 2: mat = GL_BACK;
			break;
			default: mat = GL_FRONT_AND_BACK;
		}
		glMaterialfv(mat, GL_AMBIENT, amb);
		glMaterialfv(mat, GL_DIFFUSE, dif);
		glMaterialfv(mat, GL_SPECULAR, spe);
		glMaterialf( mat, GL_SHININESS, 20);
	}
	void elasticCollision(int i, int j){
		double mass1 = radius[i];
		double mass2 = radius[j];

		double newSpeedX2 = (2*mass1*asteroidsSpeed[i].p[0]+asteroidsSpeed[j].p[0]*(mass2-mass1)) / (mass1 + mass2);
		double newSpeedX1 = (2*mass2*asteroidsSpeed[j].p[0]+asteroidsSpeed[i].p[0]*(mass1-mass2)) / (mass1 + mass2);
		double newSpeedY2 = (2*mass1*asteroidsSpeed[i].p[1]+asteroidsSpeed[j].p[1]*(mass2-mass1)) / (mass1 + mass2);
		double newSpeedY1 = (2*mass2*asteroidsSpeed[j].p[1]+asteroidsSpeed[i].p[1]*(mass1-mass2)) / (mass1 + mass2);

		asteroidsSpeed[i].p[0] = newSpeedX1;
		asteroidsSpeed[j].p[0] = newSpeedX2;
		asteroidsSpeed[i].p[1] = newSpeedY1;
		asteroidsSpeed[j].p[1] = newSpeedY2;

		asteroids[i].p[0] += newSpeedX1;
		asteroids[j].p[0] += newSpeedX2;
		asteroids[i].p[1] += newSpeedY1;
		asteroids[j].p[1] += newSpeedY2;
	}
	double randomDoubleBetween(double XMin, double XMax){
		double X = XMin + rand() * (XMax - XMin) / RAND_MAX;
		return X;
	}
	void collideRingFence(int i){
		if((asteroids[i].p[0]+radius[i])>=14+(2*radius[i])){
			asteroids[i].p[0] *= -1;
			asteroids[i].p[1] *= -1;
		}else if((asteroids[i].p[0]-radius[i])<=-14-(2*radius[i])){
			asteroids[i].p[0] *= -1;
			asteroids[i].p[1] *= -1;
		}else if((asteroids[i].p[1]-radius[i])<=-9-(2*radius[i])){
			asteroids[i].p[0] *= -1;
			asteroids[i].p[1] *= -1;
		}else if((asteroids[i].p[1]+radius[i])>=9+(2*radius[i])){
			asteroids[i].p[0] *= -1;
			asteroids[i].p[1] *= -1;
		}
	}
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
		//Rotation
		/*
		if (key == GLFW_KEY_W) alpha_1 -= w1RSpeed; //Hinten drehen
		if (key == GLFW_KEY_A) alpha_2 -= w1RSpeed; //Vorne drehen
		if (key == GLFW_KEY_S) alpha_1 += w1RSpeed; //Links drehen
		if (key == GLFW_KEY_D) alpha_2 += w1RSpeed; //Rechts drehen

		if (key == GLFW_KEY_A){
			kugel.p[0]+=-0.25;
		}
		if (key == GLFW_KEY_D){
			kugel.p[0]+=0.25;

		}*/

		if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE){
			//if(projectiles.size()<=10){
				SetMaterialColor(0, 0, .1, .1);
				Vec3 projectil;
				projectil.p[0] = shuttle.p[0];
				projectil.p[1] = shuttle.p[1];
				projectil.p[2] = 10;
				DrawSphere(projectil, .1);
				projectiles.push_back(projectil);
				projectil.p[0] = shuttleSpeed.p[0]*.1;
				projectil.p[1] = shuttleSpeed.p[1]*.1;
				projectil.p[2] = 0;
				projectilesDirection.push_back(projectil);
			//}
		}
		if (key == GLFW_KEY_UP) {
			for(unsigned i = 0;i<5;i++){
				shuttle.p[0]+=shuttleSpeed.p[0];
				shuttle.p[1]+=shuttleSpeed.p[1];
			}
		}
		if (key == GLFW_KEY_DOWN) {
			for(unsigned i = 0;i<5;i++){
				shuttle.p[0]-=shuttleSpeed.p[0];
				shuttle.p[1]-=shuttleSpeed.p[1];
			}
		}
		if (key == GLFW_KEY_LEFT){

		}
		if (key == GLFW_KEY_RIGHT){

		}
		if (key == GLFW_KEY_L) speed = 0;
		if (key == GLFW_KEY_K) speed = 1;
		if (key == GLFW_KEY_P) speed += 0.01;
		if (key == GLFW_KEY_O) speed -= 0.01;
		if (key == GLFW_KEY_M) {
			asteroids.clear();
			radius.clear();
			asteroidsSpeed.clear();
			for(int i = 0; i<10;i++){
				double x = randomDoubleBetween(-14, 14);
				double y = randomDoubleBetween(-9, 9);
				double r = randomDoubleBetween(0.8,1.2);
				Vec3 m(x,y,10);
				radius.push_back(r);
				asteroids.push_back(m);
				asteroidsSpeed.push_back(Vec3(randomDoubleBetween(-0.005, 0.005),randomDoubleBetween(-0.005, 0.005),0));
				//Vector asteriods von Vec3 zu Asteriods!!!
			}
		}

	}
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

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
	void collideBalls(int i, int j){
		double distance = pow(((asteroids[i].p[0]-asteroids[j].p[0])*(asteroids[i].p[0]-asteroids[j].p[0]))
				+((asteroids[i].p[1]-asteroids[j].p[1])*(asteroids[i].p[1]-asteroids[j].p[1])),2);
		if (distance < 0) { distance = distance * -1; }
		if(asteroids[i].p[0] + radius[i] + radius[j] > asteroids[j].p[0]
		&& asteroids[i].p[0] < radius[i] + radius[j] + asteroids[j].p[0]
		&& asteroids[i].p[1] + radius[i] + radius[j] > asteroids[j].p[1]
		&& asteroids[i].p[1] < radius[i] + radius[j] + asteroids[j].p[1]
		&& distance < radius[i] + radius[j]){
			elasticCollision(i, j);
		}
	}
	bool collideProjectile(int i, int j){
		double distance = pow(((asteroids[i].p[0]-projectiles[j].p[0])*(asteroids[i].p[0]-projectiles[j].p[0]))
						+((asteroids[i].p[1]-projectiles[j].p[1])*(asteroids[i].p[1]-projectiles[j].p[1])),2);
		if (distance < 0) { distance = distance * -1; }
		if(asteroids[i].p[0] + radius[i] + .1 > projectiles[j].p[0]
		&& asteroids[i].p[0] < radius[i] + .1 + projectiles[j].p[0]
		&& asteroids[i].p[1] + radius[i] + .1 > projectiles[j].p[1]
		&& asteroids[i].p[1] < radius[i] + .1 + projectiles[j].p[1]
		&& distance < radius[i] + radius[j]){
			projectiles.erase(projectiles.begin()+j);
			projectilesDirection.erase(projectilesDirection.begin()+j);
			if(asteroids[i].p[2]==10){
				for(unsigned k = 0;k<4;k++){
					double x;
					double y;
					if(k==0){
						x = asteroids[i].p[0]+0.8;
						y = asteroids[i].p[1];
						asteroidsSpeed.push_back(Vec3(randomDoubleBetween(0.001, 0.005),randomDoubleBetween(-0.001, 0.001),0));
					}else if(k==1){
						x = asteroids[i].p[0]-0.8;
						y = asteroids[i].p[1];
						asteroidsSpeed.push_back(Vec3(randomDoubleBetween(-0.005, -0.004),randomDoubleBetween(-0.001, 0.001),0));
					}else if(k==2){
						x = asteroids[i].p[0];
						y = asteroids[i].p[1]+0.8;
						asteroidsSpeed.push_back(Vec3(randomDoubleBetween(-0.001, 0.001),randomDoubleBetween(0.001, 0.005),0));
					}else if(k==3){
						x = asteroids[i].p[0];
						y = asteroids[i].p[1]-0.8;
						asteroidsSpeed.push_back(Vec3(randomDoubleBetween(-0.001, 0.001),randomDoubleBetween(-0.005, -0.004),0));
					}
					Vec3 m(x,y,10.001);
					radius.push_back(.6);
					asteroids.push_back(m);
				}
			}
			asteroids.erase(asteroids.begin()+i);
			asteroidsSpeed.erase(asteroidsSpeed.begin()+i);
			radius.erase(radius.begin()+i);
			return true;
		}
		return false;
	}
	void newLocation(int i){
		asteroids[i].p[0] += asteroidsSpeed[i].p[0]*speed;
		asteroids[i].p[1] += asteroidsSpeed[i].p[1]*speed;
	}
	void rotateShuttle(int angle){
		shuttleSpeed.p[0] = cos(angle)*shuttleSpeed.p[0]-sin(angle)*shuttleSpeed.p[0];
		shuttleSpeed.p[1] = sin(angle)*shuttleSpeed.p[1]-cos(angle)*shuttleSpeed.p[1];
	}
	void move(){
		for(unsigned i = 0; i<asteroids.size();i++){
			newLocation(i);
			bool deleted;
			for(unsigned t = 0; t<projectiles.size(); t++){
				if(collideProjectile(i, t)){
					deleted = true;
					break;
				}
			}
			for(unsigned j = 0; j<asteroids.size();j++){
				if(deleted==true){
					//i--;
					break;
				}
				if(i!=j){
					collideBalls(i,j);
				}
			}
			collideRingFence(i);
		}
		for(unsigned i = 0; i<projectiles.size();i++){
			projectiles[i].p[0] +=projectilesDirection[i].p[0];
			projectiles[i].p[1] +=projectilesDirection[i].p[1];
			if(projectiles[i].p[0]<-14-.1||projectiles[i].p[0]>14+.1||projectiles[i].p[1]<-9-.1||projectiles[i].p[1]>9+.1){
				projectiles.erase(projectiles.begin()+i-1);
				projectilesDirection.erase(projectilesDirection.begin()+i-1);
			}
		}
	}
	void draw(){
		for(unsigned i = 0; i<asteroids.size();i++){
			SetMaterialColor(3, 1, 1, 1);
			DrawSphere(asteroids[i], radius[i]);
		}
		if(projectiles.size()>10){
			while(projectiles.size()>10){
				projectiles.erase(projectiles.begin());
				projectilesDirection.erase(projectilesDirection.begin());
			}
		}
		for(unsigned i = 0; i<projectiles.size();i++){
			SetMaterialColor(3, .99, .1, .1);
			DrawSphere(projectiles[i], .1);
		}
	}
	void insert(){
		asteroids.empty();
		radius.empty();
		asteroidsSpeed.empty();
		for(int i = 0; i<10+ro*2;i++){
			double x = randomDoubleBetween(-14, 14);
			double y = randomDoubleBetween(-9, 9);
			double r = randomDoubleBetween(0.8,1.2);
		 	Vec3 m(x,y,10);
		 	radius.push_back(r);
		 	asteroids.push_back(m);
		 	asteroidsSpeed.push_back(Vec3(randomDoubleBetween(-0.005, 0.005),randomDoubleBetween(-0.005, 0.005),0));
		 	//Vector asteriods von Vec3 zu Asteriods!!!
		}
	}
	void Preview() {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
			if(asteroids.size()<1){
				insert();
				ro +=1;
			}
			glTranslated(0,0,-10);
			//Rotation
			glRotated(alpha_1, 1, 0, 0);
			glRotated(alpha_2, 0, 1, 0);
			glPushMatrix();
			SetMaterialColor(0, 0, .1, .1);
			DrawSphere(shuttle, shuttleR);


			//Spielfeld
			SetMaterialColor(0, 0, 0, 0.15);
			drawSquare(Vec3(-14,-9,0), Vec3(14,-9,0), Vec3(14,9,0), Vec3(-14,9,0));

			SetMaterialColor(0, 0, 0, .1);
			drawSquare(Vec3(-14,-9,5), Vec3(14,-9,5), Vec3(14,-12,5), Vec3(-14,-12,5));

			SetMaterialColor(0, 0, 0, .15);
			drawSquare(Vec3(-14,-12,5), Vec3(-14,12,5), Vec3(-16,12,5), Vec3(-16,-12,5));

			SetMaterialColor(0, 0, 0, .15);
			drawSquare(Vec3(14,-12,5), Vec3(16,-12,5), Vec3(16,12,5), Vec3(14,12,5));

			SetMaterialColor(0, 0, 0, .15);
			drawSquare(Vec3(-14,9,5), Vec3(14,9,5), Vec3(14,12,5), Vec3(-14,12,5));

			//Asteroid
			glTranslated(0, 0, -10);
			draw();
			move();

			//Ufo
			Ufo Ufo1;
			Ufo1.DrawUfo();
		glPopMatrix();
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
