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
    /** Camera types. */
    enum typeEnum {STATIC, DYNAMIC, WALK, FREE};    

    /** Size of camera step. */
    float step;

    /** Size of rotation angle step. */
    float rotationAngleStep;

    /** Size of elevation angle step. */
    float elevationAngleStep;

    /** Actual size of rotation angle. */
    float rotationAngle;

    /** Actual size of elevation angle. */
    float elevationAngle;

    /**
     * Create new camera.
     * @param _name Camera name.
     * @param _config Configuration.
     */
    Camera(const string& _name, ptree& _config);

    /**
     * Destroy camera.
     */
    virtual ~Camera(void);

    /**
     * Set up camera position.
     * @param _x X coordinate.
     * @param _y Y coordinate.
     * @param _z Z coordinate.
     */
    void setPosition(GLfloat _x, GLfloat _y, GLfloat _z);

    /**
     * Get camera position.
     * @return Camera position.
     */
    GLfloat* getPosition(void);

    /**
     * Set up view direction.
     * @param _x X coordinate.
     * @param _y Y coordinate.
     * @param _z Z coordinate.
     */
    void setViewDirection(GLfloat _x, GLfloat _y, GLfloat _z);

    /**
     * Set up upvector.
     * @param _x X coordinate.
     * @param _y Y coordinate.
     * @param _z Z coordinate.
     */
    void setUpvector(GLfloat _x, GLfloat _y, GLfloat _z);

    /**
     * Set up motion direction.
     * @param _x X coordinate.
     * @param _y Y coordinate.
     * @param _z Z coordinate.
     */
    void setMotionDirection(GLfloat _x, GLfloat _y, GLfloat _z);

    /**
     * Place camera.
     */
    void look(void);

    /**
     * Turn camera left.
     */
    void turnLeft(void);

    /**
     * Turn camera right.
     */
    void turnRight(void);

    /**
     * Pick up camera view.
     */
    void turnUp(void);

    /**
     * Put down camera view.
     */
    void turnDown(void);

    /**
     * Move camera forward.
     */
    void stepForward(void);

    /**
     * Move camera back.
     */
    void stepBackward(void);

    /**
     * Set up camera according to the given camera.
     * @param camera Given camera.
     */
    void loadConfig(const Camera* camera);

    /**
     * Set camera type.
     * @param type Camera type.
     */
    void setType(typeEnum _type);
    
    /**
     * Get camera type.
     * @return Camera type.
     */
    Camera::typeEnum getType(void);
    
    /**
     * Write out information about camera.
     * @param _os Output stream.
     * @param _camera Described camera.
     * @return Output stream.
     */
    friend ostream& operator<<(ostream& _os, Camera& _camera);

private:
    /** Camera position. */
    GLfloat position[3];

    /** View direction. */
    GLfloat viewDirection[3];

    /** Upvector. */
    GLfloat upvector[3];

    /** Motion direction. */
    GLfloat motionDirection[3];
    
    typeEnum type;

    /**
     * Load camera configuration.
     * @param _name Name of camera.
     * @param _config Configuration.
     */
    void loadConfig(const string& _name, ptree& _config);

    void (Camera::*lookFce)(void);
    
    /**
     * Place static camera.
     */
    void lookStatic(void);
    
    /**
     * Place dynamic camera.
     */
    void lookDynamic(void);

    /**
     * Place walking camera.
     */
    void lookWalk(void);

    /**
     * Place free camera.
     */
    void lookFree(void);
};

#endif	/* CAMERA_H */

