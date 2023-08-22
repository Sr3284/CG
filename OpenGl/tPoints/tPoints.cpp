#include <GL/glut.h>

void
display(){
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POINTS);
		glColor3f(1, 0, 0);	glVertex3f(-0.6, -0.75, 0);
		glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
		glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
	glEnd();

	glFlush();
}

int
main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(80, 80);
	glutInitWindowSize(400, 300);
	glutCreateWindow("Three Points");

	glutDisplayFunc(display);

	glutMainLoop();
}