#include <GL/glut.h>

static bool spin = true;
static const int FPS = 60;
static GLfloat currAngle = 0.0;

void reshape(GLint w, GLint h){
	glViewport(0, 0, w, h);
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(w <= h){
		glOrtho(-50.0, 50.0, -50.0/aspect, 50.0/aspect, -1.0, 1.0);
	} else {
		glOrtho(-50.0*aspect, 50.0*aspect, -50.0, 50.0, -1.0, 1.0);
	}
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(currAngle, 0.0, 0.0, 1.0);
	glRectf(-25.0, -25.0, 25.0, 25.0);
	glFlush();
	glutSwapBuffers();
}

void timer(int v){
	if(spin){
		currAngle += 1.0;
		if(currAngle > 360.0)
			currAngle -= 360.0;
		glutPostRedisplay();
	}
	glutTimerFunc(1000/FPS, timer, v);
}

void mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		spin = true;
	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		spin = false;
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Cubo Girando");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(100, timer, 0);
	glutMouseFunc(mouse);
	glutMainLoop();
}