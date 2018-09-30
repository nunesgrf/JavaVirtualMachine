#ifndef ___JAVA_H___
#define ___JAVA_H___

#define MAGIC_NUMBER 0xCAFEBABE;
#define CONSTANT_Utf8 1
#define CONSTANT_Integer 3
#define CONSTANT_Float 4
#define CONSTANT_Long 5
#define CONSTANT_Double 6
#define CONSTANT_Class 7
#define CONSTANT_String 8
#define CONSTANT_Fieldref 9
#define CONSTANT_Methodref 10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_NameAndType 12

#include <cstdint>
#include <iostream>
#include <vector>

#include "ByteReader.cpp"
#include "CpInfo.cpp"



class JavaClass {

    private: 
        uint32_t magicNumber;
        uint16_t minorVersion;
        uint16_t majorVersion;
        uint16_t constantPoolCounter;
        std::vector<CpInfo*> constantPool;
        uint16_t acessFlags;
        uint16_t thisClass;
        uint16_t superClass;
        uint16_t interfaceCounter;
        std::vector<uint16_t> interfaces;
        uint16_t fieldsCounter;
        //std::vector<FieldsInfo> fields;
        uint16_t methodsCounter;
        //std::vector<MethodInfo> methods;
        uint16_t attributesCounter;
        //std::vector<AttributeInfo> attributes;

        bool verify();

    public:

        void setMagic(FILE * fp);
        void setMajor(FILE * fp);
        void setMinor(FILE * fp);
        void setConstCount(FILE * fp);
        void setConstPool(FILE * fp);
        void setAcessFlag(FILE * fp);
        void setThisClass(FILE * fp);
        void setSuperClass(FILE * fp);
        void setInterCount(FILE * fp);
        void setInterface(FILE * fp);
        void setFieldCount(FILE * fp);
        void setFields(FILE * fp);
        void setMethoCount(FILE * fp);
        void setMethods(FILE * fp);
        void setAttriCount(FILE * fp);
        void setAttributes(FILE * fp);

        typeof(magicNumber) getMagic() {
            return magicNumber;
        }
        typeof(majorVersion) getMajor() {
            return majorVersion;
        }
        typeof(minorVersion) getMinor() {
            return minorVersion;
        }
        typeof(constantPoolCounter) getConstCount() {
            return constantPoolCounter;
        }
        /*std::vector<CpIntro> getConstPool() {
            return constantPool;
        }*/
        typeof(acessFlags) getFlag() {
            return acessFlags;
        }
        typeof(thisClass) getThisClass() {
            return thisClass;
        }
        typeof(superClass) getSuper() {
            return superClass;
        }
        typeof(interfaceCounter) getInterCounter() {
            return interfaceCounter;
        }
        std::vector<uint16_t> getInterfaces() {
            return interfaces;
        }
        typeof(fieldsCounter) getFieldCount() {
            return fieldsCounter;
        }
        /*std::vector<FieldsInfo> getFields() {
            return fields;
        }*/
        typeof(methodsCounter) getMethoCount() {
            return methodsCounter;
        }
        /*std::vector<MethodInfo> getMethods() {
            return methods;
        }*/
        typeof(attributesCounter) getAttriCount() {
            return attributesCounter;
        }
        /*std::vector<AttributeInfo> getAttributes() {
            return attributes;
        }*/

};

bool JavaClass::verify() {
    return true;
}

void JavaClass::setMagic(FILE * fp) {
    ByteReader<typeof(magicNumber)> bReader;
    magicNumber = bReader.byteCatch(fp);
}

void JavaClass::setMinor(FILE * fp) {
    ByteReader<typeof(minorVersion)> bReader;
    minorVersion = bReader.byteCatch(fp);
}

void JavaClass::setMajor(FILE * fp) {
    ByteReader<typeof(majorVersion)> bReader;
    majorVersion = bReader.byteCatch(fp);
}

void JavaClass::setConstCount(FILE * fp) {
    ByteReader<typeof(constantPoolCounter)> bReader;
    constantPoolCounter = bReader.byteCatch(fp);
}

