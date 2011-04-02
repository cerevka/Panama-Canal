#ifndef CAMERA_H
#define	CAMERA_H

#include <GL/gl.h>
#include <GL/freeglut.h>

class Camera {
public:
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
    Camera(void);

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
    void setPosition(GLfloat x, GLfloat y, GLfloat z);

    /**
     * Nastavi smer kamery.
     * @param GLfloat x Xova souradnice.
     * @param GLfloat y Yova souradnice.
     * @param GLfloat z Zova souradnice.
     */
    void setViewDirection(GLfloat x, GLfloat y, GLfloat z);

    /**
     * Nastavi upvector kamery.
     * @param GLfloat x Xova souradnice.
     * @param GLfloat y Yova souradnice.
     * @param GLfloat z Zova souradnice.
     */
    void setUpvector(GLfloat x, GLfloat y, GLfloat z);

    /**
     * Nastavni souradnice smeru pohybu.
     * @param GLfloat x Xova souradnice.
     * @param GLfloat y Yova souradnice.
     * @param GLfloat z Zova souradnice.
     */
    void setMotionDirection(GLfloat x, GLfloat y, GLfloat z);

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

private:
    /** Pozice x, y, z kamery. */
    GLfloat position[3];

    /** Smer x, y, z, kam se kamera diva. */
    GLfloat viewDirection[3];

    /** Up-vektor kamery. */
    GLfloat upvector[3];

    /** Smer pohybu kamery. */
    GLfloat motionDirection[3];
};

#endif	/* CAMERA_H */

