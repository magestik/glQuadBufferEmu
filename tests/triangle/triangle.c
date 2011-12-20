/*
	Demonstration Stereographics OpenGL application
   create triangle display
*/

// http://www.ds.eng.monash.edu.au/vizlab/triangle.c

#define NOSTEREO 0
#define ACTIVESTEREO 1
#define DUALSTEREO 2

//ACTIVESTEREO requires shutter type glasses
//DUALSTEREO is the passive stereo we currently have in the VISLAB

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define DTOR            0.0174532925
#define FALSE 0

typedef struct {
   double x,y,z;
} XYZ;

typedef struct {
   XYZ vp;                /* View position           */
   XYZ vd;                /* View direction vector   */
   XYZ vu;                /* View up direction       */
   XYZ pr;                /* Point to rotate about   */
   double focallength;    /* Focal Length along vd   */
   double aperture;       /* Camera aperture         */
   double eyesep;         /* Eye separation          */
	double near,far;       /* Cutting plane distances */
	int stereo;				  /* Are we in stereo mode   */
   int screenwidth;       /* Screen dimensions       */
   int screenheight;      /*                         */
	double cursordepth;    /* 3D cursor depth         */
} CAMERA;
typedef struct {
	int fullscreen;        /* Game mode or not                        */
} OPTIONS;

typedef struct {
   int h,v;
} POINT;
OPTIONS options;
CAMERA camera;
POINT mousepos = {-1,-1};
float xrot=0.0;
float yrot=0.0;
float zrot=0.0;

/*
   Handle mouse motion
   Only handled by the server
*/
void HandleMouseMotion(int x,int y)
{
   int dx,dy;
   dx = x - mousepos.h;
   dy = y - mousepos.v;

  // if (dx < 0)      dx = -1;
   //else if (dx > 0) dx =  1;
  // if (dy < 0)      dy = -1;
 //  else if (dy > 0) dy =  1;
   xrot+=dx;
   yrot+=dy;
   
   mousepos.h = x;
   mousepos.v = y;
   glutPostRedisplay();
}

GLfloat z=-10.0f; // depth into the screen.

#define CROSSPROD(p1,p2,p3) \
   p3.x = p1.y*p2.z - p1.z*p2.y; \
   p3.y = p1.z*p2.x - p1.x*p2.z; \
   p3.z = p1.x*p2.y - p1.y*p2.x

/*
   Clear the left and/or right buffer
   Call as ClearBuffer(1) for mono
           ClearBuffer(2) for stereo
           ClearBuffer(1) if in stereo and the card will clear both, eg: 4D51T
*/
void ClearBuffers(int which)
{
   if (which == 2) {
      glDrawBuffer(GL_BACK_LEFT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glDrawBuffer(GL_BACK_RIGHT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   } else {
      glDrawBuffer(GL_BACK);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }
}



/*
   This is where global OpenGL/GLUT settings are made,
	that is, things that will not change in time
*/
void CreateEnvironment(void)
{
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_DITHER);
   glDisable(GL_LINE_SMOOTH);
   glDisable(GL_POINT_SMOOTH);
   glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_CULL_FACE);
   glLineWidth(1.0);
   glPointSize(1.0);
   glClearColor(0.0,0.0,0.0,0.0);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

/*-------------------------------------------------------------------------
   Cross product between two vectors p = p1 x p2
*/
XYZ CrossProduct(XYZ p1,XYZ p2)
{
   XYZ p;

   p.x = p1.y * p2.z - p1.z * p2.y;
   p.y = p1.z * p2.x - p1.x * p2.z;
   p.z = p1.x * p2.y - p1.y * p2.x;

   return(p);
}


/*-------------------------------------------------------------------------
   Normalise a vector
*/
void Normalise(XYZ *p)
{
   double length;

   length = p->x * p->x + p->y * p->y + p->z * p->z;
   if (length > 0) {
      length = sqrt(length);
      p->x /= length;
      p->y /= length;
      p->z /= length;
   } else {
      p->x = 0;
      p->y = 0;
      p->z = 0;
   }
}

GLvoid DrawGLScene(GLvoid)
{

	 glTranslatef(0.0f,0.0f,z);                  // move z units out from the screen.
   glRotatef(xrot,1.0f,0.0f,0.0f);    // Rotate On The X Axis
    glRotatef(yrot,0.0f,1.0f,0.0f);    // Rotate On The Y Axis
    glRotatef(xrot,0.0f,0.0f,1.0f);    // Rotate On The Z Axis
  

	glScalef(10.0,10.0,10.0);
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_POLYGON);
       glColor3f(1.0,0.0,0.0);
       glVertex3f(-2.0,-2.0,-2.0);

       glColor3f(0.0,1.0,0.0);
       glVertex3f(2.0,-2.0,-2.0);

       glColor3f(0.0,0.0,1.0);
       glVertex3f(0.0,2.0,2.0);
     glEnd();

}



