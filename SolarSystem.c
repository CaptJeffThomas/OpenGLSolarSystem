/**
  * Jeff Thomas 
  * Lonely Space
 */
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "math3d.h"


/*
 * Definitions
 */

/*
 * "Constants"
 */
const GLfloat SMALL_ANGLE_INCR = 0.5f;
const GLfloat BIG_ANGLE_INCR = 1.0f;
const int TIME_INTERVAL = 120;
const GLfloat SOLAR_SYSTEM_SIZE = 10000.0f;
const int WINDOW_SIZE = 800;
const GLfloat INITIAL_CAMERA_POSITION = 1500.0f;
const int RADIUS_SCALE = 3;
const int SEPARATION_SCALE = 1;
const int AU = 700;

/*
 * New types
 */

/*
 * Color
 */
typedef struct
{
	GLfloat r, g, b;
} Color;

/*
 * Star
 */
typedef struct
{
	M3DVector3f position;
	Color color;
	GLfloat radius;
	GLfloat angle;
} Star;

/*
 * Planet
 */
typedef struct
{
	M3DVector3f position;
	Color color;
	GLfloat radius;
	GLfloat speed;
	GLfloat angle;
} Planet;

typedef Planet Satellite;

/*
 * Global variables
 */
/* Lighting values */
GLfloat whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat	lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

/* Celestial bodies */
Star theSun;
Planet theEarth, mercury, venus, mars, jupiter, saturn, uranus, neptune;
Satellite theMoon, io, europa, ganymede, callisto, titan;

/* Visualization parameters */
int solidMode;
M3DVector3f cameraPosition;
M3DVector3f cameraTarget;
M3DVector3f cameraDirection;
GLfloat cameraAngle;
int paused;

/*
 * Protoypes
 */
void RenderScene();
void SetupRC();
void ChangeSize(GLsizei w, GLsizei h);
void KeyPressedStd(unsigned char key, int x, int y);;
void drawText(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b);

void doPaint();
void initialize();
void TimerFunc(int value);

/*
 * Function definitions
 */

/*
 * Initialize the model.
 */
