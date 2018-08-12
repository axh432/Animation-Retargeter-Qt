#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{

private:
    QVector3D position;
    QVector3D viewDirection;
    QVector3D up;
    static const float MOVEMENT_INCREMENT;
    static const float ROTATION_INCREMENT;

public:
    Camera(QVector3D newPos = QVector3D(0,0,0));

    void resize(float verticalAngle, float aspectRatio, float nearPlane, float farPlane);

    QMatrix4x4 getViewMatrix();

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void lookLeft();
    void lookRight();
    void lookUp();
    void lookDown();

};

#endif // CAMERA_H
