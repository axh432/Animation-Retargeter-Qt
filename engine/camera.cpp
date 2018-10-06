#include "camera.h"
#include <QMatrix3x3>

const float Camera::MOVEMENT_INCREMENT = 5.0;
const float Camera::ROTATION_INCREMENT = 5.0;

Camera::Camera(QVector3D newPos):
    viewDirection(0.0f, 0.0f, -1.0f),
    up(0.0f, 1.0f, 0.0f),
    position(newPos)
{/*empty*/}

void Camera::resize(int w, int h){
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 1000.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

QMatrix4x4& Camera::getViewMatrix() {

   currentView.setToIdentity();
   currentView += projection;

   currentView.lookAt(position, position + viewDirection, up);

   return currentView;
}

/*
 * MoveForward
 * Moves the camera forward
 */
void Camera::moveForward(){
    position += MOVEMENT_INCREMENT * viewDirection;
}

/*
 * MoveBackward
 * Moves the camera backward
 */
void Camera::moveBackward(){
    position += -MOVEMENT_INCREMENT * viewDirection;
}

/*
 * MoveLeft
 * Rotates the camera to the left
 */
void Camera::moveLeft(){
    QVector3D strafeDirection = QVector3D::crossProduct(viewDirection, up);
    position += -MOVEMENT_INCREMENT * strafeDirection;
}

/*
 * MoveRight
 * Moves the camera to the right
 */
void Camera::moveRight(){
    QVector3D strafeDirection = QVector3D::crossProduct(viewDirection, up);
    position += MOVEMENT_INCREMENT * strafeDirection;
}

/*
 * MoveUp
 * Moves the camera upward
 */
void Camera::moveUp(){
    position += MOVEMENT_INCREMENT * up;
}

/*
 * MoveDown
 * Moves the camera downward
 */
void Camera::moveDown(){
    position += -MOVEMENT_INCREMENT * up;
}

/*
 * LookLeft
 * Rotates the camera to the left
 */
void Camera::lookLeft(){
    QMatrix4x4 rotation;
    rotation.rotate(ROTATION_INCREMENT, up);
    viewDirection = rotation * viewDirection;
}

/*
 * LookRight
 * Rotates the camera to the left
 */
void Camera::lookRight(){
    QMatrix4x4 rotation;
    rotation.rotate(-ROTATION_INCREMENT, up);
    viewDirection = rotation * viewDirection;
}

/*
 * LookUp
 * Rotates the camera upward
 */
void Camera::lookUp(){
    QVector3D toRotateAround = QVector3D::crossProduct(viewDirection, up);
    QMatrix4x4 rotation;
    rotation.rotate(ROTATION_INCREMENT, toRotateAround);
    viewDirection = rotation * viewDirection;
}

/*
 * LookDown
 * Rotates the camera downward
 */
void Camera::lookDown(){
    QVector3D toRotateAround = QVector3D::crossProduct(viewDirection, up);
    QMatrix4x4 rotation;
    rotation.rotate(-ROTATION_INCREMENT, toRotateAround);
    viewDirection = rotation * viewDirection;
}