void initialize()
{

	float separation;
	paused = 0;

	/* Celestial bodies */

        separation = AU * SEPARATION_SCALE;
        theEarth.color.r = 0.6f;
	theEarth.color.g = 0.8f;
	theEarth.color.b = 1.0f;
	theEarth.position[0] = separation;
	theEarth.position[1] =
	theEarth.position[2] = 0.0f;
	theEarth.radius = RADIUS_SCALE;
	theEarth.angle = 0.0f; 
		

	theSun.color.r = 1.0f;
	theSun.color.g = 1.0f;
	theSun.color.b = 0.8f;
	theSun.position[0] =
	theSun.position[1] =
	theSun.position[2] = 0.0f;
	theSun.radius = 100.0f * theEarth.radius;
	theSun.angle = 1.0f;

	theMoon.color.r = 0.4f;
	theMoon.color.g = 0.4f;
	theMoon.color.b = 0.6f;
	//satellites are separated from their respective host, rather than 0,0,0
	theMoon.position[0] = theEarth.position[0] +  0.00257 * separation;
	theMoon.position[1] =
	theMoon.position[2] = 0.0f;
	theMoon.radius = 0.273f * theEarth.radius;
	theMoon.angle = 0.0f;

	mercury.color.r = 0.5f;
	mercury.color.g = 0.5f;
	mercury.color.b = 0.6f;
	mercury.position[0] = 0.467 * separation;
	mercury.position[1] =
	mercury.position[2] = 0.0f;
	mercury.radius = 0.383f * theEarth.radius;
	mercury.angle = 0.0f;

	venus.color.r = 1.0f;
	venus.color.g = 1.0f;
	venus.color.b = 1.0f;
	venus.position[0] = 0.723 * separation;
	venus.position[1] =
	venus.position[2] = 0.0f;
	venus.radius =  0.950f * theEarth.radius;
	venus.angle = 0.0f;

	mars.color.r = 1.0f;
	mars.color.g = 0.8f;
	mars.color.b = 0.7f;
	mars.position[0] = 1.534  * separation;
	mars.position[1] =
	mars.position[2] = 0.0f;
	mars.radius =  0.532f * theEarth.radius;
	mars.angle = 0.0f;

	jupiter.color.r = 1.0f;
	jupiter.color.g = 1.0f;
	jupiter.color.b = 0.8f;
	jupiter.position[0] = 5.20  * separation;
	jupiter.position[1] =
	jupiter.position[2] = 0.0f;
	jupiter.radius =  10.882f * theEarth.radius;
	jupiter.angle = 0.0f;

	io.color.r = 1.0f;
	io.color.g = 1.0f;
	io.color.b = 0.2f;
	io.position[0] = jupiter.position[0] + 0.00282  * separation;
	io.position[1] =
	io.position[2] = 0.0f;
	io.radius =  0.286f * theEarth.radius;
	io.angle = 0.0f;

	europa.color.r = 0.8f;
	europa.color.g = 0.8f;
	europa.color.b = 0.8f;
	europa.position[0] = jupiter.position[0];
	europa.position[1] = 0.0f;
	europa.position[2] = 0.00448  * separation;
	europa.radius =  0.245f * theEarth.radius;
	europa.angle = 0.0f;

	ganymede.color.r = 0.6f;
	ganymede.color.g = 0.7f;
	ganymede.color.b = 0.6f;
	ganymede.position[0] = jupiter.position[0] - 0.00715  * separation;
	ganymede.position[1] =
	ganymede.position[2] = 0.0f;
	ganymede.radius =  0.413f * theEarth.radius;
	ganymede.angle = 0.0f;

	callisto.color.r = 0.6f;
	callisto.color.g = 0.5f;
	callisto.color.b = 0.6f;
	callisto.position[0] = jupiter.position[0];
	callisto.position[1] = 0.0f;
	callisto.position[2] = -0.0126f * separation;
	callisto.radius =  0.378f * theEarth.radius;
	callisto.angle = 0.0f;

	saturn.color.r = 0.8f;
	saturn.color.g = 0.8f;
	saturn.color.b = 0.6f;
	saturn.position[0] = 9.582 * separation;
	saturn.position[1] =
	saturn.position[2] = 0.0f;
	saturn.radius =  9.449f * theEarth.radius;
	saturn.angle = 0.0f;

	titan.color.r = 0.9f;
	titan.color.g = 0.8f;
	titan.color.b = 0.5f;
	titan.position[0] = saturn.position[0] + 0.00817 * separation;
	titan.position[1] =
	titan.position[2] = 0.0f;
	titan.radius =  0.404f * theEarth.radius;
	titan.angle = 0.0f;

	uranus.color.r = 0.6f;
	uranus.color.g = 0.7f;
	uranus.color.b = 1.0f;
	uranus.position[0] = 19.229f * separation;
	uranus.position[1] =
	uranus.position[2] = 0.0f;
	uranus.radius =  3.929f * theEarth.radius;
	uranus.angle = 0.0f;

	neptune.color.r = 0.5f;
	neptune.color.g = 0.6f;
	neptune.color.b = 1.0f;
	neptune.position[0] = 30.103f * separation;
	neptune.position[1] =
	neptune.position[2] = 0.0f;
	neptune.radius =  3.883f * theEarth.radius;
	neptune.angle = 0.0f;

	/* Additional parameters/variables */
	solidMode = 0;
	cameraAngle = 0.0f;
        m3dNullVector3f(cameraPosition);
    	m3dSetVectorZ(cameraPosition, INITIAL_CAMERA_POSITION);
    	m3dNullVector3f(cameraTarget);
    	m3dNullVector3f(cameraDirection);
   	m3dSetVectorY(cameraDirection, 1.0f);
}

