#include "entity.h"

Entity::Entity(unique_ptr<GLModel> modelRef, Anim* animRef, QMatrix4x4 matrix):
    millisecondsPast(0.0f),
    interpolation(0.0f),
    currentFrame(0),
    nextFrame(0),
    model(std::move(modelRef)),
    anim(animRef),
    positionOrientation(matrix),
    frozen(false),
    frozenSkeleton(nullptr)
{
    initializeOpenGLFunctions();
}

void Entity::render(QMatrix4x4& view){
    QMatrix4x4 viewPosition = view * positionOrientation;

    model->render(viewPosition);

    if(frozen){
        renderSkeleton(viewPosition);
    }
}

vector<GLfloat> Entity::vectorToGLFloats(QVector3D vector3d){

    vector<GLfloat> glFloats;

    glFloats.push_back(vector3d.x());
    glFloats.push_back(vector3d.y());
    glFloats.push_back(vector3d.z());

    return glFloats;
}

void Entity::renderSkeleton(QMatrix4x4& viewPosition){

    vector<Joint>& joints = frozenSkeleton->getJoints();
    int numJoints = joints.size();

    glPointSize( 8.0f );
    glColor3f( 0.0f, 1.0f, 0.0f );

    glPushAttrib( GL_ENABLE_BIT );

    glDisable(GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    // Draw the joint positions
    glBegin( GL_POINTS );
    {
        for ( int i = 0; i < numJoints; i++ )
        {
            glVertex3fv(vectorToGLFloats((viewPosition *joints[i].objectPos)).data());
        }
    }
    glEnd();

    // Draw the bones
    glColor3f( 0.0f, 1.0f, 0.0f );
    glBegin( GL_LINES );
    {
        for ( int i = 0; i < numJoints; i++ )
        {
            Joint& joint = joints[i];
            int parentIndex = joints[i].parent;

            if ( parentIndex > -1 )
            {
                Joint& parent = joints[parentIndex];
                glVertex3fv(vectorToGLFloats(viewPosition * joint.objectPos).data());
                glVertex3fv(vectorToGLFloats(viewPosition * parent.objectPos).data());
            }
        }
    }
    glEnd();

    glPopAttrib();

}

void Entity::freezeWithSkeleton(Skeleton* skeleton){

    model->update(skeleton);
    frozen = true;
    frozenSkeleton = skeleton;

}

void Entity::update(double delta){

    //if we are frozen then there is no need to update.
    if(frozen){
        return;
    }

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
