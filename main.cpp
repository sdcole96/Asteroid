

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "imageloader.h"
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

GLUquadricObj *quadric;

float r = 0.0;
float g = 1.0;
float b = 0.0;
int numOfAsteroids = 9;
float asteroidX[9];	//arrays for to store asteroid coords
float asteroidY[9];
float asteroidZ[9];

int asteroidsPassed = 0;



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
    GLfloat red[] = {r,g,b,0.0};
    // Set the light position
    glLightfv(GL_LIGHT0, GL_DIFFUSE,red);
    glLightfv(GL_LIGHT0, GL_POSITION,light_position);
}

void updateLighting()
{
    
}

//function to make the texture on planet
GLuint loadTexture(Image* image){
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    return textureId;
}

//integers for each texture
GLuint earTexture, sunTexture;

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
        glLoadIdentity();
        glTranslatef(asteroidX[i],asteroidY[i],asteroidZ[i]);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, sunTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gluQuadricTexture(quadric, 1);
        gluSphere(quadric, .1, 10, 10);
        //glutSolidSphere(.1,10,10);
        glDisable(GL_TEXTURE_2D);
        
    }
    
    glEnd();
    glutSwapBuffers();
}


void createPlanet(GLfloat orbitRadius, GLfloat radius, GLfloat period) //random orbit guyo
{
    glLoadIdentity();
    glTranslatef(cos(t)/5+ translateX,translateY+1,-1 + sin(t)/5);
    glutSolidSphere(radius*.1, 10, 2);
    glLoadIdentity();
    glTranslatef(cos(t+180)/5+ translateX,translateY+1,-1 + sin(t+180)/5);
    glutSolidSphere(radius*.1, 10, 2);
      glLoadIdentity();
    glTranslatef(cos(t+90)/5+ translateX,translateY+1,-1 + sin(t+90)/5);
    glutWireSphere(radius*.1, 10, 2);
}

void createShip(GLfloat orbitRadius, GLfloat radius, GLfloat period) //what creates and updates the ship
{
    glLoadIdentity();
    glTranslatef(translateX,translateY+1,-1);
    glRotatef(90,0.0,1.0, 0.0);
    glRotatef(20*t,0.0,0.0,-2.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gluQuadricTexture(quadric, 1);
    
    gluSphere(quadric,.1,50,10);
    //glutWireSphere(quadric,.1,50,10);
    glDisable(GL_TEXTURE_2D);
    
    //glutWireSphere(.1,50,10);
}



void display(void)
{
    glLoadIdentity();
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat pos[] = { 0.0, 0.0, 1.0, 0.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Image* ear = loadBMP("earth.bmp");		earTexture = loadTexture(ear);
    Image* sun = loadBMP("sun.bmp");		sunTexture = loadTexture(sun);
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluDeleteQuadric(quadric);
    glEnable(GL_FOG);
    glFogi(GL_FOG_START, -30.0);
    createShip(0,5,0); //mercury
    createPlanet(.1,.1,5);
    createAsteroids();
    
    glEnd();
    glutSwapBuffers();
}

void createStars(int intensity)
{
    
}

void shipHasBeenHit()
{
    
    for(int i = 0; i < numOfAsteroids; i++)
    {
        bool xColl = false;
        bool yColl = false;
        if(asteroidZ[i] > -1.1 && asteroidZ[i] < -.90)
        {
            if((asteroidX[i] - .1 < translateX + .1) && asteroidX[i] -.1 > translateX - .1)
            {
                xColl = true;
            }
            if((asteroidX[i] + .1 < translateX + .1) && asteroidX[i] +.1 > translateX - .1)
            {
                xColl = true;
            }
            if((asteroidY[i] + .1 < translateY + .1 + 1) && asteroidY[i] + .1 > translateY + .1 - 1 )
            {
                yColl = true;
            }
            if((asteroidY[i] - .1 < translateY + .1 +1) && asteroidY[i] - .1 > translateY + .1 - 1 )
            {
                yColl = true;
            }
        }
        if(xColl && yColl)
        {
            gameOver = true;
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
    r = cos(t/50);
    b = sin(t/50);
    initLighting();
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
                asteroidZ[i] += .1 + t*(.001);
            }
            else
            {
                asteroidX[i] = random(-50,50)/100.0; // -.5 - .5
                asteroidY[i] = random(-100,100)/200; //-1.0 - 1.0
                asteroidZ[i] -= 10;
                ++asteroidsPassed;
            }
   
    }
     
    glutPostRedisplay();
    glutSwapBuffers();
    }
}

void mouse(int x, int y)
{

}

void restart()
{
    t = 0;
    setUpAsteroids();
    gameOver = false;
    glutPostRedisplay();
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
        case ' ':
        {
            if(gameOver)
            {
                restart();
            }
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
