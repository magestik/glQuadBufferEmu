#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

/*
   Demonstration OpenGL application
   Create a model of a pulsar
*/

typedef struct {
   double x,y,z;
} XYZ;
typedef struct {
   double r,g,b;
} COLOUR;
typedef struct {
   unsigned char r,g,b,a;
} PIXELA;
typedef struct {
   XYZ vp;              /* View position           */
   XYZ vd;              /* View direction vector   */
   XYZ vu;              /* View up direction       */
   XYZ pr;              /* Point to rotate about   */
   double focallength;  /* Focal Length along vd   */
   double aperture;     /* Camera aperture         */
   double eyesep;       /* Eye separation          */
   int screenwidth,screenheight;
} CAMERA;

void Display(void);
void CreateEnvironment(void);
void MakeGeometry(void);
void MakeLighting(void);
void HandleKeyboard(unsigned char key,int x, int y);
void HandleSpecialKeyboard(int key,int x, int y);
void HandleMouse(int,int,int,int);
void HandleMainMenu(int);
void HandleSpeedMenu(int);
void HandleSpinMenu(int);
void HandleVisibility(int vis);
void HandleReshape(int,int);
void HandleMouseMotion(int,int);
void HandlePassiveMotion(int,int);
void HandleIdle(void);
void GiveUsage(char *);
void RotateCamera(int,int,int);
void TranslateCamera(int,int);
void CameraHome(int);
void Normalise(XYZ *);
XYZ  CalcNormal(XYZ,XYZ,XYZ);
int  WindowDump(int,int,int);

#define ABS(x) (x < 0 ? -(x) : (x))
#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)
#define TRUE  1
#define FALSE 0
#define ESC 27
#define PI 3.141592653589793238462643
#define DTOR            0.0174532925
#define RTOD            57.2957795
#define CROSSPROD(p1,p2,p3) \
   p3.x = p1.y*p2.z - p1.z*p2.y; \
   p3.y = p1.z*p2.x - p1.x*p2.z; \
   p3.z = p1.x*p2.y - p1.y*p2.x

/* Flags */
int fullscreen = FALSE;
int stereo = FALSE;
int showconstruct = FALSE;
int windowdump = FALSE;
int record = FALSE;
int debug = FALSE;

int currentbutton = -1;
double rotatespeed = 1;
double dtheta = 1;
CAMERA camera;
XYZ origin = {0.0,0.0,0.0};

double rotateangle = 0.0;    /* Pulsar Rotation angle */

int main(int argc,char **argv)
{
   int i;
   int mainmenu,speedmenu,spinmenu;

   camera.screenwidth = 400;
   camera.screenheight = 300;

   /* Parse the command line arguments */
   for (i=1;i<argc;i++) {
      if (strstr(argv[i],"-h") != NULL) 
         GiveUsage(argv[0]);
      if (strstr(argv[i],"-f") != NULL)
         fullscreen = TRUE;
      if (strstr(argv[i],"-s") != NULL)
         stereo = TRUE;
      if (strstr(argv[i],"-d") != NULL)
         debug = TRUE;
      if (strstr(argv[i],"-c") != NULL)
         showconstruct = TRUE;
   }

   /* Set things up and go */
   glutInit(&argc,argv);
   if (!stereo)
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   else
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STEREO);

   glutCreateWindow("Pulsar model");
   glutReshapeWindow(200,180);
   if (fullscreen)
      glutFullScreen();
   glutDisplayFunc(Display);
   glutReshapeFunc(HandleReshape);
   glutVisibilityFunc(HandleVisibility);
   glutKeyboardFunc(HandleKeyboard);
   glutSpecialFunc(HandleSpecialKeyboard);
   glutMouseFunc(HandleMouse);
   glutMotionFunc(HandleMouseMotion);
   glutSetCursor(GLUT_CURSOR_NONE);
   CreateEnvironment();
   CameraHome(0);

   /* Set up the speed menu */
   speedmenu = glutCreateMenu(HandleSpeedMenu);
   glutAddMenuEntry("Stop",1);
   glutAddMenuEntry("Slow",2);
   glutAddMenuEntry("Medium",3);
   glutAddMenuEntry("Fast",4);
   glutAddMenuEntry("Very fast",5);

   /* Set up the spin menu */
   spinmenu = glutCreateMenu(HandleSpinMenu);
   glutAddMenuEntry("1 degree",1);
   glutAddMenuEntry("2 degrees",2);
   glutAddMenuEntry("3 degrees",3);
   glutAddMenuEntry("5 degrees",4);

   /* Set up the main menu */
   mainmenu = glutCreateMenu(HandleMainMenu);
   glutAddSubMenu("Rotation",speedmenu);
   glutAddSubMenu("Camera rotation steps",spinmenu);
   glutAddMenuEntry("Toggle construction lines",1);
   glutAddMenuEntry("Quit",9);
   glutAttachMenu(GLUT_RIGHT_BUTTON);

   /* Ready to go! */
   glutMainLoop();
   return(0);
}

