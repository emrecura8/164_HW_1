/*********
   CTIS164 - Template Source Program
----------
STUDENT :Emre Cura
SECTION :02
HOMEWORK: Television Project
----------
PROBLEMS:
----------
ADDITIONAL FEATURES: there is a fish. I put an faster button. It will react like 2-3 seconds later after you pressed. It is not going to affect other features.
If you click more, they will be more faster. There is no button for slow down.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

//Working modes
#define OFF 0
#define ON 1
#define PAUSE 2
#define FAST 3

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int xB = 0, yB = 0, xA=0;
bool activeTimer = true; // Start timer to move shape
 // ACTION: Start Moving, STOP: stop
int count;

/* GLOBAL VARIABLES and CONSTANTS */

int appState = OFF;
bool onoffButton = false;
bool pauserestartButton = false;
bool fastButton = false;

// this is function signature (prototype)
// this is called "function declaration"
void onTimer(int v);

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void display_background() {
    // television
    glColor3ub(0, 0, 255);
    glRectf(-450, -280, 450, 280);

    glColor3ub(95, 95, 92);
    glRectf(-450, -280, 450, -200);

    //borders of television
    glLineWidth(3);
    glColor3ub(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-450, -280);
    glVertex2f(450, -280);
    glVertex2f(450, 280);
    glVertex2f(-450, 280);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-450, -200);
    glVertex2f(450, -200);
    glEnd();


    //feets of television
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(0, -280);
    glVertex2f(-150, -300);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(0, -280);
    glVertex2f(150, -300);
    glEnd();

    //pause/restart button
    glColor3f(0, 0, 0);
    circle(130, -235, 25);
    glColor3ub(223, 32, 32);

    glColor3ub(255, 255, 255);
    glRectf(117, -250, 125, -220);

    glColor3ub(255, 255, 255);
    glRectf(135, -250, 143, -220);


    glColor3f(90, 100, 140);
    vprint2(60, -275, 0.15, "PAUSE/RESTART");

    //on/off button
    glColor3f(0, 0, 0);
    circle(-130, -235, 25);
    glColor3ub(223, 32, 32);
    glBegin(GL_LINES);
    glVertex2f(-130, -220);
    glVertex2f(-130, -250);
    glEnd();

    glColor3f(90, 100, 140);
    vprint2(-160, -275, 0.15, "ON/OFF");

    //fast button
    glColor3f(255, 235, 0);
    circle(0, -235, 25);
    glColor3ub(223, 32, 32);

    glColor3f(90, 100, 140);
    vprint2(-35, -275, 0.15, "FASTER");
}

void display_curtains() {
    //curtains are going to cover submarine
    glColor3ub(255, 255, 255);
    glRectf(-600, -350, -452, 350);

    glColor3ub(255, 255, 255);
    glRectf(600, -350, 452, 350);

    glColor3ub(255, 255, 255);
    glRectf(-1200, -350, -600, 350);

    glColor3ub(255, 255, 255);
    glRectf(600, -350, 1200, 350);
}

