#include "../hpp/Frame.hpp"
#include "../hpp/CpAttributeInterface.hpp"
#include <iostream>

Instance::Instance(ClassLoader * toLoad) {
    
    CpAttributeInterface cpAtAux;
    
    this->name   = cpAtAux.getUTF8(toLoad->getConstPool(),toLoad->getThisClass()-1);
    this->classe = toLoad;   
}

Frame::~Frame() {
    free(this->instructions);
}

Frame::Frame(std::vector<CpInfo*> cp, MethodInfo * methd) {

    CpAttributeInterface cpAtAux;

    std::cout << "constructor" << std::endl;
    this->instructions = (Instruction*)calloc(256,sizeof(Instruction));
    std::cout << "Teste de segfault : " << this->instructions[300].bytes << endl;
    std::cout << "Isso daqui ta estranho" << std::endl;
    this->instructions->init(this->instructions); // Inicializa as instruções.

    this->pc = 0;
    this->method_reference = methd;
    this->cp_reference = cp;
    

    for(int i = 0; i < methd->attributes_count; i++) {
        AttributeInfo at = methd->attributes[i];
        if(cpAtAux.getUTF8(cp,at.name_index-1) == "Code") this->method_code = at.code;
    }
    //this->local_variables.resize(method_code.max_locals);
}

void Frame::run() {
    uint8_t opCode = this->method_code.code[pc];
    this->instructions[opCode].func();
}