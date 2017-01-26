

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
int numOfAsteroids = 1;
float asteroidX[1];	//arrays for to store asteroid coords
float asteroidY[1];
float asteroidZ[1];

float asteroidX2[99];
float asteroidY2[99];
float asteroidZ2[99];



bool upPressed = false;
bool downPressed = false;
bool leftPressed = false;
bool rightPressed = false;
bool gameOver = false;

bool eyeLevel = false;
float width;
float height;

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
    for(int i = 0; i< numOfAsteroids; i++)
    {
        asteroidX[i] = random(-100,100)/500; // -.5 - .5
        asteroidY[i] = random(-80,80)/500; //-1.0 - 1.0
        asteroidZ[i] = (float)(-1)*(i)-10;
    }
}


//loops through the coordinates of asteroids and updates them
void createAsteroids()
{
    glLoadIdentity();
    for(int i = 0; i<numOfAsteroids; i++)
    {
        glTranslatef(asteroidX[i],asteroidY[i],asteroidZ[i]);
        glutSolidSphere(.1,10,10);
    }
    
    glLoadIdentity();
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
    //createPlanet(10+2,2,5);
    
    createAsteroids();
    
    glEnd();
    glutSwapBuffers();
}

void createStars(int intensity)
{
    
}

void shipHasBeenHit()
{
    for(int i = 0; i< numOfAsteroids; i++)
    {
        if(asteroidZ[i] > -1)
        {
            if((asteroidX[i] == translateX) || (asteroidY[i] == translateY))
            {
                gameOver = true;
            }
        }
    }
}

void reshape (int w, int h)
{
    height = h;
    width = w;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90,w/h,.1,1000);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void TimerFunction()
{
    if(gameOver == false)
    {
    t += .1;
    shipHasBeenHit();
    if(leftPressed && translateX > -.75)
    {
        translateX -= .05;
    }
    else if(rightPressed && translateX < .75)
    {
        translateX += .05;
    }
    if(upPressed && translateY < -.25)
    {
        translateY += .05;
    }
    else if(downPressed && translateY > -1.75)
    {
        translateY -= .05;
    }
    
    for(int i = 0; i< numOfAsteroids; i++)
    {
            if(asteroidZ[i] < 2)
            {
                asteroidZ[i] += .1;
            }
            else
            {
                asteroidX[i] = random(-50,50)/100.0; // -.5 - .5
                asteroidY[i] = random(-100,100)/200; //-1.0 - 1.0
                asteroidZ[i] -= 10;
            }
        
    }
     
    glutPostRedisplay();
    glutSwapBuffers();
    }
}

void mouse(int x, int y)
{

}

void specialInput(int key, int x, int y)
{
    switch(key)
    {
        case 'a':   //if it isn't too far left, moves sphere to the left
        {
            leftPressed = true;
            break;
        }
        case 'd':   //if it isn't too far right, moves sphere to the left
        {
            rightPressed = true;
            break;
        }
        case 'w':   //if it isn't too far up, moves sphere up
        {
            upPressed = true;
            break;
        }
        case 's':   //if it isn't too far down, moves sphere down
        {
            downPressed = true;
            break;
        }
    }
    glutPostRedisplay();
}

void specialInputUp(int key, int x, int y)
{
    switch(key)
    {
        case 'a':   //if it isn't too far left, moves sphere to the left
        {
            leftPressed = false;
            break;
        }
        case 'd':   //if it isn't too far right, moves sphere to the left
        {
            rightPressed = false;
            break;
        }
        case 'w':   //if it isn't too far up, moves sphere up
        {
            upPressed = false;
            break;
        }
        case 's':   //if it isn't too far down, moves sphere down
        {
            downPressed = false;
            break;
        }
    }
    glutPostRedisplay();
}





int main(int argc,char * argv[])
{
    srand(time(NULL));
    setUpAsteroids();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1000, 1000);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    glutDisplayFunc(display);
    glutSpecialFunc(specialInput);
    glutSpecialUpFunc(specialInputUp);
    glutIdleFunc(TimerFunction);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouse);
    initLighting();
    glEnable(GL_DEPTH_TEST);
    // Initialization
    glOrtho(0, 1000, 0, 1000, 0, 1000);
    glutAttachMenu(GLUT_LEFT_BUTTON);
    glutMainLoop();
    return 0;
}
