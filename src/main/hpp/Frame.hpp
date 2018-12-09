/** @file Frame.hpp
 *  @brief Declarações das funções e da estrutura do Frame, utilizado para salvar resultados parciais.
 *  @bug No known bugs.
 */
#ifndef _FRAME_H_
#define _FRAME_H_

#include <cstdint>
#include <string>
#include <vector>
#include <stack>
#include <map>

#include "Stack.hpp"
#include "CpInfo.hpp"
#include "MethodInfo.hpp"
#include "AttributeInfo.hpp"
#include "ClassLoader.hpp"
#include "Instruction.hpp"
#include "Instance.hpp"

struct Operand;
struct Instruction;
struct Instance;
/**
*   @struct ArrayType
*    @brief tem como base um tipo de vetor de um tipo de struct Operand;
*/
typedef struct {
    std::vector<Operand*> * array;
} ArrayType;
/**
*   @struct Operand
*   @brief  intuito de ligar aos tipos possíveis para variáveis que serão utilizadas; 
*/

struct Operand {
    uint8_t tag;
    union {
        uint32_t type_bool;
        uint32_t type_byte;
        uint32_t type_char;
        uint32_t type_short;
        uint32_t type_int;
        uint32_t type_float;
        uint64_t type_long;
        uint64_t type_double;
    };
    std::string * type_string;
    Instance * class_instance;
    ArrayType * array_type;
};
/**
*   @struct Frame
*   @brief  objetivo de estruturar o tipo Frame;
*       Além disso, contém destrutor e run(para rodar o frame);     
*/

struct Frame {
    
    uint32_t pc; // program counter
    std::stack< Operand*> operand_stack;
    CodeAttribute method_code;
    MethodInfo * method_reference;
    std::vector<CpInfo*> cp_reference;
    std::vector< Operand* > local_variables;
    Instruction * instructions;

    Frame(std::vector<CpInfo*>,MethodInfo *);
    ~Frame();
    void run();
};

#endif