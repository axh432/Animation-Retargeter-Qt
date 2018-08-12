#ifndef QUATERNIONCREATOR_H
#define QUATERNIONCREATOR_H

#include <QQuaternion>

class QuaternionCreator
{
public:
    static QQuaternion computeUnitQuaternion(float x, float y, float z);
};

#endif // QUATERNIONCREATOR_H