/*
   This is the basic display callback routine
   It creates the geometry, lighting, and viewing position
*/
void HandleDisplay(void)
{
	XYZ r;
   double ratio,radians,wd2,ndfl;
   double left,right,top,bottom;


   /* Misc stuff needed for the frustum */
   ratio   = camera.screenwidth / (double)camera.screenheight;
	if (camera.stereo == DUALSTEREO)
		ratio /= 2;
   radians = DTOR * camera.aperture / 2;
   wd2     = camera.near * tan(radians);
   ndfl    = camera.near / camera.focallength;
   top     =   wd2;
   bottom  = - wd2;

	/* Clear the buffers */
   if (camera.stereo == ACTIVESTEREO)
		ClearBuffers(2);
   else
		ClearBuffers(1);

	/* Determine the right eye vector */
   CROSSPROD(camera.vd,camera.vu,r);
   Normalise(&r);
   r.x *= camera.eyesep / 2.0;
   r.y *= camera.eyesep / 2.0;
   r.z *= camera.eyesep / 2.0;

	if (camera.stereo == ACTIVESTEREO || camera.stereo == DUALSTEREO) {

   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
      left  = - ratio * wd2 - 0.5 * camera.eyesep * ndfl;
      right =   ratio * wd2 - 0.5 * camera.eyesep * ndfl;
      glFrustum(left,right,bottom,top,camera.near,camera.far);
		if (camera.stereo == DUALSTEREO)
			glViewport(camera.screenwidth/2,0,camera.screenwidth/2,camera.screenheight);
		else
			glViewport(0,0,camera.screenwidth,camera.screenheight);
      glMatrixMode(GL_MODELVIEW);
      glDrawBuffer(GL_BACK_RIGHT);
      glLoadIdentity();
      gluLookAt(camera.vp.x + r.x,camera.vp.y + r.y,camera.vp.z + r.z,
                camera.vp.x + r.x + camera.vd.x,
                camera.vp.y + r.y + camera.vd.y,
                camera.vp.z + r.z + camera.vd.z,
                camera.vu.x,camera.vu.y,camera.vu.z);
		DrawGLScene();

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      left  = - ratio * wd2 + 0.5 * camera.eyesep * ndfl;
      right =   ratio * wd2 + 0.5 * camera.eyesep * ndfl;
      glFrustum(left,right,bottom,top,camera.near,camera.far);
      if (camera.stereo == DUALSTEREO)
         glViewport(0,0,camera.screenwidth/2,camera.screenheight);
      else
         glViewport(0,0,camera.screenwidth,camera.screenheight);
      glMatrixMode(GL_MODELVIEW);
      glDrawBuffer(GL_BACK_LEFT);
      glLoadIdentity();
      gluLookAt(camera.vp.x - r.x,camera.vp.y - r.y,camera.vp.z - r.z,
                camera.vp.x - r.x + camera.vd.x,
                camera.vp.y - r.y + camera.vd.y,
                camera.vp.z - r.z + camera.vd.z,
                camera.vu.x,camera.vu.y,camera.vu.z);
		DrawGLScene();


	} else {  //not stereo

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
		glViewport(0,0,camera.screenwidth,camera.screenheight);
      left  = - ratio * wd2;
      right =   ratio * wd2;
      glFrustum(left,right,bottom,top,camera.near,camera.far);
      glMatrixMode(GL_MODELVIEW);
      glDrawBuffer(GL_BACK_LEFT);
      glLoadIdentity();
      gluLookAt(camera.vp.x,camera.vp.y,camera.vp.z,
                camera.vp.x + camera.vd.x,
                camera.vp.y + camera.vd.y,
                camera.vp.z + camera.vd.z,
                camera.vu.x,camera.vu.y,camera.vu.z);

		DrawGLScene();
	}

	/* Swap buffers */
	glutSwapBuffers();

}



