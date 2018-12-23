#include "md5.h"
#include <QVector3D>
#include <QQuaternion>
#include <QString>
#include <memory>
#include <vector>

using std::vector;
using std::unique_ptr;

QVector3D Skeleton::vectorSubtract(QVector3D left, QVector3D right){

    QVector3D result;

    result.setX(left.x() - right.x());
    result.setY(left.y() - right.y());
    result.setZ(left.z() - right.z());

    return result;
}

void Skeleton::applyRotations(vector<QQuaternion>& rotations){

    for(int i = 0; i < rotations.size(); i++){

        joints[i].localOrient = rotations[i] * joints[i].localOrient;

    }

}

//requires that both skeletons are from the same model
void Skeleton::getPositionalDifference(Skeleton* from, Skeleton* to){

    vector<Joint>& fromJoints = from->getJoints();
    vector<Joint>& toJoints = to->getJoints();

    vector<QVector3D> differences;

    int numJoints = fromJoints.size();

    qDebug() << "Pos Difference";

    for (int i = 0; i < numJoints; i++){

        QVector3D& fromPos = fromJoints[i].localPos;
        QVector3D& toPos = toJoints[i].localPos;

        QVector3D difference = toPos - fromPos;

        QVector3D test = difference + fromPos;

        differences.push_back(test);

    }

}

//requires that both skeletons are from the same model
vector<QQuaternion> Skeleton::getRotationalDifference(Skeleton* from, Skeleton* to){

    vector<QQuaternion> rotations;

    vector<Joint>& fromJoints = from->getJoints();
    vector<Joint>& toJoints = to->getJoints();

    int numJoints = fromJoints.size();

    for (int i = 0; i < numJoints; i++)
      {
        /*
         * you would have to use the local orient because when you change the
         * orientation of a parent you change the object orient of the child
         * and you will always be changing the parent orient
         */
        QQuaternion& fromOrient = fromJoints[i].localOrient;
        QQuaternion& toOrient = toJoints[i].localOrient;

        QQuaternion difference = toOrient * fromOrient.inverted();
        difference.normalize();

        QQuaternion testOrient = difference * fromOrient;

        rotations.push_back(difference);

        /*int parentIndex = fromJoint.parent;

        if(parentIndex < 0){
            continue;
        }

        Joint& fromParent = fromJoints[fromJoint.parent];

        Joint& toJoint = toJoints[i];
        Joint& toParent = toJoints[toJoint.parent];

        QVector3D startPos = fromJoint.objectPos - fromParent.objectPos;
        QVector3D endPos = toJoint.objectPos - toParent.objectPos;

        rotations.push_back(QQuaternion::rotationTo(startPos, endPos));*/

    }

    return rotations;

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

        QVector3D vectorToNext = Skeleton::vectorSubtract(nextPosition, previousPosition);

        vectorToNext *= interpolation;

        position += vectorToNext;

        //slerp for orient
        QQuaternion orient = QQuaternion::slerp(previousOrient, nextOrient, interpolation);

        newJoints.emplace_back(position, orient);
      }

    return unique_ptr<Skeleton>(new Skeleton(newJoints));
}

void Skeleton::testRecomputeLocalSpace(){

    vector<Joint> newJoints = joints;

    Skeleton copySkeleton(newJoints);

    qDebug() << "--- Pre Local Space ---";
    copySkeleton.printOutLocalJoints();

    copySkeleton.recomputeLocalSpace();

    qDebug() << "--- Post Local Space ---";
    copySkeleton.printOutLocalJoints();

}

void Skeleton::printOutLocalJoints(){

    for(int i = 0; i < joints.size(); i++){
        joints[i].printLocal();
    }

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

        //else object pos and orient are the same as local pos and orient.
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

        current.printContents();
        //else object pos and orient are the same as local pos and orient.
    }
}
