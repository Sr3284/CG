#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <GL/glut.h>

#define mapWidth 8
#define mapHeight 8
#define mapSize 64

typedef struct{
	int w, a, s, d;
} ButtonKeys;

float px, py, pdx, pdy, pa,
	  frame1, frame2, fps;
ButtonKeys keys;

int map[] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1
};

float degToRad(float a) {
	return a * M_PI / 180.0;
}

float FixAng(float a) {
	if(a > 360)
		a -= 360;

	if(a < 0)
		a += 360;

	return a;
}

float distance(float ax, float ay, float bx, float by, float ang) {
	return cos(degToRad(ang)) * (bx - ax) - sin(degToRad(ang)) * (by - ay);
}

void drawPlayer2d() {
	glColor3f(1, 1, 0);
	glPointSize(8);
	glLineWidth(4);

	glBegin(GL_POINTS);
		glVertex2i(px, py);
	glEnd();

	glBegin(GL_LINES);
		glVertex2i(px, py);
		glVertex2i(px + pdx * 20, py + pdy * 20);
	glEnd();
}

void drawMap2d() {

	int x, y,
		xo, yo;

	for(y = 0; y < mapHeight; y++){
		for(x = 0; x < mapWidth; x++){
			(map[y * mapWidth + x] == 1)? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
			xo = x * mapSize;
			yo = y * mapSize;

			glBegin(GL_QUADS);
				glVertex2i(0 + xo + 1, 0 + yo + 1);
				glVertex2i(0 + xo + 1, mapSize + yo - 1);
				glVertex2i(mapSize + xo - 1, mapSize + yo -1);
				glVertex2i(mapSize + xo - 1, 0 + yo + 1);
			glEnd();
		}
	}
}

