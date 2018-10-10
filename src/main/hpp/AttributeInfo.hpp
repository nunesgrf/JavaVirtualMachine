#ifndef ___ATTRIBUTEINFO_H___
#define ___ATTRIBUTEINFO_H___

#include <cstdint>
#include <vector>
#include "ClassLoader.hpp"

using namespace std;

class AttributeInfo;


class CodeException
{
  public:
    uint16_t start_pc;
    uint16_t end_pc;
    uint16_t handler_pc;
    uint16_t catch_type;
};

class Exception
{
  public:
    uint16_t attribute_name_index;
    uint16_t attribute_length;
    /* Number of exceptions */
    uint16_t number_exceptions;
    vector<uint16_t> exception_index_table;
};

class CodeAttribute
{
  public:
    uint16_t name_index;
    uint16_t max_stacks;
    uint16_t max_locals;
    /* Attribute length */
    uint32_t length;
    /* Code length */
    uint32_t code_length;
    /* It must have this length ^ */
    vector<uint8_t> code;
    uint16_t exception_length;
    /* It must have this length ^ */
    vector<CodeException *> code_exception;
    uint16_t attributes_count;
    /* It must have this length ^ */
    vector<AttributeInfo *> attributes;

    CodeAttribute read(ClassLoader, FILE*, AttributeInfo);
};

class InnerClassData
{
  public:
    uint16_t inner_class_info_index;
    uint16_t outer_class_info_index;
    uint16_t inner_name_index;
    uint16_t inner_class_access_flag;
};

class InnerClass
{
  public:
    uint16_t name_index;
    uint32_t length;
    uint16_t class_length;
    vector<InnerClassData *> inner_class_data;
    InnerClass read(ClassLoader, FILE, AttributeInfo);
};

class Synthetic
{
  public:
    uint16_t name_index;
    uint32_t length;
    Synthetic read(ClassLoader, FILE *, AttributeInfo);
};

class ConstantValue
{
  public:
    uint16_t constvalue_index;
    ConstantValue read(ClassLoader,FILE *, AttributeInfo);
};

class AttributeInfo
{
  public:
    uint16_t name_index;
    uint32_t length;
    uint8_t *info;
    /* It must have this length ^ */
    union {
        CodeAttribute code;
        ConstantValue constant_value;
        Exception exception;
        InnerClass inner_class;
        Synthetic synthetic;
    };
    
    void read(ClassLoader, FILE *);
    
};
#endif