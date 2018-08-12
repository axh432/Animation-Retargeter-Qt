#include "entity.h"

Entity::Entity(unique_ptr<Model> modelRef, unique_ptr<Anim> animRef):
    millisecondsPast(0.0f),
    interpolation(0.0f),
    currentFrame(0),
    nextFrame(0),
    model(std::move(modelRef)),
    anim(std::move(animRef))
{

}

void Entity::update(double delta){

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

vector<float> Entity::computeOpenGLVerts(){

    unique_ptr<Skeleton> skeleton  = Skeleton::interpolateSkeletons(anim->skeletons[currentFrame],
                                                                    anim->skeletons[nextFrame],
                                                                    interpolation);
    //Skeleton* skeleton = &anim->skeletons[currentFrame];

    return model->meshes[0].computeGLVertices(skeleton.get());

   // return model->meshes[0].computeGLVertices(skeleton);
}

void Entity::calculateNextFrame(int numFrames){

    nextFrame++;
    nextFrame %= numFrames;

}
