#include "entity.h"

Entity::Entity(unique_ptr<GLModel> modelRef, Anim* animRef, QMatrix4x4 matrix):
    millisecondsPast(0.0f),
    interpolation(0.0f),
    currentFrame(0),
    nextFrame(0),
    model(std::move(modelRef)),
    anim(animRef),
    positionOrientation(matrix)
{
}

void Entity::render(QMatrix4x4& view){
    QMatrix4x4 viewPosition = view * positionOrientation;

    model->render(viewPosition);
}

void Entity::update(double delta){

    this->getModel();

    int numFrames = anim->skeletons.size() - 1;
    double millisecsPerFrame = anim->header->millisecondsPerFrame;

    millisecondsPast += delta;

    qDebug() << "delta: " << delta;
    qDebug() << "millisecondsPerFrame " << anim->header->millisecondsPerFrame;
    qDebug() << "millisecondsPast " << millisecondsPast;

    if(millisecondsPast >= millisecsPerFrame){
       millisecondsPast -= millisecsPerFrame;
       currentFrame = nextFrame;
       calculateNextFrame(numFrames);
   }

    findInterpolationValue(millisecsPerFrame, millisecondsPast);

    computeOpenGLVerts();
}

void Entity::findInterpolationValue(double millisecsPerFrame, double millisecondsPast){

    if(millisecondsPast <= 0.0f){
        interpolation = 0.0f;
    }

    double onePercent = millisecsPerFrame / 100;

    //how many times does onePercent go into millisecondsPast?
    interpolation = (millisecondsPast / onePercent) / 100;

    if(interpolation > 1.0f){
        interpolation = 1.0f;
    }

    qDebug() << "interpolation value: " << interpolation;
}

void Entity::computeOpenGLVerts(){

    unique_ptr<Skeleton> skeleton  = Skeleton::interpolateSkeletons(anim->skeletons[currentFrame],
                                                                    anim->skeletons[nextFrame],
                                                                    interpolation);
    model->update(skeleton.get());
}

void Entity::calculateNextFrame(int numFrames){

    nextFrame++;
    nextFrame %= numFrames;

}
