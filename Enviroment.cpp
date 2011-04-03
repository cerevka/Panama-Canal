#include "Enviroment.h"

Enviroment * Enviroment::instance = NULL;

Enviroment::Enviroment(void) {
    // Nacte konfiguraci do prostredi a do kamery.
    loadConfig("config.xml");

    // Vytvori kamery.
    for (int i = 0; i < CAMERA_COUNT; ++i) {
        string name = lexical_cast<string>(i);
        cameras[i] = new Camera(&config, name);
    }

    // Nastavi se kamera.
    camera = cameras[0];
    // Ulozi si ukazatel na sebe sama.
    instance = this;
    mouseLeftPressed = false;
}

Enviroment::~Enviroment(void) {
    delete camera;
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

void Enviroment::reshape(int _width, int _height) {
    // Nastaveni transformace viewportu.
    glViewport(0, 0, _width, _height);

    // Nastavi projekcni transoformaci - perspektivni projekce.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble) _width / _height, 0.1, 20.0);

    // Nastavi modelovaci a zobrazovaci transofmrace na identitu.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Enviroment::keyboard(unsigned char _key, int _x, int _y) {

    switch (_key) {
        case 'q': /* 'q' key pressed -> terminate application */
        case 'Q':
            exit(0);
            break;
    }
}

void Enviroment::specialKeyboard(int _key, int _x, int _y) {

    switch (_key) {
        case GLUT_KEY_RIGHT:
        {
            // Kamera se natoci doleva.
            Enviroment::getInstance()->camera->turnLeft();
            break;
        }
        case GLUT_KEY_LEFT:
        {
            // Kamera se natoci doprava.
            Enviroment::getInstance()->camera->turnRight();
            break;
        }
        case GLUT_KEY_UP:
        {
            // Kamera se posune dopredu.
            Enviroment::getInstance()->camera->stepForward();
            break;
        }
        case GLUT_KEY_DOWN:
        {
            // Kamera se posune dozadu.
            Enviroment::getInstance()->camera->stepBackward();
            break;
        }
        case GLUT_KEY_PAGE_UP:
        {
            // Kamera zvedne pohled.
            Enviroment::getInstance()->camera->turnUp();
            break;
        }
        case GLUT_KEY_PAGE_DOWN:
        {
            // Kamera skloni pohled.
            Enviroment::getInstance()->camera->turnDown();
            break;
        }
        default:
            return;
    }
    glutPostRedisplay();
}

void Enviroment::mouse(int _button, int _state, int _x, int _y) {
    if (_button == GLUT_LEFT_BUTTON) {
        if (_state == GLUT_DOWN) {
            Enviroment::getInstance()->mouseLeftPressed = true;
            Enviroment::getInstance()->lastCoordinate[0] = _x;
            Enviroment::getInstance()->lastCoordinate[1] = _y;
        } else {
            Enviroment::getInstance()->mouseLeftPressed = false;
        }
    }

}

void Enviroment::mouseMotion(int _x, int _y) {
    Enviroment* enviroment = Enviroment::getInstance();
    if (enviroment->mouseLeftPressed == true) {
        if (_x > enviroment->lastCoordinate[0]) {
            enviroment->camera->turnLeft();
        } else {
            enviroment->camera->turnRight();
        }

        if (_y > enviroment->lastCoordinate[1]) {
            enviroment->camera->turnDown();
        } else {
            enviroment->camera->turnUp();
        }
        enviroment->lastCoordinate[0] = _x;
        enviroment->lastCoordinate[1] = _y;
    }
    glutPostRedisplay();
}

void Enviroment::idle(void) {
    /* Generate event to redraw the window. */
    glutPostRedisplay();
}

void Enviroment::menu(int _selectedItem) {
    Enviroment* enviroment = Enviroment::getInstance();
    switch (_selectedItem) {
        case 0:
            exit(0);
            break;
        case 11:
            enviroment->camera = enviroment->cameras[0];
            break;
        case 12:
            enviroment->camera = enviroment->cameras[1];
            break;
        case 13:
            enviroment->camera = enviroment->cameras[2];
    }
    glutPostRedisplay();
}

void Enviroment::init(void) {
    glEnable(GL_DEPTH_TEST);

    // Predni strany polygonu jsou vyplneny, zadni strany maji jen obrysy.
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    // Zahazuji se zadni strany polygony.
    glCullFace(GL_BACK);

    // Zapnuti osvetleni.
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
}

void Enviroment::createMenu(void) {
    // Vytvoreni podmenicek.
    int cameraMenu = glutCreateMenu(Enviroment::menu);
    glutAddMenuEntry("Static view 1", 11);
    glutAddMenuEntry("Static view 2", 12);
    glutAddMenuEntry("Walk", 13);

    // Vytvoreni hlavniho menu.
    glutCreateMenu(Enviroment::menu);
    glutAddSubMenu("Camera", cameraMenu);
    glutAddMenuEntry("Quit", 0);

    // Menu se bude otevirat pravym mysitkem.
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Enviroment::drawScene(void) {
    // Nastavi pozici a orientaci kamery.
    Enviroment::getInstance()->camera->look();

    //glPushMatrix();

    /* Move the car model to stand on the xz plane. */
    //glTranslated(0.0, 0.25, 0.0);

    /* Draw the car model. */
    //drawModel( 0, "data/bronco.obj" );

    //glPopMatrix();

    //drawCircle(R);
}

void Enviroment::drawAxes(float _length) {
    glDisable(GL_LIGHTING);

    /* Draw x-axis. */
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(_length, 0.0, 0.0);
    glEnd();

    /* Draw y-axis. */
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, _length, 0.0);
    glEnd();

    /* Draw z-axis. */
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, _length);
    glEnd();

    glEnable(GL_LIGHTING);
}

void Enviroment::loadConfig(const string& _file) {
    // Nacte se XML soubor.
    try {
        read_xml(_file, config);

        // Nastavi se promenne prostredi.
        windowWidth = config.get<int>("config.window.width");
        windowHeight = config.get<int>("config.window.height");
        windowTitle = config.get<string > ("config.window.title"); 
    } catch (xml_parser_error) {
        cerr << "Enviroment::loadConfig -> XML Parser Error." << endl;
    } catch (ptree_bad_path) {
        cerr << "Enviroment::loadConfig -> PTree Bad Path." << endl;
    } catch (ptree_bad_data) {
        cerr << "Enviroment::loadConfig -> PTree Bad Data." << endl;
    }
}
