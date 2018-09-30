#ifndef ___JAVA_H___
#define ___JAVA_H___

#define MAGIC_NUMBER 0xCAFEBABE;

#include <cstdint>
#include <iostream>
#include <vector>

#include "ByteReader.cpp"

class JavaClass {

    private: 
        uint32_t magicNumber;
        uint16_t minorVersion;
        uint16_t majorVersion;
        uint16_t constantPoolCounter;
        //std::vector<CpInfo> constantPool;
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
    //ByteReader<typeof(constantPool)> bReader;
    //constantPool = bReader.byteCatch(fp);
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