/*
   This is where global OpenGL/GLUT settings are made, 
   that is, things that will not change in time 
*/
void CreateEnvironment(void)
{
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_LINE_SMOOTH);
   glDisable(GL_POINT_SMOOTH);
   glEnable(GL_POLYGON_SMOOTH); 
   glShadeModel(GL_SMOOTH);    
   glDisable(GL_DITHER);
   glDisable(GL_CULL_FACE);

   glLineWidth(1.0);
   glPointSize(1.0);

   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glFrontFace(GL_CW);
   glClearColor(0.0,0.0,0.0,0.0);         /* Background colour */
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

/*
   This is the basic display callback routine
   It creates the geometry, lighting, and viewing position
   In this case it rotates the camera around the scene
*/
void Display(void)
{
   XYZ r;
   double dist,ratio,radians,scale,wd2,ndfl;
   double left,right,top,bottom,near=0.1,far=10000;

   /* Clip to avoid extreme stereo */
   if (stereo)
      near = camera.focallength / 5;

   /* Misc stuff */
   ratio  = camera.screenwidth / (double)camera.screenheight;
   radians = DTOR * camera.aperture / 2;
   wd2     = near * tan(radians);
   ndfl    = near / camera.focallength;

   /* Clear the buffers */
   glDrawBuffer(GL_BACK_LEFT);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   if (stereo) {
      glDrawBuffer(GL_BACK_RIGHT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   if (stereo) {

      /* Derive the two eye positions */
      CROSSPROD(camera.vd,camera.vu,r);
      Normalise(&r);
      r.x *= camera.eyesep / 2.0;
      r.y *= camera.eyesep / 2.0;
      r.z *= camera.eyesep / 2.0;

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      left  = - ratio * wd2 - 0.5 * camera.eyesep * ndfl;
      right =   ratio * wd2 - 0.5 * camera.eyesep * ndfl;
      top    =   wd2;
      bottom = - wd2;
      glFrustum(left,right,bottom,top,near,far);

      glMatrixMode(GL_MODELVIEW);
      glDrawBuffer(GL_BACK_RIGHT);
      glLoadIdentity();
      gluLookAt(camera.vp.x + r.x,camera.vp.y + r.y,camera.vp.z + r.z,
                camera.vp.x + r.x + camera.vd.x,
                camera.vp.y + r.y + camera.vd.y,
                camera.vp.z + r.z + camera.vd.z,
                camera.vu.x,camera.vu.y,camera.vu.z);
      MakeLighting();
      MakeGeometry();

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      left  = - ratio * wd2 + 0.5 * camera.eyesep * ndfl;
      right =   ratio * wd2 + 0.5 * camera.eyesep * ndfl;
      top    =   wd2;
      bottom = - wd2;
      glFrustum(left,right,bottom,top,near,far);

      glMatrixMode(GL_MODELVIEW);
      glDrawBuffer(GL_BACK_LEFT);
      glLoadIdentity();
      gluLookAt(camera.vp.x - r.x,camera.vp.y - r.y,camera.vp.z - r.z,
                camera.vp.x - r.x + camera.vd.x,
                camera.vp.y - r.y + camera.vd.y,
                camera.vp.z - r.z + camera.vd.z,
                camera.vu.x,camera.vu.y,camera.vu.z);
      MakeLighting();
      MakeGeometry();

   } else {

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      left  = - ratio * wd2;
      right =   ratio * wd2;
      top    =   wd2;
      bottom = - wd2;
      glFrustum(left,right,bottom,top,near,far);

      glMatrixMode(GL_MODELVIEW);
      glDrawBuffer(GL_BACK_LEFT);
      glLoadIdentity();
      gluLookAt(camera.vp.x,camera.vp.y,camera.vp.z,
                camera.vp.x + camera.vd.x,
                camera.vp.y + camera.vd.y,
                camera.vp.z + camera.vd.z,
                camera.vu.x,camera.vu.y,camera.vu.z);
      MakeLighting();
      MakeGeometry();
   }

   /* glFlush(); This isn't necessary for double buffers */
   glutSwapBuffers();

   if (record || windowdump)
      WindowDump(camera.screenwidth,camera.screenheight,stereo);

   /* Next angle for rotating the pulsar */
   rotateangle += rotatespeed;
}

/*
   Create the geometry for the pulsar
*/
void MakeGeometry(void)
{
   int i,j,k;
   double cradius = 5.3;         /* Final radius of the cone */
   double clength = 30;            /* Cone length */
   double sradius = 10;            /* Final radius of sphere */
   double r1,r2;                  /* Min and Max radius of field lines */
   double x,y,z;
   XYZ p[4],n[4];
   COLOUR grey = {0.7,0.7,0.7};
   COLOUR white = {1.0,1.0,1.0};
   GLfloat specular[4] = {1.0,1.0,1.0,1.0};
   GLfloat shiny[1] = {5.0};
   char cmd[64];

   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shiny);

   /* Top level rotation  - spin */
   glPushMatrix();
   glRotatef(rotateangle,0.0,1.0,0.0);

   /* Axis of rotation */
   if (showconstruct) {
      glColor3f(white.r,white.g,white.b);
      glBegin(GL_LINES);
      glVertex3f(0.0,-60.0,0.0);
      glVertex3f(0.0,60.0,0.0);
      glEnd();
   }

   /* Rotation about spin axis */
   glPushMatrix();
   glRotatef(45.0,0.0,0.0,1.0);

   /* Magnetic axis */
   if (showconstruct) {
      glColor3f(white.r,white.g,white.b);
      glBegin(GL_LINES);
      glVertex3f(0.0,-60.0,0.0);
      glVertex3f(0.0,60.0,0.0);
      glEnd();
   }

   /* Light in center */
   glColor3f(white.r,white.g,white.b);
   glutSolidSphere(5.0,16,8);

   /* Spherical center */
   for (i=0;i<360;i+=5) {
      for (j=-80;j<80;j+=5) {

         p[0].x = sradius * cos(j*DTOR) * cos(i*DTOR);
         p[0].y = sradius * sin(j*DTOR);
         p[0].z = sradius * cos(j*DTOR) * sin(i*DTOR);
         n[0] = p[0];

         p[1].x = sradius * cos((j+5)*DTOR) * cos(i*DTOR);
         p[1].y = sradius * sin((j+5)*DTOR);
         p[1].z = sradius * cos((j+5)*DTOR) * sin(i*DTOR);
         n[1] = p[1];

         p[2].x = sradius * cos((j+5)*DTOR) * cos((i+5)*DTOR);
         p[2].y = sradius * sin((j+5)*DTOR);
         p[2].z = sradius * cos((j+5)*DTOR) * sin((i+5)*DTOR);
         n[2] = p[2];

         p[3].x = sradius * cos(j*DTOR) * cos((i+5)*DTOR);
         p[3].y = sradius * sin(j*DTOR);
         p[3].z = sradius * cos(j*DTOR) * sin((i+5)*DTOR);
         n[3] = p[3];

         glBegin(GL_POLYGON);
         if (i % 20 == 0)
            glColor3f(1.0,0.0,0.0);
         else
            glColor3f(0.5,0.0,0.0);
         for (k=0;k<4;k++) {
            glNormal3f(n[k].x,n[k].y,n[k].z);
            glVertex3f(p[k].x,p[k].y,p[k].z);
         }
         glEnd();
      }
   }

   /* Draw the cones */
   for (j=-1;j<=1;j+=2) {
      for (i=0;i<360;i+=10) {
         
         p[0]   = origin;
         n[0]   = p[0];
         n[0].y = -1;

         p[1].x = cradius * cos(i*DTOR);
         p[1].y = j*clength;
         p[1].z = cradius * sin(i*DTOR);
         n[1]   = p[1];
         n[1].y = 0;

         p[2].x = cradius * cos((i+10)*DTOR);
         p[2].y = j*clength;
         p[2].z = cradius * sin((i+10)*DTOR);
         n[2]   = p[2];
         n[2].y = 0;

         glBegin(GL_POLYGON);
         if (i % 30 == 0)
            glColor3f(0.0,0.2,0.0);
         else
            glColor3f(0.0,0.5,0.0);
         for (k=0;k<3;k++) {
            glNormal3f(n[k].x,n[k].y,n[k].z);
            glVertex3f(p[k].x,p[k].y,p[k].z);
         }
         glEnd();
      }
   }

   /* Draw the field lines */
   r1 = 12;
   r2 = 16;
   for (j=0;j<360;j+=20) {
      glPushMatrix();
      glRotatef((double)j,0.0,1.0,0.0);
      glBegin(GL_LINE_STRIP);
      glColor3f(grey.r,grey.g,grey.b);
      for (i=-140;i<140;i++) {
         x = r1 + r1 * cos(i*DTOR);
         y = r2 * sin(i*DTOR);
         z = 0;
         glVertex3f(x,y,z);   
      }   
      glEnd();
      glPopMatrix();      
   }

   glPopMatrix(); /* Pulsar axis rotation */
   glPopMatrix(); /* Pulsar spin */
}

/*
   Set up the lighing environment
*/
void MakeLighting(void)
{
   GLfloat fullambient[4] = {1.0,1.0,1.0,1.0};
   GLfloat position[4] = {0.0,0.0,0.0,0.0};
   GLfloat ambient[4]  = {0.2,0.2,0.2,1.0};
   GLfloat diffuse[4]  = {1.0,1.0,1.0,1.0};
   GLfloat specular[4] = {0.0,0.0,0.0,1.0};

   /* Turn off all the lights */
   glDisable(GL_LIGHT0);
   glDisable(GL_LIGHT1);
   glDisable(GL_LIGHT2);
   glDisable(GL_LIGHT3);
   glDisable(GL_LIGHT4);
   glDisable(GL_LIGHT5);
   glDisable(GL_LIGHT6);
   glDisable(GL_LIGHT7);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);

   /* Turn on the appropriate lights */
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT,fullambient);
   glLightfv(GL_LIGHT0,GL_POSITION,position);
   glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
   glEnable(GL_LIGHT0);

   /* Sort out the shading algorithm */
   glShadeModel(GL_SMOOTH);

   /* Turn lighting on */
   glEnable(GL_LIGHTING);
}

