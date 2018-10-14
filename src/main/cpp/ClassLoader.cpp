#include "../hpp/ClassLoader.hpp"
#include "AttributeInfo.cpp"
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

    for(int i = 0; i < this->getConstCount()-1; i++) {
        auto a = this->constantPool[i]->UTF8.bytes;
        free(a);
    }
    
    for(auto a : constantPool) {
        free(a);
    }
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
    
    ByteReader<uint8_t>  OneByte;
    ByteReader<uint16_t> TwoByte;
    ByteReader<uint32_t> FourByte;
    
    /* Iterate over the size of constant pool */
    for(int i = 0; i < this->getConstCount() - 1; i++) {

        /* Allocate a constante pool */
        CpInfo * cp = (CpInfo *)calloc(1, sizeof(*cp));

        /* Puts into the vector of constant pools  */
        this->constantPool.push_back(cp);
        
        /* For the i-est constant pool vector, it catches the tag for it */
        this->constantPool[i]->tag = OneByte.byteCatch(fp);
        
        if(DEBUG) std::cout << int(this->constantPool[i]->tag) << std::endl;

        switch(this->constantPool[i]->tag) {

            
            case CONSTANT_Utf8: 

                /* It reads two bytes from the file */    
                this->constantPool[i]->UTF8.length = TwoByte.byteCatch(fp);
                /**
                 * A alocação via calloc é feita aqui com o entuito de settar os valores como 0
                 * inicialmente. Alocala-se um total de lenght+1 posições para que ao final possa se inserir
                 * o caractere '\0' sem acessar uma região de memória não garantida para nosso
                 * programa.
                 */
                this->constantPool[i]->UTF8.bytes = (uint8_t *) calloc(this->constantPool[i]->UTF8.length+1,sizeof(uint8_t));

                for (int j = 0; j < this->constantPool[i]->UTF8.length; j++) {
                    /* Reads one byte from file */
                    uint8_t xd = OneByte.byteCatch(fp);
                    /* It pushes into the UTF8 array */
                    this->constantPool[i]->UTF8.bytes[j] = xd;
                }
                /* Concatenates \0 for string last char */
                this->constantPool[i]->UTF8.bytes[this->constantPool[i]->UTF8.length] = '\0';
                break;

            case CONSTANT_Integer: 
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Integer.bytes = FourByte.byteCatch(fp);   
                break;

            case CONSTANT_Float:
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Float.bytes = FourByte.byteCatch(fp);
                break;

            case CONSTANT_Long:
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Long.high_bytes = FourByte.byteCatch(fp);
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Long.low_bytes  = FourByte.byteCatch(fp);
                

                break;

            case CONSTANT_Double:
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Double.high_bytes = FourByte.byteCatch(fp);      
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Double.low_bytes  = FourByte.byteCatch(fp);
                

                break;

            case CONSTANT_Class:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->Class.name_index = TwoByte.byteCatch(fp);
                
                break;

            case CONSTANT_String:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->String.string_index = TwoByte.byteCatch(fp);
                

                break;

            case CONSTANT_Fieldref:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->Fieldref.class_index = TwoByte.byteCatch(fp); 
                /* Reads 2 bytes of the file */
                this->constantPool[i]->Fieldref.name_and_type_index = TwoByte.byteCatch(fp);
                

                break;

            case CONSTANT_Methodref:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->Methodref.class_index = TwoByte.byteCatch(fp);
                this->constantPool[i]->Methodref.name_and_type_index = TwoByte.byteCatch(fp);
                break;

            case CONSTANT_InterfaceMethodref:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->InterfaceMethodref.class_index = TwoByte.byteCatch(fp);          
                /* Reads 2 bytes of the file */
                this->constantPool[i]->InterfaceMethodref.name_and_type_index = TwoByte.byteCatch(fp);
                break;

            case CONSTANT_NameAndType:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->NameAndType.name_index = TwoByte.byteCatch(fp);      
                /* Reads 2 bytes of the file */
                this->constantPool[i]->NameAndType.descriptor_index = TwoByte.byteCatch(fp);
                break;

            default:
                if(DEBUG) std::cout << "A invalid tag was detected" << std::endl;
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
        FieldInfo * field = (FieldInfo *)calloc(1, sizeof(*field));

        /* Puts into the vector of fields  */
        this->fields.push_back(field);
        this->fields[i]->setFieldInfo(fp,field);
    }
}

void ClassLoader::setMethodCount(FILE * fp) {
    ByteReader<typeof(methodsCounter)> bReader;
    methodsCounter = bReader.byteCatch(fp);
}

void ClassLoader::setMethods(FILE * fp) {
    //ByteReader<typeof(methods)> bReader;
    //methods = bReader.byteCatch(fp);
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
