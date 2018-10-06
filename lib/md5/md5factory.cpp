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

void Md5Factory::buildModel(DataBuffer * buffer, QString name, std::vector<Model>& store){

    modelFactory->buildModel(buffer, name, store);
}

void Md5Factory::buildAnim(DataBuffer * buffer, QString name, std::vector<Anim>& store){

    animFactory->buildAnim(buffer, name, store);
}
