#include "../hpp/Frame.hpp"
#include "../hpp/CpAttributeInterface.hpp"
#include "../hpp/GLOBAL_file.hpp"
#include "../hpp/Interpreter.hpp"
#include <iostream>
#include <string>

/*
 * @brief Incrementa o program counter.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
void InstructionImpl::nop(Frame * this_frame) {
    this_frame->pc++;
}

 void InstructionImpl::ldc(Frame * this_frame){

   this_frame->pc++;
   Operand * op = (Operand*)calloc(1,sizeof(Operand));
   CpAttributeInterface cpAttrAux;
   
   /* Pegando o indice do para acessar a constante no pool de constantes */
   uint8_t index = this_frame->method_code.code[this_frame->pc++];
   
   /* Referência para a pool de constantes */
   CpInfo * Cp = this_frame->cp_reference[index-1];

   /* Obtem-se a flag para acessar o tipo da constante */
   op->tag = Cp->tag;

   /* Verifica o tipo da tag */
   switch(op->tag){
      case CONSTANT_String:
      {
         /* Caso seja string, acessar o UTF8 correspondente a ela */
         std::string utf8s = cpAttrAux.getUTF8(this_frame->cp_reference,Cp->String.string_index-1);
         op->type_string = new std::string(utf8s);
         break;
      }
      case CONSTANT_Float:
      {
          /* Caso seja float, acessar os bytes correspondentes a ele */
         op->type_float = Cp->Float.bytes;
         break;
      }
      case CONSTANT_Integer:
      {
         /* Caso seja int, acessar os bytes correspondentes a ele */
         op->type_int = Cp->Integer.bytes;
         break;
      }
      default:
      {
         std::cout << "BUG LDC SWITCH DEFAULT";
         break;
      }
   }

   this_frame->operand_stack.push(op);
    
 }
 void InstructionImpl::invokespecial(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::invokevirtual(Frame * this_frame){

   CpAttributeInterface cpAttrAux;
   this_frame->pc++;
   /* Recupera o valor dos indices que formam a referencia a pool de constantes */
   uint16_t indexbyte1 = this_frame->method_code.code[this_frame->pc++];
   uint16_t indexbyte2 = this_frame->method_code.code[this_frame->pc++];

   /* Constrói a referência */
   uint16_t index = (indexbyte1 << 8) | indexbyte2;

   /* Constrói uma referência ao metodo */
   CpInfo * method_reference = this_frame->cp_reference[index-1];

   /* A partir da referencia do metodo, constrói-se a reference para o nome e o tipo do método */
   CpInfo * name_and_type = this_frame->cp_reference[method_reference->Methodref.name_and_type_index-1];

   /* Para descobrir o qual o metodo, pegamos o nome da classe, nome do metodo e descritor dele */
   std::string class_name = cpAttrAux.getUTF8(this_frame->cp_reference,method_reference->Methodref.class_index-1);
   std::string method_name = cpAttrAux.getUTF8(this_frame->cp_reference,name_and_type->NameAndType.name_index-1);
   std::string method_deor = cpAttrAux.getUTF8(this_frame->cp_reference,name_and_type->NameAndType.name_index-1);

   /* Inicia-se o procedimento para verificar se o metodo a ser chamado é um print f */
   if((class_name == "java/io/PrintStream") && (method_name == "println" || method_deor == "print")){
      if(method_deor != "()V"){
         Operand * op = this_frame->operand_stack.top();
         this_frame->operand_stack.pop();

         switch(op->tag) {
            case CONSTANT_String:
               std::cout << (*op->type_string);
               break;
            case CONSTANT_Integer:
               std::cout << op->type_int;
               break;
            case CONSTANT_Float:
               float converted_operand;
               memcpy(&converted_operand,&op->type_float,sizeof(float));
               std::cout << converted_operand;
               break;
            case CONSTANT_Byte:
               std::cout << (int) op->type_byte;
               break;
            case CONSTANT_Char:
               std::cout << (char) op->type_char;
               break;
            case CONSTANT_Short:
               std::cout << (short) op->type_short;
               break;
            case CONSTANT_Boolean:
               std::cout << (bool) op->type_bool;
               break;
            case CONSTANT_Long:
               std::cout << (long) op->type_long;
               break;
            case CONSTANT_Empty:
               printf("null");
               break;
            case CONSTANT_Double: {
               double converted_operand;
               memcpy(&converted_operand, &op->type_double, sizeof(double));
               std::cout << converted_operand; 
               break;
            }
            case CONSTANT_Class: {
               Instance * class_instance = op->class_instance;
               ClassLoader * class_loader = class_instance->classe;
               std::string class_name = cpAttrAux.getUTF8(class_loader->getConstPool(),class_loader->getThisClass());
               std::cout << class_name << "@" << class_instance;
               break;
            }
         }

         if(method_name == "println") std::cout << std::endl;
      }
   }
 }

/*
 * @brief desempilha um operando do topo da pilha de operandos, 
 * guardando sua referência do array de variáveis locais na posiçao 1
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::astore_1(Frame * this_frame){
   this_frame->pc++;
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   this_frame->local_variables.at(1) = op;
 }

/*
 * @brief desempilha um operando do topo da pilha de operandos, 
 * guardando sua referência do array de variáveis locais na posiçao 2
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::astore_2(Frame * this_frame){
   InstructionImpl::nop(this_frame);
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   this_frame->local_variables.at(2) = op;
   this_frame->pc++;
 }

 /*
 * @brief desempilha um operando do topo da pilha de operandos, 
 * guardando sua referência do array de variáveis locais na posiçao 3
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::astore_3(Frame * this_frame){
   InstructionImpl::nop(this_frame);
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   this_frame->local_variables.at(3) = op;
   this_frame->pc++;
     
 }
 void InstructionImpl::getstatic(Frame * this_frame){

    InstructionImpl::nop(this_frame);

    Interpreter aux;
    MethodsArea container;
    CpAttributeInterface cpAt;
    uint16_t pos = this_frame->method_code.code[this_frame->pc++]; 
    pos = (pos << 8) + this_frame->method_code.code[this_frame->pc++];

    CpInfo * field = this_frame->cp_reference[pos-1];
    CpInfo * name_n_type = this_frame->cp_reference[field->Fieldref.name_and_type_index-1];

    std::string className = cpAt.getUTF8(this_frame->cp_reference,field->Fieldref.class_index-1);
    if(className == "java/lang/System") return; // Não tenta acessar caso seja um java/lang/Syste,.

    auto classloader         = aux.getClassInfo(className);
    std::string variableName = cpAt.getUTF8(classloader->getConstPool(),name_n_type->NameAndType.name_index-1);
    auto staticField         = container.getStaticfield(className,variableName);
    this_frame->operand_stack.push(staticField);
 }

/*
 * @brief retorna uma referência a um objeto, contido em um array de objetos e coloca tal referência na stack
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */ 
 void InstructionImpl::aaload(Frame * this_frame){
   InstructionImpl::nop(this_frame);
   Operand * index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * array = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   if(array == NULL) std::cout << "NullPointerException" << std::endl;
   if(index->type_int < 0 ||  index->type_int >= array->array_type.array.size()) std::cout << "ArrayIndexOutOfBoundsException" << std::endl;

   Operand * op = array->array_type.array.at(index->type_int);
   this_frame->operand_stack.push(op);
     
 }

/*
 * @brief Empilha uma referência nula a um objeto na pilha de operandos
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::aconst_null(Frame * this_frame){
    Operand * op = (Operand*)calloc(1,sizeof(Operand));
    op->tag = CONSTANT_Empty;
    this_frame->operand_stack.push(op);
    this_frame->pc++;
 }

/** @brief Empilha o inteiro -1 na stack de operandos
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iconst_m1(Frame * this_frame){

   Operand * op = (Operand*)calloc(1,sizeof(Operand));
   op->tag = CONSTANT_Integer;
   op->type_int = -1;
   this_frame->operand_stack.push(op);
   this_frame->pc++;
 }

  /** @brief Empilha 0 na stack de operandos
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iconst_0(Frame * this_frame){

   Operand * op = (Operand*)calloc(1,sizeof(Operand));
   op->tag = CONSTANT_Integer;
   op->type_int = 0;
   this_frame->operand_stack.push(op);
   this_frame->pc++;
 }
 /** @brief Empilha 1 na stack de operandos
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iconst_1(Frame * this_frame){
   
   Operand * op = (Operand*)calloc(1,sizeof(Operand));
   op->tag = CONSTANT_Integer;
   op->type_int = 1;
   this_frame->operand_stack.push(op);
   this_frame->pc++;
 }

/** @brief Empilha 2 na stack de operandos
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iconst_2(Frame * this_frame){

   Operand * op = (Operand*)calloc(1,sizeof(Operand));
   op->tag = CONSTANT_Integer;
   op->type_int = 2;
   this_frame->operand_stack.push(op);   
   this_frame->pc++;
 }

 /** @brief Empilha 3 na stack de operandos
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iconst_3(Frame * this_frame){

    Operand * op = (Operand*)calloc(1,sizeof(Operand));
    op->tag = CONSTANT_Integer;
    op->type_int = 3;
    this_frame->operand_stack.push(op);
    this_frame->pc++;
   
 }

 /** @brief Empilha 4 na stack de operandos
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iconst_4(Frame * this_frame){
   Operand * op = (Operand*)calloc(1,sizeof(Operand));
   op->tag = CONSTANT_Integer;
   op->type_int = 4;
   this_frame->operand_stack.push(op); 
   this_frame->pc++;  
 }

 /** @brief Empilha 5 na stack de operandos
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iconst_5(Frame * this_frame){
   
   Operand * op = (Operand*)calloc(1,sizeof(Operand));
   op->tag = CONSTANT_Integer;
   op->type_int = 5;
   this_frame->operand_stack.push(op);   
   this_frame->pc++;   
 }

 /** @brief Busca o campo de um objeto
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::getfield(Frame * this_frame){
   CpAttributeInterface cpAtAux;
   /* Busca o indexbyte1 e indexbyte2 no código seguinte a instruçao getfield */
   uint8_t indexbyte1 = this_frame->method_code.code[this_frame->pc++];
   uint8_t indexbyte2 = this_frame->method_code.code[this_frame->pc++];

   /* Forma o indice para a pool de constantes com os indexbyte1 e indexbyte2 */
   uint16_t cp_index = (indexbyte1 << 8) | indexbyte2;

   /* Acessa uma referencia a field na pool de constantes */
   CpInfo * field_ref = this_frame->cp_reference[cp_index-1];

   /* A partir da referência pro field, acessamos uma referencia na pool de constantes para name_and_type */
   CpInfo * name_and_type = this_frame->cp_reference[field_ref->Fieldref.name_and_type_index-1];

   /* Com a referencia do name_and_type, pegam-se o nome da classe e o nome do field */
   std::string class_name = cpAtAux.getUTF8(this_frame->cp_reference,field_ref->Fieldref.name_and_type_index-1);
   std::string field_name = cpAtAux.getUTF8(this_frame->cp_reference,name_and_type->NameAndType.name_index-1);

   this_frame->operand_stack.pop();
     
 }

 void InstructionImpl::aload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::aload_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::aload_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::aload_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::aload_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::void_return(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 /**
 * @brief Incrementa uma variavel local em uma constante
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iinc(Frame * this_frame){
    
    auto index         = this_frame->method_code.code[this_frame->pc++];
    auto constantValue = this_frame->method_code.code[this_frame->pc++];

    this_frame->local_variables.at(index) += constantValue;
    this_frame->pc++;
 }

 /**
 * @brief Passa um valor variável determinada pelo @param index para a pilha de operandos.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iload(Frame * this_frame){
    this_frame->pc++; //NÃO ESTOU CERTO SOBRE ISTO.
    auto index = this_frame->method_code.code[this_frame->pc++];
    auto op    = this_frame->local_variables.at((int)index);

    this_frame->operand_stack.push(op);   
 }

/**
 * @brief Passa um valor variável na posição 0 do vetor de variáveis para a pilha de operandos.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iload_0(Frame * this_frame){
   
    auto op = this_frame->local_variables.at(0);
    this_frame->operand_stack.push(op);
    this_frame->pc++;   
 }

 /**
 * @brief Passa um valor variável na posição 1 do vetor de variáveis para a pilha de operandos.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iload_1(Frame * this_frame){
    auto op = this_frame->local_variables.at(1);
    this_frame->operand_stack.push(op);
    this_frame->pc++;
     
 }

 /**
 * @brief Passa um valor variável na posição 2 do vetor de variáveis para a pilha de operandos.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iload_2(Frame * this_frame){
    auto op = this_frame->local_variables.at(2);
    this_frame->operand_stack.push(op);
    this_frame->pc++;
     
 }

 /**
 * @brief Passa um valor variável na posição 3 do vetor de variáveis para a pilha de operandos.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iload_3(Frame * this_frame){
    auto op = this_frame->local_variables.at(0);
    this_frame->operand_stack.push(op);
    this_frame->pc++;  
 }

 /**
 * @brief Empilha a constante long 0 na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lconst_0(Frame * this_frame){
     Operand *op   = (Operand*)malloc(sizeof(Operand));

     op->tag       = CONSTANT_Long;
     op->type_long = 0;

     this_frame->operand_stack.push(op);
     this_frame->pc++;
 }

 /**
 * @brief Empilha a constante long 1 na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lconst_1(Frame * this_frame){
     Operand *op   = (Operand*)malloc(sizeof(Operand));

     op->tag       = CONSTANT_Long;
     op->type_long = 1;

     this_frame->operand_stack.push(op);
     this_frame->pc++;
 }

 /**
 * @brief Empilha a constante float 0.0 na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fconst_0(Frame * this_frame){
     Operand *op   = (Operand*)malloc(sizeof(Operand));

     op->tag = CONSTANT_Float;
     op->type_float = 0.0;

     this_frame->operand_stack.push(op);
     this_frame->pc++;
 }

 /**
 * @brief Empilha a constante float 1.0 na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fconst_1(Frame * this_frame){
     Operand *op   = (Operand*)malloc(sizeof(Operand));

     op->tag = CONSTANT_Float;
     op->type_float = 1.0;

     this_frame->operand_stack.push(op);
     this_frame->pc++;
     
 }

  /**
 * @brief Empilha a constante float 2.0 na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fconst_2(Frame * this_frame){
     Operand *op   = (Operand*)malloc(sizeof(Operand));

     op->tag = CONSTANT_Float;
     op->type_float = 2.0;

     this_frame->operand_stack.push(op);
     this_frame->pc++;
     
 }

 /**
 * @brief Empilha a constante double 0.0 na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dconst_0(Frame * this_frame){
     Operand *op   = (Operand*)malloc(sizeof(Operand));

     op->tag = CONSTANT_Double;
     op->type_float = 0.0;

     this_frame->operand_stack.push(op);
     this_frame->pc++;   
 }

 /**
 * @brief Empilha a constante double 1.0 na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dconst_1(Frame * this_frame){
     Operand *op   = (Operand*)malloc(sizeof(Operand));

     op->tag = CONSTANT_Double;
     op->type_float = 1.0;

     this_frame->operand_stack.push(op);
     this_frame->pc++;
 }

/**
 * @brief Empilha na pilha de operandos um inteiro composto por um byte encontrado no em this_frame->method_code.code
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::bipush(Frame * this_frame){
    this_frame->pc++; // NÃO ESTOU CERTO DISTO.
    
    Operand * op = (Operand*)calloc(1,sizeof(Operand));
    auto byte    = this_frame->method_code.code[this_frame->pc++];

    op->tag      = CONSTANT_Integer;
    op->type_int = (int8_t)byte;

    this_frame->operand_stack.push(op);
 }

 /**
 * @brief Empilha int composto por byte de argumento na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::sipush(Frame * this_frame){

    Operand * op = (Operand*)calloc(1,sizeof(Operand));

    auto byte_1  = this_frame->method_code.code[this_frame->pc++];
    auto byte_2  = this_frame->method_code.code[this_frame->pc++];

    op->tag      = CONSTANT_Integer;
    op->type_int = (byte_1 << 8) | byte_2;

    this_frame->operand_stack.push(op);
 }

/**
 * @brief Empilha long indicado no indice 0 do array de variáveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lload_0(Frame * this_frame){
    
    auto op = this_frame->local_variables.at(0);
    this_frame->operand_stack.push(op);
    this_frame->pc++;
 }

/**
 * @brief Empilha long indicado no indice 1 do array de variáveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lload_1(Frame * this_frame){
    
    auto op = this_frame->local_variables.at(1);
    this_frame->operand_stack.push(op);
    this_frame->pc++;
 }

 /**
 * @brief Empilha long indicado no indice 2 do array de variáveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lload_2(Frame * this_frame){

    auto op = this_frame->local_variables.at(2);
    this_frame->operand_stack.push(op);
    this_frame->pc++;
 }

 /**
 * @brief Empilha long indicado no indice 3 do array de variáveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lload_3(Frame * this_frame){
    
    auto op = this_frame->local_variables.at(3);
    this_frame->operand_stack.push(op);
    this_frame->pc++;
 }

/**
 * @brief Empilha float indicado no indice 0 do array de variáveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fload_0(Frame * this_frame){

    auto op    = this_frame->local_variables.at(0);
    this_frame->operand_stack.push(op);
    this_frame->pc++;   
 }

/**
 * @brief Empilha float indicado no indice 1 do array de variáveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fload_1(Frame * this_frame){
    
    auto op    = this_frame->local_variables.at(1);
    this_frame->operand_stack.push(op);
    this_frame->pc++; 
 }

 /**
 * @brief Empilha float indicado no indice 2 do array de variáveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fload_2(Frame * this_frame){
    
    auto op    = this_frame->local_variables.at(2);
    this_frame->operand_stack.push(op);
    this_frame->pc++;
 }

/**
 * @brief Empilha float indicado no indice 3 do array de variáveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fload_3(Frame * this_frame){
    auto op    = this_frame->local_variables.at(3);
    this_frame->operand_stack.push(op);
    this_frame->pc++;
 }

/** @brief Carrega um long das variáveis locais
 * @param *this_frame ponteiro que aponta para o frame atual
 * @return void
 */ 
 void InstructionImpl::lload(Frame * this_frame){
   this_frame->pc++;
   uint8_t index = this_frame->method_code.code[this_frame->pc++];
   Operand * op = this_frame->local_variables.at(index);
   this_frame->operand_stack.push(op);
 }

/** @brief Dá push em um valor de preciso dupla de uma variável local para a
 *  pilha de operandos.
 * @param *this_frame ponteiro que aponta para o frame atual
 * @return void
 */
 void InstructionImpl::dload(Frame * this_frame){
    this_frame->pc++; // NÃO ESTOU CERTO DISTO.

    auto index = this_frame->method_code.code[this_frame->pc++];
    auto op    = this_frame->local_variables.at((int)index);

    this_frame->operand_stack.push(op);    
 }

 /**
 * @brief empilha double  indicado no indice 0 do array de variaveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dload_0(Frame * this_frame){

    auto op    = this_frame->local_variables.at(0);
    this_frame->operand_stack.push(op);
    this_frame->pc++;  
 }

 /**
 * @brief empilha double  indicado no indice 1 do array de variaveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dload_1(Frame * this_frame){

    auto op    = this_frame->local_variables.at(1);
    this_frame->operand_stack.push(op);
    this_frame->pc++;       
 }

 /**
 * @brief empilha double  indicado no indice 2 do array de variaveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dload_2(Frame * this_frame){

    auto op    = this_frame->local_variables.at(2);
    this_frame->operand_stack.push(op);
    this_frame->pc++;  
 }

 /**
 * @brief empilha double  indicado no indice 3 do array de variaveis locais na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dload_3(Frame * this_frame){

    auto op    = this_frame->local_variables.at(3);
    this_frame->operand_stack.push(op);
    this_frame->pc++;  
 }

 /**
 * @brief Armazena valor double da pilha de operandos no array de variaveis
 *  locais no indice index
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dstore(Frame * this_frame){
    this_frame->pc++; // NÃO ESTOU SEGURO DISTO.

    auto index = this_frame->method_code.code[this_frame->pc++];
    auto op    = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at((int)index) = op;   
 }

 /**
 * @brief Armazena valor double da pilha de operandos no array de variaveis locais no indice 0
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dstore_0(Frame * this_frame){
    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(0) = op;
    this_frame->pc++;   
 }

 void InstructionImpl::dadd(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dsub(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dmul(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ddiv(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 /**
 * @brief Armazena valor double da pilha de operandos no array de variaveis locais no indice 1
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dstore_1(Frame * this_frame){

    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(1) = op;
    this_frame->pc++;   
 }

 /**
 * @brief Armazena valor double da pilha de operandos no array de variaveis locais no indice 2
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dstore_2(Frame * this_frame){

    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(2) = op;
    this_frame->pc++;   
 }

/**
 * @brief Armazena valor double da pilha de operandos no array de variaveis locais no indice 3
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dstore_3(Frame * this_frame){

    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(3) = op;
    this_frame->pc++;   
     
 }

 /**
 * @brief Armazena float do topo da pilha de operandos no array de variaveis
 *  locais
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fstore(Frame * this_frame){

    this_frame->pc++; // NÃO ESTOU SEGURO DISTO.

    auto index = this_frame->method_code.code[this_frame->pc++];
    auto op    = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at((int)index) = op; 
 }

/**
 * @brief Armazena float do topo da pilha de operandos no array de variaveis locais no indice 0
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fstore_0(Frame * this_frame){
    
    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(0) = op;
    this_frame->pc++;   
 }

/**
 * @brief Armazena float do topo da pilha de operandos no array de variaveis locais no indice 1
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fstore_1(Frame * this_frame){

    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(1) = op;
    this_frame->pc++;   
     
 }

 /**
 * @brief Armazena float do topo da pilha de operandos no array de variaveis locais no indice 2
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fstore_2(Frame * this_frame){
    
    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(2) = op;
    this_frame->pc++;   
 }

 /**
 * @brief Armazena float do topo da pilha de operandos no array de variaveis locais no indice 3
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fstore_3(Frame * this_frame){

    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(3) = op;
    this_frame->pc++;   
 }


/**
 * @brief Armazena long do topo da pilha de operandos no array de variaveis
 *  locais
 * @param *curr_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lstore(Frame * this_frame){

    this_frame->pc++; // NÃO ESTOU SEGURO DISTO.

    auto index = this_frame->method_code.code[this_frame->pc++];
    auto op    = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at((int)index) = op;    
 }

 /**
* @brief Armazena long do topo da pilha de operandos no array de variaveis locais no indice 0
* @param Frame *curr_frame Ponteiro para o frame atual
* @return void
*/
 void InstructionImpl::lstore_0(Frame * this_frame){
    
    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(0) = op;
    this_frame->pc++;    
 }

 /**
* @brief Armazena long do topo da pilha de operandos no array de variaveis locais no indice 1
* @param Frame *curr_frame Ponteiro para o frame atual
* @return void
*/
 void InstructionImpl::lstore_1(Frame * this_frame){
    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(1) = op;
    this_frame->pc++;   
     
 }

 /**
* @brief Armazena long do topo da pilha de operandos no array de variaveis locais no indice 2
* @param Frame *curr_frame Ponteiro para o frame atual
* @return void
*/
 void InstructionImpl::lstore_2(Frame * this_frame){
    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(2) = op;
    this_frame->pc++;   
 }

 /**
* @brief Armazena long do topo da pilha de operandos no array de variaveis locais no indice 3
* @param Frame *curr_frame Ponteiro para o frame atual
* @return void
*/
 void InstructionImpl::lstore_3(Frame * this_frame){
    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(3) = op;
    this_frame->pc++;   
 }

 /**
 * @brief Empilha float indicado no indice determinado do array de variáveis locais na
 *  pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fload(Frame * this_frame){
    this_frame->pc++; // NÃO ESTOU CERTO DISTO.

    auto index = this_frame->method_code.code[this_frame->pc++];
    auto op    = this_frame->local_variables.at((int)index);

    this_frame->operand_stack.push(op);
 }
 
 void InstructionImpl::istore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::istore_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::istore_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);

    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    this_frame->local_variables.at(1) = op;
 }
 void InstructionImpl::istore_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::istore_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::f2d(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::f2i(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::l2d(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::l2f(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::l2i(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::i2f(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::i2s(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::d2f(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 /*
 * @brief Desempilha 2 inteiros da pilha de operandos e empilha a soma deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iadd(Frame * this_frame){
    InstructionImpl::nop(this_frame);

    Operand *operand_1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand *operand_2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand *result = (Operand *) malloc(sizeof(Operand));

    result->tag = CONSTANT_Integer;
    result->type_int = operand_1->type_int + operand_2->type_int;

    this_frame->operand_stack.push(result);
     
 }
 void InstructionImpl::iand(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::d2l(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::d2i(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ior(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dup_x1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dup_x2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dup2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dup2_x1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dup2_x2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::f2l(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 /**
 * @brief Soma do tipo long. Retira os dois operando do topo da pilha, soma-os e coloca o resultado
 * no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::ladd(Frame * this_frame){
    
    Operand * result = (Operand*)calloc(1,sizeof(result));
    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    result->tag = CONSTANT_Long;
    result->type_long = op2->type_long + op1->type_long;

    this_frame->operand_stack.push(result);
    this_frame->pc++;
 }

/**
 * @brief Subtração do tipo long. Retira os dois operando do topo da pilha, subtrai-os e coloca o resultado
 * no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lsub(Frame * this_frame){

    Operand * result = (Operand*)calloc(1,sizeof(result));
    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    result->tag = CONSTANT_Long;
    result->type_long = op2->type_long - op1->type_long;

    this_frame->operand_stack.push(result);
    this_frame->pc++;
 }

 /**
 * @brief Divisão do tipo long. Retira os dois operando do topo da pilha, divide-os e coloca o resultado
 * no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::ldiv(Frame * this_frame){

    Operand * result = (Operand*)calloc(1,sizeof(result));
    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    result->tag = CONSTANT_Long;
    result->type_long = op2->type_long / op1->type_long;

    this_frame->operand_stack.push(result);
    this_frame->pc++;
     
 }
 void InstructionImpl::laload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::iaload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::faload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::daload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::baload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::caload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::saload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::lastore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fastore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dastore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::aastore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::bastore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::castore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::sastore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::pop(Frame * this_frame){
    InstructionImpl::nop(this_frame);
    this_frame->operand_stack.pop(); 
 }

 void InstructionImpl::pop2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if(op->tag != CONSTANT_Double && op->tag != CONSTANT_Long) this_frame->operand_stack.pop(); 
 }

 void InstructionImpl::astore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::astore_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(0) = op; 
 }

 void InstructionImpl::if_icmpge(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::ins_goto(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::i2l(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::i2d(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::invokestatic(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::invokedynamic(Frame * this_frame) {
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lreturn(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::new_obj(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dup(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::putfield(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::putstatic(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::ldc_w(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ldc2_w(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

/*
 * @brief Desempilha dois inteiros e empilha a subtração dos mesmos.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::isub(Frame * this_frame){
    InstructionImpl::nop(this_frame);
    Operand *operand_1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand *operand_2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand *result = (Operand *) malloc(sizeof(Operand));

    result->tag = CONSTANT_Integer;
    result->type_int = operand_2->type_int - operand_1->type_int;

    this_frame->operand_stack.push(result);
     
 }
 void InstructionImpl::invokeinterface(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::areturn(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dreturn(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::freturn(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ireturn(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 /* 
 * @brief Desempilha dois floats da pilha e empilha adição deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fadd(Frame * this_frame){
    InstructionImpl::nop(this_frame);
    
    float value_1, value_2;
    std::cout << "INIT" << std::endl;
    

    Operand * op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand * op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    
    Operand * result = (Operand*) calloc(1,sizeof(result));
    
    std::memcpy(&value_1,&op1->type_float,sizeof(float));
    std::memcpy(&value_2,&op2->type_float,sizeof(float));
    value_1 += value_2;

    result->tag = CONSTANT_Float;
    std::memcpy(&result->type_float,&value_1,sizeof(float));
    this_frame->operand_stack.push(result);

 }
 void InstructionImpl::fsub(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fdiv(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fmul(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::frem(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::drem(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fneg(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lookupswitch(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::if_icmpne(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::if_icmpeq(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ifeq(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ifne(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iflt(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ifge(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ifgt(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ifle(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::if_icmplt(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::if_icmpgt(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::if_icmple(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::if_acmpeq(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::if_acmpne(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::newarray(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::anewarray(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::athrow(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::arraylength(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iastore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

/**
 * @brief Realiza uma permutação entre os 2 primeiros elementos da pilha.
 * @param *thisframe ponteiro para o frame.
 * @return void
 */
 void InstructionImpl::swap(Frame * this_frame){
    InstructionImpl::nop(this_frame);
    
    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->operand_stack.push(op1);
    this_frame->operand_stack.push(op2);
 }

 /*
 * @brief Desempilha 2 inteiros da pilha de operandos e empilha a multiplicação entre eles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::imul(Frame * this_frame){
    InstructionImpl::nop(this_frame);

    Operand *operand_1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand *operand_2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand *result = (Operand *) malloc(sizeof(Operand));

    result->tag = CONSTANT_Integer;
    result->type_int = operand_2->type_int * operand_1->type_int;

    this_frame->operand_stack.push(result);
     
 }

 /**
 * @brief multiplicação do tipo long. Retira os dois operando do topo da pilha, multiplica-os e coloca o resultado
 * no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lmul(Frame * this_frame){

    Operand * result = (Operand*)calloc(1,sizeof(result));
    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    result->tag = CONSTANT_Long;
    result->type_long = op2->type_long * op1->type_long;

    this_frame->operand_stack.push(result);
    this_frame->pc++;    
 }

 /*
 * @brief Desempilha dois inteiros da pilha de operandos e empilha o resultado da divisão entre eles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::idiv(Frame * this_frame){
    InstructionImpl::nop(this_frame);
    
    Operand *operand_1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand *operand_2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand *result = (Operand *) malloc(sizeof(Operand));

    result->tag = CONSTANT_Integer;
    result->type_int = operand_2->type_int / operand_1->type_int;

    this_frame->operand_stack.push(result);

 }

 /**
 * @brief Calcula o modulo entre dois inteiros.
 * @param *curr_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::irem(Frame * this_frame){

    Operand *operand_1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand *operand_2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand *result = (Operand *) malloc(sizeof(Operand));

    result->tag = CONSTANT_Integer;
    result->type_int = operand_2->type_int % operand_1->type_int;

    this_frame->operand_stack.push(result);
    this_frame->pc++;
     
 }

 void InstructionImpl::lrem(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ineg(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lneg(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dneg(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ishl(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lshl(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ishr(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lshr(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iushr(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lushr(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::land(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lor(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ixor(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lxor(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }


 void InstructionImpl::ifnull(Frame * this_frame){
    
    int shift;
    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if(!op->type_int) {
      shift = this_frame->method_code.code[this_frame->pc + 1];
      shift = (shift << 8) | this_frame->method_code.code[this_frame->pc + 2];
    }
    else shift = 3;
    
    this_frame->pc += shift;
 }

 void InstructionImpl::ifnonnull(Frame * this_frame){

    int shift;
    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if(op->type_int) {
      shift = this_frame->method_code.code[this_frame->pc + 1];
      shift = (shift << 8) | this_frame->method_code.code[this_frame->pc + 2];
    }
    else shift = 3;
    
    this_frame->pc += shift;
     
 }

 void InstructionImpl::ret(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::tableswitch(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::i2b(Frame * this_frame) {
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::i2c(Frame * this_frame) {
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::lcmp(Frame * this_frame) {
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::fcmpl(Frame * this_frame) {
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fcmpg(Frame * this_frame) {
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dcmpl(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dcmpg(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 /*Goto é um nome reservado pelo c */
 void InstructionImpl::i_goto(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::goto_w(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::instanceof(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::wide(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::multianewarray(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::monitorexit(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::monitorenter(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::jsr(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::jsr_w(Frame * this_frame){
     std::cout << "teste bem sucedido" << std::endl;
 }
 void InstructionImpl::checkcast(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

