#include "Base.h"
#include "QuadTree.h"
#include "Shapes.h"
#include "GeoContext.hpp"
#include "GL\freeglut.h"
#include "GL\gl.h"


using namespace GeoDraw;

void render();
void Renderline(const Line &line);
void RenderAABB(const AABB &aabb);

Geocontext context;

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Geometry Drawing");
	context.LineDraw = Renderline;
	context.AddLine(vector2(100, 100), vector2(500, 500));
	context.AddLine(vector2(600, 600), vector2(800, 800));
	context.AddLine(vector2(700, 700), vector2(900, 600));
	context.AddLine(vector2(400, 400), vector2(300, 700));
	context.AddLine(vector2(-100, -100), vector2(-500, -500));
	context.AddLine(vector2(-100, 100), vector2(-500, 500));
	context.AddLine(vector2(100, -100), vector2(500, -500));

	context.SetAABBDrawFunc(RenderAABB);
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

void RenderAABB(const AABB &aabb) {
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	float left = aabb.getleft() / 1000;
	float top = aabb.gettop() / 1000;
	float width = aabb.getwidth() / 1000;
	float height = aabb.getheight() / 1000;
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(left,top,0.0);
	glVertex3f(left + width, top, 0.0);
	glVertex3f(left + width, top - height, 0.0);
	glVertex3f(left, top - height, 0.0);
	glEnd();
}

void render() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	context.Draw();
	glEnd();
	glFlush();
}