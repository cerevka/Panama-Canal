#include "Enviroment.h"

Enviroment * Enviroment::instance = NULL;

Enviroment::Enviroment(void) {
    // Nacte konfiguraci do prostredi a do kamery.
    loadConfig("config.xml");

    // Ulozi si ukazatel na sebe sama.
    instance = this;
    mouseLeftPressed = false;
    sunAngle = 90.0;
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
            break;
        case 21:
            // Zapinani a vypinani slunce.
            if (enviroment->lights[0]->getState() == true) {
                enviroment->lights[0]->switchOff();
            } else {
                enviroment->lights[0]->switchOn();
            }
            break;
        case 22:
            // Ovladani svetel majaku.
            if (enviroment->lights[1]->getState() == true) {
                enviroment->lights[1]->switchOff();
            } else {
                enviroment->lights[1]->switchOn();
            }
    }
    glutPostRedisplay();
}

void Enviroment::init(void) {
    glEnable(GL_DEPTH_TEST);

    // Predni strany polygonu jsou vyplneny, zadni strany maji jen obrysy.
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    // Zahazuji se zadni strany polygony.
    glCullFace(GL_BACK);

    // Zapnuti osvetleni.
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);

    // Vytvori kamery.
    for (int i = 0; i < CAMERA_COUNT; ++i) {
        try {
            string name = lexical_cast<string > (i);
            cameras[i] = new Camera(&config, name);
        } catch (bad_lexical_cast exception) {
            cerr << "Enviroment::constructor -> Bad Lexical Cast." << endl;
        }
    }
    // Nastavi se aktualni kamera.
    camera = cameras[2];

    // Inicializuji se svetla.
    initLights();
}

void Enviroment::initLights(void) {
    // Vytvori se slunce.
    lights[0] = new Light(0);
    lights[0]->setPosition(1.0, 1.0, 0.0);
    lights[0]->setAmbient(0.0, 0.0, 0.0, 1.0);
    lights[0]->setDiffuse(1.0, 1.0, 1.0, 1.0);
    lights[0]->setSpecular(1.0, 1.0, 1.0);
    //lights[0]->switchOn();

    // Vytvori se reflektor.
    lights[1] = new Light(1);
    lights[1]->setPosition(0.0, 0.0, 0.0, 1.0);
    lights[1]->setAmbient(0.0, 0.0, 0.0, 1.0);
    lights[1]->setDiffuse(1.0, 1.0, 1.0, 1.0);
    lights[1]->setSpecular(0.0, 0.0, 0.0, 1.0);
    lights[1]->setSpotCutOff(10.0);
    lights[1]->switchOn();


}

void Enviroment::createMenu(void) {
    // Vytvoreni podmenicek.
    int cameraMenu = glutCreateMenu(Enviroment::menu);
    glutAddMenuEntry("Static view 1", 11);
    glutAddMenuEntry("Static view 2", 12);
    glutAddMenuEntry("Walk", 13);

    int lightMenu = glutCreateMenu(Enviroment::menu);
    glutAddMenuEntry("Switch on/off sun", 21);
    glutAddMenuEntry("Switch on/off lighthouse", 22);

    // Vytvoreni hlavniho menu.
    glutCreateMenu(Enviroment::menu);
    glutAddSubMenu("Camera", cameraMenu);
    glutAddSubMenu("Lights", lightMenu);
    glutAddMenuEntry("Quit", 0);

    // Menu se bude otevirat pravym mysitkem.
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Enviroment::drawScene(void) {
    // Nastavi pozici a orientaci kamery.
    camera->look();

    cout << *(lights[1]);

    glPushMatrix();
    glLoadIdentity();
    lights[1]->setPosition(0, 0, 0, 1);
    glPopMatrix();

    // Nakresli zem - zeleny obdelnik v rovine XZ.
    /*
    glNormal3f(0.0, 1.0, 0.0);

    glBegin(GL_QUADS);
    
    glVertex3d(-5.0, 0.0, -5.0);
    glVertex3d(-5.0, 0.0, 5.0);
    glVertex3d(5.0, 0.0, 5.0);
    glVertex3d(5.0, 0.0, -5.0);
    glEnd();
     */
    glColor3d(0.3, 0.8, 0.3);
    drawPlane(100);

    // Nakresli cajnik.
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslated(0.0, 0.5, 0.0);
    glutSolidTeapot(0.5);
    drawAxes(1.0);
    glPopMatrix();
}

void Enviroment::drawAxes(float _length) {
    glDisable(GL_LIGHTING);

    // Nakresli osu x.
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(_length, 0.0, 0.0);
    glEnd();

    // Nakresli osu y.
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, _length, 0.0);
    glEnd();

    // Nakresli osu z.
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
    } catch (xml_parser_error exception) {
        cerr << "Enviroment::loadConfig -> XML Parser Error." << endl;
    } catch (ptree_bad_path exception) {
        cerr << "Enviroment::loadConfig -> PTree Bad Path." << endl;
    } catch (ptree_bad_data exception) {
        cerr << "Enviroment::loadConfig -> PTree Bad Data." << endl;
    }
}

/// Draws a green plane (the ground) under car as a grid.

/**
 \param[in]  subdiv		Number of splits in plain subdivision => grid resolution is subdiv x subdiv.
 \param[in]  index		Material index (see material.inc).
 */
void Enviroment::drawPlane(int subdiv) {
    const float size = 12; // plane size
   
    glNormal3f(0.0, 1.0, 0.0);

    
    float d = size / subdiv;
    float start = -size / 2.0;

    float x = start;
    for (int i = 0; i < subdiv; i++) {
        float z = start;

        glBegin(GL_QUAD_STRIP);

        glVertex3d(x + d, 0.0, z);
        glVertex3d(x, 0.0, z);

        for (int j = 1; j < subdiv; j++) {
            glVertex3d(x + d, 0.0, z + d);
            glVertex3d(x, 0.0, z + d);
            z += d;
        }
        x += d;

        glEnd();
    }
}
