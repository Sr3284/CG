#include <GL/glut.h>
#include <cmath>

static int year = 0,
			  day = 0;

static GLfloat u = 0.0,
					du = 0.1;

void wireSphere(GLfloat radius, int slices, int stacks){
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutWireSphere(radius, slices, stacks);
	glPopMatrix();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	glColor3f(1.0, 1.0, 0.0);
	wireSphere(1.0, 15, 15);

	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	wireSphere(0.2, 15, 15);
	glColor3f(1, 1, 1);
	glBegin(GL_LINE);
		glVertex3f(0, -0.3, 0);
		glVertex3f(0, 0.3, 0);
	glEnd();

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void timer(int v){
	u += du;
	day = (day + 1) % 360;
	year = (year + 2) % 360;
	glLoadIdentity();
	gluLookAt(20 * cos(u / 8.0) + 12,
				 5 * sin(u / 8.0) + 1,
				 10 * cos(u / 8.0) + 2,
				 0, 0, 0, 0, 1, 0);
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, v);
}

void reshape(GLint w, GLint h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 40.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Cometa");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer, 0);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}