void JavaClass::setConstPool(FILE * fp) {
    
    ByteReader<uint8_t>  OneByte;
    ByteReader<uint16_t> TwoByte;
    ByteReader<uint32_t> FourByte;

    for(int i = 0; i < this->getConstCount(); i++) {

        CpInfo * cp = (CpInfo *)malloc(sizeof(*cp));

        constantPool.push_back(cp);
        this->constantPool[i]->tag = OneByte.byteCatch(fp);

        switch(this->constantPool[i]->tag) {
            case CONSTANT_Utf8: 
        
                this->constantPool[i]->UTF8.length = TwoByte.byteCatch(fp);

                for(int j = 0; j < this->constantPool[i]->UTF8.length; j++) {
                    //this->constantPool[i]->UTF8.bytes.push_back(OneByte.byteCatch(fp));
                    /**
                     * Desse jeito acima funciona e apresenta os resultados corretos, 
                     * mas o Valgrind acusa uma série de erros que seria interessante 
                     * verificar depois.
                     */
                    OneByte.byteCatch(fp);
                    /**
                     * Com isso, uso esta linha acima simplesmente para ignorar o tamanho
                     * do vector em bytes
                     */
                }
                break;
            case CONSTANT_Integer: 
                this->constantPool[i]->Integer.bytes = FourByte.byteCatch(fp);
                break;
            case CONSTANT_Float: 
                this->constantPool[i]->Float.bytes = FourByte.byteCatch(fp);
                break;
            case CONSTANT_Long: 
                this->constantPool[i]->Long.high_bytes = FourByte.byteCatch(fp);
                this->constantPool[i]->Long.low_bytes  = FourByte.byteCatch(fp);
                break;
            case CONSTANT_Double: 
                this->constantPool[i]->Double.high_bytes = FourByte.byteCatch(fp);
                this->constantPool[i]->Double.low_bytes  = FourByte.byteCatch(fp);
                break;
            case CONSTANT_Class:
                this->constantPool[i]->Class.name_index = TwoByte.byteCatch(fp);
                break;
            case CONSTANT_String: 
                this->constantPool[i]->String.string_index = TwoByte.byteCatch(fp);
                break;
            case CONSTANT_Fieldref:
                this->constantPool[i]->Fieldref.class_index = TwoByte.byteCatch(fp);
                this->constantPool[i]->Fieldref.name_and_type_index = TwoByte.byteCatch(fp);
                break;
            case CONSTANT_Methodref: 
                this->constantPool[i]->Methodref.class_index = TwoByte.byteCatch(fp);
                this->constantPool[i]->Methodref.name_and_type_index = TwoByte.byteCatch(fp);
                break;
            case CONSTANT_InterfaceMethodref: 
                this->constantPool[i]->InterfaceMethodref.class_index = TwoByte.byteCatch(fp);
                this->constantPool[i]->InterfaceMethodref.name_and_type_index = TwoByte.byteCatch(fp);
                break;
            case CONSTANT_NameAndType: 
                this->constantPool[i]->NameAndType.name_index = TwoByte.byteCatch(fp);
                this->constantPool[i]->NameAndType.descriptor_index = TwoByte.byteCatch(fp);
                break;
            default:
                //TODO: Criar um 'default' melhor.
                std::cout << "Não entrou em nenhuma" << std::endl;    
        }
        free(cp);
    }
}

void JavaClass::setAcessFlag(FILE * fp) {
    ByteReader<typeof(acessFlags)> bReader;
    acessFlags = bReader.byteCatch(fp);
}

void JavaClass::setThisClass(FILE * fp) {
    ByteReader<typeof(thisClass)> bReader;
    thisClass = bReader.byteCatch(fp);
}
void JavaClass::setSuperClass(FILE * fp) {
    ByteReader<typeof(superClass)> bReader;
    superClass = bReader.byteCatch(fp);
}

void JavaClass::setInterCount(FILE * fp) {
    ByteReader<typeof(interfaceCounter)> bReader;
    interfaceCounter = bReader.byteCatch(fp);
}

void JavaClass::setInterface(FILE * fp) {
    //ByteReader<typeof(interfaces)> bReader;
    //interfaces = bReader.byteCatch(fp);
}

void JavaClass::setFieldCount(FILE * fp) {
    ByteReader<typeof(fieldsCounter)> bReader;
    fieldsCounter = bReader.byteCatch(fp);
}

void JavaClass::setFields(FILE * fp) {
    //ByteReader<typeof(fields)> bReader;
    //fields = bReader.byteCatch(fp);
}

void JavaClass::setMethoCount(FILE * fp) {
    ByteReader<typeof(methodsCounter)> bReader;
    methodsCounter = bReader.byteCatch(fp);
}

void JavaClass::setMethods(FILE * fp) {
    //ByteReader<typeof(methods)> bReader;
    //methods = bReader.byteCatch(fp);
}

void JavaClass::setAttriCount(FILE * fp) {
    ByteReader<typeof(attributesCounter)> bReader;
    attributesCounter = bReader.byteCatch(fp);
}

void JavaClass::setAttributes(FILE * fp) {
   // ByteReader<typeof(attributes)> bReader;
   // attributes = bReader.byteCatch(fp);
}

#endif // ___JAVA_H___