#ifndef LIGHT_H
#define	LIGHT_H

#include <GL/gl.h>

#include <boost/lexical_cast.hpp>

#include <boost/property_tree/ptree.hpp>

#include <string>
#include <iostream>

using namespace std;
using namespace boost;
using namespace boost::property_tree;

class Light {
public:
    /** Indikace pohybujiciho se svetla. */
    bool dynamic;

    /**
     * Vytvori nove svetlo.
     */
    Light(int _number, ptree& _config);

    /**
     * Zdestruuje svetlo.
     */
    virtual ~Light(void);

    /**
     * Rozsviti svetlo.
     */
    void switchOn(void);

    /**
     * Zhasne svetlo.
     */
    void switchOff(void);

    /**
     * Nastavi ambientni slozku svetla.
     * @param GLfloat _r Cervena slozka.
     * @param GLfloat _g Zelena slozka.
     * @param GLfloat _b Modra slozka.
     * @param GLfloat _a Pruhlednost.
     */
    void setAmbient(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1.0);

    /**
     * Nastavi difuzni slozku svetla.
     * @param GLfloat _r Cervena slozka.
     * @param GLfloat _g Zelena slozka.
     * @param GLfloat _b Modra slozka.
     * @param GLfloat _a Pruhlednost.
     */
    void setDiffuse(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1.0);

    /**
     * Nastavi zrcadlovou slozku svetla.
     * @param GLfloat _r Cervena slozka.
     * @param GLfloat _g Zelena slozka.
     * @param GLfloat _b Modra slozka.
     * @param GLfloat _a Pruhlednost.
     */
    void setSpecular(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1.0);

    /**
     * Nastavi pozici svetla.
     * @param GLfloat _x Xova souradnice.
     * @param GLfloat _y Yova souradnice.
     * @param GLfloat _z Zova souradnice.
     * @param GLfloat _w Wova souradnice.
     */
    void setPosition(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w = 0.0);

    /**
     * Znovu umisti svetlo (kvuli transformacim).
     */
    void rePosition(void);

    /**
     * Nastavi smer bodoveho svetla.
     * @param GLfloat _x Xova souradnice.
     * @param GLfloat _y Yova souradnice.
     * @param GLfloat _z Zova souradnice.
     */
    void setSpotDirection(GLfloat _x, GLfloat _y, GLfloat _z);

    /**
     * Znovu nasmeruje svetlo (kvuli transformacim).
     */
    void reSpotDirection(void);

    /**
     * Nastaveni exponent bodoveho svetla.
     * @param GLfloat _exponent Exponent.
     */
    void setSpotExponent(GLfloat _exponent);

    /**
     * Nastavi hranicni uhel vyrezu bodoveho svetla.
     * @param GLfloat _cutOff Uhel ve stupnich.
     */
    void setSpotCutOff(GLfloat _cutOff);

    /**
     * Nastavi utlum.
     * @param GLfloat _constant Konstatni slozka.
     * @param GLfloat _linear Linearni slozka.
     * @param GLfloat _quadratic Kvadraticka slozka.
     */
    void setAttenuation(GLfloat _constant, GLfloat _linear, GLfloat _quadratic);

    /**
     * Vrati stav svetla.
     * @return false = zhasnuto, true rozsviceno.
     */
    bool getState(void);

    friend ostream & operator<<(ostream& os, Light& light);

private:

    /** Oznaceni svetla. */
    GLenum name;

    // Cislo svelta.
    int id;

    /** Ambientni slozka barvy svetla. */
    GLfloat ambient[4];

    /** Difusni slozka barvy svetla. */
    GLfloat diffuse[4];

    /** Zrcadlova slozka barvy svetla. */
    GLfloat specular[4];

    /** Pozice svetelneho zdroje. */
    GLfloat position[4];

    /** Smer bodoveho svetla. */
    GLfloat spotDirection[3];

    /** Exponent bodoveho svetla. */
    GLfloat spotExponent;

    /** Hranicni uhel vyrezu bodoveho svetla. */
    GLfloat spotCutOff;

    /** Faktor utlumu.
     * 0 konstatni
     * 1 linearni
     * 2 kvadraticky
     */
    GLfloat attenuation[3];

    /** Indikace zapnuti/vypnuti. */
    bool state;

    void loadConfig(const string& _name, ptree& _config);
};

#endif	/* LIGHT_H */

