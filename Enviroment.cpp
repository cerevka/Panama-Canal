#include "Enviroment.h"

Environment * Environment::instance = NULL;
float Environment::dynamicViewAngle = 0.0;

Environment::Environment(void) {
    // Load configuration.
    loadConfig("config.xml");

    instance = this;
    mouseLeftPressed = false;
    sunAngle = 90.0;    
}

Environment::~Environment(void) {
    delete camera;
}

Environment * Environment::getInstance(void) {
    if (instance == NULL) {
        instance = new Environment();
    }
    return instance;
}

void Environment::display(void) {
    // Clear window.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start draw scene.
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    Environment::instance->drawScene();
    glPopMatrix();

    // End draw scene.

    // Switch buffers (double buffering).
    glutSwapBuffers();
}

void Environment::reshape(int _width, int _height) {
    // Set up viewport transformation.
    glViewport(0, 0, _width, _height);

    // Set up perspective transformation - perspective projection.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble) _width / _height, 0.1, 20.0);

    // Set up model-view matrix to identity.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Environment::keyboard(unsigned char _key, int _x, int _y) {
    switch (_key) {
        case 'q':
        case 'Q':
            // Terminate application.
            exit(0);
            break;
    }
}

void Environment::specialKeyboard(int _key, int _x, int _y) {
    Environment * enviroment = Environment::instance;
    switch (_key) {
        case GLUT_KEY_RIGHT:
        {
            // Camera turns left.
            enviroment->camera->turnLeft();
            break;
        }
        case GLUT_KEY_LEFT:
        {
            // Camera turns right.
            enviroment->camera->turnRight();
            break;
        }
        case GLUT_KEY_UP:
        {
            // Camera goes forward.
            enviroment->camera->stepForward();
            break;
        }
        case GLUT_KEY_DOWN:
        {
            // Camera goes back.
            enviroment->camera->stepBackward();
            break;
        }
        case GLUT_KEY_PAGE_UP:
        {
            // Camera picks up view.
            enviroment->camera->turnUp();
            break;
        }
        case GLUT_KEY_PAGE_DOWN:
        {
            // Camera puts down view. 
            enviroment->camera->turnDown();
            break;
        }
        default:
            return;
    }
    glutPostRedisplay();
}

