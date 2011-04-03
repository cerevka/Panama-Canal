#include <math.h>

#include "Camera.h"
#include "Enviroment.h"

Camera::Camera(const ptree* _config, const string& _name) {
    rotationAngle = 0.0;
    elevationAngle = 0.0;

    loadConfig(_config, _name);
}

Camera::~Camera() {
}

void Camera::setPosition(GLfloat _x, GLfloat _y, GLfloat _z) {
    position[0] = _x;
    position[1] = _y;
    position[2] = _z;
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

    // Spocte smerove souradnice.
    setViewDirection(
            cos(rotationAngle * DEG_TO_RAD) * cos(elevationAngle * DEG_TO_RAD),
            sin(elevationAngle * DEG_TO_RAD),
            sin(rotationAngle * DEG_TO_RAD) * cos(elevationAngle * DEG_TO_RAD)
            );

    // Spocte souradnice pohybu.
    setMotionDirection(
            cos(rotationAngle * DEG_TO_RAD),
            0.0,
            sin(rotationAngle * DEG_TO_RAD)
            );

    // Spocte souradnice upvectoru.
    setUpvector(
            -cos(rotationAngle * DEG_TO_RAD) * sin(elevationAngle * DEG_TO_RAD),
            cos(elevationAngle * DEG_TO_RAD),
            -sin(rotationAngle * DEG_TO_RAD) * sin(elevationAngle * DEG_TO_RAD)
            );

    gluLookAt(
            position[0], position[1], position[2], // pozice kamery
            position[0] + viewDirection[0], position[1] + viewDirection[1], position[2] + viewDirection[2], // smer kamery
            upvector[0], upvector[1], upvector[2] // up vektor
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

void Camera::loadConfig(const ptree* _config, const string& _name) {
    string path = "config.cameras.camera-" + _name + ".";
    try {
        // Nastavi se promenne kamere.
        setPosition(
                _config->get<float>(path + "position.x"),
                _config->get<float>(path + "position.y"),
                _config->get<float>(path + "position.z")
                );
        setViewDirection(
                _config->get<float>(path + "direction.x"),
                _config->get<float>(path + "direction.y"),
                _config->get<float>(path + "direction.z")
                );
        setUpvector(
                _config->get<float>(path + "upvector.x"),
                _config->get<float>(path + "upvector.y"),
                _config->get<float>(path + "upvector.z")
                );
        setMotionDirection(
                _config->get<float>(path + "motion.x"),
                _config->get<float>(path + "motion.y"),
                _config->get<float>(path + "motion.z")
                );
        step = _config->get<float>(path + "motion.step");
        rotationAngleStep = _config->get<float>(path + "motion.angle.rotation");
        elevationAngleStep = _config->get<float>(path + "motion.angle.elevation");
    } catch (xml_parser_error) {
        cerr << "Camera "+_name+"::loadConfig -> XML Parser Error." << endl;
    } catch (ptree_bad_path) {
        cerr << "Camera "+_name+"::loadConfig -> PTree Bad Path." << endl;
    } catch (ptree_bad_data) {
        cerr << "Camera "+_name+"::loadConfig -> PTree Bad Data." << endl;
    }
}



