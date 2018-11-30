#include "../hpp/ClassLoader.hpp"
#include "../hpp/ByteReader.hpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;

/**
 * @brief Construtor da classe da ClassLoader;
 * A ideia é chamar todos os Set's aqui de maneira
 * garantir a qualidade dos dados.
 */

ClassLoader::ClassLoader(FILE * fp) {
    this->setMagic(fp);

    if((this->getMagic() == 0xCAFEBABE)) {
        this->setMinor(fp);
        this->setMajor(fp);
        this->setConstCount(fp);
        this->setConstPool(fp);
        this->setAccessFlag(fp);
        this->setThisClass(fp);
        this->setSuperClass(fp);
        this->setInterCount(fp);
        this->setInterface(fp);
        this->setFieldCount(fp);
        this->setFields(fp);
        this->setMethodCount(fp);
        this->setMethods(fp);
        this->setAttributesCount(fp);
        this->setAttributes(fp);
    }
    else std::cout << "invalid file pointer" << std::endl; // TODO: Modificar esse Else para algum throw ou retorno.
}

/**
 * Aqui são feitas as desalocações
 * da ClassLoader
 */

ClassLoader::~ClassLoader() {

    //std::cout << "ClassLoader::~ClassLoader begin" << std::endl;
    for(auto a : interfaces) {
        a->~InterfaceInfo();
        free(a);
    }

    for(auto a : attributes) {
        a->~AttributeInfo();
        free(a);
    }

    for(auto a : fields) {
        a->~FieldInfo();
        free(a);
    }

    for(auto a : methods) {
        a->~MethodInfo();
        free(a);
    }

    for(auto a : constantPool) {
        a->~CpInfo();
        free(a);
    }
    //std::cout << "ClassLoader::~ClassLoader end" << std::endl;
}

void ClassLoader::setMagic(FILE * fp) {
    ByteReader<typeof(magicNumber)> bReader;
    magicNumber = bReader.byteCatch(fp);
}

void ClassLoader::setMinor(FILE * fp) {
    ByteReader<typeof(minorVersion)> bReader;
    minorVersion = bReader.byteCatch(fp);
}

void ClassLoader::setMajor(FILE * fp) {
    ByteReader<typeof(majorVersion)> bReader;
    majorVersion = bReader.byteCatch(fp);
}

void ClassLoader::setConstCount(FILE * fp) {
    ByteReader<typeof(constantPoolCounter)> bReader;
    constantPoolCounter = bReader.byteCatch(fp);
}

void ClassLoader::setConstPool(FILE * fp) {

    for(int i = 0; i < this->getConstCount() - 1; i++) {

        CpInfo * cp = (CpInfo *)calloc(1, sizeof(*cp)); /* Allocate a constante pool */
        cp->read(fp);
        this->constantPool.push_back(cp); /* Puts into the vector of constant pools  */
        if ((cp->tag == CONSTANT_Double) || (cp->tag == CONSTANT_Long)) { 
            CpInfo * cp2 = (CpInfo *)calloc(1, sizeof(*cp));
            cp2->tag = CONSTANT_Empty;
            this->constantPool.push_back(cp2);
            i++;
        }
    }
}


void ClassLoader::setAccessFlag(FILE * fp) {
    ByteReader<uint16_t> bReader;
    accessFlags = bReader.byteCatch(fp);
}

void ClassLoader::setThisClass(FILE * fp) {
    ByteReader<typeof(thisClass)> bReader;
    thisClass = bReader.byteCatch(fp);
}

void ClassLoader::setSuperClass(FILE * fp) {
    ByteReader<typeof(superClass)> bReader;
    superClass = bReader.byteCatch(fp);
}

void ClassLoader::setInterCount(FILE * fp) {
    ByteReader<typeof(interfaceCounter)> bReader;
    interfaceCounter = bReader.byteCatch(fp);
}

void ClassLoader::setInterface(FILE * fp) {

    /* Iterate over the size of interface */
    for(int i = 0; i < this->getInterCounter(); i++) {

        /* Allocate a interface */
        InterfaceInfo * interface = (InterfaceInfo *)calloc(1, sizeof(*interface));

        /* Puts into the vector of interfaces  */
        this->interfaces.push_back(interface);
        this->interfaces[i]->setInterfaceInfo(fp);
    }

}

void ClassLoader::setFieldCount(FILE * fp) {
    ByteReader<typeof(fieldsCounter)> bReader;
    fieldsCounter = bReader.byteCatch(fp);
}

void ClassLoader::setFields(FILE * fp) {
    /* Iterate over the size of fields */
    for(int i = 0; i < this->getFieldCount(); i++) {

        /* Allocate a interface */
        FieldInfo* field = (FieldInfo *)calloc(1, sizeof(FieldInfo));

        /* Puts into the vector of fields  */
        field->read(fp,this->constantPool);
        this->fields.push_back(field);
    }
}

void ClassLoader::setMethodCount(FILE * fp) {
    ByteReader<typeof(methodsCounter)> bReader;
    methodsCounter = bReader.byteCatch(fp);
}

void ClassLoader::setMethods(FILE * fp) {
    for (int i = 0; i < this->getMethoCount(); i++) {

        MethodInfo *mi = (MethodInfo *)calloc(1, sizeof(MethodInfo));
        mi->read(fp,this->constantPool);
        this->methods.push_back(mi);
    }
}

void ClassLoader::setAttributesCount(FILE * fp) {
    ByteReader<typeof(attributesCounter)> bReader;
    attributesCounter = bReader.byteCatch(fp);
}

void ClassLoader::setAttributes(FILE * fp) {
   for(int j = 0; j < this->getAttriCount(); j++){
        AttributeInfo *attribute = (AttributeInfo *)calloc(1, sizeof(*attribute));
        attribute->read(fp,this->constantPool);
        this->attributes.push_back(attribute);
   }
}

std::vector<FieldInfo*> ClassLoader::getFields(){
    return this->fields;
}

std::vector<MethodInfo*> ClassLoader::getMethods(){
    return this->methods;
}

std::vector<AttributeInfo*> ClassLoader::getAttributes(){
    return this->attributes;
}