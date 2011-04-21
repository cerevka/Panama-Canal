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
    /** Indicator of movable light. */
    bool dynamic;

    /**
     * Create new light.
     */
    Light(int _number, ptree& _config);

    /**
     * Destroy the light.
     */
    virtual ~Light(void);

    /**
     * Switch on the light.
     */
    void switchOn(void);

    /**
     * Switch of the light.
     */
    void switchOff(void);

    /**
     * Set up ambient color.
     * @param _r Red color.
     * @param _g Green color.
     * @param _b Blue color.
     * @param _a Alpha canal.
     */
    void setAmbient(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1.0);

    /**
     * Set up diffuse color.
     * @param _r Red color.
     * @param _g Green color.
     * @param _b Blue color.
     * @param _a Alpha canal.
     */
    void setDiffuse(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1.0);

    /**
     * Set up specular color.
     * @param _r Red color.
     * @param _g Green color.
     * @param _b Blue color.
     * @param _a Alpha canal.
     */
    void setSpecular(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1.0);

    /**
     * Set up position of light.
     * @param _x X coordinate.
     * @param _y Y coordinate.
     * @param _z Z coordinate.
     * @param _w W coordinate.
     */
    void setPosition(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w = 0.0);

    /**
     * Replace light in the same position. Due to tranformation.
     */
    void rePosition(void);

    /**
     * Set up direction of spot light.
     * @param _x X coordinate.
     * @param _y Y coordinate.
     * @param _z Z coordinate.
     */
    void setSpotDirection(GLfloat _x, GLfloat _y, GLfloat _z);

    /**
     * Set up direction of spot light again.
     */
    void reSpotDirection(void);

    /**
     * Set up exponent of spot light.
     * @param _exponent Exponent.
     */
    void setSpotExponent(GLfloat _exponent);

    /**
     * Set up cutoff angle of spot light.
     * @param _cutOff Angle in degrees.
     */
    void setSpotCutOff(GLfloat _cutOff);

    /**
     * Set up attenuation.
     * @param _constant Constant element.
     * @param _linear Linear element.
     * @param _quadratic Quadratic element.
     */
    void setAttenuation(GLfloat _constant, GLfloat _linear, GLfloat _quadratic);

    /**
     * Get state of light.
     * @return false = off, true = on.
     */
    bool getState(void);

    friend ostream & operator<<(ostream& os, Light& light);

private:

    /** Light name. */
    GLenum name;

    // Light identifier.
    int id;

    /** Ambient color. */
    GLfloat ambient[4];

    /** Diffuse color. */
    GLfloat diffuse[4];

    /** Specular color. */
    GLfloat specular[4];

    /** Position of light. */
    GLfloat position[4];

    /** Direction of spot light. */
    GLfloat spotDirection[3];

    /** Exponent of spot light. */
    GLfloat spotExponent;

    /** Cutoff angle of spot light. */
    GLfloat spotCutOff;

    /** Factor of attenuation.
     * [0] Constant element.
     * [1] Linear element.
     * [2] Quadratic element.
     */
    GLfloat attenuation[3];

    /** Indicator of on/off. */
    bool enabled;  

    /**
     * Set up light from configuration.
     * @param _name Light name.
     * @param _config Configuration.
     */
    void loadConfig(const string& _name, ptree& _config);
};

#endif	/* LIGHT_H */

