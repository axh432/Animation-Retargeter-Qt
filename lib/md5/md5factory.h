#ifndef MD5FACTORY_H
#define MD5FACTORY_H

#include <memory>
#include "md5.h"
#include "md5modelfactory.h"
#include "md5animfactory.h"
#include "lib/TextProcessor/databuffer.h"

using std::unique_ptr;

class Md5Factory
{
public:
    Md5Factory();
    unique_ptr<Model> buildModel(DataBuffer* buffer);
    unique_ptr<Anim>  buildAnim(DataBuffer* buffer);

private:
    unique_ptr<Md5ModelFactory> modelFactory;
    unique_ptr<Md5AnimFactory>  animFactory;
};


#endif // MD5FACTORY_H
