#ifndef ENTITY_H
#define ENTITY_H

#include "lib/md5/md5.h"
#include <memory>
#include <QMatrix4x4>


class Entity
{
private:
    unique_ptr<GLModel> model;
    Anim* anim;
    double millisecondsPast;
    double interpolation;
    int currentFrame;
    int nextFrame;
    QMatrix4x4 positionOrientation;

    void calculateNextFrame(int numFrames);
    void computeOpenGLVerts();
    void findInterpolationValue(double millisecsPerFrame, double millisecondsPast);

public:
    Entity(unique_ptr<GLModel> model, Anim* anim, QMatrix4x4 matrix);

    void render(QMatrix4x4& view);
    void update(double delta);
    inline GLModel* getModel(){ return model.get(); }
    inline void setPositionOrientation(QMatrix4x4 posOrient){ this->positionOrientation = posOrient; }
    inline QMatrix4x4 getPositionOrientation(){ return positionOrientation; }
};

#endif // ENTITY_H
