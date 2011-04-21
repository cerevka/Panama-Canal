#include <math.h>

#include "Camera.h"
#include "Enviroment.h"

Camera::Camera(const string& _name, ptree& _config) {
    loadConfig(_name, _config);
}

Camera::~Camera() {
}

void Camera::setPosition(GLfloat _x, GLfloat _y, GLfloat _z) {
    position[0] = _x;
    position[1] = _y;
    position[2] = _z;
}

GLfloat* Camera::getPosition(void) {
    return position;
}

void Camera::setViewDirection(GLfloat _x, GLfloat _y, GLfloat _z) {
    viewDirection[0] = _x;
    viewDirection[1] = _y;
    viewDirection[2] = _z;
}

void Camera::setUpvector(GLfloat _x, GLfloat _y, GLfloat _z) {
    upvector[0] = _x;
    upvector[1] = _y;
    upvector[2] = _z;
}

void Camera::setMotionDirection(GLfloat _x, GLfloat _y, GLfloat _z) {
    motionDirection[0] = _x;
    motionDirection[1] = _y;
    motionDirection[2] = _z;
}

void Camera::look(void) {
    // Calculate direction coordinates.
    setViewDirection(
            cos(Environment::DegToRad(rotationAngle)) * cos(Environment::DegToRad(elevationAngle)),
            sin(Environment::DegToRad(elevationAngle)),
            sin(Environment::DegToRad(rotationAngle)) * cos(Environment::DegToRad(elevationAngle))
            );

    // Calculate upvector coordinates.
    setUpvector(
            -cos(Environment::DegToRad(rotationAngle)) * sin(Environment::DegToRad(elevationAngle)),
            cos(Environment::DegToRad(elevationAngle)),
            -sin(Environment::DegToRad(rotationAngle)) * sin(Environment::DegToRad(elevationAngle))
            );

    (this->*lookFce)();
}

void Camera::lookStatic(void) {
    gluLookAt(
            position[0], position[1], position[2], // camera position
            position[0] + viewDirection[0], position[1] + viewDirection[1], position[2] + viewDirection[2], // view direction
            upvector[0], upvector[1], upvector[2] // upvector
            );
}

void Camera::lookDynamic(void) {
    setPosition(cos(Environment::DegToRad(Environment::dynamicViewAngle)) * 5, 5, sin(Environment::DegToRad(Environment::dynamicViewAngle) * 5));
    setViewDirection(0.0, 0.0, 0.0);
    gluLookAt(
            position[0], position[1], position[2], // camera position
            viewDirection[0], viewDirection[1], viewDirection[2], // view direction
            upvector[0], upvector[1], upvector[2] // upvector
            );
}

void Camera::lookWalk(void) {
    position[1] = 0.5;

    // Calculate motion coordinates.
    setMotionDirection(
            cos(Environment::DegToRad(rotationAngle)),
            0.0,
            sin(Environment::DegToRad(rotationAngle))
            );

    gluLookAt(
            position[0], position[1], position[2], // camera position
            position[0] + viewDirection[0], position[1] + viewDirection[1], position[2] + viewDirection[2], // view direction
            upvector[0], upvector[1], upvector[2] // upvector
            );

}

void Camera::lookFree(void) {
    // Calculate motion coordinates.
    setMotionDirection(
            cos(Environment::DegToRad(rotationAngle)),
            sin(Environment::DegToRad(elevationAngle)),
            sin(Environment::DegToRad(rotationAngle))
            );

    gluLookAt(
            position[0], position[1], position[2], // camera position
            position[0] + viewDirection[0], position[1] + viewDirection[1], position[2] + viewDirection[2], // view direction
            upvector[0], upvector[1], upvector[2] // upvector
            );
}

void Camera::turnLeft(void) {
    rotationAngle += rotationAngleStep;
    if (rotationAngle > 360.0) {
        rotationAngle -= 360.0;
    }
}

void Camera::turnRight(void) {
    rotationAngle -= rotationAngleStep;
    if (rotationAngle < -360.0) {
        rotationAngle += 360.0;
    }
}

