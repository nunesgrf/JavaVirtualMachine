#include "../hpp/Frame.hpp"
#include "../hpp/CpAttributeInterface.hpp"

#include <iostream>

/** @brief Destrutor de frame, libera o que é alocado no construtor de frame.
 * @param sem parâmetros
 * @return void
 */
Frame::~Frame() {
    free(this->instructions);
}

/** @brief Construtor de frame, faz a settagem das variáveis do frame.
 * @param cp vetor de de CpInfo* @param methd método sobre o qual o frame será criado.
 * @return 
 */
Frame::Frame(std::vector<CpInfo*> cp, MethodInfo * methd) {

    CpAttributeInterface cpAtAux;
    
    this->instructions = (Instruction*)calloc(256,sizeof(Instruction));
    this->instructions->init(this->instructions); // Inicializa as instruções.

    this->pc = 0;
    this->method_reference = methd;
    this->cp_reference = cp;

    for(int i = 0; i < methd->attributes_count; i++) {
        AttributeInfo at = methd->attributes[i];
        if(cpAtAux.getUTF8(cp,at.name_index-1) == "Code") this->method_code = at.code;
    }
    this->local_variables.resize(method_code.max_locals);
    Operand * op = (Operand*)calloc(1,sizeof(op));
    std::fill(this->local_variables.begin(),this->local_variables.end(),op);
}

/** @brief método de execução do código armazenado em um frame.
 * @param sem parâmetros.
 * @return void
 */
void Frame::run() {
    

    uint8_t opCode = this->method_code.code[pc];
    //std::cout << (int)this->pc << " : " << this->instructions[opCode].name << std::endl;
    //getchar();
    this->instructions[opCode].func(this);
    
}
