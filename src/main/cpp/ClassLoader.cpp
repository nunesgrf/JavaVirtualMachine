/** @file ClassLoader.cpp
*    @brief Arquivo que obtém os bytecodes do .class e realiza o carregamento dessas informações;
*    
*/

#include "../hpp/ClassLoader.hpp"
#include "../hpp/ByteReader.hpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;

/**@fn Classloader
 * @brief Construtor da classe da ClassLoader;
 * A ideia é chamar todos os Set's aqui de maneira
 * garantir a qualidade dos dados.
 * @param fp = Arquivo .class
 * @return void;
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

/**@fn ~ClassLoader
  @brief Aqui são feitas as desalocações da ClassLoader;
  @param fp =  destructor;
  @return destructor;
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

/** @class ClassLoader::setMagic
 *  @brief Método que busca identificar o formato da classe. Tem o valor de 0xCAFEBABE;
 *  @param Arquivo .class
 *  @return void;
 */ 
void ClassLoader::setMagic(FILE * fp) {
    ByteReader<typeof(magicNumber)> bReader;
    magicNumber = bReader.byteCatch(fp);
}
/*! @class Class::setMinor
 *  @brief Método como Minor busca identificar a menor versão acessivel do formato do arquivo de classe;
 *  @param Arquivo .class
 *  @return void; 
 */ 
void ClassLoader::setMinor(FILE * fp) {
    ByteReader<typeof(minorVersion)> bReader;
    minorVersion = bReader.byteCatch(fp);
}
/*! @class setMajor
 *  @brief Método como Major busca identificar a maior versão acessivel do formato do arquivo de classe;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 
void ClassLoader::setMajor(FILE * fp) {
    ByteReader<typeof(majorVersion)> bReader;
    majorVersion = bReader.byteCatch(fp);
}
/**
 *  @brief Método com o objetivo de setar a quantidade de constant pool mais 1;
 *  @param Arquivo .class
 *  @return void; 
 *
 */ 
void ClassLoader::setConstCount(FILE * fp) {
    ByteReader<typeof(constantPoolCounter)> bReader;
    constantPoolCounter = bReader.byteCatch(fp);
}
/**
 *  @brief Método com o objetivo de representar uma tebela de estruturas como strings, classes, interfaces, entre outros;
 *  O formato para cada constant pool na tabela é indicado com uma tag;
 *  @param Arquivo .class
 *  @return void; 
 */ 
void ClassLoader::setConstPool(FILE * fp) {

    for(int i = 0; i < this->getConstCount() - 1; i++) {

        CpInfo * cp = (CpInfo *)calloc(1, sizeof(CpInfo)); /* Allocate a constante pool */
        cp->read(fp);
        this->constantPool.push_back(cp); /* Puts into the vector of constant pools  */
        if ((cp->tag == CONSTANT_Double) || (cp->tag == CONSTANT_Long)) { 
            CpInfo * cp2 = (CpInfo *)calloc(1, sizeof(CpInfo));
            cp2->tag = CONSTANT_Empty;
            this->constantPool.push_back(cp2);
            i++;
        }
    }
}

/**
 *  @brief O valor setado através desse método é uma mascara de flags que denotam o acesso das propriedades da classe ou interface;
 *  @param Arquivo .class
 *  @return void;
 */ 

void ClassLoader::setAccessFlag(FILE * fp) {
    ByteReader<uint16_t> bReader;
    accessFlags = bReader.byteCatch(fp);
}

/**
 *  @brief Método que seta um index valido pego da tabela de constatnt pool que indica uma CONSTANT_Class_info;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 
void ClassLoader::setThisClass(FILE * fp) {
    ByteReader<typeof(thisClass)> bReader;
    thisClass = bReader.byteCatch(fp);
}


/**
 *  @brief Caso o valor do index passado seja diferente de zero, é tratado como uma CONSTANT_Class_info, caso contrário é tratado como objeto;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 
void ClassLoader::setSuperClass(FILE * fp) {
    ByteReader<typeof(superClass)> bReader;
    superClass = bReader.byteCatch(fp);
}


/**
 *  @brief Método que seta a quantidade de superinterfaces e tipo interface;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 

void ClassLoader::setInterCount(FILE * fp) {
    ByteReader<typeof(interfaceCounter)> bReader;
    interfaceCounter = bReader.byteCatch(fp);
}

/**
 *  @brief Método que seta uma array contendo as interfaces pegas a partir de index válidos no contant_poll;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 
void ClassLoader::setInterface(FILE * fp) {

    /* Iterate over the size of interface */
    for(int i = 0; i < this->getInterCounter(); i++) {

        /* Allocate a interface */
        InterfaceInfo * interface = (InterfaceInfo *)calloc(1, sizeof(InterfaceInfo));

        /* Puts into the vector of interfaces  */
        this->interfaces.push_back(interface);
        this->interfaces[i]->setInterfaceInfo(fp);
    }

}

/**
 *  @brief Método com o objetivo de setar a quantidade de estruturas do tipo field_info na tabela de fields ;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 
void ClassLoader::setFieldCount(FILE * fp) {
    ByteReader<typeof(fieldsCounter)> bReader;
    fieldsCounter = bReader.byteCatch(fp);
}


/**
 *  @brief Método que seta uma array de field_info ;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 
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

/**
 *  @brief Método seta a quantidade de estruturas method_info  ;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 
void ClassLoader::setMethodCount(FILE * fp) {
    ByteReader<typeof(methodsCounter)> bReader;
    methodsCounter = bReader.byteCatch(fp);
}

/**
 *  @brief Método que seta uma array de methods_info ;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 
void ClassLoader::setMethods(FILE * fp) {
    for (int i = 0; i < this->getMethoCount(); i++) {

        MethodInfo *mi = (MethodInfo *)calloc(1, sizeof(MethodInfo));
        mi->read(fp,this->constantPool);
        this->methods.push_back(mi);
    }
}

/**
 *  @brief Método que seta a quantidade de estruturas attribute_info ;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 
void ClassLoader::setAttributesCount(FILE * fp) {
    ByteReader<typeof(attributesCounter)> bReader;
    attributesCounter = bReader.byteCatch(fp);
}

/**
 *  @brief Método que seta uma array de attribute_info ;
 *  @param Arquivo .class
 *  @return void;
 *
 */ 
void ClassLoader::setAttributes(FILE * fp) {
   for(int j = 0; j < this->getAttriCount(); j++){
        AttributeInfo *attribute = (AttributeInfo *)calloc(1, sizeof(AttributeInfo));
        attribute->read(fp,this->constantPool);
        this->attributes.push_back(attribute);
   }
}

/**
 *  @brief Método que retona os field_info ;
 *  
 *  @param Arquivo .class
 *  @return Field_info
 */ 
std::vector<FieldInfo*> ClassLoader::getFields(){
    return this->fields;
}

/**
 *  @brief Método que retona os methods_info ;
 *  @param Arquivo .class
 *  @return Field_info
 */ 
std::vector<MethodInfo*> ClassLoader::getMethods(){
    return this->methods;
}

/**
 *  @brief Método que retona os attributes_info ;
 *  @param Arquivo .class
 *  @return Attribute_info
 */ 
std::vector<AttributeInfo*> ClassLoader::getAttributes(){
    return this->attributes;
}
