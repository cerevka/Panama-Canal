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

    if (Enviroment::getInstance()->dynamicView != true) {
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
    } else {
        gluLookAt(
                position[0], position[1], position[2], // pozice kamery
                viewDirection[0], viewDirection[1], viewDirection[2], // smer kamery
                upvector[0], upvector[1], upvector[2] // up vektor
                );
    }

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
    setViewDirection(camera->viewDirection[0], camera->viewDirection[1], camera->viewDirection[2]);
    setUpvector(camera->upvector[0], camera->upvector[1], camera->upvector[2]);
    setMotionDirection(camera->motionDirection[0], camera->motionDirection[1], camera->motionDirection[2]);
    rotationAngle = camera->rotationAngle;
    elevationAngle = camera->elevationAngle;
}

void Camera::loadConfig(const string& _name, ptree& _config) {
    string path = "config.cameras.camera-" + _name + ".";
    try {
        // Nastavi se promenne kamere.
        setPosition(
                _config.get<float>(path + "position.x"),
                _config.get<float>(path + "position.y"),
                _config.get<float>(path + "position.z")
                );
        setViewDirection(
                _config.get<float>(path + "direction.x"),
                _config.get<float>(path + "direction.y"),
                _config.get<float>(path + "direction.z")
                );
        setUpvector(
                _config.get<float>(path + "upvector.x"),
                _config.get<float>(path + "upvector.y"),
                _config.get<float>(path + "upvector.z")
                );
        step = _config.get<float>(path + "motion.step");
        rotationAngleStep = _config.get<float>(path + "motion.angle.rotation-step");
        elevationAngleStep = _config.get<float>(path + "motion.angle.elevation-step");
        rotationAngle = _config.get<float>(path + "rotation");
        elevationAngle = _config.get<float>(path + "elevation");
    } catch (ptree_bad_path exception) {
        cerr << "Camera " + _name + "::loadConfig -> PTree Bad Path." << endl;
        exit(1);
    } catch (ptree_bad_data exception) {
        cerr << "Camera " + _name + "::loadConfig -> PTree Bad Data." << endl;
        exit(1);
    }
}

ostream & operator<<(ostream& os, Camera& camera) {
    os << "Camera" << endl;
    os << "- position(" << camera.position[0] << ", " << camera.position[1] << ", " << camera.position[2] << ")" << endl;
    os << "- viewDirection(" << camera.viewDirection[0] << ", " << camera.viewDirection[1] << ", " << camera.viewDirection[2] << ")" << endl;
    os << "- upvector(" << camera.upvector[0] << ", " << camera.upvector[1] << ", " << camera.upvector[2] << ")" << endl;
    os << "- motionDirection(" << camera.motionDirection[0] << ", " << camera.motionDirection[1] << ", " << camera.motionDirection[2] << ")" << endl;
    os << "- step(" << camera.step << ")" << endl;
    os << "- rotationAngleStep(" << camera.rotationAngleStep << ")" << endl;
    os << "- elevationAngleStep(" << camera.elevationAngleStep << ")" << endl;
    os << "- rotationAngle(" << camera.rotationAngle << ")" << endl;
    os << "- elevationAngle(" << camera.elevationAngle << ")" << endl;
    os << endl;
    return os;
}



