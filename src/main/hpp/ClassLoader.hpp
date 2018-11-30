/** @file ClassLoader.hpp
 *  @brief Declarações das funções do ClassLoader para salvar todos os bytes de .class corretamente.
 *  @bug No known bugs.
 */
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
#define CONSTANT_Boolean  90
#define CONSTANT_Byte     91
#define CONSTANT_Char     92
#define CONSTANT_Short    93
#define CONSTANT_Array 94
#define CONSTANT_Empty 0
#define typeof __typeof__

#include <cstdint>
#include <iostream>
#include <cstring>
#include <vector>
#include <memory>
#include <cstdio>

#include "CpInfo.hpp"
#include "MethodInfo.hpp"
#include "InterfaceInfo.hpp"
#include "FieldInfo.hpp"
#include "AttributeInfo.hpp"

class CpInfo;
class FieldInfo;
class MethodInfo;
class AttributeInfo;
class InterfaceInfo;

class ClassLoader {

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
      std::vector<CpInfo*> constantPool;

      /* The value of the access_flags item is a mask of flags used to denote
       access permissions to and properties of this class or interface */
      uint16_t accessFlags;


      uint16_t thisClass;
      uint16_t superClass;

      /* The value of the interfaces_count item gives the number of direct
       superinterfaces of this class or interface type. */
      uint16_t interfaceCounter;

      /* Each value in the interfaces array must be a valid
       index into the constant_pool table */
      std::vector<InterfaceInfo*> interfaces;

      /* The value of the fields_count item gives the number
       of field_info structures in the fields table */
      uint16_t fieldsCounter;

      /* The fields table includes only those fields that are declared by this class or interface.
      It does not include items representing fields that are
      inherited from superclasses or superinterfaces. */
      std::vector<FieldInfo *> fields;

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


      std::vector<AttributeInfo *> attributes;

      void setMagic(FILE * fp);
      void setMajor(FILE * fp);
      void setMinor(FILE * fp);
      void setConstCount(FILE * fp);
      void setConstPool(FILE * fp);
      void setAccessFlag(FILE * fp);
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

    public:

        uint32_t getMagic() {
          return magicNumber;
        }
        uint16_t getMajor() {
            return majorVersion;
        }
        uint16_t getMinor() {
            return minorVersion;
        }
        uint16_t getConstCount() {
            return constantPoolCounter;
        }
        std::vector<CpInfo*> getConstPool() {
            return constantPool;
        }

        uint16_t getFlag() {
            return accessFlags;
        }
        uint16_t getThisClass() {
            return thisClass;
        }
        uint16_t getSuper() {
            return superClass;
        }
        uint16_t getInterCounter() {
            return interfaceCounter;
        }

        std::vector<InterfaceInfo*> getInterfaces() {
            return interfaces;
        }

        uint16_t getFieldCount() {
            return fieldsCounter;
        }

        std::vector<FieldInfo*> getFields();

        uint16_t getMethoCount() {
            return methodsCounter;
        }

        std::vector<MethodInfo*> getMethods();

        uint16_t getAttriCount() {
            return attributesCounter;
        }

        std::vector<AttributeInfo*> getAttributes();

        bool DEBUG = true;
        ~ClassLoader();
        ClassLoader(FILE * fp);
};

#endif // ___JAVA_H___
