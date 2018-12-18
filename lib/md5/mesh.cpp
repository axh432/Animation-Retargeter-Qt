#include "md5.h"
#include <QVector3D>
#include <QQuaternion>
#include <QString>
#include <memory>
#include <vector>
#include <iostream>


Mesh::Mesh(
    QString         newMaterialName,
    vector<GLfloat>   newTextureCoords,
    vector<Vert>    newVerts,
    vector<GLuint>  newTris,
    vector<Weight>  newWeights
     ):
    materialName(newMaterialName),
    textureCoords(std::move(newTextureCoords)),
    verts(std::move(newVerts)),
    tris(std::move(newTris)),
    weights(std::move(newWeights)){

}

Mesh::~Mesh(){
}

vector<GLfloat> Mesh::computeGLVertices(Skeleton* skeleton){

    vector<GLfloat> glvertices;
    vector<Joint>& joints = skeleton->getJoints();

    this->getWeights();

    Joint& joint = joints[0];
    QQuaternion& orient = joint.objectOrient;
    QVector3D& pos = joint.objectPos;
    Vert& vert = verts[0];
    Weight& weight = weights[0];

    if(weights.empty()){
        std::cout << "weights are empty!" << std::endl;
    }

    if(verts.empty()){
        std::cout << "verts are empty!" << std::endl;
    }

    /*std::cout << "Vert " << vert.index << vert.weightCount << vert.startWeight << std::endl;
    std::cout << "Weight " << weight.index << weight.bias << weight.joint << std::endl;
    std::cout << "Joint " << orient.x() << orient.y() << orient.z() << std::endl; */

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
