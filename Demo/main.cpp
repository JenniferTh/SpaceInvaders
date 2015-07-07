#define GLFW_DLL
#define GLUT_DISABLE_ATEXIT_HACK
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
#include "Ufo.h"
#include <gl/glut.h>
#include <string>
#include <sstream>

using namespace std;
static double window_width_ = 1024; //Fenster Breite
static double window_height_ = 768; //Fenster Höhe
static double alpha_1 = 0; //Rotation Koordinatensystem
static double alpha_2 = 0;
int rou = 0; //Counter für die Anzahl der Runde, in der man sich befindet

//Asteriods
vector<Vec3> asteroids; //Vec3 Objekte die Mittelpunkte der Asteroiden kennzeichnen
vector<Vec3> asteroidsSpeed; //Vec3 Objekte mit Geschwindigkeiten der Asteroiden
vector<double> radius; //Vector mit Radien aller Asteroiden
double speed = 3; //Geschwindigkeit der Asteroiden

//Shuttle
Ufo Ufo1; //Ufo-Objekt
Vec3 ufoSpeed(0.1,0.1,0); //Geschwindigkeit des Ufos
vector<Vec3> projectiles; //Alle Geschosse eines Ufos
vector<Vec3> projectilesSpeed; //Geschwindigkeit der Geschosse
double speedProjectil = 0.3; //Anpassung der Projectilgeschwindigkeit

