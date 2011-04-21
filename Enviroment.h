#ifndef ENVIROMENT_H
#define	ENVIROMENT_H

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <cmath>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>

#include "Camera.h"
#include "Light.h"

using namespace std;
using namespace boost;
using namespace boost::property_tree;

#define CAMERA_COUNT 5
#define LIGHT_COUNT 2

/**
 * Look after environment of OpenGL.
 */
class Environment {
public:
    /** Actual angle of dynamic camera. */
    static float dynamicViewAngle;

    /**
     * Window width.
     */
    int windowWidth;

    /**
     * Window height.
     */
    int windowHeight;

    /**
     * Window title.
     */
    string windowTitle;    

    /**
     * Destroy environment.
     */
    virtual ~Environment(void);

    /**
     * Return instance of the singleton object.
     * @return Instance Environment.
     */
    static Environment * getInstance(void);

    /**
     * Draw content of main window.
     * Callback pro GLUT.
     */
    static void display(void);

    /**
     * Reshape main windows.
     * Callback for Glut for resizing window.
     * @param _width Window width.
     * @param _height Window height.
     */
    static void reshape(int _width, int _height);

    /**
     * Catch inputs of keyboard.
     * @param key Key code.
     * @param _x X coordinate of mouse.
     * @param _y Y coordinate of mouse.
     */
    static void keyboard(unsigned char _key, int _x, int _y);

    /**
     * Catch inputs of special keys.
     * @param _key Key code.
     * @param _x X coordinate of mouse.
     * @param _y Y coordinate of mouse.
     */
    static void specialKeyboard(int _key, int _x, int _y);

    /**
     * Catch inputs of mouse.
     * @param _button Button identifier.
     * @param _state State identifier.
     * @param _x X coordinate.
     * @param _y Y coordinate.
     */
    static void mouse(int _button, int _state, int _x, int _y);

    /**
     * Catch move of mouse.
     * @param _x X coordinate.
     * @param _y Y coordinate.
     */
    static void mouseMotion(int _x, int _y);

    /**
     * It is called, when there is nothing to call.
     */
    static void idle(void);

    /**
     * Control context menu.
     * @param _selectedItem Menu item identifier.
     */
    static void menu(int _selectedItem);

    /**
     * Convert degrees to radians.
     * @param _deg Degrees.
     * @return Radians.
     */
    static float DegToRad(float _deg);

    /**
     * Convert radians to degrees.
     * @param _rad Radians.
     * @return Degrees.
     */
    static float RadToDeg(float _rad);

    /**
     * Initialize OpenGL.
     */
    void init(void);

    /**
     * Initialize cameras.
     */
    void initCameras(void);

    /**
     * Initialize lights.
     */
    void initLights(void);

    /**
     * Create context menu.
     */
    void createMenu(void);

    /**
     * Draw scene.
     */
    void drawScene(void);

    /**
     * Draw x, y and z axes.
     * @param _length Length of axes.
     */
    void drawAxes(float _length);

private:

    /** Object instance.  */
    static Environment * instance;

    /** Configuration. */
    ptree config;

    /** Actual camera. */
    Camera* camera;

    /** All cameras. */
    Camera* cameras[CAMERA_COUNT];

    /** All lights. */
    Light* lights[LIGHT_COUNT];

    /** Actual angle of sun. */
    float sunAngle;

    /** Indicator of left mouse press. */
    bool mouseLeftPressed;

    /** Last coordinates of mouse. */
    int lastCoordinate[2];

    /**
     * Private constructor - singleton.
     */
    Environment(void);

    /**
     * Load configuration from XML file.
     * @param _file
     */
    void loadConfig(const string& _file);

    /**
     * Draw green plane.
     * @param _subdiv Count of squares.
     */
    void drawPlane(int _subdiv);

    /**
     * Draw boat.
     */
    void drawBoat(void);

};

#endif	/* ENVIROMENT_H */

