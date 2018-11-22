#include "md5modelfactory.h"

#include <vector>
#include <memory>

#include "md5.h"
#include "lib/TextProcessor/databuffer.h"
#include "quaternioncreator.h"

using std::vector;
using std::unique_ptr;

void Md5ModelFactory::buildModel(DataBuffer * buffer, QString name, std::vector<Model>& store){

    qDebug() << "Building Model header";
    unique_ptr<ModelHeader> header = buildHeader(buffer);
    ModelHeader& headerRef = *header.get();
    //assert that md5Version == 10


    qDebug() << "Building Joints";
    vector<Joint> joints = buildJoints(buffer, headerRef);

    qDebug() << "Building Meshes";
    vector<Mesh> meshes = buildMeshes(buffer, headerRef);

    qDebug() << "Creating the model";
    unique_ptr<Skeleton> skeleton(new Skeleton(joints));

    store.emplace_back(name, std::move(header), std::move(meshes), std::move(skeleton));
}

unique_ptr<ModelHeader> Md5ModelFactory::buildHeader(DataBuffer* buffer){

    unique_ptr<ModelHeader> header(new ModelHeader);

    //needs a try catch
    header->md5Version = buffer->popInt();
    header->numJoints = buffer->popInt();
    header->numMeshes = buffer->popInt();

    qDebug() << "md5Version: " << header->md5Version;
    qDebug() << "numJoints: " << header->numJoints;
    qDebug() << "numMeshes: " << header->numMeshes;

    return std::move(header);
}

vector<Joint> Md5ModelFactory::buildJoints(DataBuffer* buffer, ModelHeader& header){

    vector<Joint> joints;

    for(int i = 0; i < header.numJoints; i++){
        buildJoint(buffer, joints);
    }

    return std::move(joints);
}

void Md5ModelFactory::buildJoint(DataBuffer* buffer, vector<Joint>& joints){

    //needs a try/catch
    QString name = buffer->popString();
    int parent = buffer->popInt();

    float px = buffer->popFloat();
    float py = buffer->popFloat();
    float pz = buffer->popFloat();

    float ox = buffer->popFloat();
    float oy = buffer->popFloat();
    float oz = buffer->popFloat();

    QVector3D pos(px, py, pz);
    QQuaternion orient = QuaternionCreator::computeUnitQuaternion(ox, oy, oz);

    qDebug() << "joint " << joints.size() << " :" << name << " " << parent;
    qDebug() << "pos (" << pos.x() << ", " << pos.y() << ", " << pos.z() << ")";
    qDebug() << "orient (" << orient.x() << ", " << orient.y() << ", " << orient.z() << ")";

    joints.emplace_back(name, parent, pos, orient);
}

vector<Mesh> Md5ModelFactory::buildMeshes(DataBuffer* buffer, ModelHeader& header){

    vector<Mesh> meshes;

   /* for(int i = 0; i < header.numMeshes; i++){
        buildMesh(buffer, meshes);
    }*/

    for(int i = 0; i < 1; i++){
        buildMesh(buffer, meshes);
    }

    return std::move(meshes);

}

void Md5ModelFactory::buildMesh(DataBuffer* buffer, vector<Mesh>& meshes){

    QString materialName = buffer->popString();
    vector<Vert> verts;
    vector<GLuint> tris;
    vector<Weight> weights;
    vector<float> textureCoords;

    qDebug() << "building verts";
    buildVerts(buffer, verts, textureCoords);
    qDebug() << "building tris";
    buildTris(buffer, tris);
    qDebug() << "building weights";
    buildWeights(buffer, weights);

    meshes.emplace_back(materialName, std::move(textureCoords), std::move(verts), std::move(tris), std::move(weights));
}

void Md5ModelFactory::buildVerts(DataBuffer* buffer, vector<Vert>& verts, vector<float>& textureCoords){
    int numVerts = buffer->popInt();

    for(int i = 0; i < numVerts; i++){
        buildVert(buffer, verts, textureCoords);
    }

}

void Md5ModelFactory::buildVert(DataBuffer* buffer, vector<Vert>& verts, vector<float>& textureCoords){

    //try catch
    int index = buffer->popInt();
    float coordU = buffer->popFloat();
    float coordV = buffer->popFloat();
    int startWeight = buffer->popInt();
    int weightCount = buffer->popInt();

    //qDebug() << "Vert " << index << ", " << coordU << ", " << coordV << ", " << startWeight << ", " << weightCount;

    verts.emplace_back(index, startWeight, weightCount);
    textureCoords.emplace_back(coordU);
    textureCoords.emplace_back(coordV);
}

void Md5ModelFactory::buildTris(DataBuffer* buffer, vector<GLuint>& tris){
    int numTris = buffer->popInt();

    for(int i = 0; i < numTris; i++){
        buildTri(buffer, tris);
    }
}

void Md5ModelFactory::buildTri(DataBuffer* buffer, vector<GLuint>& tris){

    //try catch
    int index = buffer->popInt();
    int vertIndex1 = buffer->popInt();
    int vertIndex2 = buffer->popInt();
    int vertIndex3 = buffer->popInt();

    //qDebug() << "Tri " << index << ", " << vertIndex1 << ", " << vertIndex2 << ", " << vertIndex3;

    tris.push_back(vertIndex1);
    tris.push_back(vertIndex2);
    tris.push_back(vertIndex3);
}

void Md5ModelFactory::buildWeights(DataBuffer* buffer, vector<Weight>& weights){
    int numWeights = buffer->popInt();

    qDebug() << "numWeights " << numWeights;

    for(int i = 0; i < numWeights; i++){
        buildWeight(buffer, weights);
    }

}

void Md5ModelFactory::buildWeight(DataBuffer* buffer, vector<Weight>& weights){

    //try catch
    int index = buffer->popInt();
    int joint = buffer->popInt();
    float bias = buffer->popFloat();

    float px = buffer->popFloat();
    float py = buffer->popFloat();
    float pz = buffer->popFloat();

    QVector3D pos(px, py, pz);

   //qDebug() << "weight " << index << ", " << joint << ", " << bias;
   //qDebug() << "pos (" << pos.x() << ", " << pos.y() << ", " << pos.z() << ")";

    weights.emplace_back(index, joint, bias, pos);
}
