#include <math.h>

#include "Camera.h"
#include "Enviroment.h"

Camera::Camera() {
    viewingAngle = 0.0;
    elevationAngle = 0.0;
}

Camera::~Camera() {
}

void Camera::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

void Camera::setViewDirection(GLfloat x, GLfloat y, GLfloat z) {
    viewDirection[0] = x;
    viewDirection[1] = y;
    viewDirection[2] = z;
}

void Camera::setUpvector(GLfloat x, GLfloat y, GLfloat z) {
    upvector[0] = x;
    upvector[1] = y;
    upvector[2] = z;
}

void Camera::setMotionDirection(GLfloat x, GLfloat y, GLfloat z) {
    motionDirection[0] = x;
    motionDirection[1] = y;
    motionDirection[2] = z;
}

void Camera::look(void) {

    // Spocte smerove souradnice.
    setViewDirection(
            cos(viewingAngle * DEG_TO_RAD) * cos(elevationAngle * DEG_TO_RAD),
            sin(elevationAngle * DEG_TO_RAD),
            sin(viewingAngle * DEG_TO_RAD) * cos(elevationAngle * DEG_TO_RAD)
            );

    // Spocte souradnice pohybu.
    setMotionDirection(
            cos(viewingAngle * DEG_TO_RAD),
            0.0,
            sin(viewingAngle * DEG_TO_RAD)
            );

    // Spocte souradnice upvectoru.
    setUpvector(
            -cos(viewingAngle * DEG_TO_RAD) * sin(elevationAngle * DEG_TO_RAD),
            cos(elevationAngle * DEG_TO_RAD),
            -sin(viewingAngle * DEG_TO_RAD) * sin(elevationAngle * DEG_TO_RAD)
            );

    gluLookAt(
            position[0], position[1], position[2], // pozice kamery
            position[0] + viewDirection[0], position[1] + viewDirection[1], position[2] + viewDirection[2], // smer kamery
            upvector[0], upvector[1], upvector[2] // up vektor
            );
}

void Camera::turnLeft(void) {
    viewingAngle += angle;
    if (viewingAngle > 360.0) {
        viewingAngle -= 360.0;
    }
}

void Camera::turnRight(void) {
    viewingAngle -= angle;
    if (viewingAngle < -360.0) {
        viewingAngle += 360.0;
    }
}

void Camera::turnUp(void) {
    elevationAngle += angle;
    if (elevationAngle > 75.0) {
        elevationAngle = 75.0;
    }
}

void Camera::turnDown(void) {
    elevationAngle -= angle;
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



