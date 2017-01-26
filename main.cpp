

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>

double t = 0;
double earth = 1;
double scale = .01;
double speed = 10;
float translateX = 0.0;
float translateY = -1.0;
float sunRadius = 10;


// Light source position
GLfloat light_position[] = {-1.0, 0.0, 2.0, 0.0};
GLfloat light_position2[] = {1.0, 0.0, 0.0, 0.0};
GLfloat emit[] = {.5, .5, .5, 0.0};
GLfloat red[] = {1.0,0.0,0.0,0.0};
int numOfAsteroids = 99;
float asteroidX[99];	//arrays for to store asteroid coords
float asteroidY[99];
float asteroidZ[99];

bool eyeLevel = false;


void initLighting()
{
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    // Set the light position
    glLightfv(GL_LIGHT0, GL_POSITION,light_position);
    glLightfv(GL_LIGHT1, GL_POSITION,light_position2);
    
}

float random(int min, int max) //create a random number in a specified range
{
    int randNum;
    randNum = rand()%(max-min + 1) + min;
    return randNum;
}

//STARS

void setUpAsteroids()	//to initialise array of random coords for star within coordinate space
{
    for (int i = 0; i < numOfAsteroids; ++i)
    {
        asteroidX[i] = random(-150,150)/75.0; // -1.5 - 1.5
        asteroidY[i] = random(-100,100)/50.0; //-1.0 - 1.0
        asteroidZ[i] = -5;
    }
}


//loops through the coordinates of asteroids and updates them
void createAsteroids()
{
    glLoadIdentity();
    for(int i = 0; i<numOfAsteroids; ++i)
    {
        glTranslatef(asteroidX[i],asteroidY[i],asteroidZ[i]);
        glutSolidSphere(10*scale,50,10);
    }
    glEnd();
    glutSwapBuffers();
}


void createPlanet(GLfloat orbitRadius, GLfloat radius, GLfloat period) //random orbit guyo
{
    glLoadIdentity();
    glTranslatef(cos(2*t)*.5+translateX,translateY,sin(2*t));
    glutWireSphere(radius*scale,50,10);
}

void createShip(GLfloat orbitRadius, GLfloat radius, GLfloat period) //what creates and updates the ship
{
    glLoadIdentity();
    if(eyeLevel)
    {
        glRotatef(90,1.0,0.0,0.0);
    }
    glTranslatef(translateX,translateY+1,-1);
    glRotatef(90,0.0,1.0, 0.0);
    glRotatef(20*t,0.0,0.0,-2.0);
    glutWireSphere(.1,50,10);
}


void display(void)
{
    glLoadIdentity();
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat pos[] = { 0.0, 0.0, 1.0, 0.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    createShip(0,5,0); //mercury
    createPlanet(10+2,2,5);
    
    createAsteroids();
    
    glEnd();
    glutSwapBuffers();
}

void createStars(int intensity)
{
    
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,w/h,.1,1000);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void TimerFunction()
{
    t += .1;
    for(int i = 0; i< numOfAsteroids; ++i)
    {
        if(asteroidZ[i] < 1)
        {
            asteroidZ[i] += .05;
        }
        else
        {
            asteroidZ[i] = -10.0;
        }
    }
    glutPostRedisplay();
}

void mouse(int x, int y)
{

}

void specialInput(int key, int x, int y)
{
    switch(key)
    {
        case 'a':   //if it isn't too far left, moves sphere to the left
            if(translateX > -.5)
            {
            translateX -= .25;
            }
            break;
        case 'd':   //if it isn't too far right, moves sphere to the left
            if(translateX < .5)
            {
            translateX += .25;
            }
            break;
        case 'w':   //if it isn't too far up, moves sphere up
        {
            if(translateY < -.5)
            {
            translateY += .25;
            }
            break;
        }
        case 's':   //if it isn't too far down, moves sphere down
        {
            if(translateY > -1.5)
            {
            translateY -= .25;
            }
            break;
        }
    }
    glutPostRedisplay();
}

int main(int argc,char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1000, 1000);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    glutDisplayFunc(display);
    glutSpecialFunc(specialInput);
    glutIdleFunc(TimerFunction);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouse);
    initLighting();
    glEnable(GL_DEPTH_TEST);
    // Initialization
    glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);
    glutAttachMenu(GLUT_LEFT_BUTTON);
    setUpAsteroids();
    glutMainLoop();
    return 0;
}
