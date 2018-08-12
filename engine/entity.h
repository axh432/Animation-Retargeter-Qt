#ifndef ENTITY_H
#define ENTITY_H

#include "lib/md5/md5.h"
#include <memory>


class Entity
{
private:
    unique_ptr<Model> model;
    unique_ptr<Anim> anim;
    double millisecondsPast;
    double interpolation;
    int currentFrame;
    int nextFrame;

    void calculateNextFrame(int numFrames);

public:
    Entity(unique_ptr<Model> model, unique_ptr<Anim> anim);

    void update(double delta);
    vector<float> computeOpenGLVerts();
    void findInterpolationValue(double millisecsPerFrame, double millisecondsPast);
    inline Model* getModel(){ return model.get(); }
};

#endif // ENTITY_H