/*
 * Paints the "model".
 */
void doPaint()
{
        /* Taking care of the viewing/camera transformation */
        glPushMatrix();

	/* Paint the sun */
	glRotated(theSun.angle, 0.0, 1.0, 0.0);
	glDisable(GL_LIGHTING);
        glColor3f(theSun.color.r, theSun.color.g, theSun.color.b);
        if (solidMode)
	    glutSolidSphere(theSun.radius, 20, 20);
        else
            glutWireSphere(theSun.radius, 20, 20);
        glEnable(GL_LIGHTING);

	/* Move the light after we draw the sun! */
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

	/* Paint Mercury */
	glPushMatrix();
        glRotated(mercury.angle, 0.0, 1.0, 0.0);
	glTranslated(mercury.position[0], mercury.position[1], mercury.position[2]);
        glRotated(mercury.angle, 0.0, 1.0, 0.0);
	glColor3f(mercury.color.r, mercury.color.g, mercury.color.b);
        if (solidMode)
	    glutSolidSphere(mercury.radius, 20, 20);
        else
	    glutWireSphere(mercury.radius, 20, 20);

	/* Paint Venus */
	glPushMatrix();
        glRotated(venus.angle, 0.0, 1.0, 0.0);
	glTranslated(venus.position[0], venus.position[1], venus.position[2]);
        glRotated(venus.angle, 0.0, 1.0, 0.0);
	glColor3f(venus.color.r, venus.color.g, venus.color.b);
        if (solidMode)
	    glutSolidSphere(venus.radius, 20, 20);
        else
	    glutWireSphere(venus.radius, 20, 20);

	/* Paint the earth */
	glPushMatrix();
        glRotated(theEarth.angle, 0.0, 1.0, 0.0);
	glTranslated(theEarth.position[0], theEarth.position[1], theEarth.position[2]);
        glRotated(theEarth.angle, 0.0, 1.0, 0.0);
	glColor3f(theEarth.color.r, theEarth.color.g, theEarth.color.b);
        if (solidMode)
	    glutSolidSphere(theEarth.radius, 20, 20);
        else
	    glutWireSphere(theEarth.radius, 20, 20);

	/* Paint the moon */
	glPushMatrix();
        glRotated(theMoon.angle, 0.0, 1.0, 0.0);
        glTranslated(theMoon.position[0], theMoon.position[1], theMoon.position[2]);
	glColor3f(theMoon.color.r, theMoon.color.g, theMoon.color.b);
	if (solidMode)
		glutSolidSphere(theMoon.radius, 20, 20);
	else
		glutWireSphere(theMoon.radius, 20, 20);

        /* Paint Mars */
	glPushMatrix();
        glRotated(mars.angle, 0.0, 1.0, 0.0);
	glTranslated(mars.position[0], mars.position[1], mars.position[2]);
        glRotated(mars.angle, 0.0, 1.0, 0.0);
	glColor3f(mars.color.r, mars.color.g, mars.color.b);
        if (solidMode)
	    glutSolidSphere(mars.radius, 20, 20);
        else
	    glutWireSphere(mars.radius, 20, 20);

		
        /* Paint Jupiter */
	glPushMatrix();
        glRotated(jupiter.angle, 0.0, 1.0, 0.0);
	glTranslated(jupiter.position[0], jupiter.position[1], jupiter.position[2]);
        glRotated(jupiter.angle, 0.0, 1.0, 0.0);
	glColor3f(jupiter.color.r, jupiter.color.g, jupiter.color.b);
        if (solidMode)
	    glutSolidSphere(jupiter.radius, 20, 20);
        else
	    glutWireSphere(jupiter.radius, 20, 20);

        /* Paint Io */
	glPushMatrix();
        glRotated(io.angle, 0.0, 1.0, 0.0);
        glTranslated(io.position[0], io.position[1], io.position[2]);
	glColor3f(io.color.r, io.color.g, io.color.b);
	if (solidMode)
		glutSolidSphere(io.radius, 20, 20);
	else
		glutWireSphere(io.radius, 20, 20);

        /* Paint Europa */
	glPushMatrix();
        glRotated(europa.angle, 0.0, 1.0, 0.0);
        glTranslated(europa.position[0], europa.position[1], europa.position[2]);
	glColor3f(europa.color.r, europa.color.g, europa.color.b);
	if (solidMode)
		glutSolidSphere(europa.radius, 20, 20);
	else
		glutWireSphere(europa.radius, 20, 20);

        /* Paint Ganymede Only moon here with no Z rotation*/
	glPushMatrix();
        glRotated(ganymede.angle, 0.0, 1.0, 0.0);
        glTranslated(ganymede.position[0], ganymede.position[1], ganymede.position[2]);
	glColor3f(ganymede.color.r, ganymede.color.g, ganymede.color.b);
	if (solidMode)
		glutSolidSphere(ganymede.radius, 20, 20);
	else
		glutWireSphere(ganymede.radius, 20, 20);

        /* Paint Callisto */
	glPushMatrix();
        glRotated(callisto.angle, 0.0, 1.0, 0.0);
        glTranslated(callisto.position[0], callisto.position[1], callisto.position[2]);
	glColor3f(callisto.color.r, callisto.color.g, callisto.color.b);
	if (solidMode)
		glutSolidSphere(callisto.radius, 20, 20);
	else
		glutWireSphere(callisto.radius, 20, 20);


        /* Paint Saturn */
	glPushMatrix();
        glRotated(saturn.angle, 0.0, 1.0, 0.0);
	glTranslated(saturn.position[0], saturn.position[1], saturn.position[2]);
        glRotated(saturn.angle, 0.0, 1.0, 0.0);
	glColor3f(saturn.color.r, saturn.color.g, saturn.color.b);
        if (solidMode)
	    glutSolidSphere(saturn.radius, 20, 20);
        else
	    glutWireSphere(saturn.radius, 20, 20);
	GLUquadricObj* quad = gluNewQuadric();
        gluQuadricDrawStyle(quad, GLU_LINE);
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluQuadricTexture(quad, GL_TRUE);
	glColor4f(1, 1, 1, 0.65);
	gluDisk(quad, 0.000448f * AU * SEPARATION_SCALE, 0.000936f * AU * SEPARATION_SCALE, 10, 10);

        /* Paint Titan */
	glPushMatrix();
        glRotated(titan.angle, 0.0, 1.0, 0.0);
        glTranslated(titan.position[0], titan.position[1], titan.position[2]);
	glColor3f(titan.color.r, titan.color.g, titan.color.b);
	if (solidMode)
		glutSolidSphere(titan.radius, 20, 20);
	else
		glutWireSphere(titan.radius, 20, 20);

        /* Paint Uranus */
	glPushMatrix();
        glRotated(uranus.angle, 0.0, 1.0, 0.0);
	glTranslated(uranus.position[0], uranus.position[1], uranus.position[2]);
        glRotated(uranus.angle, 0.0, 1.0, 0.0);
	glColor3f(uranus.color.r, uranus.color.g, uranus.color.b);
        if (solidMode)
	    glutSolidSphere(uranus.radius, 20, 20);
        else
	    glutWireSphere(uranus.radius, 20, 20);

        /* Paint Neptune */
	glPushMatrix();
        glRotated(neptune.angle, 0.0, 1.0, 0.0);
	glTranslated(neptune.position[0], neptune.position[1], neptune.position[2]);
        glRotated(neptune.angle, 0.0, 1.0, 0.0);
	glColor3f(neptune.color.r, neptune.color.g, neptune.color.b);
        if (solidMode)
	    glutSolidSphere(neptune.radius, 20, 20);
        else
	    glutWireSphere(neptune.radius, 20, 20);
    
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

	
}

