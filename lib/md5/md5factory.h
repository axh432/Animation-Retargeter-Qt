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
    void buildModel(DataBuffer* buffer, QString name, std::vector<Model>& store);
    void buildAnim(DataBuffer* buffer, QString name, std::vector<Anim>& store);

private:
    unique_ptr<Md5ModelFactory> modelFactory;
    unique_ptr<Md5AnimFactory>  animFactory;
};


#endif // MD5FACTORY_H
