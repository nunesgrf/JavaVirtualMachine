#ifndef ___ATTRIBUTEINFO_H___
#define ___ATTRIBUTEINFO_H___

#include "ClassLoader.hpp"

using namespace std;

class AttributeInfo;
class CodeException;
class CodeAttribute;
class Synthetic;
class ConstantValue;
class ClassLoader;

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
    /* Number of exceptions */
    uint16_t number_exceptions;
    uint16_t* exception_index_table;

    void read(FILE *);
};

class CodeAttribute
{
  public:
    uint16_t max_stacks;
    uint16_t max_locals;
    /* Code length */
    uint32_t code_length;
    /* It must have this length ^ */
    uint8_t *code;
    uint16_t exception_table_length;
    /* It must have this length ^ */
    CodeException *code_exception_table;
    uint16_t attributes_count;
    /* It must have this length ^ */
    AttributeInfo *attributes;

    void read(FILE*);
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
    uint16_t class_length;
    InnerClassData *inner_class_data;
    void read(FILE*);
};


class ConstantValue
{
  public:
    uint16_t constvalue_index;
    void read(FILE *);
};

class AttributeInfo
{
  public:
    /* To do: A better way to do this */
    uint16_t name_index;
    uint32_t length;
    /* It must have this length ^ */
    uint8_t *info;
    union {
        CodeAttribute code;
        ConstantValue constant_value;
        Exception exception;
        InnerClass inner_class;
    };
    void read(FILE *, std::vector<CpInfo *>);
};
#endif