/*
 * Renders the model/scene.
 */
void RenderScene()
{
    /* Clear the color buffer and depth buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Draw the object(s) */
        doPaint();

	/* Display camera info as text*/
	char camInfo[30] = "Camera Distance: ";
	char distance[8];  
	snprintf(distance, sizeof distance, "%f", cameraPosition[2] / AU);
	strcat(camInfo, distance);
	strcat(camInfo, " AU");
	drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, camInfo, 1.0f, 1.0f, 1.0f);

        /* Update the screen */
        glutSwapBuffers();
 
}

/*
 * Initializes the rendering context.
 */
void SetupRC()
{
    /* Set the background color */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    /* Set color shading model to flat */
    glShadeModel(GL_SMOOTH);

    /* Counter-clockwise polygons are front facing */
    glFrontFace(GL_CCW);

	/* Enable depth buffer test */
	glEnable(GL_DEPTH_TEST);

	/* Enable culling */
	glEnable(GL_CULL_FACE);

	/* Lighting stuff */
	/* Enable lighting */
	glEnable(GL_LIGHTING);

	/* Set light model to use ambient light specified by ambientLight[] */
	/* Setup and enable light 0 */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	/* Enable material color tracking */
	glEnable(GL_COLOR_MATERIAL);

	/* Front material ambient and diffuse colors track glColor */
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
}

