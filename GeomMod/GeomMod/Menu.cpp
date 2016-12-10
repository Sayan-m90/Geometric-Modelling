#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <pu.h>


/* called when open menu item selected */
void Open_CB(puObject *pWidget)
{
	printf("Open Menu item selected\n");
}

/* called when save menu item selected */
void Save_CB(puObject *pWidget)
{
	printf("Save Menu item selected\n");
}

/* called when quit menu item selected */
void Quit_CB(puObject *pWidget)
{
	exit(0);
}

/* the text items that will appear on the menu */
char *MenuItems[] =
{
	"Quit",
	"----",
	"Save",
	"Open",
	NULL		/* last item must be NULL */
};

/* The functions that will be called if one of the menu items above is selected */
puCallback MenuCallbacks[] =
{
	Quit_CB,
	NULL,		/* note NULL can be passed in to prevent the item calling a function */
	Save_CB,
	Open_CB,
	NULL		/* last item must be NULL */
};

void InitUI()
{
	puSetDefaultStyle(PUSTYLE_SMALL_SHADED);
	puSetDefaultColourScheme(0.5f, 0.7f, 0.9f);

	/* create a new menu */
	puMenuBar *Menu = new puMenuBar(-1);

	/* add the first menu "File" */
	Menu->add_submenu("File", MenuItems, MenuCallbacks); /* Add "file" */

														 /* This MUST be called after adding your menus */
	Menu->close();
}

/* custom OpenGL Initialisation */
void Init()
{
	glClearColor(0.5, 0.5, 0.5, 0.5);
}

/* The display function - called when redraw is needed */
void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	/* display the user interface */
	puDisplay();
	glutSwapBuffers();
}

/* called when a mouse button is pressed or released */
void MouseButton(int button, int state, int x, int y)
{
	/* see if the mouse clicked on any of the user interface controls */
	puMouse(button, state, x, y);
	glutPostRedisplay();
}

/* called when the mouse moves */
void MouseMove(int x, int y)
{
	/* Pass information about the position of the mouse to pui */
	puMouse(x, y);
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("pui - example3");

	glutDisplayFunc(Display);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMove);

	puInit();
	Init();
	InitUI();

	glutMainLoop();

}
