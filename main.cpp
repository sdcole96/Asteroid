
#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "SOIL2.h"
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>

// Light source position
GLfloat light_position[] = {-1.0, 0.0, 2.0, 0.0};
GLfloat light_position2[] = {1.0, 0.0, 2.0, 0.0};
GLfloat light_position3[] = {0.0, 1.0, 2.0, 0.0};
GLfloat bigLightPos[] = {0.0, 0.0, -10.0,0.0};


double t = 0;
double earth = 1;
double scale = .01;
double speed = 10;
float translateX = 0.0;
float translateY = -1.0;
float sunRadius = 10;
float r = 0.0;
float g = 1.0;
float b = 0.0;
int numOfAsteroids = 9;
float r1 = 0.0;
float r2 = 0.0;
float r3 = 1.0;
float g1;
float g2;
float g3;
float asteroidX[9];	//arrays for to store asteroid coords
float asteroidY[9];
float asteroidZ[9];

int asteroidsPassed = 0;

bool upPressed = false;
bool downPressed = false;
bool leftPressed = false;
bool rightPressed = false;
bool gameOver = false;
float width;
float height;

GLUquadricObj *quadric;
GLuint earthTex;
GLuint sunTex;

using namespace std;

//handles various lights and updates with timerticks
void initLighting()
{
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat red[] = {r,g,b,0.0};
    GLfloat blue[] = {r1,r2,r3,0.0};
    GLfloat green[] = {g1, g2, g3, 0.0};
    GLfloat white[] = {1.0,1.0,1.0,0.0};
    // Set the light position
    glLightfv(GL_LIGHT0, GL_DIFFUSE,red);
    glLightfv(GL_LIGHT0, GL_AMBIENT, red);
    glLightfv(GL_LIGHT0, GL_POSITION,light_position);
    
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,blue);
    glLightfv(GL_LIGHT1, GL_POSITION,light_position2);
    
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,green);
    glLightfv(GL_LIGHT2, GL_POSITION,light_position3);
    
    glEnable(GL_LIGHT3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE,white);
    glLightfv(GL_LIGHT3, GL_AMBIENT, white);
    glLightfv(GL_LIGHT3, GL_EMISSION, white);
    glLightfv(GL_LIGHT3, GL_POSITION,bigLightPos);
}


//produces random number between min and max
float random(int min, int max) //create a random number in a specified range
{
    int randNum;
    randNum = rand()%(max-min + 1) + min;
    return randNum;
}


//initialise array of random coords for star within coordinate space
void setUpAsteroids()
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
        //adding texture to asteroid
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, sunTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gluQuadricTexture(quadric, 1);
        gluSphere(quadric, .1, 20, 10);
    }
    
    glEnd();
    glutSwapBuffers();
}

//displays the 3 orbiting mini spheres around the ship
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

//displays the actual ship
void createShip(GLfloat orbitRadius, GLfloat radius, GLfloat period) //what creates and updates the ship
{
    glLoadIdentity();
    glTranslatef(translateX,translateY+1,-1);
    glRotatef(90,0.0,1.0, 0.0);
    glRotatef(20*t,0.0,0.0,-2.0);
    
    //adding texture to ship
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earthTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gluQuadricTexture(quadric, 1);
    gluSphere(quadric,.1,50,10);
    
}

//displays score on screen
void glutStroke()
{
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(-1.8,1.2,-2.0);
    glScalef(0.005f, 0.005f, 0.005f);
    
    std::string s = std::to_string(asteroidsPassed);
    char const *pchar = s.c_str();
    
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, pchar[0]);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, pchar[1]);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, pchar[2]);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, pchar[3]);
    glPopMatrix();
    glutSwapBuffers();
    
    glEnd();
}

//initializes and calls necessary display methods
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
    //enable quadric
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluDeleteQuadric(quadric);
    glEnable(GL_FOG);
    glFogi(GL_FOG_START, -30.0);
    createShip(0,5,0); //mercury
    createPlanet(.1,.1,5);
    createAsteroids();
    glutStroke();
    glEnd();
    glutSwapBuffers();
}


//handles collision detection
void shipHasBeenHit()
{
    
    for(int i = 0; i < numOfAsteroids; i++)
    {
        bool xColl = false;
        bool yColl = false;
        float astY = asteroidY[i] + .1;
        float transY = translateY + .1 + 1;
        if(asteroidZ[i] > -1.1 && asteroidZ[i] < -.95)
        {
            if((asteroidX[i] - .1 < translateX + .1) && asteroidX[i] -.1 > translateX - .1)
            {
                xColl = true;
            }
            if((asteroidX[i] + .1 < translateX + .1) && asteroidX[i] +.1 > translateX - .1)
            {
                xColl = true;
            }
            if((asteroidY[i] + .1 < translateY + .1 + 1) && asteroidY[i] + .1 > translateY - .1 + 1 )
            {
                yColl = true;
            }
            if((asteroidY[i] - .1 < translateY + .1 +1) && asteroidY[i] - .1 > translateY - .1 + 1 )
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
    gluPerspective(90,w/h,.2,30);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//increments global variable t
void TimerFunction()
{
    r = cos(t);
    
    b = sin(t);
    r1 = 1.0;
    r2 = sin(-t);
    r3 = 1.0;
    g1 = 1.0;
    g2 = -sin(-t/2);
    g3 = -cos(t/2);
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
            if(asteroidZ[i] < 0)
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
    else
        glutStroke();
}

//starts game over
void restart()
{
    t = 0;
    setUpAsteroids();
    gameOver = false;
    glutPostRedisplay();
}

//takes keyboard input and flags a "holding" bool on
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


//takes keyboard release, using booleans to bypass the poor hold lag in openGL
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

//loads textures in
void loadTexture(GLuint* texture, char* path){
    *texture = SOIL_load_OGL_texture(path,
                                     SOIL_LOAD_AUTO,
                                     SOIL_CREATE_NEW_ID,
                                     SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
                                     );
    if(*texture == NULL){
        printf("[Texture loader] \"%s\" failed to load!\n", path);
    }
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
    initLighting();
    glEnable(GL_DEPTH_TEST);
    // Initialization
    glOrtho(0, 1000, 0, 1000, 0, 1000);
    glutAttachMenu(GLUT_LEFT_BUTTON);
    
    // ----- Texture stuff -------
    string str = "earth.bmp";
    char * writable = new char[str.size() + 1];
    copy(str.begin(), str.end(), writable);
    writable[str.size()] = '\0'; // don't forget the terminating 0
    // don't forget to free the string after finished using it
    string str2 = "sun.bmp";
    char * writable2 = new char[str2.size() + 1];
    copy(str2.begin(), str2.end(), writable2);
    writable2[str2.size()] = '\0';
    
    loadTexture(&earthTex, writable);
    loadTexture(&sunTex, writable2);
    //------------------------------
    
    glutMainLoop();
    delete[] writable;
    return 0;
}
