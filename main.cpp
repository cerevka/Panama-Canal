/* 
 * Semestral project for subject Y36PGR (Graphics programming).
 * Author: Tomas Cerevka
 *
 * Created on March 26, 2011, 2:10 PM
 */

#include <cstdlib>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include "Enviroment.h"

using namespace std;

/**
 * Enter point.
 * @param argc Count of arguments.
 * @param argv Arguments.
 * @return Return value.
 */
int main(int argc, char** argv) {
    Environment * environment = Environment::getInstance();   

    // Initialize the GLUT library.
    glutInit(&argc, argv);  

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(5, 5);
    glutInitWindowSize(environment->windowWidth, environment->windowHeight);

    // Create main window and set up callbacks.
    glutCreateWindow(environment->windowTitle.c_str());
    glutDisplayFunc(Environment::display);
    glutReshapeFunc(Environment::reshape);
    glutKeyboardFunc(Environment::keyboard);
    glutSpecialFunc(Environment::specialKeyboard);
    glutMouseFunc(Environment::mouse);
    glutMotionFunc(Environment::mouseMotion);
    glutIdleFunc(Environment::idle);
    
    environment->createMenu();

    environment->init();

    // Start main loop.
    glutMainLoop();

    delete environment;

    return 0;
}

