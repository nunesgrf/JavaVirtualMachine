#include "../hpp/Frame.hpp"
#include "CpAttributeInterface.cpp"

Frame::Frame(std::vector<CpInfo*> cp, MethodInfo * methd) {

    CpAttributeInterface cpAtAux;

    this->pc = 0;
    this->method_reference = methd;
    this->cp_reference = cp;

    for(int i = 0; i < methd->attributes_count; i++) {
        AttributeInfo at = methd->attributes[i];
        if(cpAtAux.getUTF8(cp,at.name_index-1) == "Code") this->method_code = at.code;
    }
}