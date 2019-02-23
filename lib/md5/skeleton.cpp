#include "md5.h"
#include <QVector3D>
#include <QQuaternion>
#include <QString>
#include <memory>
#include <vector>

using std::vector;
using std::unique_ptr;

Skeleton::Skeleton(vector<Joint> newJoints):
    joints(std::move(newJoints))
{
    for(int i = 0; i < joints.size(); i++){
        jointMap.insert(joints[i].name, i);
    }
}

Skeleton::Skeleton(const Skeleton& original){
    this->joints = original.joints;

    for(int i = 0; i < joints.size(); i++){
        jointMap.insert(joints[i].name, i);
    }

}

Skeleton::Skeleton(const Skeleton* original){
    this->joints = original->joints;

    for(int i = 0; i < joints.size(); i++){
        jointMap.insert(joints[i].name, i);
    }
}

int Skeleton::getJointIndex(QString name){

    if(jointMap.contains(name)){
        return jointMap.find(name).value();
    }else{
        return -1;
    }
}

void Skeleton::applyModificationsDifferentSkeleton(vector<JointModification>& jointMods, vector<int>& mappings){

    for(int i = 0; i < mappings.size(); i++){

        int fromJointIndex = mappings[i];

        if(fromJointIndex != -1){
            joints[i].localOrient = jointMods[fromJointIndex].rotation * joints[i].localOrient;
            joints[i].localPos  = jointMods[fromJointIndex].translation + joints[i].localPos;
        }

    }

    recomputeObjectSpace();
}

void Skeleton::applyModifications(vector<JointModification>& jointMods){

    for(int i = 0; i < jointMods.size(); i++){

        joints[i].localOrient = jointMods[i].rotation * joints[i].localOrient;
        joints[i].localPos  = jointMods[i].translation + joints[i].localPos;

    }

    recomputeObjectSpace();
}

vector<JointModification> Skeleton::getDifferences(Skeleton* from, Skeleton* to){

    vector<Joint>& fromJoints = from->getJoints();
    vector<Joint>& toJoints = to->getJoints();

    vector<JointModification> differences;

    int numJoints = fromJoints.size();

    for (int i = 0; i < numJoints; i++){

        Joint& fromJoint = fromJoints[i];
        Joint& toJoint = toJoints[i];

        differences.emplace_back(getRotationalDifference(fromJoint, toJoint), getTranslationalDifference(fromJoint, toJoint));

    }

    return differences;
}

QVector3D Skeleton::getTranslationalDifference(Joint& from, Joint& to){

    QVector3D& fromPos = from.localPos;
    QVector3D& toPos = to.localPos;

    //return toPos - fromPos;
    return QVector3D();
}

QQuaternion Skeleton::getRotationalDifference(Joint& from, Joint& to){

    QQuaternion& fromOrient = from.localOrient;
    QQuaternion& toOrient = to.localOrient;

    QQuaternion difference = toOrient * fromOrient.inverted();
    difference.normalize();

    return difference;
}

unique_ptr<Skeleton> Skeleton::interpolateSkeletons(Skeleton& previous, Skeleton& next, float interpolation){

    vector<Joint>& previousJoints = previous.getJoints();
    vector<Joint>& nextJoints = next.getJoints();
    vector<Joint> newJoints;

    int numJoints = previousJoints.size();

    for (int i = 0; i < numJoints; i++)
      {
        Joint& previousJoint = previousJoints[i];
        Joint& nextJoint = nextJoints[i];

        QVector3D& previousPosition = previousJoint.objectPos;
        QVector3D& nextPosition = nextJoint.objectPos;

        QQuaternion& previousOrient = previousJoint.objectOrient;
        QQuaternion& nextOrient = nextJoint.objectOrient;

        //linear interp for pos
        QVector3D position = previousPosition;

        QVector3D vectorToNext = nextPosition - previousPosition;

        vectorToNext *= interpolation;

        position += vectorToNext;

        //slerp for orient
        QQuaternion orient = QQuaternion::slerp(previousOrient, nextOrient, interpolation);

        newJoints.emplace_back(position, orient);
      }

    return std::move(unique_ptr<Skeleton>(new Skeleton(newJoints)));
}

void Skeleton::recomputeLocalSpace(){

    for(int i = 0; i < joints.size(); i++){

        int parentIndex = joints[i].parent;
        Joint& current = joints[i];

        //does the joint have a parent?
        if(parentIndex > -1){

            //get the parent.
            Joint& parent = joints[parentIndex];
            QQuaternion invertedParentOrientation = parent.objectOrient.inverted();

            //compute the local space orientation of the joint.
            current.localOrient = invertedParentOrientation * current.objectOrient;

            //add this new position to the object position of the parent
            QVector3D rotatedPos = current.objectPos - parent.objectPos;

            //rotate the local position of the current joint.
            current.localPos = invertedParentOrientation.rotatedVector(rotatedPos);

        }

        //else local pos and orient are the same as object pos and orient.
    }

}

void Skeleton::recomputeObjectSpace() {

    for(int i = 0; i < joints.size(); i++){

        int parentIndex = joints[i].parent;
        Joint& current = joints[i];

        //does the joint have a parent?
        if(parentIndex > -1){

            //get the parent.
            Joint& parent = joints[parentIndex];

            //rotate the local position of the current joint.
            QVector3D rotatedPos = parent.objectOrient.rotatedVector(current.localPos);

            //add this new position to the object position of the parent
            current.objectPos = rotatedPos + parent.objectPos;

            //compute the object space orientation of the joint.
            current.objectOrient = parent.objectOrient * current.localOrient;
            current.objectOrient.normalize(); //this is left in, in david henry's code

        }
        //else object pos and orient are the same as local pos and orient.
    }
}