void Camera::turnUp(void) {
    elevationAngle += elevationAngleStep;
    if (elevationAngle > 75.0) {
        elevationAngle = 75.0;
    }
}

void Camera::turnDown(void) {
    elevationAngle -= elevationAngleStep;
    if (elevationAngle < -75.0) {
        elevationAngle = -75.0;
    }
}

void Camera::stepForward(void) {
    position[0] += step * motionDirection[0];
    position[1] += step * motionDirection[1];
    position[2] += step * motionDirection[2];
}

void Camera::stepBackward(void) {
    position[0] -= step * motionDirection[0];
    position[1] -= step * motionDirection[1];
    position[2] -= step * motionDirection[2];
}

void Camera::loadConfig(const Camera* camera) {
    setPosition(camera->position[0], camera->position[1], camera->position[2]);
    rotationAngle = camera->rotationAngle;
    elevationAngle = camera->elevationAngle;
}

void Camera::loadConfig(const string& _name, ptree& _config) {
    string path = "config.cameras.camera-" + _name + ".";
    try {
        // Set up variables of camera.
        setPosition(
                _config.get<float>(path + "position.x"),
                _config.get<float>(path + "position.y"),
                _config.get<float>(path + "position.z")
                );
        rotationAngle = _config.get<float>(path + "rotation");
        elevationAngle = _config.get<float>(path + "elevation");
        string type = _config.get<string > (path + "type");
        if (type == "static") {
            // Static camera which can not move by keyboard or mouse.
            setType(STATIC);
        } else if (type == "dynamic") {
            // Dynamic camera with own motion function.
            setType(DYNAMIC);
        } else if (type == "walk") {
            // Dynamic camera which emulate walking - only in xz plain.
            setType(WALK);
        } else if (type == "free") {
            // Dynamic camera which can move absolutely free.
            setType(FREE);
        }

        // Only movable camera needs this information.
        if (type == "walk" || type == "free") {
            step = _config.get<float>(path + "motion.step");
            rotationAngleStep = _config.get<float>(path + "motion.angle.rotation-step");
            elevationAngleStep = _config.get<float>(path + "motion.angle.elevation-step");
        }
    } catch (ptree_bad_path exception) {
        cerr << "Camera " + _name + "::loadConfig -> PTree Bad Path." << endl;
        exit(1);
    } catch (ptree_bad_data exception) {

        cerr << "Camera " + _name + "::loadConfig -> PTree Bad Data." << endl;
        exit(1);
    }
}

void Camera::setType(typeEnum _type) {
    switch (_type) {
        case STATIC:
            lookFce = &Camera::lookStatic;
            break;
        case DYNAMIC:
            lookFce = &Camera::lookDynamic;
            break;
        case WALK:
            lookFce = &Camera::lookWalk;
            break;
        case FREE:
            lookFce = &Camera::lookFree;
            break;
    }
    type = _type;
}

Camera::typeEnum Camera::getType(void) {
    return type;
}

ostream & operator<<(ostream& _os, Camera& _camera) {
    _os << "Camera" << endl;
    _os << "- position(" << _camera.position[0] << ", " << _camera.position[1] << ", " << _camera.position[2] << ")" << endl;
    _os << "- viewDirection(" << _camera.viewDirection[0] << ", " << _camera.viewDirection[1] << ", " << _camera.viewDirection[2] << ")" << endl;
    _os << "- upvector(" << _camera.upvector[0] << ", " << _camera.upvector[1] << ", " << _camera.upvector[2] << ")" << endl;
    _os << "- motionDirection(" << _camera.motionDirection[0] << ", " << _camera.motionDirection[1] << ", " << _camera.motionDirection[2] << ")" << endl;
    _os << "- step(" << _camera.step << ")" << endl;
    _os << "- rotationAngleStep(" << _camera.rotationAngleStep << ")" << endl;
    _os << "- elevationAngleStep(" << _camera.elevationAngleStep << ")" << endl;
    _os << "- rotationAngle(" << _camera.rotationAngle << ")" << endl;
    _os << "- elevationAngle(" << _camera.elevationAngle << ")" << endl;
    _os << endl;
    return _os;
}



