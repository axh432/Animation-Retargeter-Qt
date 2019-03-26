#include "entity.h"

Entity::Entity(shared_ptr<GLModel> modelRef, Anim* animRef, QMatrix4x4 matrix):
    millisecondsPast(0.0f),
    interpolation(0.0f),
    currentFrame(0),
    nextFrame(0),
    model(modelRef),
    anim(animRef),
    positionOrientation(matrix),
    frozen(false),
    hidden(false),
    showBones(false),
    currentSkeleton(nullptr),
    currentAnimState(AnimState::BINDPOSE),
    currentVisualState(VisualState::VISIBLE)
{
    initializeOpenGLFunctions();
    changeAnimState(AnimState::BINDPOSE);
    changeVisualState(VisualState::VISIBLE);
}

Entity::Entity(shared_ptr<GLModel> modelRef, QMatrix4x4 matrix):
    millisecondsPast(0.0f),
    interpolation(0.0f),
    currentFrame(0),
    nextFrame(0),
    model(modelRef),
    anim(nullptr),
    positionOrientation(matrix),
    frozen(false),
    hidden(false),
    showBones(false),
    currentSkeleton(nullptr),
    currentAnimState(AnimState::BINDPOSE),
    currentVisualState(VisualState::VISIBLE)
{
    initializeOpenGLFunctions();
    changeAnimState(AnimState::BINDPOSE);
    changeVisualState(VisualState::VISIBLE);
}

void Entity::changeAnimState(AnimState newAnimState){

    if(newAnimState == AnimState::BINDPOSE){
        bindPose();
    }

    if(newAnimState == AnimState::PAUSE){

        switch(currentAnimState){
            case PLAY:
                pause();
                break;

            default:
                break;
        }

    }

    if(newAnimState == AnimState::PLAY){

        switch(currentAnimState){
            case BINDPOSE:
                if(anim){
                    play();
                }
                break;

            case PAUSE:
                play();
                break;

            default:
                break;
        }

    }

}

void Entity::changeVisualState(VisualState newVisualState){

    if(newVisualState == VisualState::HIDDEN){

        switch(currentVisualState){

        case VISIBLE:
            hide();
            break;

        case BONES:
            hide();
            break;

        }

    }

    if(newVisualState == VisualState::VISIBLE){

        switch(currentVisualState){

        case HIDDEN:
            show();
            break;

        case BONES:
            show();
            break;

        }

    }

    if(newVisualState == VisualState::BONES){

        switch(currentVisualState){

        case HIDDEN:
            bones();
            break;

        case BONES:
            show();
            break;

        }

    }

}

void Entity::render(QMatrix4x4& view){

    if(hidden){
        return;
    }

    QMatrix4x4 viewPosition = view * positionOrientation;

    model->render(viewPosition);

    //if(showBones){
        renderSkeleton(viewPosition, model->getBindPose()->getJoints().size());
    //}
}

vector<GLfloat> Entity::vectorToGLFloats(QVector3D vector3d){

    vector<GLfloat> glFloats;

    glFloats.push_back(vector3d.x());
    glFloats.push_back(vector3d.y());
    glFloats.push_back(vector3d.z());

    return glFloats;
}

void Entity::renderSkeleton(QMatrix4x4& viewPosition, int upToJointIndex){

    vector<Joint>& joints = currentSkeleton->getJoints();
    int numberOfJointsToDraw;

    if(upToJointIndex > -1 && upToJointIndex < joints.size()){
        numberOfJointsToDraw = upToJointIndex + 1; //offset for the loop
    }else{
        numberOfJointsToDraw = joints.size();
    }

    glPointSize( 8.0f );
    glColor3f( 0.0f, 1.0f, 0.0f );

    glPushAttrib( GL_ENABLE_BIT );

    glDisable(GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    // Draw the joint positions
    glBegin( GL_POINTS );
    {
        for ( int i = 0; i < numberOfJointsToDraw; i++ )
        {
            glVertex3fv(vectorToGLFloats((viewPosition *joints[i].objectPos)).data());
        }
    }
    glEnd();

    // Draw the bones
    glColor3f( 0.0f, 1.0f, 0.0f );
    glBegin( GL_LINES );
    {
        for ( int i = 0; i < numberOfJointsToDraw; i++ )
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

void Entity::hide(){
    hidden = true;
    showBones = false;

    currentVisualState = VisualState::HIDDEN;
}

void Entity::freezeWithSkeleton(Skeleton* retargetedSkeleton){
    currentSkeleton = retargetedSkeleton;
    model->update(currentSkeleton);
    frozen = true;

    millisecondsPast = 0.0f;
    interpolation = 0.0f;
    currentFrame = 0;
    nextFrame = 0;

    currentAnimState = AnimState::PAUSE;
}


void Entity::show(){
    hidden = false;
    showBones = false;

    currentVisualState = VisualState::VISIBLE;
}

void Entity::bones(){
    hidden = false;
    showBones = true;

    currentVisualState = VisualState::BONES;
}

void Entity::bindPose(){
    currentSkeleton = model->getBindPose();
    model->update(currentSkeleton);
    frozen = true;

    millisecondsPast = 0.0f;
    interpolation = 0.0f;
    currentFrame = 0;
    nextFrame = 0;

    currentAnimState = AnimState::BINDPOSE;
}

void Entity::pause(){

    frozen = true;

    millisecondsPast = 0.0f;
    interpolation = 0.0f;

    currentAnimState = AnimState::PAUSE;
}

void Entity::play(){
    frozen = false;

    currentAnimState = AnimState::PLAY;
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

    interpolatedSkeleton = std::move(Skeleton::interpolateSkeletons(anim->skeletons[currentFrame],
                                                                    anim->skeletons[nextFrame],
                                                                    interpolation));
    currentSkeleton = interpolatedSkeleton.get();

    model->update(currentSkeleton);
}

void Entity::calculateNextFrame(int numFrames){

    nextFrame++;
    nextFrame %= numFrames;

}

