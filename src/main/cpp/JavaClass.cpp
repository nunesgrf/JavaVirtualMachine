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
#include "MethodInfo.cpp"
#include "FieldInfo.cpp"
#include "AttributeInfo.cpp"




class JavaClass {

    private:
      /* The magic item supplies the magic number identifying the class file format */
      uint32_t magicNumber;
      /* The values of the minor_version and major_version items are the 
      minor and major version numbers of this class file. 
      Together, a major and a minor version number determine 
      the version of the class file format. */
      uint16_t minorVersion;
      uint16_t majorVersion;

      /* The value of the constant_pool_count item is equal to the
       number of entries in the constant_pool table plus one. */
      uint16_t constantPoolCounter;

      /* The constant_pool is a table of structures  */
      std::vector<CpInfo *> constantPool;

      /* The value of the access_flags item is a mask of flags used to denote
       access permissions to and properties of this class or interface */
      uint16_t acessFlags;


      uint16_t thisClass;
      uint16_t superClass;

      /* The value of the interfaces_count item gives the number of direct
       superinterfaces of this class or interface type. */
      uint16_t interfaceCounter;

      /* Each value in the interfaces array must be a valid
       index into the constant_pool table */
      std::vector<uint16_t> interfaces;

      /* The value of the fields_count item gives the number
       of field_info structures in the fields table */
      uint16_t fieldsCounter;

      /* The fields table includes only those fields that are declared by this class or interface. 
      It does not include items representing fields that are 
      inherited from superclasses or superinterfaces. */
      std::vector<FieldsInfo *> fields;

      /* The value of the methods_count item gives
       the number of method_info structures in the methods table. */
      uint16_t methodsCounter;

      /*  The method_info structures represent all methods declared by this class or interface type,
       including instance methods, class methods, instance initialization methods, 
       and any class or interface initialization method.  */
      std::vector<MethodInfo *> methods;

      /*The value of the attributes_count item gives the number
       of attributes in the attributes table of this class. */
      uint16_t attributesCounter;

      /* */
      std::vector<AttributeInfo *> attributes;

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
        void setMethodCount(FILE * fp);
        void setMethods(FILE * fp);
        void setAttributesCount(FILE * fp);
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
        typeof(constantPool) getConstPool() {
            return constantPool;
        }
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
        typeof(fields) getFields() {
            return fields;
        }
        typeof(methodsCounter) getMethoCount() {
            return methodsCounter;
        }
        typeof(methods) getMethods() {
            return methods;
        }
        typeof(attributesCounter) getAttriCount() {
            return attributesCounter;
        }
        typeof(attributes) getAttributes() {
            return attributes;
        }

        bool DEBUG = false;
        ~JavaClass();

};

JavaClass::~JavaClass() {
    for (auto a : constantPool) {
        free(a);
    }
}

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
    
    int a = 0;

    /* Iterate over the size of constant pool */
    for(int i = 0; i < this->getConstCount(); i++) {

        /* Allocate a constante pool */
        CpInfo * cp = (CpInfo *)calloc(1, sizeof(*cp));

        /* Puts into the vector of constant pools  */
        this->constantPool.push_back(cp);
        
        /* For the i-est constant pool vector, it catches the tag for it */
        this->constantPool[i]->tag = OneByte.byteCatch(fp);
        a+=1;

        switch(this->constantPool[i]->tag) {

            case CONSTANT_Utf8: 

                /* It reads two bytes from the file */    
                this->constantPool[i]->UTF8.length = TwoByte.byteCatch(fp);
                a+=2;


                for(int j = 0; j < this->constantPool[i]->UTF8.length; j++) {
                    /* Reads one byte from file */
                    uint8_t xd = OneByte.byteCatch(fp);
                    a+=1;

                    /* It pushes into the UTF8 array */
                    this->constantPool[i]->UTF8.bytes.push_back(xd);
                    /* Concatenates \0 for string last char */
                    this->constantPool[i]->UTF8.bytes[this->constantPool[i]->UTF8.length] = '\0';
                }
                
                break;

            case CONSTANT_Integer: 
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Integer.bytes = FourByte.byteCatch(fp);
                a+=4;
                break;

            case CONSTANT_Float:
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Float.bytes = FourByte.byteCatch(fp);
                a+=4;

                break;

            case CONSTANT_Long:
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Long.high_bytes = FourByte.byteCatch(fp);

                /* Reads 4 bytes of the file */
                this->constantPool[i]->Long.low_bytes  = FourByte.byteCatch(fp);
                a+=8;

                break;

            case CONSTANT_Double:
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Double.high_bytes = FourByte.byteCatch(fp);
                
                /* Reads 4 bytes of the file */
                this->constantPool[i]->Double.low_bytes  = FourByte.byteCatch(fp);
                a+=8;

                break;

            case CONSTANT_Class:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->Class.name_index = TwoByte.byteCatch(fp);
                a+=2;
                break;

            case CONSTANT_String:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->String.string_index = TwoByte.byteCatch(fp);
                a+=2;

                break;

            case CONSTANT_Fieldref:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->Fieldref.class_index = TwoByte.byteCatch(fp);
                
                /* Reads 2 bytes of the file */
                this->constantPool[i]->Fieldref.name_and_type_index = TwoByte.byteCatch(fp);
                a+=4;

                break;

            case CONSTANT_Methodref:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->Methodref.class_index = TwoByte.byteCatch(fp);
                this->constantPool[i]->Methodref.name_and_type_index = TwoByte.byteCatch(fp);
                a+=4;

                break;

            case CONSTANT_InterfaceMethodref:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->InterfaceMethodref.class_index = TwoByte.byteCatch(fp);
                
                /* Reads 2 bytes of the file */
                this->constantPool[i]->InterfaceMethodref.name_and_type_index = TwoByte.byteCatch(fp);
                a+=4;

                break;

            case CONSTANT_NameAndType:
                /* Reads 2 bytes of the file */
                this->constantPool[i]->NameAndType.name_index = TwoByte.byteCatch(fp);
                
                /* Reads 2 bytes of the file */
                this->constantPool[i]->NameAndType.descriptor_index = TwoByte.byteCatch(fp);
                a+=4;

                break;

            default:
                if(DEBUG) std::cout << "A invalid tag was detected" << std::endl;
        }

    }
    if(!DEBUG) std::cout << "Valor de A: " << a << std::endl;
    
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

void JavaClass::setMethodCount(FILE * fp) {
    ByteReader<typeof(methodsCounter)> bReader;
    methodsCounter = bReader.byteCatch(fp);
}

void JavaClass::setMethods(FILE * fp) {
    //ByteReader<typeof(methods)> bReader;
    //methods = bReader.byteCatch(fp);
}

void JavaClass::setAttributesCount(FILE * fp) {
    ByteReader<typeof(attributesCounter)> bReader;
    attributesCounter = bReader.byteCatch(fp);
}

void JavaClass::setAttributes(FILE * fp) {
   // ByteReader<typeof(attributes)> bReader;
   // attributes = bReader.byteCatch(fp);
}

#endif // ___JAVA_H___