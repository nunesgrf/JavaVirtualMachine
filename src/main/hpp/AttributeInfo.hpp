/** @file AttributeInfo.hpp
 *  @brief Declarações das funções do AttributeInfo para tratamento dos atributos do arquivo .class.
 *  @bug No known bugs.
 */
#ifndef ___ATTRIBUTEINFO_H___
#define ___ATTRIBUTEINFO_H___

//#include "ClassLoader.hpp"
#include <cstdint>
#include <vector>
#include <fstream>
#include "CpInfo.hpp"

using namespace std;


class AttributeInfo;
class CodeException;
class CodeAttribute;
class Synthetic;
class ConstantValue;
class ClassLoader;
class NumberTableAttribute_lineNumber;
/** @class CodeException
*   @brief classe contém start_pc, end_pc, handler_pc e catch_type - todos uint16; 
*
*/
class CodeException
{
  public:
    uint16_t start_pc;
    uint16_t end_pc;
    uint16_t handler_pc;
    uint16_t catch_type;
};
/** @class Exception
*   @brief classe contém number_exceptions e exception_index_table - todos uint16;
*       Além contém métodos como destrutor, leitor e print    
*
*/
class Exception
{
  public:
    /* Number of exceptions */
    uint16_t number_exceptions;
    uint16_t* exception_index_table;

    ~Exception();   
    void read(FILE *);
    void print(std::vector<CpInfo *>);
};
/** @class CodeAttribute
*   @brief classe Atributos que consiste em max_stacks, max_locals, code_length(uint32), ponteiro para code(uint8), exception_table_length, attribute_count e attributes - restante uint16;
*       Além contém métodos como destrutor, leitor e print    
*
*/
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

    ~CodeAttribute();
    void read(FILE*,std::vector<CpInfo*>);
    void print(std::vector<CpInfo*>);
};
/** @class InnerClassData
*   @brief classe contém inner_class_info_index, outer_class_info_index, inner_name_index e inner_class_access_flag; 
*
*/
class InnerClassData
{
  public:
    uint16_t inner_class_info_index;
    uint16_t outer_class_info_index;
    uint16_t inner_name_index;
    uint16_t inner_class_access_flag;
};
/** @class InnerClass
*   @brief classe contém class_length e ponteiro para inner_class_data - todos uint16;
*       Além disso contém metodos como read e print
*/
class InnerClass
{
  public:
    uint16_t class_length;
    InnerClassData *inner_class_data;
    void read(FILE*);
    void print(std::vector<CpInfo*>);
};

/** @class ConstantValue
*   @brief classe contém 
*       Além contém métodos como destrutor, leitor e print    
*
*/
class ConstantValue
{
  public:
    uint16_t constvalue_index;
    void read(FILE *);
    void print(std::vector<CpInfo*>);
};
/** @class SourceFile
*   @brief classe contém sourceFileIndex - todos uint16;
*       Além contém métodos como  leitor e print;    
*
*/
class SourceFile 
{
  public:

    uint16_t sourceFileIndex;
    void read(FILE *);
    void print(std::vector<CpInfo*>);
};
/** @class NumberTableAttribute
*   @brief classe contém length e ponteiro para line_number_table- todos uint16;
*       Além contém métodos como leitor e print;    
*
*/
class NumberTableAttribute
{
  public:

    uint16_t length;
    NumberTableAttribute_lineNumber *line_number_table;


    void read(FILE *);
    void print();
};
/** @class NumberTableAttribute_lineNumber
*   @brief classe contém strat_pc e lineNumber - todos uint16;
*
*/
class NumberTableAttribute_lineNumber
{
  public:

    uint16_t start_pc;
    uint16_t lineNumber;

};
/** @class AttributeInfo
*   @brief classe contém name_index e length(uint32) - todos uint16;
*        Há também uma union que tem como principio variar de acordo com o que for chamado, assim seu tipo irá depender do name_index enviado;   
*       Além contém métodos como destrutor, leitor e print    
*
*/
class AttributeInfo
{
  public:
    /* To do: A better way to do this */
    uint16_t name_index;
    uint32_t length;
    /* It must have this length ^ */
    union {
        CodeAttribute code;
        ConstantValue constant_value;
        Exception exception;
        InnerClass inner_class;
        SourceFile sourceFile;
        NumberTableAttribute lineNumber;
        uint8_t *info;
    };

    ~AttributeInfo();
    void read(FILE *, std::vector<CpInfo *>);
    void print(std::vector<CpInfo *>);
};
#endif
