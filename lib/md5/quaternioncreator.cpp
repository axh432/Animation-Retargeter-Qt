#include "quaternioncreator.h"
#include <cmath>

QQuaternion QuaternionCreator::computeUnitQuaternion(float x, float y, float z){
    float t = 1.0f - (x * x) - (y * y) - (z * z);
    float w = 0.0f;

    if (t >= 0.0f)
      {
        w = -sqrt (t);
      }

    return QQuaternion(w, x, y, z);
}