/*
 * Resizes the window.
 * @param w Width of the window.
 * @param h Height of the window.
 */
void ChangeSize(GLsizei w, GLsizei h)
{
    GLdouble zNear = 1.0;
    GLdouble zFar = 3*SOLAR_SYSTEM_SIZE;
    GLdouble angle = 45.0;
    GLdouble aspectRatio;

    /* Prevent a divide by zero, when window is too short you cant make a
     * window of zero width)
     */
    if (h == 0)
    	h = 1;

    /* Set the viewport to be the entire window */
    glViewport(0, 0, w, h);

    /* Select the projection matrix */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* Calculate the aspect ratio of the window */
    aspectRatio = (GLfloat)w / (GLfloat)h;

    /* Set the projection */
    gluPerspective(angle, aspectRatio, zNear, zFar);

    /* Select the modelviw matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Look at... */
    gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
              cameraTarget[0], cameraTarget[1], cameraTarget[2],
              cameraDirection[0], cameraDirection[1], cameraDirection[2]);
}

/*
 * Callback used for key pressed event.
 * @param key ASCII code of the key.
 * @param x X-coordinate of the mouse in the window.
 * @param y Y-coordinate of the mouse in the window.
 */
void KeyPressedStd(unsigned char key, int x, int y)
{
    /* Check the key pressed */
	switch (key) {
		case 's':
		case 'S': solidMode = 1; break;
		case 'w':
		case 'W': solidMode = 0; break;
		case 'r':
		case 'R':
		/* Reset visualization parameters*/
			cameraAngle = 0.0f;
   			m3dNullVector3f(cameraPosition);
    			m3dSetVectorZ(cameraPosition, INITIAL_CAMERA_POSITION);
    			m3dNullVector3f(cameraTarget);
    			m3dNullVector3f(cameraDirection);
    			m3dSetVectorY(cameraDirection, 1.0f);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], cameraTarget[0], cameraTarget[1], cameraTarget[2], cameraDirection[0], cameraDirection[1], cameraDirection[2]);
			break;
		case '+':
		/* Move the camera away from the model */
			m3dSetVectorZ(cameraPosition, cameraPosition[2] + 100);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], cameraTarget[0], cameraTarget[1], cameraTarget[2], cameraDirection[0], cameraDirection[1], cameraDirection[2]);
			break;
		case '-':
		/* Move the camera towards the model */
		   if(cameraPosition[2] - 100 > theSun.radius * 1.5){
			m3dSetVectorZ(cameraPosition, cameraPosition[2] - 100);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], cameraTarget[0], cameraTarget[1], cameraTarget[2], cameraDirection[0], cameraDirection[1], cameraDirection[2]);
		   }	
			break;
		case 'u':
		case 'U':
        /* Rotate the camera CCW direction */
			cameraAngle +=  60;

			glMatrixMode(GL_MODELVIEW);
       			glLoadIdentity();
	
			gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], cameraTarget[0], cameraTarget[1], cameraTarget[2], cameraDirection[0], cameraDirection[1], cameraDirection[2]);
			glRotatef(cameraAngle, 0, 1, 0);
			break;

		case 'd':
		case 'D':
        /* Rotate the camera CW direction */
			cameraAngle -=  60;
 
			glMatrixMode(GL_MODELVIEW);
		        glLoadIdentity();
	
			gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], cameraTarget[0], cameraTarget[1], cameraTarget[2], cameraDirection[0], cameraDirection[1], cameraDirection[2]);
			glRotatef(cameraAngle, 0, 1, 0);
			break;
		case 'p':
		case 'P':
			paused = !paused;
			break;
		case 'x':
		case 'X':
			exit(0);
			break;
		default: return;
	}
    glutPostRedisplay();
}

