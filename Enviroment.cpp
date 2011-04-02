#include "Enviroment.h"

Enviroment * Enviroment::instance = NULL;

Enviroment::Enviroment(void) {
    // Vytvori kameru.
    camera = new Camera();

    // Nacte konfiguraci do prostredi a do kamery.
    loadConfig("config.xml");

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

void Enviroment::mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            Enviroment::getInstance()->mouseLeftPressed = true;
            Enviroment::getInstance()->lastCoordinate[0] = x;
            Enviroment::getInstance()->lastCoordinate[1] = y;
        } else {
            Enviroment::getInstance()->mouseLeftPressed = false;
        }
    }

}

void Enviroment::mouseMotion(int x, int y) {
    Enviroment* enviroment = Enviroment::getInstance();
    if (enviroment->mouseLeftPressed == true) {
        if (x > enviroment->lastCoordinate[0]) {
            enviroment->camera->turnLeft();
        } else {
            enviroment->camera->turnRight();
        }

        if (y > enviroment->lastCoordinate[1]) {
            enviroment->camera->turnDown();
        } else {
            enviroment->camera->turnUp();
        }
        enviroment->lastCoordinate[0] = x;
        enviroment->lastCoordinate[1] = y;
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
    // Nastavi pozici a orientaci kamery.
    Enviroment::getInstance()->camera->look();

    glPushMatrix();

    /* Move the car model to stand on the xz plane. */
    glTranslated(0.0, 0.25, 0.0);
    /* Draw the car model. */
    //drawModel( 0, "data/bronco.obj" );

    glPopMatrix();

    //drawCircle(R);
}

void Enviroment::loadConfig(const string& file) {
    // Nacte se XML soubor.
    try {
        read_xml(file, config);

        // Nastavi se promenne prostredi.
        windowWidth = config.get<int>("config.window.width");
        windowHeight = config.get<int>("config.window.height");
        windowTitle = config.get<string > ("config.window.title");

        // Nastavi se promenne kamere.
        camera->setPosition(
                config.get<float>("config.camera.position.x"),
                config.get<float>("config.camera.position.y"),
                config.get<float>("config.camera.position.z")
                );
        camera->setViewDirection(
                config.get<float>("config.camera.direction.x"),
                config.get<float>("config.camera.direction.y"),
                config.get<float>("config.camera.direction.z")
                );
        camera->setUpvector(
                config.get<float>("config.camera.upvector.x"),
                config.get<float>("config.camera.upvector.y"),
                config.get<float>("config.camera.upvector.z")
                );
        camera->setMotionDirection(
                config.get<float>("config.camera.motion.x"),
                config.get<float>("config.camera.motion.y"),
                config.get<float>("config.camera.motion.z")
                );
        camera->step = config.get<float>("config.camera.motion.step");
        camera->rotationAngleStep = config.get<float>("config.camera.motion.angle.rotation");
        camera->elevationAngleStep = config.get<float>("config.camera.motion.angle.elevation");

    } catch (xml_parser_error) {
        cerr << "Enviroment::loadConfig -> XML Parser Error." << endl;
    } catch (ptree_bad_path) {
        cerr << "Enviroment::loadConfig -> PTree Bad Path." << endl;
    } catch (ptree_bad_data) {
        cerr << "Enviroment::loadConfig -> PTree Bad Data." << endl;
    }
}
