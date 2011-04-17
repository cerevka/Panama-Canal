#include "Enviroment.h"

Enviroment * Enviroment::instance = NULL;

Enviroment::Enviroment(void) {
    // Nacte konfiguraci do prostredi a do kamery.
    loadConfig("config.xml");

    // Ulozi si ukazatel na sebe sama.
    instance = this;
    mouseLeftPressed = false;
    sunAngle = 90.0;
    dynamicViewAngle = 0.0;
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
    Enviroment * enviroment = Enviroment::instance;
    switch (_key) {
        case GLUT_KEY_RIGHT:
        {
            // Kamera se natoci doleva.
            enviroment->camera->turnLeft();
            break;
        }
        case GLUT_KEY_LEFT:
        {
            // Kamera se natoci doprava.
            enviroment->camera->turnRight();
            break;
        }
        case GLUT_KEY_UP:
        {
            // Kamera se posune dopredu.
            enviroment->camera->stepForward();
            break;
        }
        case GLUT_KEY_DOWN:
        {
            // Kamera se posune dozadu.
            enviroment->camera->stepBackward();
            break;
        }
        case GLUT_KEY_PAGE_UP:
        {
            // Kamera zvedne pohled.
            enviroment->camera->turnUp();
            break;
        }
        case GLUT_KEY_PAGE_DOWN:
        {
            // Kamera skloni pohled.
            enviroment->camera->turnDown();
            break;
        }
        default:
            return;
    }
    glutPostRedisplay();
}

void Enviroment::mouse(int _button, int _state, int _x, int _y) {
    Enviroment * enviroment = Enviroment::instance;
    if (_button == GLUT_LEFT_BUTTON) {
        if (_state == GLUT_DOWN) {
            enviroment->mouseLeftPressed = true;
            enviroment->lastCoordinate[0] = _x;
            enviroment->lastCoordinate[1] = _y;
        } else {
            enviroment->mouseLeftPressed = false;
        }
    }

}

void Enviroment::mouseMotion(int _x, int _y) {
    Enviroment* enviroment = Enviroment::instance;
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
    Enviroment* enviroment = Enviroment::instance;
    /*
    if (enviroment->dynamicView == true) {
        enviroment->dynamicViewAngle += 0.3;
        //GLfloat* position = enviroment->camera->getPosition();
        enviroment->camera->setPosition(cos(enviroment->dynamicViewAngle * DEG_TO_RAD) * 5, 5, sin(enviroment->dynamicViewAngle * DEG_TO_RAD) * 5);
        enviroment->camera->setViewDirection(0.0, 0.0, 0.0);
    }*/
    // TODO pohyb kamery

    // Prekresluje.
    glutPostRedisplay();
}

void Enviroment::menu(int _selectedItem) {
    Enviroment* enviroment = Enviroment::instance;
    switch (_selectedItem) {
        case 0:
            exit(0);
            break;
        case 11:
            // Staticky pohled 1.
            enviroment->camera = enviroment->cameras[0];
            break;
        case 12:
            // Staticky pohled 2.
            enviroment->camera = enviroment->cameras[1];
            break;
        case 13:
            // Dynamicky pohled.
            enviroment->camera = enviroment->cameras[2];
            break;
        case 14:
            // Chodici kamera.
            enviroment->cameras[3]->loadConfig(enviroment->camera);
            enviroment->camera = enviroment->cameras[3];
            break;
        case 15:
            // Volna kamera.
            enviroment->cameras[4]->loadConfig(enviroment->camera);
            enviroment->camera = enviroment->cameras[4];
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
            // Ovladani baterky.
            if (enviroment->lights[1]->getState() == true) {
                enviroment->lights[1]->switchOff();
            } else {
                enviroment->lights[1]->switchOn();
            }
    }
    glutPostRedisplay();
}

float Enviroment::DegToRad(float _deg) {
    return _deg * (M_PI / 180);
}

float Enviroment::RadToDeg(float _rad) {
    return _rad * (180 / M_PI);
}

void Enviroment::init(void) {
    glEnable(GL_DEPTH_TEST);

    // Predni strany polygonu jsou vyplneny, zadni strany maji jen obrysy.
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    // Zahazuji se zadni strany polygony.
    //glCullFace(GL_BACK);

    // Vytvori kamery.
    for (int i = 0; i < CAMERA_COUNT; ++i) {
        try {
            string name = lexical_cast<string > (i);
            cameras[i] = new Camera(name, config);
        } catch (bad_lexical_cast exception) {
            cerr << "Enviroment::constructor -> Bad Lexical Cast." << endl;
            exit(1);
        }
    }
    // Nastavi se aktualni kamera.
    camera = cameras[3];

    // Zapnuti osvetleni.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        lights[i] = new Light(i, config);
    }
}