/*
   Deal with plain key strokes
*/
void HandleKeyboard(unsigned char key,int x, int y)
{
   switch (key) {
   case ESC:                            /* Quit */
   case 'Q':
   case 'q': 
      exit(0); 
      break;
   case 'c':                           /* Toggle constructs */
   case 'C':
      showconstruct = !showconstruct;
      break;
   case 'h':                           /* Go home     */
   case 'H':
      CameraHome(0);
      break;
   case '[':                           /* Roll anti clockwise */
      RotateCamera(0,0,-1);
      break;
   case ']':                           /* Roll clockwise */
      RotateCamera(0,0,1);
      break;
   case 'i':                           /* Translate camera up */
   case 'I':
      TranslateCamera(0,1);
      break;
   case 'k':                           /* Translate camera down */
   case 'K':
      TranslateCamera(0,-1);
      break;
   case 'j':                           /* Translate camera left */
   case 'J':
      TranslateCamera(-1,0);
      break;
   case 'l':                           /* Translate camera right */
   case 'L':
      TranslateCamera(1,0);
      break;
   case 'w':                           /* Write the image to disk */
   case 'W':
      windowdump = !windowdump;
      break;
   case 'r':
   case 'R':
      record = !record;
      break;
   }
}

/*
   Deal with special key strokes
*/
void HandleSpecialKeyboard(int key,int x, int y)
{
   switch (key) {
   case GLUT_KEY_LEFT:
      RotateCamera(-1,0,0);
      break;
   case GLUT_KEY_RIGHT:
      RotateCamera(1,0,0);
      break;
   case GLUT_KEY_UP:
      RotateCamera(0,1,0);
      break;
   case GLUT_KEY_DOWN:
      RotateCamera(0,-1,0);
      break;
   }
}

