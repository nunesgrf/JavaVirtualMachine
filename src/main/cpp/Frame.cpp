#include "../hpp/Frame.hpp"
#include "../hpp/CpAttributeInterface.hpp"
#include <iostream>

Instance::Instance(ClassLoader * toLoad) {
    
    CpAttributeInterface cpAtAux;
    
    this->name   = cpAtAux.getUTF8(toLoad->getConstPool(),toLoad->getThisClass()-1);
    this->classe = toLoad;   
}


Frame::Frame(std::vector<CpInfo*> cp, MethodInfo * methd) {

    CpAttributeInterface cpAtAux;

    this->pc = 0;
    this->method_reference = methd;
    this->cp_reference = cp;

    for(int i = 0; i < methd->attributes_count; i++) {
        AttributeInfo at = methd->attributes[i];
        if(cpAtAux.getUTF8(cp,at.name_index-1) == "Code") this->method_code = at.code;
    }
    //this->local_variables.resize(method_code.max_locals);
}

void Frame::run() {}