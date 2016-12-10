
//////////////////////////////////////////////////////// 
// 
// This should be your first OpenGL/Glut program 
// do nothing but draw a yellow rectangular polygon at the 
// center of the window 
// 
//////////////////////////////////////////////////////// 


#include "header.h"

Button editflag = { 5,5, 100,25, 0,0,"Edit" }; //, deleteflag, duplicateflag, rationalBezier, resetflag, redraw;
Button deleteflag = { 115,5, 100,25, 0,0,"Delete" };
Button duplicateflag = { 225,5, 100,25, 0,0,"Duplicate" };
Button rationalBezier = { 335,5, 100,25, 0,0,"Rational" };
Button resetflag = { 445,5, 100,25, 0,0,"Reset" };
Button redraw = { 555,5, 100,25, 0,0,"Redraw" };
void display()
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, winw, winh, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	*	Draw the 2D overlay
	*/
	ButtonDraw(&editflag);
	ButtonDraw(&deleteflag);
	ButtonDraw(&duplicateflag);
	ButtonDraw(&resetflag);
	ButtonDraw(&redraw);
	ButtonDraw(&rationalBezier);
	/*
	*	Bring the back buffer to the front and vice-versa.
	*/
	glutSwapBuffers();
	//glClear(GL_COLOR_BUFFER_BIT);
	//glColor4f(1, 1, 0, 1);
	
	//glBegin(GL_POLYGON);
	

	//glVertex2f(-0.5, -0.5);
	//glVertex2f(-0.5, 0.5);
	//glVertex2f(0.5, 0.5);
	//glVertex2f(0.5, -0.5);
	//glEnd();
	//glFlush();

}
/*
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(winw, winh);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("03 - Mouse Motion");

	glutDisplayFunc(display);
	glutReshapeFunc(Resize);
	//glutMouseFunc(MouseButton);
	//glutMotionFunc(MouseMotion);
	//glutPassiveMotionFunc(MousePassiveMotion);

	Init();

	glutMainLoop();
	//glutInit(&argc, argv);
	//glutCreateWindow("Curves");
	//glutDisplayFunc(display);
	//glutMainLoop();
}
*/