/*
   Deal with plain key strokes
	Pass the key on to the client
	This is only handled by the server
*/
void HandleKeyboard(unsigned char key,int x, int y)
{
	/* Handle keyboard commands */
   switch (key) {
   case 'Q':
   case 'q': 
		exit(0);
		break;
	
	}
}



/*
   Handle a window reshape/resize
*/
void HandleReshape(int w,int h)
{
   camera.screenwidth = w;
   camera.screenheight = h;
}


/*
	Move the camera to the home position 
*/
void CameraHome(void)
{
	XYZ origin = {0,0,0};
	camera.aperture = 60;
	camera.focallength = 100;
	camera.eyesep = 4;
	camera.near = camera.focallength / 10;
	camera.far = camera.focallength * 10;

	camera.vp.x = 0;		//view position
	camera.vp.y = 0;
	camera.vp.z = 50;
   camera.vd.x = 0;		//view direction
   camera.vd.y = 0;
   camera.vd.z = -1;		//looks down z-axis
   camera.vu.x = 0;
   camera.vu.y = 1;		//view up direction
   camera.vu.z = 0;
	camera.pr = origin;

	Normalise(&camera.vd);
	Normalise(&camera.vu);
	
}




int main(int argc,char **argv)
{
	int i;


   /* The camera attributes are passed from server to client as necessary */
   CameraHome();
	camera.stereo       = NOSTEREO;
   camera.cursordepth  = camera.focallength;
   camera.screenwidth  = 400;
   camera.screenheight = 300;

	options.fullscreen   = FALSE;


	/* Parse the command line arguments */
   for (i=1;i<argc;i++) {
      if (strcmp(argv[i],"-h") == 0)
         fprintf(stderr,"%s -f (for fullscreen) -s (for active stereo) -ss (for passive stereo)\n",argv[0]);
      if (strcmp(argv[i],"-f") == 0)
         options.fullscreen = !options.fullscreen;
      if (strcmp(argv[i],"-s") == 0)
         camera.stereo = ACTIVESTEREO;
		if (strcmp(argv[i],"-ss") == 0) {
         camera.stereo = DUALSTEREO;
			camera.screenwidth *= 2;
		}
   }

   /* Set things up and go */
   glutInit(&argc,argv);
   if (camera.stereo == ACTIVESTEREO)
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STEREO);
	else
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	/* Create the window and handlers */
   glutCreateWindow("triangle");
	glutReshapeWindow(camera.screenwidth,camera.screenheight);
   if (options.fullscreen)
      glutFullScreen();
   glutDisplayFunc(HandleDisplay);
	glutReshapeFunc(HandleReshape);

	/*
		The client doesn't deal with user input
		Turn of the mouse since it is a killer in passive stereo
		to have a mouse on only one screen.
	*/
   glutKeyboardFunc(HandleKeyboard);
   glutMotionFunc(HandleMouseMotion);
	glutSetCursor(GLUT_CURSOR_NONE);
   CreateEnvironment();

	/* Ready to go! */
   glutMainLoop();
   return(0);
}
