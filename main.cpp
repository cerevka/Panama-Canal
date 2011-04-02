/* 
 * Semestralni prace pro predmet Y36PGR (Programování grafiky).
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
 * Vstupni bod programu.
 * @param argc Pocet argumentu.
 * @param argv Argumenty.
 * @return Navratova hodnota.
 */
int main(int argc, char** argv) {
    Enviroment * enviroment = Enviroment::getInstance();   

    /* Initialize the GLUT library. */
    glutInit(&argc, argv);  

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(5, 5);
    glutInitWindowSize(enviroment->windowWidth, enviroment->windowHeight);

    // Vytvoreni hlavniho okna a nastaveni callbacku.
    glutCreateWindow(enviroment->windowTitle.c_str());
    glutDisplayFunc(Enviroment::display);
    glutReshapeFunc(Enviroment::reshape);
    glutKeyboardFunc(Enviroment::keyboard);
    glutSpecialFunc(Enviroment::specialKeyboard);
    //glutIdleFunc(Enviroment::idle);

    // Vytvoreni hlavniho menu.
    glutCreateMenu(Enviroment::menu);
    glutAddMenuEntry("Task 1 - Circular motion of car", 1);
    glutAddMenuEntry("Task 2 - Circular motion of camera", 2);
    glutAddMenuEntry("Task 3 - Free motion in xz-plane (WALK mode)", 3);
    glutAddMenuEntry("Bonus - WALK mode + up/down", 4);
    glutAddMenuEntry("Quit", 5);

    // Menu se bude otevirat pravym mysitkem.
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    enviroment->init(); 

    // Spusti hlavni smysku.
    glutMainLoop();   

    delete enviroment;

    return 0;
}

