#include <GL/glut.h> 
#include <GL/gl.h> 
#include <iostream>
#include <string.h>
using namespace std;
#include <stdlib.h>
#include <stdio.h>

extern int winw;
extern int winh;


struct Button
{
	int   x;							/* top left x coord of the button */
	int   y;							/* top left y coord of the button */
	int   w;							/* the width of the button */
	int   h;							/* the height of the button */
	int	  state;						/* the state, 1 if pressed, 0 otherwise */
	int	  highlighted;					/* is the mouse cursor over the control? */
	char* label;						/* the text label of the button */
										//ButtonCallback callbackFunction;	/* A pointer to a function to call if the button is pressed */
};

typedef struct Button Button;

struct Mouse
{
	int x;		/*	the x coordinate of the mouse cursor	*/
	int y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/
	int mmb;	/*	is the middle button pressed?	*/
	int rmb;	/*	is the right button pressed?	*/

				/*
				*	These two variables are a bit odd. Basically I have added these to help replicate
				*	the way that most user interface systems work. When a button press occurs, if no
				*	other button is held down then the co-ordinates of where that click occured are stored.
				*	If other buttons are pressed when another button is pressed it will not update these
				*	values.
				*
				*	This allows us to "Set the Focus" to a specific portion of the screen. For example,
				*	in maya, clicking the Alt+LMB in a view allows you to move the mouse about and alter
				*	just that view. Essentually that viewport takes control of the mouse, therefore it is
				*	useful to know where the first click occured....
				*/
	int xpress; /*	stores the x-coord of when the first button press occurred	*/
	int ypress; /*	stores the y-coord of when the first button press occurred	*/
};

/*
*	rename the structure from "struct Mouse" to just "Mouse"
*/
typedef struct Mouse Mouse;
void Font(void *, char *, int, int);
int ButtonClickTest(Button *, int, int);
void ButtonRelease(Button *, int, int);
void ButtonPress(Button *, int, int);
void ButtonPassive(Button *, int, int);
void ButtonDraw(Button *);
void Init();
void Draw3D();
void Draw2D();
void Draw();
void Resize(int, int);