int totalScore = 0; //Score

	void DrawSphere(const Vec3& ctr, double r){
	int i, j,
	n1 = 12, n2 = 24;
	//Höhere Auflösung
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
		/*Massen der Kugeln*/
		double newSpeedX2 = (2*mass1*asteroidsSpeed[i].p[0]+asteroidsSpeed[j].p[0]*(mass2-mass1)) / (mass1 + mass2);
		double newSpeedX1 = (2*mass2*asteroidsSpeed[j].p[0]+asteroidsSpeed[i].p[0]*(mass1-mass2)) / (mass1 + mass2);
		double newSpeedY2 = (2*mass1*asteroidsSpeed[i].p[1]+asteroidsSpeed[j].p[1]*(mass2-mass1)) / (mass1 + mass2);
		double newSpeedY1 = (2*mass2*asteroidsSpeed[j].p[1]+asteroidsSpeed[i].p[1]*(mass1-mass2)) / (mass1 + mass2);
		/*Neue Geschwindigkeiten anhand der alten Geschwindigkeiten und der Massen der Kugel berechnet*/
		asteroidsSpeed[i].p[0] = newSpeedX1;
		asteroidsSpeed[j].p[0] = newSpeedX2;
		asteroidsSpeed[i].p[1] = newSpeedY1;
		asteroidsSpeed[j].p[1] = newSpeedY2;
		/*Neue Geschwindigkeiten setzten*/
		asteroids[i].p[0] += newSpeedX1;
		asteroids[j].p[0] += newSpeedX2;
		asteroids[i].p[1] += newSpeedY1;
		asteroids[j].p[1] += newSpeedY2;
		/*auseinander setzten um erneute Kollison zu vermeiden*/
	}
	double randomDoubleBetween(double XMin, double XMax){
		double X = XMin + rand() * (XMax - XMin) / RAND_MAX;
		return X;
		//Gibt ein double in der Range zwischen XMIn und XMAx aus
	}
	void collideRingFence(int i){
		bool tausch; //prüft ob der Asteroid auf die andere seite versetzt werden muss
		if((asteroids[i].p[0])>=14+(2*radius[i])){
			tausch = true;
		}else if((asteroids[i].p[0])<=-14-(2*radius[i])){
			tausch = true;
		}else if((asteroids[i].p[1])<=-9-(2*radius[i])){
			tausch = true;
		}else if((asteroids[i].p[1])>=9+(2*radius[i])){
			tausch = true;
		}else{
			tausch = false;
		}
		if(tausch==true){
			bool moeglich = true; //Prüft, ob die neue Position belegt ist
			for(unsigned j = 0;j<asteroids.size();j++){
				Vec3 temp(-0.9*asteroids[i].p[0],-0.9*asteroids[i].p[1],asteroids[i].p[2]);
				//Vector erstellt mit den potentiell neuen Koordinaten
				double distance = (temp-asteroids[j]).LengthXY();
				if (distance < 0) { distance = distance * -1; }
				if(distance < radius[i] + radius[j]){
					moeglich = false;
				}
			}
			if (moeglich) {
				asteroids[i].p[0] *= -0.9;
				asteroids[i].p[1] *= -0.9;
				//Wenn der Tausch möglich ist, also kein anderer Asteroid an der neuen Position ist, wird das Objekt verschoben
			}
		}
	}
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
		if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE){
			SetMaterialColor(0, 0, .1, .1);
			Vec3 projectil = Ufo1.GetSpitze();
			DrawSphere(projectil, .1);
			projectiles.push_back(projectil);
			projectil.p[0] = 0;
			projectil.p[1] = ufoSpeed.p[1]*speedProjectil;
			projectil.p[2] = 0;
			projectilesSpeed.push_back(projectil);
			//Erzeugen eines Projektils, mit der Position an der Spitze des Ufos und der Geschwindigkeit des Ufo mal eines Wertes
		}
		if (key == GLFW_KEY_RIGHT) {
			for(int i = 0;i<5;i++){
			Vec3 tipShuttle = Ufo1.GetSpitze();
			tipShuttle.p[0] = tipShuttle.p[0]+ufoSpeed.p[0];
			Ufo1.SetSpitze(tipShuttle, ufoSpeed);
			}
			//Bewegen des Ufos nach Rechts
		}
		if (key == GLFW_KEY_LEFT) {
			for(int i = 0;i<5;i++){
			Vec3 tipShuttle = Ufo1.GetSpitze();
			tipShuttle.p[0] = tipShuttle.p[0]-ufoSpeed.p[0];
			Ufo1.SetSpitze(tipShuttle, ufoSpeed);
			}
			//Bewegen des Ufos nach Links
		}
		if (key == GLFW_KEY_UP){
			for(int i = 0;i<5;i++){
			Vec3 tipShuttle = Ufo1.GetSpitze();
			tipShuttle.p[1] = tipShuttle.p[1]+ufoSpeed.p[1];
			Ufo1.SetSpitze(tipShuttle, ufoSpeed);
			}
			//Bewegen des Ufos nach Oben
		}
		if (key == GLFW_KEY_DOWN){
			for(int i = 0;i<5;i++){
			Vec3 tipShuttle = Ufo1.GetSpitze();
			tipShuttle.p[1] = tipShuttle.p[1]-ufoSpeed.p[0];
			Ufo1.SetSpitze(tipShuttle, ufoSpeed);
			}
			//Bewegen des Ufos nach Unten
		}

		//Testfunktionen
		if (key == GLFW_KEY_L) speed = 0; //Kugeln stoppen
		if (key == GLFW_KEY_K) speed = 3; //Kugeln wieder auf Anfangsgeschwindigkeit setzen
		if (key == GLFW_KEY_P) speed += 0.1; //Geschwindigkeit der Kugeln erhöhen
		if (key == GLFW_KEY_O) speed -= 0.1; //Geschwindigkeit der Kugeln senken
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
				//Asteroiden resetten
			}
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
			//zeichnet Flächen
	}
	void collideBalls(int i, int j){
		double distance = (asteroids[i]-asteroids[j]).LengthXY();
		if (distance < 0) { distance = distance * -1; }
		if(distance <= radius[i] + radius[j]){
			elasticCollision(i, j);
		}
		//Kollison prüfen
	}
	void insertNewBall(double ra, Vec3 m, Vec3 s, int j){
		//erzeugung eines Splitterasteroiden
		bool collide = false;
		//Abfrage ob an der Stelle, an der ein neuer Asteroid erzeugt werden soll, bereits einer vorhanden ist
		for(unsigned i = 0; i<asteroids.size();i++){
			double distance = (asteroids[i]-m).LengthXY();
			if (distance < 0) { distance = distance * -1; }
			if(distance < radius[i] + ra){
				if(i!=j){
					collide = true;
				}
			}
		}
		if(collide==false){
			radius.push_back(ra);
			asteroids.push_back(m);
			asteroidsSpeed.push_back(s);
			//Ein Asteroid kann nur erzeugt werden, wenn der Platz frei ist
		}
	}
	bool collideProjectile(int i, int j){
		//Überprüft, ob ein Asteroid abgeschossen wurde
		double distance = ((asteroids[i].p[0]-projectiles[j].p[0])*(asteroids[i].p[0]-projectiles[j].p[0]))
				+((asteroids[i].p[1]-projectiles[j].p[1])*(asteroids[i].p[1]-projectiles[j].p[1]));
		if (distance < 0) { distance = distance * -1; }
		if(distance < radius[i] + 0.1){
			projectiles.erase(projectiles.begin()+j);
			projectilesSpeed.erase(projectilesSpeed.begin()+j);
			if(radius[i]>=0.8){
				//Wenn großer Asteroid: vier neue Splitter erzeugen
				insertNewBall(.6, Vec3(1,1,0)+asteroids[i],Vec3(0.001,0.001,0), i);
				insertNewBall(.6, Vec3(-1,1,0)+asteroids[i],Vec3(-0.001,0.001,0), i);
				insertNewBall(.6, Vec3(1,-1,0)+asteroids[i],Vec3(0.001,-0.001,0), i);
				insertNewBall(.6, Vec3(-1,-1,0)+asteroids[i],Vec3(-0.001,-0.001,0), i);
			}
			asteroids.erase(asteroids.begin()+i);
			asteroidsSpeed.erase(asteroidsSpeed.begin()+i);
			radius.erase(radius.begin()+i);
			totalScore+=1;
			return true;
		}
		return false;
	}
	void newLocation(int i){
		asteroids[i].p[0] += asteroidsSpeed[i].p[0]*speed;
		asteroids[i].p[1] += asteroidsSpeed[i].p[1]*speed;
		//Bewegen des jeweiligen Asteroiden
	}
	void move(){
		for(unsigned i = 0; i<asteroids.size();i++){
			newLocation(i);
			//Bewegen der Asteroid
			bool deleted; //Prüft, ob der Asteroid abgeschossen wurde
			for(unsigned t = 0; t<projectiles.size(); t++){
				if(collideProjectile(i, t)){
					deleted = true;
					//wenn er abgeschossen wurde deleted == true
					break;
				}
			}
			for(unsigned j = 0; j<asteroids.size();j++){
				if(deleted==true){
					break;
					//wenn der Asteroid bereits abgeschossen ist, wird die Überprüfung, ob er mit anderen Asteroiden kollidiert abgebrochen, da er nicht mehr existiert
				}
				if(i!=j){
					collideBalls(i,j);
					//Wenn der zu prüfende Asteroid nicht er selbst ist: Prüfung nach Kollsion
				}
			}
			collideRingFence(i);
			//Prüfungen, ob der Asteroid das Spielfeld verlassen hat
		}
		for(unsigned i = 0; i<projectiles.size();i++){
			projectiles[i].p[0] +=projectilesSpeed[i].p[0];
			projectiles[i].p[1] +=projectilesSpeed[i].p[1];
			//Bewegung des Projektils
			if(projectiles[i].p[0]<-14-.1||projectiles[i].p[0]>14+.1||projectiles[i].p[1]<-9-.1||projectiles[i].p[1]>9+.1){
				projectiles.erase(projectiles.begin()+i-1);
				projectilesSpeed.erase(projectilesSpeed.begin()+i-1);
				//Hat das Projektil das Spielfeld verlassen, wird es gelöscht
			}
		}
		for(unsigned i = 0;i<asteroids.size();i++){
			if(Ufo1.collision(asteroids[i], radius[i])==true){
				asteroids.clear();
				radius.clear();
				asteroidsSpeed.clear();
				rou = -10;
				//wenn das Ufo mit einem der Asteroiden kollidiert ist, wird die Runde auf -10 gesetzt und das Spiel ist somit vorbei, da keine neuen Asteroiden erzeugt werden können
			}
		}
	}
	void drawScore(const char *score, int length, int x, int y){
		glMatrixMode(GL_PROJECTION);
		double *matrix = new double[16];
		glGetDoublev(GL_PROJECTION_MATRIX, matrix);
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -5, 5);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glLoadIdentity();
		glRasterPos2f(x, y);
		for(int i=0; i<length; i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)score[i]);
		}
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(matrix);
		glMatrixMode(GL_MODELVIEW);

	}
	void draw(){
		for(unsigned i = 0; i<asteroids.size();i++){
			SetMaterialColor(3, 1, 1, 1);
			DrawSphere(asteroids[i], radius[i]);
			//zeichnen der Asteroiden
		}
		if(projectiles.size()>10){
			while(projectiles.size()>10){
				projectiles.erase(projectiles.begin());
				projectilesSpeed.erase(projectilesSpeed.begin());
			}
			//Es können maximal 10 Geschosse auf dem Spielfeld sein
		}
		for(unsigned i = 0; i<projectiles.size();i++){
			SetMaterialColor(3, .99, .1, .1);
			DrawSphere(projectiles[i], .1);
			//zeichnet alle Projektile
		}
	}
	void insert(){
		asteroids.clear();
		radius.clear();
		asteroidsSpeed.clear();
		for(int i = 0; i<10+rou;i++){
			double x = randomDoubleBetween(-14, 14);
			double y = randomDoubleBetween(-9, 9);
			double r = randomDoubleBetween(0.8,1.2);
		 	Vec3 m(x,y,10);
		 	radius.push_back(r);
		 	asteroids.push_back(m);
		 	asteroidsSpeed.push_back(Vec3(randomDoubleBetween(-0.005, 0.005),randomDoubleBetween(-0.005, 0.005),0));
		 	//Erzeugen der Asterioden mit random Geschwindigkeiten, Größen und Mittelpunkten
		}
	}
	void Preview() {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
			SetMaterialColor(3, 0.9, 0.1, 0);
			if(rou!=-10){
				Ufo1.DrawUfo();
			}
			//Ufo gezeichnet. Wenn das Spiel vorbei ist (rou = -10), wird das Ufo nicht mehr gezeichnet
			if(asteroids.size()<1){
				insert();
				if(round<0){
					rou +=1;
				}
				//neue Runde wird generiert
			}
			glTranslated(0,0,-10);
			glRotated(alpha_1, 1, 0, 0);
			glRotated(alpha_2, 0, 1, 0);
			glPushMatrix();
			//Spielfeld
			SetMaterialColor(0, 0, 0, .15);
			drawSquare(Vec3(-14,-9,0), Vec3(14,-9,0), Vec3(14,9,0), Vec3(-14,9,0));

			SetMaterialColor(0, 0, 0, .1);
			drawSquare(Vec3(-14,-9,5), Vec3(14,-9,5), Vec3(14,-12,5), Vec3(-14,-12,5));

			SetMaterialColor(0, 0, 0, .15);
			drawSquare(Vec3(-14,-12,5), Vec3(-14,12,5), Vec3(-16,12,5), Vec3(-16,-12,5));

			SetMaterialColor(0, 0, 0, .15);
			drawSquare(Vec3(14,-12,5), Vec3(16,-12,5), Vec3(16,12,5), Vec3(14,12,5));

			SetMaterialColor(0, 0, 0, .15);
			drawSquare(Vec3(-14,9,5), Vec3(14,9,5), Vec3(14,12,5), Vec3(-14,12,5));

			glTranslated(0, 0, -10);
			draw();
			//Zeichnen der Objekte
			move();
			//Bewegen der Objekte
			glPopMatrix();
	}
	std::string int2str(int x) {
		    // converts int to string
		    std::stringstream ss;
		    ss << x;
		    return ss.str( );
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
			glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
			//score ausgeben in main
			std::string score = "Score: " + int2str(totalScore);
			drawScore(score.data(), score.size(), 10, 10);
			// make it appear (before this, it's hidden in the rear buffer)
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glPopMatrix();
		glfwTerminate();
		printf("Goodbye!\n");
		return 0;
	}
