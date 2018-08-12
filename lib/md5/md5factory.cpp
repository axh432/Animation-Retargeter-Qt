#include "md5factory.h"
#include <vector>
#include <memory>
#include "md5animfactory.h"
#include "md5modelfactory.h"
#include "md5.h"
#include "lib/TextProcessor/databuffer.h"

using std::vector;
using std::unique_ptr;

Md5Factory::Md5Factory(): modelFactory(new Md5ModelFactory()), animFactory(new Md5AnimFactory()){ /*empty*/ }

unique_ptr<Model> Md5Factory::buildModel(DataBuffer * buffer){

    return modelFactory->buildModel(buffer);
}

unique_ptr<Anim> Md5Factory::buildAnim(DataBuffer * buffer){

    return animFactory->buildAnim(buffer);
}
