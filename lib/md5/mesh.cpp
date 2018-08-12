#include "md5.h"
#include <QVector3D>
#include <QQuaternion>
#include <QString>
#include <memory>
#include <vector>
#include <iostream>


vector<float> Mesh::computeGLVertices(Skeleton* skeleton){

    vector<float> glvertices;
    vector<Joint>& joints = skeleton->getJoints();

    Joint& joint = joints[0];
    QQuaternion& orient = joint.objectOrient;
    QVector3D& pos = joint.objectPos;
    Vert& vert = verts[0];
    Weight& weight = weights[0];


    //std::cout << "Vert " << vert.index << vert.weightCount << vert.startWeight << std::endl;
    //std::cout << "Weight " << weight.index << weight.bias << weight.joint << std::endl;
    //std::cout << "Joint " << orient.x() << orient.y() << orient.z() << std::endl;

    for(int vertIndex = 0; vertIndex < verts.size(); vertIndex++){

        QVector3D finalPos(0, 0, 0);
        Vert& currentVert = verts[vertIndex];

        //iterate through each weight associated with this vert.
        for(int weightIndex = 0; weightIndex < currentVert.weightCount; weightIndex++){

            Weight& currentWeight = weights[currentVert.startWeight + weightIndex];

            Joint& joint  = joints[currentWeight.joint];

            //rotate the currentWeight position by the joint's object orientation
            QVector3D rotatedPos = joint.objectOrient.rotatedVector(currentWeight.pos);

            //sum all the weights multiplied by their bias
            finalPos += (joint.objectPos + rotatedPos) * currentWeight.bias;

        }

        glvertices.push_back(finalPos.x());
        glvertices.push_back(finalPos.y());
        glvertices.push_back(finalPos.z());
    }

    //std::cout << "GL Verts: " << glvertices[0] << ", " << glvertices[1] << ", " << glvertices[2] << std::endl;

    return glvertices;

}
