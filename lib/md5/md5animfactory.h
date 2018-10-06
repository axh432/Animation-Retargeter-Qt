#include "md5.h"
#include "lib/TextProcessor/databuffer.h"
#include "quaternioncreator.h"
#include <memory>

using std::unique_ptr;

#ifndef MD5ANIMFACTORY_H
#define MD5ANIMFACTORY_H


class Md5AnimFactory
{
public:
    void buildAnim(DataBuffer* buffer, QString name, std::vector<Anim>& store);

    vector<Skeleton> buildFrameSkeletons(vector<JointInfo>& hierarchy,
                                        vector<Joint>& baseFrame,
                                        vector<Frame>& frames,
                                        AnimHeader& header);


    void buildFrameSkeleton(vector<Skeleton>& skeletons,
                            vector<JointInfo>& hierarchy,
                            vector<Joint>& baseFrame,
                            vector<Frame>& frames,
                            int frameIndex);

    vector<Frame> buildFrames(DataBuffer* buffer, AnimHeader& header);


    void buildFrame(DataBuffer* buffer,
                    vector<Frame>& frames,
                    AnimHeader& header);

    vector<Joint> buildBaseFrame(DataBuffer* buffer,
                                AnimHeader& header);

    void buildJoint(DataBuffer* buffer,
                    vector<Joint>& baseFrame);

    vector<Bound> buildBounds(DataBuffer* buffer,
                                AnimHeader& header);


    void buildBound(DataBuffer* buffer, vector<Bound>& bounds);

    unique_ptr<AnimHeader> buildHeader(DataBuffer* buffer);

    vector<JointInfo> buildHierarchy(DataBuffer* buffer, AnimHeader& header);

    void buildJointInfo(DataBuffer* buffer, vector<JointInfo>& hierarchy);


};

#endif // MD5ANIMFACTORY_H
