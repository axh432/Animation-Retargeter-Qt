#include "md5.h"
#include <vector>
#include <memory>

#include "lib/TextProcessor/databuffer.h"

using std::vector;
using std::unique_ptr;


#ifndef MD5MODELFACTORY_H
#define MD5MODELFACTORY_H

class Md5ModelFactory
{
public:
    void buildModel(DataBuffer* buffer, QString name, std::vector<Model>& store);

private:
    unique_ptr<ModelHeader> buildHeader(DataBuffer* buffer);

    vector<Joint> buildJoints(DataBuffer* buffer, ModelHeader& header);
    void buildJoint(DataBuffer* buffer, vector<Joint>& joints);

    vector<Mesh> buildMeshes(DataBuffer* buffer, ModelHeader& header);
    void buildMesh(DataBuffer* buffer, vector<Mesh>& meshes);

    void buildVerts(DataBuffer* buffer, vector<Vert>& verts, vector<float>& textureCoords);
    void buildVert(DataBuffer* buffer, vector<Vert>& verts, vector<float>& textureCoords);

    void buildTris(DataBuffer* buffer, vector<GLuint>& tris);
    void buildTri(DataBuffer* buffer, vector<GLuint>& tris);

    void buildWeights(DataBuffer* buffer, vector<Weight>& weights);
    void buildWeight(DataBuffer* buffer, vector<Weight>& weights);
};

#endif // MD5MODELFACTORY_H
