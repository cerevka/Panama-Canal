#ifndef ENVIROMENT_H
#define	ENVIROMENT_H

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;
using namespace boost::property_tree;

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
    static void reshape(int width, int height);

    /**
     * Odchytava stisky klavesy.
     * @param unsigned char key Kod klavesy.
     * @param int x Xova souradnice mysi.
     * @param int y Yova souradnice mysi.
     */
    static void keyboard(unsigned char key, int x, int y);

    /**
     * Odchytava stisky specialnich klaves.
     * @param int key Kod klavesy.
     * @param int x Xova souradnice mysi.
     * @param int y Yova souradnice mysi.
     */
    static void specialKeyboard(int key, int x, int y);

    /**
     * Vola se, pokud se zrovna nic jineho nedeje.
     */
    static void idle(void);

    /**
     * Ovlada kontextova menu.
     * @param int selectedItem ID zvolene polozky.s
     */
    static void menu(int selectedItem);

    /**
     * Inicializuje OpenGL.
     */
    void init(void);

    /**
     * Kresli scenu.
     */
    void drawScene(void);

private:

    /**
     * Ukazatel na instanci Enviroment;
     */
    static Enviroment * instance;

    /**
     * Uchovava nactene nastaveni.
     */
    ptree config;

    /**
     * Soukromy konstruktor - singleton.
     */
    Enviroment(void);

    /**
     * Nacte nastaveni z daneho souboru.
     * @param file
     */
    void loadConfig(const string &file);

};

#endif	/* ENVIROMENT_H */