/*
   Rotate (ix,iy) or roll (iz) the camera about the focal point
   ix,iy,iz are flags, 0 do nothing, +- 1 rotates in opposite directions
   Correctly updating all camera attributes
*/
void RotateCamera(int ix,int iy,int iz)
{
   XYZ vp,vu,vd;
   XYZ right;
   XYZ newvp,newr;
   double radius,dd,radians;
   double dx,dy,dz;

   vu = camera.vu;
   Normalise(&vu);
   vp = camera.vp;
   vd = camera.vd;
   Normalise(&vd);
   CROSSPROD(vd,vu,right);
   Normalise(&right);
   radians = dtheta * PI / 180.0;

   /* Handle the roll */
   if (iz != 0) {
      camera.vu.x += iz * right.x * radians;
      camera.vu.y += iz * right.y * radians;
      camera.vu.z += iz * right.z * radians;
      Normalise(&camera.vu);
      return;
   }

   /* Distance from the rotate point */
   dx = camera.vp.x - camera.pr.x;
   dy = camera.vp.y - camera.pr.y;
   dz = camera.vp.z - camera.pr.z;
   radius = sqrt(dx*dx + dy*dy + dz*dz);

   /* Determine the new view point */
   dd = radius * radians;
   newvp.x = vp.x + dd * ix * right.x + dd * iy * vu.x - camera.pr.x;
   newvp.y = vp.y + dd * ix * right.y + dd * iy * vu.y - camera.pr.y;
   newvp.z = vp.z + dd * ix * right.z + dd * iy * vu.z - camera.pr.z;
   Normalise(&newvp);
   camera.vp.x = camera.pr.x + radius * newvp.x;
   camera.vp.y = camera.pr.y + radius * newvp.y;
   camera.vp.z = camera.pr.z + radius * newvp.z;

   /* Determine the new right vector */
   newr.x = camera.vp.x + right.x - camera.pr.x;
   newr.y = camera.vp.y + right.y - camera.pr.y;
   newr.z = camera.vp.z + right.z - camera.pr.z;
   Normalise(&newr);
   newr.x = camera.pr.x + radius * newr.x - camera.vp.x;
   newr.y = camera.pr.y + radius * newr.y - camera.vp.y;
   newr.z = camera.pr.z + radius * newr.z - camera.vp.z;

   camera.vd.x = camera.pr.x - camera.vp.x;
   camera.vd.y = camera.pr.y - camera.vp.y;
   camera.vd.z = camera.pr.z - camera.vp.z;
   Normalise(&camera.vd);

   /* Determine the new up vector */
   CROSSPROD(newr,camera.vd,camera.vu);
   Normalise(&camera.vu);

   if (debug)
      fprintf(stderr,"Camera position: (%g,%g,%g)\n",
         camera.vp.x,camera.vp.y,camera.vp.z);
}

