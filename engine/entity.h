#ifndef ENTITY_H
#define ENTITY_H

#include "lib/md5/md5.h"
#include <memory>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include "ui/commands.h"

using std::shared_ptr;

class Entity : protected QOpenGLFunctions
{
private:
    shared_ptr<GLModel> model;
    Anim* anim;
    double millisecondsPast;
    double interpolation;
    int currentFrame;
    int nextFrame;
    QMatrix4x4 positionOrientation;
    bool frozen;
    bool hidden;
    bool showBones;
    Skeleton* currentSkeleton;
    unique_ptr<Skeleton> interpolatedSkeleton;
    AnimState currentAnimState;
    VisualState currentVisualState;

    void renderSkeleton(QMatrix4x4& viewPosition, int upToJointIndex);
    vector<GLfloat> vectorToGLFloats(QVector3D vector3d);
    void calculateNextFrame(int numFrames);
    void computeOpenGLVerts();
    void findInterpolationValue(double millisecsPerFrame, double millisecondsPast);
    void bindPose();
    void pause();
    void play();
    void show();
    void hide();
    void bones();


public:
    Entity(shared_ptr<GLModel> model, Anim* anim, QMatrix4x4 matrix);
    Entity(shared_ptr<GLModel> model, QMatrix4x4 matrix);

    void render(QMatrix4x4& view);
    void update(double delta);
    inline GLModel* getModel(){ return model.get(); }
    inline Anim* getAnim(){ return anim; }
    void freezeWithSkeleton(Skeleton* retargetedSkeleton);
    inline void setPositionOrientation(QMatrix4x4 posOrient){ this->positionOrientation = posOrient; }
    inline QMatrix4x4 getPositionOrientation(){ return positionOrientation; }
    void changeAnimState(AnimState newAnimState);
    void changeVisualState(VisualState newVisualState);

};

#endif // ENTITY_H
