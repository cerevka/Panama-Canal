#include "Enviroment.h"

Enviroment * Enviroment::instance = NULL;

Enviroment::Enviroment(void) {
    instance = this;
}

Enviroment::~Enviroment(void) {
}

Enviroment * Enviroment::getInstance(void) {
    if (instance == NULL) {
        instance = new Enviroment();
    }
    return instance;
}

void Enviroment::display(void) {
    // Vycisti okno.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Zacatek kresleni sceny.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Zavola funkci kreslici scenu.
    Enviroment::instance->drawScene();

    // Nakresli zem - zeleny obdelnik v rovine XZ.
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glColor3d(0.3, 0.8, 0.3);
    glVertex3d(-5.0, 0.0, -5.0);
    glVertex3d(-5.0, 0.0, 5.0);
    glVertex3d(5.0, 0.0, 5.0);
    glVertex3d(5.0, 0.0, -5.0);
    glEnd();
    glEnable(GL_LIGHTING);

    glPopMatrix();

    // Konec kresleni sceny.

    // Prepne buffery (double buffering).
    glutSwapBuffers();
}

void Enviroment::reshape(int width, int height) {
    // Nastaveni transformace viewportu.
    glViewport(0, 0, width, height);

    // Nastavi projekcni transoformaci - perspektivni projekce.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble) width / height, 0.1, 20.0);

    // Nastavi modelovaci a zobrazovaci transofmrace na identitu.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Enviroment::keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case 'q': /* 'q' key pressed -> terminate application */
        case 'Q':
            exit(0);
            break;
    }
}

void Enviroment::specialKeyboard(int key, int x, int y) {

    switch (key) {
        case GLUT_KEY_RIGHT:
        { // rotate camera to the left            
            break;
        }
        case GLUT_KEY_LEFT:
        { // rotate camera to the right            
            break;
        }
        case GLUT_KEY_UP:
        { // move camera forward
            break;
        }
        case GLUT_KEY_DOWN:
        { // move camera backward            
            break;
        }
        case GLUT_KEY_PAGE_UP:
        { // rotate camera view direction up
            break;
        }
        case GLUT_KEY_PAGE_DOWN:
        { // rotate camera view direction up
            break;
        }
        default:
            return;
    }
    glutPostRedisplay();
}

void Enviroment::idle(void) {


    /* Generate event to redraw the window. */
    glutPostRedisplay();
}

void Enviroment::menu(int menuItem) {

    switch (menuItem) {
        case 1:
            break;
    }
}


void Enviroment::init(void) {
    glEnable(GL_DEPTH_TEST);

    // Predni strany polygonu jsou vyplneny, zadni strany maji jen obrysy.
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    // Zahazuji se zadni strany polygonu.
    glCullFace(GL_BACK);

    // Zapnuti osvetleni.
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
}

void Enviroment::drawScene(void) {
    // Nastavni pozici a orientaci kamery.
    gluLookAt(
            0.0, 2.0, 8.0, // eye = position
            0.0, 0.0, 0.0, // center = point onto which we are looking (view direction = eye-center)
            0.0, 1.0, 0.0 // up vector (camera orientation along the view direction)
            );

    glPushMatrix();

    /* Move the car model to stand on the xz plane. */
    glTranslated(0.0, 0.25, 0.0);
    /* Draw the car model. */
    //drawModel( 0, "data/bronco.obj" );

    glPopMatrix();

    //drawCircle(R);
}
