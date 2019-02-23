#ifndef GLMESH_H
#define GLMESH_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include "md5.h"
#include "engine/material.h"
#include "engine/gldata.h"


class GLMesh : protected QOpenGLFunctions{
public:
    GLMesh(Mesh* mesh, unique_ptr<GLData> glData, Material* material);
    void update(Skeleton* skeleton);
    void render(QMatrix4x4& mvp_matrix);
    //void checkGLValidity();

private:
    unique_ptr<GLData> glData;
    Mesh* mesh;
    Material* material;

    //void ifBuffersNotCreatedCreateBuffers();
};

class GLModel{
public:
    GLModel(std::vector<GLMesh> meshes, Skeleton* bindPose);
    void update(Skeleton* skeleton);
    void render(QMatrix4x4& mvp_matrix);
    //void checkGLValidity();

    inline std::vector<GLMesh>& getGLMeshes(){ return meshes; }
    inline Skeleton* getBindPose() { return bindPose; }

private:
    std::vector<GLMesh> meshes;
    Skeleton* bindPose;
};

#endif // GLMESH_H