void display_submarine() {

    //main body
    glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_POLYGON);
    glVertex2f(-360+xB, 40);
    glVertex2f(-320 + xB, 100);
    glVertex2f(-140 + xB, 100);
    glVertex2f(-100 + xB, 40);
    glVertex2f(-100 + xB, -40);
    glVertex2f(-140 + xB, -100);
    glVertex2f(-320 + xB, -100);
    glVertex2f(-360 + xB, -40);
    glEnd();

    glColor3ub(35, 172, 196);
    glRectf(-160 + xB, 0, -100 + xB, 60);

    //frontside
    glColor3f(0.1, 0.1, 0.1);
    circle(-150 + xB, 0, 100);
    glColor3ub(223, 32, 32);

    //back side
    glColor3f(0.1, 0.1, 0.1);
    circle(-310 + xB, 0, 100);
    glColor3ub(223, 32, 32);

    //propellers
    glColor3ub(95, 95, 92);
    glRectf(-430 + xB, -20, -410 + xB, 20);

    glColor3ub(95, 95, 92);
    glBegin(GL_TRIANGLES);
    glVertex2f(-430 + xB, 20);
    glVertex2f(-410 + xB, 20);
    glVertex2f(-420 + xB, 60);

    glColor3ub(95, 95, 92);
    glBegin(GL_TRIANGLES);
    glVertex2f(-430 + xB, -20);
    glVertex2f(-410 + xB, -20);
    glVertex2f(-420 + xB, -60);
    glEnd();

    //frontside window
    glColor3ub(35, 172, 196);
    glRectf(-200 + xB, 0, -90 + xB, 76);

    //other windows
    glColor3ub(35, 172, 196);
    circle(-250 + xB, 40, 40);

    glColor3ub(35, 172, 196);
    circle(-340 + xB, 40, 40);

    //entrance
    glColor3f(0.1, 0.1, 0.1);
    glRectf(-240 + xB, 100, -160 + xB, 120);

    glColor3ub(95,95,92);
    vprint2(-240 + xB, -85, 0.15, "USS SEAWOLF");

    //flag
    glColor3ub(191, 10, 48);
    glRectf(-340 + xB, -90, -310 + xB, -70);

    glColor3ub(0, 40, 104);
    glRectf(-340 + xB, -80, -325 + xB, -70);

    glColor3ub(255, 255, 255);
    glRectf(-340 + xB, -87, -310 + xB, -84);

    glColor3ub(255, 255, 255);
    glRectf(-340 + xB, -81, -310 + xB, -78);

    glColor3ub(255, 255, 255);
    glRectf(-325 + xB, -75, -310 + xB, -72);

    glColor3ub(255, 255, 255);
    glPointSize(1.5);
    glBegin(GL_POINTS);
    glVertex2f(-328 + xB, -72);
    glVertex2f(-332 + xB, -74);
    glVertex2f(-330 + xB, -76);
    glVertex2f(-336 + xB, -73);
    glVertex2f(-334 + xB, -75);
    glEnd();
}

void display_fish() {
    glColor3ub(217, 233, 20);
    glRectf(0 - xB, 210, 30 - xB, 250);

    //eye
    glColor3ub(0, 0, 0);
    circle(5 - xB, 240, 5);
    //tail
    glColor3ub(233, 158, 20);
    glBegin(GL_TRIANGLES);
    glVertex2f(50 - xB, 210);
    glVertex2f(50 - xB, 250);
    glVertex2f(30 - xB, 230);
    glEnd();
}

void display_on() {
    glColor3f(0, 0, 0);
    circle(-130, -235, 25);
    glColor3ub(223, 32, 32);
    glBegin(GL_LINES);
    glVertex2f(-130, -220);
    glVertex2f(-130, -250);
    glEnd();

    glColor3f(90, 100, 140);
    vprint2(-160, -275, 0.15, "ON/OFF");

}
void display_off() {
    glColor3ub(152, 52, 69);
    glRectf(-450, -200, 450, 280);

}

void display_pause() {
    glColor3f(0, 0, 0);
    circle(130, -235, 25);

    glColor3ub(255, 255, 255);
    glBegin(GL_TRIANGLES);
    glVertex2f(117, -220);
    glVertex2f(117, -250);
    glVertex2f(143, -235);
    glEnd();


}



//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    // common background graphics for all states.
    display_background();
    display_submarine();
    display_fish();
    display_curtains();

    switch (appState) {
    case ON: display_on(); break;
    case OFF: display_off(); break;
    case PAUSE: display_pause(); break;
    case FAST: break;

    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }
    
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        if (appState == ON && onoffButton == true) {
            appState = OFF;
            activeTimer = false;
        }
        else if (appState == OFF && onoffButton == true) {
            appState = ON;
            activeTimer = true;
        }
        else if (appState == ON && pauserestartButton == true) {
            appState = PAUSE;
            activeTimer = false;
        }
        else if ((appState == ON || appState == PAUSE) && pauserestartButton == true) {
            appState = ON;
            activeTimer = true;
        }
        if (appState == ON && fastButton == true) {
            glutTimerFunc(1000, onTimer, 0);
        }
    }



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
bool checkCircle(float px, float py, float cx, float cy, float R) {
    float dx = px - cx;
    float dy = py - cy;
    float d = sqrt(dx * dx + dy * dy);
    return d <= R;
}

void onMove(int x, int y) {
    // Write your codes here.
    int mx = x - winWidth / 2;
    int my = winHeight / 2 - y;

    onoffButton = checkCircle(mx, my, -130, -235, 25);
    pauserestartButton = checkCircle(mx, my, 130, -235, 25);
    fastButton = checkCircle(mx, my, 0, -235, 25);
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (activeTimer == true) {
        if (xB <= 1600) {
            xB ++ ;
        }
        if (xB == 920)
            xB = -450;
    }
    if (activeTimer == false) {
        xB += 0;
    }
    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("TV Project - Emre Cura");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}