void Enviroment::createMenu(void) {
    // Vytvoreni podmenicek.
    int cameraMenu = glutCreateMenu(Enviroment::menu);
    glutAddMenuEntry("Static view 1", 11);
    glutAddMenuEntry("Static view 2", 12);
    glutAddMenuEntry("Dynamic view", 13);
    glutAddMenuEntry("Walk", 14);
    glutAddMenuEntry("Free camera", 15);

    int lightMenu = glutCreateMenu(Enviroment::menu);
    glutAddMenuEntry("Switch on/off sun", 21);
    glutAddMenuEntry("Switch on/off flashlight", 22);

    // Vytvoreni hlavniho menu.
    glutCreateMenu(Enviroment::menu);
    glutAddSubMenu("Camera", cameraMenu);
    glutAddSubMenu("Lights", lightMenu);
    glutAddMenuEntry("Quit", 0);

    // Menu se bude otevirat pravym mysitkem.
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Enviroment::drawScene(void) {

    Enviroment * enviroment = Enviroment::instance;
    // Nastavi pozici a orientaci kamery.
    glLoadIdentity();
    enviroment->camera->look();

    //cout << *camera;

    // Repozicuji se staticka svetla.
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        if (lights[i]->dynamic == false) {
            lights[i]->rePosition();
            lights[i]->reSpotDirection();
        }
    }

    // Nakresli se podklad.
    glPushMatrix();
    drawPlane(100);
    glPopMatrix();

    // Nakresli cajnik.
    glPushMatrix();
    glTranslated(0.0, 0.0, 0.0);
    GLfloat ambient[] = {0.4, 0.4, 0.4, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    GLfloat diffuse[] = {0.8, 0.8, 0.8, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

    //glutSolidTeapot(0.5);
    drawShip();
    drawAxes(2.0);
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
        exit(1);
    } catch (ptree_bad_path exception) {
        cerr << "Enviroment::loadConfig -> PTree Bad Path." << endl;
        exit(1);
    } catch (ptree_bad_data exception) {
        cerr << "Enviroment::loadConfig -> PTree Bad Data." << endl;
        exit(1);
    }
}

/**
 * Vykresli zeleny podklad jako mrizku.
 * @param subdiv Pocet casti.
 */
void Enviroment::drawPlane(int subdiv) {
    const float size = 12; // plane size

    glNormal3f(0.0, 1.0, 0.0);

    GLfloat ambient[] = {0.02f, 0.52f, 0.02f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    GLfloat diffuse[] = {0.01f, 0.51f, 0.01f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    GLfloat specular[] = {0.40f, 0.40f, 0.40f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 10.f);

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

void Enviroment::drawShip(void) {
    //glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // Dno.
    {
        GLfloat verteces[] = {
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -0.3, 0.0, -0.75,
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, +0.3, 0.0, -0.75,
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, +0.3, 0.0, 0.25,
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -0.3, 0.0, 0.25
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Dno spicky.
    {
        GLfloat verteces[] = {
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -0.3, 0.0, 0.25,
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.3, 0.0, 0.25,
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.75
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    // Zadni stena
    {
        GLfloat verteces[] = {
            1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, -0.3, 0.2, -0.75,
            1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, -0.3, 0.0, -0.75,
            1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.3, 0.0, -0.75,
            1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.3, 0.2, -0.75
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Prava stena.
    {
        GLfloat verteces[] = {
            1.0, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, -0.3, 0.2, -0.75,
            1.0, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, -0.3, 0.0, -0.75,
            1.0, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, -0.3, 0.0, 0.25,
            1.0, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, -0.3, 0.2, 0.25
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Leva stena.
    {
        GLfloat verteces[] = {
            1.0, 0.5, 0.0, 0.0, -1.0, 0.0, 0.0, 0.3, 0.2, -0.75,
            1.0, 0.5, 0.0, 0.0, -1.0, 0.0, 0.0, 0.3, 0.0, -0.75,
            1.0, 0.5, 0.0, 0.0, -1.0, 0.0, 0.0, 0.3, 0.0, 0.25,
            1.0, 0.5, 0.0, 0.0, -1.0, 0.0, 0.0, 0.3, 0.2, 0.25
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Prava spicka.
    {
        GLfloat verteces[] = {
            0.0, 0.5, 0.5, 0.0, 1.0, 0.0, 0.6, -0.3, 0.2, 0.25,
            0.0, 0.5, 0.5, 0.0, 1.0, 0.0, 0.6, -0.3, 0.0, 0.25,
            0.0, 0.5, 0.5, 0.0, 1.0, 0.0, 0.6, 0.0, 0.0, 0.75,
            0.0, 0.5, 0.5, 0.0, 1.0, 0.0, 0.6, 0.0, 0.2, 0.75
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Leva spicka.
    {
        GLfloat verteces[] = {
            0.5, 0.0, 0.5, 0.0, -1.0, 0.0, 0.6, 0.3, 0.2, 0.25,
            0.5, 0.0, 0.5, 0.0, -1.0, 0.0, 0.6, 0.3, 0.0, 0.25,
            0.5, 0.0, 0.5, 0.0, -1.0, 0.0, 0.6, 0.0, 0.0, 0.75,
            0.5, 0.0, 0.5, 0.0, -1.0, 0.0, 0.6, 0.0, 0.2, 0.75
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Vnitrni zadni stena.
    {
        GLfloat verteces[] = {
            0.5, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, -0.25, 0.2, -0.7,
            0.5, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, -0.25, 0.05, -0.7,
            0.5, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.25, 0.05, -0.7,
            0.5, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.25, 0.2, -0.7
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Vnitrni prava stena.
    {
        GLfloat verteces[] = {
            0.0, 0.5, 0.3, 0.0, -1.0, 0.0, 0.0, -0.25, 0.2, -0.7,
            0.0, 0.5, 0.3, 0.0, -1.0, 0.0, 0.0, -0.25, 0.05, -0.7,
            0.0, 0.5, 0.3, 0.0, -1.0, 0.0, 0.0, -0.25, 0.05, 0.2,
            0.0, 0.5, 0.3, 0.0, -1.0, 0.0, 0.0, -0.25, 0.2, 0.2
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Vnitrni leva stena.
    {
        GLfloat verteces[] = {
            0.0, 0.5, 0.3, 0.0, 1.0, 0.0, 0.0, 0.25, 0.2, -0.7,
            0.0, 0.5, 0.3, 0.0, 1.0, 0.0, 0.0, 0.25, 0.05, -0.7,
            0.0, 0.5, 0.3, 0.0, 1.0, 0.0, 0.0, 0.25, 0.05, 0.2,
            0.0, 0.5, 0.3, 0.0, 1.0, 0.0, 0.0, 0.25, 0.2, 0.2
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Vnitrni dno.
    {
        GLfloat verteces[] = {
            0.5, 0.5, 0.3, 0.0, 0.0, 1.0, 0.0, -0.25, 0.05, -0.7,
            0.5, 0.5, 0.3, 0.0, 0.0, 1.0, 0.0, 0.25, 0.05, -0.7,
            0.5, 0.5, 0.3, 0.0, 0.0, 1.0, 0.0, 0.25, 0.05, 0.2,
            0.5, 0.5, 0.3, 0.0, 0.0, 1.0, 0.0, -0.25, 0.05, 0.2
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Predni stena vnitrku.
    {
        GLfloat verteces[] = {
            0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, -0.25, 0.2, 0.2,
            0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, -0.25, 0.05, 0.2,
            0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.25, 0.05, 0.2,
            0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.25, 0.2, 0.2
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Vrchni plocha vzadu.
    {
        GLfloat verteces[] = {
            0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, -0.3, 0.2, -0.75,
            0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.3, 0.2, -0.75,
            0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.25, 0.2, -0.70,
            0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, -0.25, 0.2, -0.70
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Vrchni plocha vpravo.
    {
        GLfloat verteces[] = {
            0.0, 0.3, 1.0, 0.0, 0.0, 1.0, 0.0, -0.25, 0.2, -0.7,
            0.0, 0.3, 1.0, 0.0, 0.0, 1.0, 0.0, -0.25, 0.2, 0.2,
            0.0, 0.3, 1.0, 0.0, 0.0, 1.0, 0.0, -0.3, 0.2, 0.25,
            0.0, 0.3, 1.0, 0.0, 0.0, 1.0, 0.0, -0.3, 0.2, -0.75
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Vrchni plocha vlevo.
    {
        GLfloat verteces[] = {
            0.0, 0.3, 1.0, 0.0, 0.0, 1.0, 0.0, 0.3, 0.2, -0.75,
            0.0, 0.3, 1.0, 0.0, 0.0, 1.0, 0.0, 0.3, 0.2, 0.25,
            0.0, 0.3, 1.0, 0.0, 0.0, 1.0, 0.0, 0.25, 0.2, 0.20,
            0.0, 0.3, 1.0, 0.0, 0.0, 1.0, 0.0, 0.25, 0.2, -0.70
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Vrchni plocha vpredu.
    {
        GLfloat verteces[] = {
            0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, -0.25, 0.2, 0.2,
            0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, -0.3, 0.2, 0.25,
            0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.3, 0.2, 0.25,
            0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.25, 0.2, 0.2
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    // Vrchni plocha spicky.
    {
        GLfloat verteces[] = {
            0.0, 0.5, 0.5, 0.0, 0.0, 1.0, 0.0, -0.3, 0.2, 0.25,
            0.0, 0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 0.3, 0.2, 0.25,
            0.0, 0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.2, 0.75
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }










    glDisable(GL_COLOR_MATERIAL);
    //glEnable(GL_LIGHTING);
}