void drawRays2d() {
	glColor3f(0, 1, 1);
	glBegin(GL_QUADS);
		glVertex2i(526, 0); glVertex2i(1006, 0);
		glVertex2i(1006, 160); glVertex2i(526, 160);
	glEnd();

	glColor3f(0.7, 0.7, 0.7);
	glBegin(GL_QUADS);
		glVertex2i(526, 160); glVertex2i(1006, 160);
		glVertex2i(1006, 320); glVertex2i(526, 320);
	glEnd();

	int r, mx, my, mp, dof, side;
	float vx, vy, rx, ry, ra, xo, yo, disV, disH;

	ra = FixAng(pa + 30);

	for(r = 0; r < 60; r++){
		// vertical intersec
		dof = 0;
		side = 0;
		disV = 100000;
		float Tan = tan(degToRad(ra));

		if(cos(degToRad(ra)) > 0.001){	// left
			rx =(((int)px >> 6) << 6) + 64;
			ry = (px - rx) * Tan + py;
			xo = 64;
			yo = -xo * Tan;
		} else if(cos(degToRad(ra)) < -0.001){	// right
			rx = (((int)px >> 6) << 6) -0.0001;
			ry = (px - rx) * Tan + py;
			xo = -64;
			yo = -xo * Tan;
		} else {	// up or down
			rx = px;
			ry = py;
			dof = 10;
		}

		while(dof < 10){
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapWidth + mx;

			if(mp > 0 && mp < mapWidth * mapHeight && map[mp] == 1){
				// hit
				dof = 10;
				disV=cos(degToRad(ra)) * (rx - px) - sin(degToRad(ra)) * (ry - py);
			} else {
				// check next
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		vx = rx;
		vy = ry;

		// horizontal intersec
		dof = 0;
		disH = 100000;
		Tan = 1.0 / Tan;

		if(sin(degToRad(ra)) > 0.001){	// up
			ry = (((int)py >> 6) << 6) - 0.0001;
			rx = (py - ry) * Tan + px;
			yo =-64;
			xo =-yo * Tan;
		} else if(sin(degToRad(ra)) < -0.001){	// down
			ry = (((int)py >> 6) << 6) + 64;
			rx = (py - ry) * Tan + px;
			yo = 64;
			xo = -yo * Tan;
		} else {	// left or right
			rx = px;
			ry = py;
			dof = 10;
		}

		while(dof < 10){
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapWidth + mx;

			if(mp > 0 && mp < mapWidth * mapHeight && map[mp] == 1){
				// hit
				dof = 10;
				disH = cos(degToRad(ra)) * (rx - px) - sin(degToRad(ra)) * (ry - py);
			} else {
				// check next
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}

		glColor3f(0.8, 0, 0);
		if(disV < disH){
			rx = vx;
			ry = vy;
			disH = disV;
			glColor3f(0.6, 0, 0);
		}
		glLineWidth(2);
		glBegin(GL_LINES);
			glVertex2i(px, py);
			glVertex2i(rx, ry);
		glEnd();

		// arruma o "olho de peixe"
		int ca = FixAng(pa - ra);
		disH *= cos(degToRad(ca));

		int lineH = (mapSize * 320) / (disH);
		if(lineH > 320)
			lineH = 320;
		int lineOff = 160 - (lineH >> 1);

		glLineWidth(8);
		glBegin(GL_LINES);
			glVertex2i(r * 8 + 530, lineOff);
			glVertex2i(r * 8 + 530, lineOff + lineH);
		glEnd();

		ra = FixAng(ra - 1);	// next ray
	}
}

void ButtonDown(unsigned char key, int x, int y){
	if(key == 'w')
		keys.w = 1;
	if(key == 'a')
		keys.a = 1;
	if(key == 's')
		keys.s = 1;
	if(key == 'd')
		keys.d = 1;
}

void ButtonUp(unsigned char key, int x, int y){
	if(key == 'w')
		keys.w = 0;
	if(key == 'a')
		keys.a = 0;
	if(key == 's')
		keys.s = 0;
	if(key == 'd')
		keys.d = 0;
}

void init(){
	glClearColor(0.3, 0.3, 0.3, 0);
	gluOrtho2D(0, 1024, 510, 0);
	px = 150; py = 400; pa = 90;
	pdx= cos(degToRad(pa));
	pdy= -sin(degToRad(pa));
}

void display(){
	//fps
	frame2 = glutGet(GLUT_ELAPSED_TIME);
	fps = (frame2 - frame1);
	frame1 = glutGet(GLUT_ELAPSED_TIME);

	// controls
	int xo = 0,
		yo = 0,
		ipx, ipxAdd, ipxSub,
		ipy, ipyAdd, ipySub;

	(pdx < 0)? xo = -20 : xo = 20;
	(pdy < 0)? yo = -20 : yo = 20;

	ipx = px / 64.0;
	ipxAdd = (px + xo) / 64.0;
	ipxSub = (px - xo) / 64.0;

	ipy = py / 64.0;
	ipyAdd = (py + yo) / 64.0;
	ipySub = (py - yo) / 64.0;

	if(keys.w == 1){
		if(map[ipy * mapWidth + ipxAdd] == 0){
			px += pdx * 0.2 * fps;
		}
		if(map[ipyAdd * mapWidth + ipx] == 0){
			py += pdy * 0.2 * fps;
		}
	}
	if(keys.a == 1){
		pa += 0.2 * fps;
		pa = FixAng(pa);
		pdx = cos(degToRad(pa));
		pdy = -sin(degToRad(pa));
	}
	if(keys.s == 1){
		if(map[ipy * mapWidth + ipxSub] == 0){
			px -= pdx * 0.2 * fps;
		}
		if(map[ipySub * mapWidth + ipx] == 0){
			py -= pdy * 0.2 * fps;
		}
	}
	if(keys.d == 1){
		pa -= 0.2 * fps;
		pa = FixAng(pa);
		pdx = cos(degToRad(pa));
		pdy = -sin(degToRad(pa));
	}
	glutPostRedisplay();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap2d();
	drawPlayer2d();
	drawRays2d();
	glutSwapBuffers();
}

void resize(int w, int h){
	glutReshapeWindow(800, 600);
}

int main(int argc, char* argv[]){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(80, 80);
	glutCreateWindow("Raycasting");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(ButtonDown);
	glutKeyboardUpFunc(ButtonUp);
	glutMainLoop();
}