/*
 * Timer function.
 */
void TimerFunc(int value)
{
    /* Update angles 
       This dictates the orbital period.  The divisors were obtained from
       a table of astronomical data.  They are relative to Earth's orbit around
       the sun */
   if(!paused){
    theSun.angle += SMALL_ANGLE_INCR;
    theEarth.angle += BIG_ANGLE_INCR;
    theMoon.angle += BIG_ANGLE_INCR / 0.0795;
    mercury.angle += BIG_ANGLE_INCR / 0.241;
    venus.angle += BIG_ANGLE_INCR / 0.615;
    mars.angle += BIG_ANGLE_INCR / 1.881;
    jupiter.angle += BIG_ANGLE_INCR;
    io.angle += BIG_ANGLE_INCR / 0.00489;
    europa.angle += BIG_ANGLE_INCR / 0.00972;
    ganymede.angle += BIG_ANGLE_INCR / 0.0196;
    callisto.angle += BIG_ANGLE_INCR / 0.457;
    saturn.angle += BIG_ANGLE_INCR / 29.457;
    titan.angle += BIG_ANGLE_INCR / 0.0437;
    uranus.angle += BIG_ANGLE_INCR / 84.323;
    neptune.angle += BIG_ANGLE_INCR / 164.79;
   }
    /* Force it to redisplay */
    glutPostRedisplay();
   
    /* Do it again... */
    glutTimerFunc(TIME_INTERVAL, TimerFunc, 1);
}

/* Credit goes to R P Fletcher for this neat char display function */ 
void drawText(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b)
{
    int lines;
    char* p;

    glMatrixMode(GL_PROJECTION);
     glPushMatrix();
     glLoadIdentity();
     glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 
	    0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
     glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
      glColor3f(r,g,b);
      glRasterPos2i(x, y);
      for(p = s, lines = 0; *p; p++) {
	  if (*p == '\n') {
	      lines++;
	      glRasterPos2i(x, y-(lines*18));
	  }
	  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
      }
      glPopMatrix();
     glMatrixMode(GL_PROJECTION);
     glPopMatrix();
     glMatrixMode(GL_MODELVIEW);
}


/*
 * Main function.
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return Status code.
 */
int main (int argc, char **argv)
{
	/* Initialize the model */
	initialize();

    /* Initialization process */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutCreateWindow("Solar System");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(KeyPressedStd);
    glutTimerFunc(TIME_INTERVAL, TimerFunc, 1);

    SetupRC();

    /* "Main" loop */
    glutMainLoop();

    return EXIT_SUCCESS;
}
/* End of file -------------------------------------------------------------- */




