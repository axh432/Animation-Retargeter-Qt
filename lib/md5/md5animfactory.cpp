#include "md5animfactory.h"
#include <memory>

#include "md5.h"
#include "lib/TextProcessor/databuffer.h"
#include "quaternioncreator.h"

using std::unique_ptr;

unique_ptr<Anim> Md5AnimFactory::buildAnim(DataBuffer* buffer){

    qDebug() << "Building Anim header";
    unique_ptr<AnimHeader> header = buildHeader(buffer);
    AnimHeader& headerRef = *header.get();

    //assert that md5Version == 10

    qDebug() << "Building Hierarchy";
    vector<JointInfo> hierarchy = buildHierarchy(buffer, headerRef);

    qDebug() << "Building Bounds";
    vector<Bound> bounds = buildBounds(buffer, headerRef);

    qDebug() << "Building BaseFrame";
    vector<Joint> baseFrame = buildBaseFrame(buffer, headerRef);

    qDebug() << "Building Frames";
    vector<Frame> frames = buildFrames(buffer, headerRef);

    qDebug() << "Building Frame Skeletons";
    vector<Skeleton> frameSkeletons = buildFrameSkeletons(hierarchy, baseFrame, frames, headerRef);

    qDebug() << "Creating Anim";
    unique_ptr<Anim> anim(new Anim(std::move(header), std::move(frameSkeletons), std::move(bounds)));

    return std::move(anim);

}

vector<Skeleton> Md5AnimFactory::buildFrameSkeletons(vector<JointInfo>& hierarchy, vector<Joint>& baseFrame, vector<Frame>& frames, AnimHeader& header){

    vector<Skeleton> skeletons;

    for(int i = 0; i < header.numFrames; i++){
        buildFrameSkeleton(skeletons, hierarchy, baseFrame, frames, i);
    }

    return skeletons;
}

void Md5AnimFactory::buildFrameSkeleton(vector<Skeleton>& skeletons, vector<JointInfo>& hierarchy, vector<Joint>& baseFrame, vector<Frame>& frames, int frameIndex){

    vector<Joint> joints(baseFrame);

    for(int jointIndex = 0; jointIndex < joints.size(); jointIndex++){

        float ox, oy, oz = 0.0f;

        Joint& joint = joints[jointIndex];
        JointInfo& jointInfo = hierarchy[jointIndex];

        joint.name = jointInfo.name;
        joint.parent = jointInfo.parent;

        int floatIndex = jointInfo.startIndex;

        //find what needs to be replaced indicated by flags
        //check bit 1
        if((1 & jointInfo.flags) == 1){
            joint.localPos.setX(frames[frameIndex].componentValues[floatIndex]);
            floatIndex++;
        }

        //check bit 2
        if((2 & jointInfo.flags) == 2){
            joint.localPos.setY(frames[frameIndex].componentValues[floatIndex]);
            floatIndex++;
        }

        //check bit 3
        if((4 & jointInfo.flags) == 4){
            joint.localPos.setZ(frames[frameIndex].componentValues[floatIndex]);
            floatIndex++;
        }

        //check bit 4
        if((8 & jointInfo.flags) == 8){
            ox = frames[frameIndex].componentValues[floatIndex];
            floatIndex++;
        }else{
            ox = joint.localOrient.x();
        }

        //check bit 5
        if((16 & jointInfo.flags) == 16){
            oy = frames[frameIndex].componentValues[floatIndex];
            floatIndex++;
        }else{
            oy = joint.localOrient.y();
        }

        //check bit 6
        if((32 & jointInfo.flags) == 32){
            oz = frames[frameIndex].componentValues[floatIndex];
            floatIndex++;
        }else{
            oz = joint.localOrient.z();
        }

        //compute the w component of the joint.
        joint.localOrient = QuaternionCreator::computeUnitQuaternion(ox, oy, oz);

    }

    Skeleton skeleton(joints);
    skeleton.recomputeObjectSpace();

    skeletons.push_back(skeleton);
}

vector<Frame> Md5AnimFactory::buildFrames(DataBuffer* buffer, AnimHeader& header){
    vector<Frame> frames;

    for(int i = 0; i < header.numFrames; i++){
        buildFrame(buffer, frames, header);
    }

    return std::move(frames);

}

void Md5AnimFactory::buildFrame(DataBuffer* buffer, vector<Frame>& frames, AnimHeader& header){


    int index = buffer->popInt();
    vector<float> values;

    for(int i = 0; i < header.numAnimatedComponents; i++){
        values.push_back(buffer->popFloat());
    }

    frames.emplace_back(index, values);
}

vector<Joint> Md5AnimFactory::buildBaseFrame(DataBuffer* buffer, AnimHeader& header){
    vector<Joint> baseFrame;

    for(int i = 0; i < header.numJoints; i++){
        buildJoint(buffer, baseFrame);
    }

    return std::move(baseFrame);
}

void Md5AnimFactory::buildJoint(DataBuffer* buffer, vector<Joint>& baseFrame){

    //needs a try/catch
    float px = buffer->popFloat();
    float py = buffer->popFloat();
    float pz = buffer->popFloat();

    float ox = buffer->popFloat();
    float oy = buffer->popFloat();
    float oz = buffer->popFloat();

    QVector3D pos(px, py, pz);
    QQuaternion orient = QuaternionCreator::computeUnitQuaternion(ox, oy, oz);

    baseFrame.emplace_back(QString(""), -1, pos, orient);

}

vector<Bound> Md5AnimFactory::buildBounds(DataBuffer* buffer, AnimHeader& header){

    vector<Bound> bounds;

    for(int i = 0; i < header.numFrames; i++){
        buildBound(buffer, bounds);
    }

    return bounds;

}

void Md5AnimFactory::buildBound(DataBuffer* buffer, vector<Bound>& bounds){

    //try / catch
    float px = buffer->popFloat();
    float py = buffer->popFloat();
    float pz = buffer->popFloat();

    float p2x = buffer->popFloat();
    float p2y = buffer->popFloat();
    float p2z = buffer->popFloat();

    QVector3D corner1(px, py, pz);
    QVector3D corner2(p2x, p2y, p2z);

    bounds.emplace_back(corner1, corner2);
}

unique_ptr<AnimHeader> Md5AnimFactory::buildHeader(DataBuffer* buffer){

    unique_ptr<AnimHeader> header(new AnimHeader);

    //try / catch
    header->md5Version = buffer->popInt();
    header->numFrames = buffer->popInt();
    header->numJoints = buffer->popInt();
    header->frameRate = (float) buffer->popInt();
    //millisecondsPerFrame = 1 second / frames per second.
    header->millisecondsPerFrame = (1000.0f / header->frameRate);
    header->numAnimatedComponents = buffer->popInt();

    return std::move(header);

}

vector<JointInfo> Md5AnimFactory::buildHierarchy(DataBuffer* buffer, AnimHeader& header){

    vector<JointInfo> hierarchy;

    for(int i = 0; i < header.numJoints; i++){
        buildJointInfo(buffer, hierarchy);
    }

    return std::move(hierarchy);

}

void Md5AnimFactory::buildJointInfo(DataBuffer* buffer, vector<JointInfo>& hierarchy){

    //try / catch
    QString name = buffer->popString();
    int parent = buffer->popInt();
    int flags = buffer->popInt();
    int startIndex = buffer->popInt();

    hierarchy.emplace_back(name, parent, flags, startIndex);
}