/*
   Translate (pan) the camera view point
   In response to i,j,k,l keys
   Also move the camera rotate location in parallel
*/
void TranslateCamera(int ix,int iy)
{
   XYZ vp,vu,vd;
   XYZ right;
   XYZ newvp,newr;
   double radians,delta;

   vu = camera.vu;
   Normalise(&vu);
   vp = camera.vp;
   vd = camera.vd;
   Normalise(&vd);
   CROSSPROD(vd,vu,right);
   Normalise(&right);
   radians = dtheta * PI / 180.0;
   delta = dtheta * camera.focallength / 90.0;

   camera.vp.x += iy * vu.x * delta;
   camera.vp.y += iy * vu.y * delta;
   camera.vp.z += iy * vu.z * delta;
   camera.pr.x += iy * vu.x * delta;
   camera.pr.y += iy * vu.y * delta;
   camera.pr.z += iy * vu.z * delta;

   camera.vp.x += ix * right.x * delta;
   camera.vp.y += ix * right.y * delta;
   camera.vp.z += ix * right.z * delta;
   camera.pr.x += ix * right.x * delta;
   camera.pr.y += ix * right.y * delta;
   camera.pr.z += ix * right.z * delta;
}

/*
   Handle mouse events
   Right button events are passed to menu handlers
*/
void HandleMouse(int button,int state,int x,int y)
{
   if (state == GLUT_DOWN) {
      if (button == GLUT_LEFT_BUTTON) {
         currentbutton = GLUT_LEFT_BUTTON;
      } else if (button == GLUT_MIDDLE_BUTTON) {
         currentbutton = GLUT_MIDDLE_BUTTON;
      } 
   }
}

