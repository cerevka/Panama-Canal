#ifndef CAMERA_H
#define	CAMERA_H

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <boost/property_tree/ptree.hpp>

#include <string>

using namespace std;
using namespace boost::property_tree;

class Camera {
public:
    /** Ruzne typy kamer. */
    enum typeEnum {STATIC, WALK, FREE};

    /** Velikost kroku kamery. */
    float step;

    /** Velikost uhlu rotace. */
    float rotationAngleStep;

    /** Velikost uhlu naklonu. */
    float elevationAngleStep;

    /** Aktualni uhel rotace v horizontalni poloze. */
    float rotationAngle;

    /** Aktualni uhel rotace ve vertikalni poloze. */
    float elevationAngle;

    /**
     * Vytvori novou kameru.
     */
    Camera(const string& _name, ptree& _config);

    /**
     * Znici kameru.
     */
    virtual ~Camera(void);

    /**
     * Nastavi pozici kamery.
     * @param GLfloat x Xova souradnice.
     * @param GLfloat y Yova souradnice.
     * @param GLfloat z Zova souradnice.
     */
    void setPosition(GLfloat _x, GLfloat _y, GLfloat _z);

    /**
     * Vrati souradnice kamery.
     * @return
     */
    GLfloat* getPosition(void);

    /**
     * Nastavi smer kamery.
     * @param GLfloat x Xova souradnice.
     * @param GLfloat y Yova souradnice.
     * @param GLfloat z Zova souradnice.
     */
    void setViewDirection(GLfloat _x, GLfloat _y, GLfloat _z);

    /**
     * Nastavi upvector kamery.
     * @param GLfloat x Xova souradnice.
     * @param GLfloat y Yova souradnice.
     * @param GLfloat z Zova souradnice.
     */
    void setUpvector(GLfloat _x, GLfloat _y, GLfloat _z);

    /**
     * Nastavni souradnice smeru pohybu.
     * @param GLfloat x Xova souradnice.
     * @param GLfloat y Yova souradnice.
     * @param GLfloat z Zova souradnice.
     */
    void setMotionDirection(GLfloat _x, GLfloat _y, GLfloat _z);

    /**
     * Umisti kameru.
     */
    void look(void);

    /**
     * Pootoci kameru doleva.
     */
    void turnLeft(void);

    /**
     * Pootoci kameru doprava.
     */
    void turnRight(void);

    /**
     * Zvedne kameru.
     */
    void turnUp(void);

    /**
     * Snizi kameru.
     */
    void turnDown(void);

    /**
     * Posune kameru dopredu.
     */
    void stepForward(void);

    /**
     * Posune kameru dozadu.
     */
    void stepBackward(void);

    /**
     * Nastavi se dle nastaveni jine kamery.
     * @param camera
     */
    void loadConfig(const Camera* camera);

    /**
     * Nastavi typ kamery.
     * @param type Typ kamery.
     */
    void setType(typeEnum _type);

    /**
     * Vypise konfiguraci kamery na vystup.
     * @param ostream& os Vystupni stream.
     * @param Camera& camera Vypisovany objekt.
     * @return ostream& Vystupni stream.
     */
    friend ostream& operator<<(ostream& os, Camera& camera);

private:
    /** Pozice x, y, z kamery. */
    GLfloat position[3];

    /** Smer x, y, z, kam se kamera diva. */
    GLfloat viewDirection[3];

    /** Up-vektor kamery. */
    GLfloat upvector[3];

    /** Smer pohybu kamery. */
    GLfloat motionDirection[3];

    void loadConfig(const string& _name, ptree& _config);

    void (Camera::*lookFce)(void);

    /**
     * Umisti statickou kameru.
     */
    void lookStatic(void);

    /**
     * Umisti chodici kameru kameru.
     */
    void lookWalk(void);

    /**
     * Umisti volnou kameru.
     */
    void lookFree(void);
};

#endif	/* CAMERA_H */

