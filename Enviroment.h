#ifndef ENVIROMENT_H
#define	ENVIROMENT_H

#define DEG_TO_RAD (3.14159/180.0)

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

#define CAMERA_COUNT 4
#define LIGHT_COUNT 2

/**
 * Trida starajici se o prostredi OpenGL. Singleton.
 */
class Enviroment {
public:

    /**
     * Sirka okna.
     */
    int windowWidth;

    /**
     * Vyska okna.
     */
    int windowHeight;

    /**
     * Titulek okna.
     */
    string windowTitle;

    bool dynamicView;

    float dynamicViewAngle;   


    /**
     * Destruuje Enviroment.
     */
    virtual ~Enviroment(void);

    /**
     * Vrati instanci prostredi.
     * @return Enviroment Instance Enviroment.
     */
    static Enviroment * getInstance(void);

    /**
     * Kresli obsah hlavniho okna.
     * Callback pro GLUT.
     */
    static void display(void);

    /**
     * Meni velikost okna.
     * Callback pro GLUT volany pri zmene velikosti okna.
     * @param int width Sirka okna.
     * @param int height Vyska okna.
     */
    static void reshape(int _width, int _height);

    /**
     * Odchytava stisky klavesy.
     * @param unsigned char key Kod klavesy.
     * @param int x Xova souradnice mysi.
     * @param int y Yova souradnice mysi.
     */
    static void keyboard(unsigned char _key, int _x, int _y);

    /**
     * Odchytava stisky specialnich klaves.
     * @param int key Kod klavesy.
     * @param int x Xova souradnice mysi.
     * @param int y Yova souradnice mysi.
     */
    static void specialKeyboard(int _key, int _x, int _y);

    /**
     * Odchytava klikani mysi.
     * @param int button Identifikator tlacitka.
     * @param int state Identifikator stavu tlacitka.
     * @param int x Xova souradnice.
     * @param int y Yova souradnice.
     */
    static void mouse(int _button, int _state, int _x, int _y);

    /**
     * Odchytava pohyb mysi.
     * @param int x Xova souradnice.
     * @param int y Yova souradnice.
     */
    static void mouseMotion(int _x, int _y);

    /**
     * Vola se, pokud se zrovna nic jineho nedeje.
     */
    static void idle(void);

    /**
     * Ovlada kontextova menu.
     * @param int selectedItem ID zvolene polozky.s
     */
    static void menu(int _selectedItem);

    /**
     * Inicializuje OpenGL.
     */
    void init(void);

    /**
     * Vytvori menu.
     */
    void createMenu(void);

    /**
     * Kresli scenu.
     */
    void drawScene(void);

    /**
     * Vykresli osy x, y, z.
     * @param float _length Delka os.
     */
    void drawAxes(float _length);

private:

    /** Instance prostredi.  */
    static Enviroment * instance;

    /** Nastaveni. */
    ptree config;

    /** Pouzivana kamera. */
    Camera* camera;

    /** Kamery k dispozici. */
    Camera* cameras[CAMERA_COUNT];

    /** Svetla. */
    Light* lights[LIGHT_COUNT];

    float sunAngle;

    bool mouseLeftPressed;

    int lastCoordinate[2];

    /**
     * Soukromy konstruktor - singleton.
     */
    Enviroment(void);

    /**
     * Nacte nastaveni z daneho souboru.
     * @param file
     */
    void loadConfig(const string& _file);

    void drawPlane(int subdiv);

    void drawShip(void);

};

#endif	/* ENVIROMENT_H */