/*
   Handle the main menu
*/
void HandleMainMenu(int whichone)
{
   switch (whichone) {
   case 1:
      showconstruct = !showconstruct;
      break;
   case 9: 
      exit(0); 
      break;
   }
}

/*
   Handle the speed menu
   The rotate speed is in degrees
*/
void HandleSpeedMenu(int whichone)
{
   switch (whichone) {
   case 1: rotatespeed = 0.0; break;
   case 2: rotatespeed = 0.3; break;
   case 3: rotatespeed = 1;   break;
   case 4: rotatespeed = 3;   break;
   case 5: rotatespeed = 10;  break;
   }
}

/*
   Handle the camera spin menu
*/
void HandleSpinMenu(int whichone)
{
   switch (whichone) {
   case 1: dtheta = 1; break;
   case 2: dtheta = 2; break;
   case 3: dtheta = 3; break;
   case 4: dtheta = 5; break;
   }
}

/*
   How to handle visibility
*/
void HandleVisibility(int visible)
{
   if (visible == GLUT_VISIBLE)
      glutIdleFunc(HandleIdle);
   else
      glutIdleFunc(NULL);
}

/*
   What to do on an idle event
*/
void HandleIdle(void)
{
   glutPostRedisplay();
}

/*
   Handle a window reshape/resize
*/
void HandleReshape(int w,int h)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glViewport(0,0,(GLsizei)w,(GLsizei)h);
   camera.screenwidth = w;
   camera.screenheight = h;
}

/*
   Display the program usage information
*/
void GiveUsage(char *cmd)
{
   fprintf(stderr,"Usage: %s [-h] [-f] [-s] [-c] [-q n]\n",cmd);
   fprintf(stderr,"          -h   this text\n");
   fprintf(stderr,"          -f   full screen\n");
   fprintf(stderr,"          -s   stereo\n");
   fprintf(stderr,"          -c   show construction lines\n");
   fprintf(stderr,"Key Strokes\n");
   fprintf(stderr,"  arrow keys   rotate left/right/up/down\n");
   fprintf(stderr,"  left mouse   rotate\n");
   fprintf(stderr,"middle mouse   roll\n");
   fprintf(stderr,"           c   toggle construction lines\n");
   fprintf(stderr,"           i   translate up\n");
   fprintf(stderr,"           k   translate down\n");
   fprintf(stderr,"           j   translate left\n");
   fprintf(stderr,"           l   translate right\n");
   fprintf(stderr,"           [   roll clockwise\n");
   fprintf(stderr,"           ]   roll anti clockwise\n");
   fprintf(stderr,"           q   quit\n");
   exit(-1);
}

void Normalise(XYZ *p)
{
   double length;

   length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
   if (length != 0) {
      p->x /= length;
      p->y /= length;
      p->z /= length;
   } else {
      p->x = 0;
      p->y = 0;
      p->z = 0;
   }
}

