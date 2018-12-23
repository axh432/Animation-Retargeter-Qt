#include <QVector3D>
#include <QQuaternion>
#include <QOpenGLBuffer>
#include <QString>
#include <memory>
#include <vector>
#include <qopengl.h>

#include "glmesh.h"

using std::vector;
using std::unique_ptr;

#ifndef MD5_H
#define MD5_H


//Model

//TODO: if the object is deleted and a method still holds a reference to it then you're going to be in trouble.


struct ModelHeader{
    int md5Version;
    int numJoints;
    int numMeshes;
};

struct Vert{
public:
    Vert(int newIndex, int newStartWeight, int newWeightCount):
        index(newIndex), startWeight(newStartWeight), weightCount(newWeightCount)
        {/*empty*/}

    int index;
    int startWeight;
    int weightCount;
};

struct Weight{
public:
    Weight(int newIndex, int newJoint, float newBias, QVector3D newPos):
        index(newIndex), joint(newJoint), bias(newBias), pos(newPos)
    { /*empty*/ }

    int index;
    int joint;
    float bias;
    QVector3D pos;
};

struct Joint{
public:
    Joint(QVector3D pos, QQuaternion orient):
        objectPos(pos),
        objectOrient(orient)
        {/*empty*/}

    Joint(
        QString newName,
        int newParent,
        QVector3D newLocalPos,
        QQuaternion newLocalOrient
        ):
        name(newName),
        parent(newParent),
        localPos(newLocalPos),
        localOrient(newLocalOrient),
        objectPos(newLocalPos),
        objectOrient(newLocalOrient)
        { /*empty*/ }

    inline void printContents(){ qDebug() << name << ": "
                                          << parent
                                          << ", LP-" << localPos
                                          << ", LO-" << localOrient
                                          << ", OP-" << objectPos
                                          << ", OO-" << objectOrient;
                               }

    inline void printLocal(){ qDebug() << name << ": "
                                          << ", localPos-" << localPos
                                          << ", localOrient-" << localOrient;
                               }

    QString name;
    int parent;
    QVector3D localPos;
    QQuaternion localOrient;
    QVector3D objectPos;
    QQuaternion objectOrient;
};

class Skeleton {
public:
    Skeleton(vector<Joint> newJoints):
        joints(std::move(newJoints))
    {/*empty*/}

    //TODO: this needs input validation with a static factory method
    vector<Joint>& getJoints(){ return joints; }
    void recomputeObjectSpace();
    void recomputeLocalSpace();
    void testRecomputeLocalSpace();
    void printOutLocalJoints();
    void applyRotations(vector<QQuaternion>& rotations);
    static QVector3D vectorSubtract(QVector3D left, QVector3D right);
    static unique_ptr<Skeleton> interpolateSkeletons(Skeleton& previous, Skeleton& next, float interpolation);
    static vector<QQuaternion> getRotationalDifference(Skeleton* from, Skeleton* to);
    static void getPositionalDifference(Skeleton* from, Skeleton* to);

private:
    vector<Joint> joints;
};

class Mesh {
public:
    Mesh(
        QString newMaterialName,
        vector<GLfloat>   newTextureCoords,
        vector<Vert>    newVerts,
        vector<GLuint>  newTris,
        vector<Weight>  newWeights
         );

    ~Mesh();

     //TODO: this needs input validation with a static factory method
    inline QString&         getMaterialName(){ return materialName; }
    inline vector<GLuint>&  getTris(){ return tris; }
    inline vector<GLfloat>&   getTextureCoords(){ return textureCoords; }
    inline vector<Vert>&    getVerts(){ return verts; }
    inline vector<Weight>&  getWeights(){ return weights; }

    vector<float>   computeGLVertices(Skeleton* skel);

private:
    QString         materialName;
    vector<Vert>    verts;
    vector<GLuint>  tris;
    vector<Weight>  weights;
    vector<float>   textureCoords;
};

//Animation

struct AnimHeader{
    int md5Version;
    int numFrames;
    int numJoints;
    float frameRate;
    double millisecondsPerFrame;
    int numAnimatedComponents;
};



struct JointInfo{
public:
    JointInfo(
        QString name,
        int parent,
        int flags,
        int startIndex
        ):
        name(name),
        parent(parent),
        flags(flags),
        startIndex(startIndex)
        { /*empty*/ }

    QString name;
    int parent;
    int flags;
    int startIndex;
};

struct Bound{
public:
    Bound(QVector3D newCorner1, QVector3D newCorner2):
        corner1(newCorner1), corner2(newCorner2)
    {/* empty */}

    QVector3D corner1;
    QVector3D corner2;
};

struct Frame{
public:
    Frame(int newIndex, vector<float> newComponentValues):
        index(newIndex), componentValues(std::move(newComponentValues))
        {/* empty */}

    int index;
    vector<float> componentValues;
};

struct Anim{
public:
    Anim(QString name, unique_ptr<AnimHeader> newHeader, vector<Skeleton> newSkeletons, vector<Bound> newBounds):
        name(name), header(std::move(newHeader)), skeletons(std::move(newSkeletons)), bounds(std::move(newBounds))
        {/*empty*/}

    QString name;
    unique_ptr<AnimHeader> header;
    vector<Skeleton> skeletons;
    vector<Bound> bounds;
};

struct Model{
public:
    Model(QString name, unique_ptr<ModelHeader> header, vector<Mesh> newMeshes, unique_ptr<Skeleton> newBindPose):
        name(name), header(std::move(header)), meshes(std::move(newMeshes)), bindPose(std::move(newBindPose))
        {/*empty*/}

    QString name;
    unique_ptr<ModelHeader> header;
    vector<Mesh> meshes;
    unique_ptr<Skeleton> bindPose;
};

/*struct AnimInfo{
    int currentFrame;
    int nextFrame;
    double lastTime;
    double maxTime;
};*/


#endif // MD5_H
