#include "Base.h"
#include "QuadTree.h"
#include "Shapes.h"
#include "GeoContext.hpp"
#include "GL\freeglut.h"
#include "GL\gl.h"


using namespace GeoDraw;

void render();
void Renderline(const Line &line);

Geocontext context;

int main(int argc, char **argv) {
	context.LineDraw = Renderline;
	context.AddLine(vector2(500, 500), vector2(-500, -500));
	glutInit(&argc, argv);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Geometry Drawing");
	glutDisplayFunc(render);
	glutMainLoop();
}

void Renderline(const Line &line) {
	glLineWidth(line.getthickness());
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(line.getstart().x / 1000, line.getstart().y / 1000, 0.0);
	glVertex3f(line.getend().x / 1000, line.getend().y / 1000, 0.0);
    glEnd();
}

void render() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	context.Draw();
	glEnd();
	glFlush();
}