void Environment::mouse(int _button, int _state, int _x, int _y) {
    Environment * enviroment = Environment::instance;
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

void Environment::mouseMotion(int _x, int _y) {
    Environment* enviroment = Environment::instance;
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

void Environment::idle(void) {
    Environment* enviroment = Environment::instance;
    if (enviroment->camera->getType() == Camera::DYNAMIC) {
        dynamicViewAngle += 0.3;
    }
    /*
    if (enviroment->dynamicView == true) {
        enviroment->dynamicViewAngle += 0.3;
        //GLfloat* position = enviroment->camera->getPosition();
        enviroment->camera->setPosition(cos(enviroment->dynamicViewAngle * DEG_TO_RAD) * 5, 5, sin(enviroment->dynamicViewAngle * DEG_TO_RAD) * 5);
        enviroment->camera->setViewDirection(0.0, 0.0, 0.0);
    }*/
    // TODO pohyb kamery

    // Redraw scene.
    glutPostRedisplay();
}

void Environment::menu(int _selectedItem) {
    Environment* enviroment = Environment::instance;
    switch (_selectedItem) {
        case 0:
            exit(0);
            break;
        case 11:
            // Static view 1.
            enviroment->camera = enviroment->cameras[0];
            break;
        case 12:
            // Static view 2.
            enviroment->camera = enviroment->cameras[1];
            break;
        case 13:
            // Dynamic view.
            enviroment->camera = enviroment->cameras[2];
            break;
        case 14:
            // Walking camera.
            enviroment->cameras[3]->loadConfig(enviroment->camera);
            enviroment->camera = enviroment->cameras[3];
            break;
        case 15:
            // Free camera.
            enviroment->cameras[4]->loadConfig(enviroment->camera);
            enviroment->camera = enviroment->cameras[4];
            break;
        case 21:
            // Switch on/off the sun.
            if (enviroment->lights[0]->getState() == true) {
                enviroment->lights[0]->switchOff();
            } else {
                enviroment->lights[0]->switchOn();
            }
            break;
        case 22:
            // Control of flashlight.
            if (enviroment->lights[1]->getState() == true) {
                enviroment->lights[1]->switchOff();
            } else {
                enviroment->lights[1]->switchOn();
            }
    }
    glutPostRedisplay();
}

float Environment::DegToRad(float _deg) {
    return _deg * (M_PI / 180);
}

float Environment::RadToDeg(float _rad) {
    return _rad * (180 / M_PI);
}

void Environment::init(void) {
    glEnable(GL_DEPTH_TEST);

    // Front side of polygons are filled, back side as well.
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    // Culling of back side of polygon.
    glCullFace(GL_BACK);

    initCameras();
    initLights();
}

void Environment::initCameras(void) {
    // Create cameras.
    for (int i = 0; i < CAMERA_COUNT; ++i) {
        try {
            string name = lexical_cast<string > (i);
            cameras[i] = new Camera(name, config);
        } catch (bad_lexical_cast exception) {
            cerr << "Environment::constructor -> Bad Lexical Cast." << endl;
            exit(1);
        }
    }

    // Set up the actual camera.
    camera = cameras[3];
}

void Environment::initLights(void) {
    // Set up lighting.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    // Create lights.
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        lights[i] = new Light(i, config);
    }
}

void Environment::createMenu(void) {
    // Create submenus.
    int cameraMenu = glutCreateMenu(Environment::menu);
    glutAddMenuEntry("Static view 1", 11);
    glutAddMenuEntry("Static view 2", 12);
    glutAddMenuEntry("Dynamic view", 13);
    glutAddMenuEntry("Walk", 14);
    glutAddMenuEntry("Free camera", 15);

    int lightMenu = glutCreateMenu(Environment::menu);
    glutAddMenuEntry("Switch on/off sun", 21);
    glutAddMenuEntry("Switch on/off flashlight", 22);

    // Create main menu.
    glutCreateMenu(Environment::menu);
    glutAddSubMenu("Camera", cameraMenu);
    glutAddSubMenu("Lights", lightMenu);
    glutAddMenuEntry("Quit", 0);

    // Right button opens main menu.
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Environment::drawScene(void) {
    Environment * enviroment = Environment::instance;    
    glLoadIdentity();
    
    // Set camera position and camera direction.
    enviroment->camera->look();

    cout << *camera;

    // Replace static lights.
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        if (lights[i]->dynamic == false) {
            lights[i]->rePosition();
            lights[i]->reSpotDirection();
        }
    }

    // Draw green plane.
    glPushMatrix();
    drawPlane(100);
    glPopMatrix();

    // Draw boat.
    glPushMatrix();
    glTranslated(0.0, 0.0, 0.0);
    drawBoat();
    drawAxes(2.0);
    glPopMatrix();
}

void Environment::drawAxes(float _length) {
    glDisable(GL_LIGHTING);

    // Draw x axis.
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(_length, 0.0, 0.0);
    glEnd();

    // Draw y axis.
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, _length, 0.0);
    glEnd();

    // Draw z axis.
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, _length);
    glEnd();

    glEnable(GL_LIGHTING);
}

void Environment::loadConfig(const string& _file) {
    // Load configuration from XML file.
    try {
        read_xml(_file, config);

        // Set up environment variables.
        windowWidth = config.get<int>("config.window.width");
        windowHeight = config.get<int>("config.window.height");
        windowTitle = config.get<string > ("config.window.title");
    } catch (xml_parser_error exception) {
        cerr << "Environment::loadConfig -> XML Parser Error." << endl;
        exit(1);
    } catch (ptree_bad_path exception) {
        cerr << "Environment::loadConfig -> PTree Bad Path." << endl;
        exit(1);
    } catch (ptree_bad_data exception) {
        cerr << "Environment::loadConfig -> PTree Bad Data." << endl;
        exit(1);
    }
}

/**
 * Draw plain as grid.
 * @param subdiv Count of squares.
 */
void Environment::drawPlane(int subdiv) {
    const float size = 12; // Plane size.

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

void Environment::drawBoat(void) {   
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat ambient[] = {0.4, 0.4, 0.4, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    GLfloat diffuse[] = {0.8, 0.8, 0.8, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // Bottom.
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

    // Bottom of head.
    {
        GLfloat verteces[] = {
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -0.3, 0.0, 0.25,
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.3, 0.0, 0.25,
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.75
        };
        glInterleavedArrays(GL_C4F_N3F_V3F, 0, verteces);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    // Back side.
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

    // Right side.
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

    // Left side.
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

    // Right head.
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

    // Left head.
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

    // Inner back side.
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

    // Inner right side.
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

    // Inner left side.
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

    // Inner bottom.
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

    // Inner front side.
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

    // Top back cover.
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

    // Top right cover.
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

    // Top left cover.
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

    // Top front cover.
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

    // Top head cover.
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
}