XYZ CalcNormal(XYZ p,XYZ p1,XYZ p2)
{
   XYZ n,pa,pb;

   pa.x = p1.x - p.x;
   pa.y = p1.y - p.y;
   pa.z = p1.z - p.z;
   pb.x = p2.x - p.x;
   pb.y = p2.y - p.y;
   pb.z = p2.z - p.z;
   Normalise(&pa);
   Normalise(&pb);
  
   n.x = pa.y * pb.z - pa.z * pb.y;
   n.y = pa.z * pb.x - pa.x * pb.z;
   n.z = pa.x * pb.y - pa.y * pb.x;
   Normalise(&n);

   return(n);
}

/*
   Move the camera to the home position 
*/
void CameraHome(int mode)
{
   camera.aperture = 50;
   camera.focallength = 70;
   camera.eyesep = camera.focallength / 20;
   camera.pr = origin;

/*
   camera.vp.x = camera.focallength; 
   camera.vp.y = 0; 
   camera.vp.z = 0;
   camera.vd.x = -1;
   camera.vd.y = 0;
   camera.vd.z = 0;
*/
   /* Special camera position so the beam crosses the view */
   camera.vp.x = 39;
   camera.vp.y = 53;
   camera.vp.z = 22;
   camera.vd.x = -camera.vp.x; 
   camera.vd.y = -camera.vp.y; 
   camera.vd.z = -camera.vp.z;

   camera.vu.x = 0;  
   camera.vu.y = 1; 
   camera.vu.z = 0;
}

/*
   Handle mouse motion
*/
void HandleMouseMotion(int x,int y)
{
   static int xlast=-1,ylast=-1;
   int dx,dy;

   dx = x - xlast;
   dy = y - ylast;
   if (dx < 0)      dx = -1;
   else if (dx > 0) dx =  1;
   if (dy < 0)      dy = -1;
   else if (dy > 0) dy =  1;

   if (currentbutton == GLUT_LEFT_BUTTON)
      RotateCamera(-dx,dy,0);
   else if (currentbutton == GLUT_MIDDLE_BUTTON)
      RotateCamera(0,0,dx);

   xlast = x;
   ylast = y;
}

/*
   Write the current view to a PPM file
   Do the right thing for stereo, ie: two images
*/
int WindowDump(int width,int height,int stereo)
{
   int i,j;
   FILE *fptr;
   static int counter = 0;
   char fname[32];
   unsigned char *image;

   /* Allocate our buffer for the image */
   if ((image = malloc(3*width*height*sizeof(char))) == NULL) {
      fprintf(stderr,"WindowDump - Failed to allocate memory for image\n");
      return(FALSE);
   }

   /* Open the file */
   sprintf(fname,"L_%04d.ppm",counter);
   if ((fptr = fopen(fname,"w")) == NULL) {
      fprintf(stderr,"WindowDump - Failed to open file for window dump\n");
      return(FALSE);
   }

   /* Copy the image into our buffer */
   glReadBuffer(GL_BACK_LEFT);
   glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

   /* Write the PPM file */
   fprintf(fptr,"P3\n%d %d\n255\n",width,height);
   for (j=height-1;j>=0;j--) {
      for (i=0;i<width;i++) {
         fputc(image[3*j*width+3*i+0],fptr);
         fputc(image[3*j*width+3*i+1],fptr);
         fputc(image[3*j*width+3*i+2],fptr);
      }
   }
   fclose(fptr);
   if (stereo) {

      /* Open the file */
      sprintf(fname,"R_%04d.ppm",counter);
      if ((fptr = fopen(fname,"w")) == NULL) {
         fprintf(stderr,"WindowDump - Failed to open file for window dump\n");
         return(FALSE);
      }

      /* Copy the image into our buffer */
      glReadBuffer(GL_BACK_RIGHT);
      glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

      /* Write the PPM file */
      fprintf(fptr,"P3\n%d %d\n255\n",width,height);
      for (j=height-1;j>=0;j--) {
         for (i=0;i<width;i++) {
            fputc(image[3*j*width+3*i+0],fptr);
            fputc(image[3*j*width+3*i+1],fptr);
            fputc(image[3*j*width+3*i+2],fptr);
         }
      }
      fclose(fptr);
   }

   free(image);
   counter++;
   return(TRUE);
}

