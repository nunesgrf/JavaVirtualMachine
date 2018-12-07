#include "../hpp/Frame.hpp"
#include "../hpp/CpAttributeInterface.hpp"
#include "../hpp/GLOBAL_file.hpp"
#include "../hpp/Interpreter.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

namespace patch {
  template < typename T > std::string to_string( const T& n ) {
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
  }
}

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

 /*
 * @brief Invoca um metodo de instancia.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::invokespecial(Frame * this_frame){

   CpAttributeInterface cpAttrAux;
   Interpreter auxInterpreter;
   std::vector<Operand*> instance_arguments;
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
   std::string method_desc = cpAttrAux.getUTF8(this_frame->cp_reference,name_and_type->NameAndType.descriptor_index-1);

	if ( ((class_name == "java/lang/Object" || class_name == "java/lang/String") && method_name == "<init>") || (class_name == "java/lang/StringBuilder" && method_name == "<init>")) {
      /* Caso seja uma classe String ou StringBuilder, desempilhar (dispensar) o operando do topo da pilha de operandos */
		if (class_name == "java/lang/String" || class_name == "java/lang/StringBuilder") {
			this_frame->operand_stack.pop();
		}
      /* Caso o nome do metodo seja <init>, entao um novo class loader deve ser inicializado */
		else if (method_name == "<init>") {
			Operand * class_variable = this_frame->local_variables.at(0);
         auxInterpreter.loadVariables(class_variable->class_instance);
		}
		return;

   }
   /* Caso o nome da classe nao contenha a substring 'Java', ela nao foi implementado e a saida da instrucao é forçada */
   else if (class_name.find("java/") != std::string::npos) {
      std::cout << "Classe em questao nao implementada" << std::endl;
      getchar();
      exit(1);
   }
   else {

      /* Conta-se os argumentos presentes na inicializaçao da instancia */
      int count_args = 0;
      uint8_t counter = 1;

      /* Enquanto nao chegamos ao final do descritor do metodo, que é delimitado pelo ')', faz-se: */
      while (method_desc[counter] != ')') {
         /* Recupera o tipo da instancia criada */
         char created_type = method_desc[counter];
         /* Se o tipo em questao for um objeto */
         if (created_type == 'L') {
               count_args++;
               while (method_desc[++counter] != ';');
         }
         /* Se o tipo em questao tiver mais de uma dimensao */
         else if (created_type == '[') {
            count_args++;
            while (method_desc[++counter] == '[');
            if (method_desc[counter] == 'L') while (method_desc[++counter] != ';');
         }
         else count_args++;
         counter++;
      }

      for (int i = 0; i < count_args; ++i) {
         /* Para o numero de argumentos, desempilhamos um operando da pilha de operandos */
         Operand * instance_argument = this_frame->operand_stack.top();
         this_frame->operand_stack.pop();
         /* Inserimos no vetor de instancias */
         instance_arguments.insert(instance_arguments.begin(), instance_argument);
         /* Se a instancia for do tipo double ou do tipo long, criamos um operando do tipo empty space */
         if (instance_argument->tag == CONSTANT_Double || instance_argument->tag == CONSTANT_Long) {
            Operand * new_op = auxInterpreter.createType("P");
            instance_arguments.insert(instance_arguments.begin()+1,new_op);
         }
      }

      /* Desempilhamos o operando que refere-se a propria classe */
      Operand * current_class = this_frame->operand_stack.top();
      this_frame->operand_stack.pop();
      
      instance_arguments.insert(instance_arguments.begin(), current_class);
      Instance * reference_class = current_class->class_instance;

      MethodInfo * searched_method_info = auxInterpreter.findMethodByNameOrDescriptor(current_class->class_instance->classe, method_name, method_desc);
      Frame *new_frame = new Frame(current_class->class_instance->classe->getConstPool(),searched_method_info);
      
      for (int j = 0; (unsigned)j < instance_arguments.size(); ++j)
         new_frame->local_variables.at(j) = instance_arguments.at(j);

      auxInterpreter.frame_stack.push(new_frame);
   }
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
   std::string method_deor = cpAttrAux.getUTF8(this_frame->cp_reference,name_and_type->NameAndType.descriptor_index-1);

   /* Inicia-se o procedimento para verificar se o metodo a ser chamado é um print f */
   if((class_name == "java/io/PrintStream") && (method_name == "println" || method_name == "print")){

      if(method_deor != "()V"){
         Operand * op = this_frame->operand_stack.top();
         this_frame->operand_stack.pop();

         switch(op->tag) {
            case CONSTANT_String:
               std::cout << *(op->type_string);
               break;
            case CONSTANT_Integer:

               std::cout << (int32_t)op->type_int;
               break;
            case CONSTANT_Float:
               float converted_operand;
               memcpy(&converted_operand,&op->type_float,sizeof(float));
               
               std::cout << (float)converted_operand;
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
               std::cout << (double)converted_operand;
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
   else if(class_name == "java/lang/StringBuilder" && method_name == "append") {
      MethodsArea maux;
      Operand * op = this_frame->operand_stack.top();
      this_frame->operand_stack.pop();
      Operand * string_operand = this_frame->operand_stack.top();
      this_frame->operand_stack.pop();

      switch(op->tag){
         case CONSTANT_String:
            *string_operand->type_string += (*op->type_string);
            break;
         case CONSTANT_Integer:
            *string_operand->type_string += (
                                 patch::to_string(op->type_int));
            break;
         case CONSTANT_Long:
            *string_operand->type_string += (
                                 patch::to_string(op->type_long));
            break;
         case CONSTANT_Float:
            *string_operand->type_string += (
                              patch::to_string(op->type_float));
            break;
         case CONSTANT_Double:
            *string_operand->type_string += (
                              patch::to_string(op->type_double));
            break;
         case CONSTANT_Short:
            *string_operand->type_string += (
                                 patch::to_string(op->type_short));
            break;
         case CONSTANT_Char:
            *string_operand->type_string += (
                                 patch::to_string(op->type_char));
            break;
         case CONSTANT_Byte:
            *string_operand->type_string += (
                                 patch::to_string(op->type_byte));
            break;
         case CONSTANT_Boolean:
            if (op->type_bool == 0)
               *string_operand->type_string += "false";
            else
               *string_operand->type_string += "true";
            break;
         case CONSTANT_Class:
            // @TODO colocar enderec
            *string_operand->type_string += op->class_instance->name+"@";
            break;
         case CONSTANT_Array:
            *string_operand->type_string += "Array[]";
            break;
      }
            this_frame->operand_stack.push(string_operand);
   }
   else if(class_name == "java/lang/String" && method_name == "length") {
     auto strOp = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     Operand * strLen = (Operand*)calloc(1,sizeof(Operand));
     strLen->tag = CONSTANT_Integer;
     strLen->type_int = strOp->type_string->size();

     this_frame->operand_stack.push(strLen);
   }
   else {
   
      int argsCount  = 0;
      uint16_t count = 1;
      while(method_deor.at(count) != ')') {

         if(method_deor.at(count) == 'L') {
            while(method_deor.at(++count) != ';');
         }

         else if (method_deor.at(count) == '[') {
            while(method_deor.at(++count) != '[');

            if(method_deor[count] == 'L') while(method_deor.at(++count) != ';');
         }
         argsCount++;
         count++;
      }

      std::vector<Operand*> args;
      
      for(int i = 0; i <argsCount; ++i) { //verificar esta linha.
         
         auto arg = this_frame->operand_stack.top();
         this_frame->operand_stack.pop();

         args.insert(args.begin(),arg);
         if (arg->tag == CONSTANT_Double || arg->tag == CONSTANT_Long) args.insert(args.begin()+1, Interpreter::createType("P"));
      }
      auto this_class = this_frame->operand_stack.top();
      this_frame->operand_stack.pop();

      args.insert(args.begin(),this_class);
      auto instance = this_class->class_instance;

      MethodsArea auxMeth;
//fdasfds
      auto methods = auxMeth.findMethodByNameOrDeor(instance->classe,method_name,method_deor);
      auto newFrame = new Frame(instance->classe->getConstPool(),methods);

      for(unsigned i = 0; i < args.size(); ++i) {
        newFrame->local_variables.at(i) = args.at(i);
      }
      Interpreter auxInter;
      auxInter.frame_stack.push(newFrame);
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

 /**
  * @brief Carrega as classes estáticas na pilha para a execução.
  * @param *this_frame ponteiro para o frame atual.
  * @return void
  */
 void InstructionImpl::getstatic(Frame * this_frame){

    this_frame->pc++;

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
   if((int)index->type_int < 0 ||  index->type_int >= array->array_type->array->size()) std::cout << "ArrayIndexOutOfBoundsException" << std::endl;

   Operand * op = array->array_type->array->at(index->type_int);
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
    this_frame->pc++;
    this_frame->operand_stack.push(this_frame->local_variables.at(0));

 }
  /** @brief Uma referencia do objeto na posicao 1 do vetor de variaveis locais é colocada na pilha de operandos
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::aload_1(Frame * this_frame){
   this_frame->pc++;
   this_frame->operand_stack.push(this_frame->local_variables.at(1));
 }
 void InstructionImpl::aload_2(Frame * this_frame){
    this_frame->pc++;
    this_frame->operand_stack.push(this_frame->local_variables.at(2));

 }
 void InstructionImpl::aload_3(Frame * this_frame){
    this_frame->pc++;
    this_frame->operand_stack.push(this_frame->local_variables.at(3));

 }
 void InstructionImpl::void_return(Frame * this_frame){
    Interpreter auxInter;

    this_frame->pc++;
    auxInter.frame_stack.pop();
 }

 /**
 * @brief Incrementa uma variavel local em uma constante
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iinc(Frame * this_frame){
   this_frame->pc++;    

	int8_t  field = this_frame->method_code.code[this_frame->pc+1];
	int8_t value = this_frame->method_code.code[this_frame->pc+2];

	this_frame->local_variables.at((int)field)->type_int += value;
     
    for(int i = 0; i < 3; i++) this_frame->pc++;
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
    auto op = this_frame->local_variables.at(3);
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
     op->type_double = 0.0;

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
   Operand * op = this_frame->local_variables.at((int)index);
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

 /*
 * @brief Desempilha 2 doubles da pilha de operandos e empilha a soma deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dadd(Frame * this_frame){
    this_frame->pc++;

    Operand *operand_1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand *operand_2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    
    double value_1, value_2, value_3;
    memcpy(&value_1,&operand_1->type_double, sizeof(double));
    memcpy(&value_2,&operand_2->type_double, sizeof(double));
    value_3 = value_1 + value_2;
	Operand *result = (Operand *) malloc(sizeof(Operand));

    result->tag = CONSTANT_Double;
    memcpy(&result->type_double,&value_3, sizeof(uint64_t));

    this_frame->operand_stack.push(result);

 }
 /*
 * @brief Desempilha 2 doubles da pilha de operandos e empilha a subtração deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dsub(Frame * this_frame){
     this_frame->pc++;

     Operand *operand_1 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();
     Operand *operand_2 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     double value_1, value_2, value_3;
     memcpy(&value_1,&operand_1->type_double, sizeof(double));
     memcpy(&value_2,&operand_2->type_double, sizeof(double));
     value_3 = value_1 - value_2;
     Operand *result = (Operand *) malloc(sizeof(Operand));


     result->tag = CONSTANT_Double;
     memcpy(&result->type_double,&value_3, sizeof(uint64_t));


     this_frame->operand_stack.push(result);

 }
 /*
 * @brief Desempilha 2 doubles da pilha de operandos e empilha a produto deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dmul(Frame * this_frame){
     this_frame->pc++;

     Operand *operand_1 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();
     Operand *operand_2 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     double value_1, value_2, value_3;
     memcpy(&value_1,&operand_1->type_double, sizeof(double));
     memcpy(&value_2,&operand_2->type_double, sizeof(double));
     value_3 = value_1 * value_2;
     Operand *result = (Operand *) malloc(sizeof(Operand));

     result->tag = CONSTANT_Double;
     memcpy(&result->type_double,&value_3, sizeof(uint64_t));

     this_frame->operand_stack.push(result);

 }

 /*
 * @brief Desempilha 2 doubles da pilha de operandos e empilha a divisão deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::ddiv(Frame * this_frame){
     this_frame->pc++;

     Operand *operand_1 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();
     Operand *operand_2 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     double value_1, value_2, value_3;
     memcpy(&value_1,&operand_1->type_double, sizeof(double));
     memcpy(&value_2,&operand_2->type_double, sizeof(double));
     
     value_3 = (double((double)value_2/(double)value_1));
     Operand *result = (Operand *) malloc(sizeof(Operand));

     result->tag = CONSTANT_Double;  
     memcpy(&result->type_double,&value_3, sizeof(uint64_t));

     this_frame->operand_stack.push(result);
    
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
 * @param *this_frame Ponteiro para o frame atual
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
* @param Frame *this_frame Ponteiro para o frame atual
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
* @param Frame *this_frame Ponteiro para o frame atual
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
* @param Frame *this_frame Ponteiro para o frame atual
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
* @param Frame *this_frame Ponteiro para o frame atual
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

 /*
* @brief Armazena um inteiro no array de variaveis locais no valor indicado
*   pelo indice.
* @param *this_frame Ponteiro para o frame atual
* @return void
*/
 void InstructionImpl::istore(Frame * this_frame){

   this_frame->pc++;

   auto index = this_frame->method_code.code[this_frame->pc++];
   auto op    = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   this_frame->local_variables.at((int)index) = op;

 }

 /**
 * @brief Armazena um inteiro no array de variaveis locais no indice 0
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::istore_0(Frame * this_frame){
    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    this_frame->local_variables.at(0) = op;
    this_frame->pc++;
   
 }

 /**
 * @brief Armazena um inteiro no array de variaveis locais no indice 1
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::istore_1(Frame * this_frame){

    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(1) = op;
    this_frame->pc++;

 }

 /**
 * @brief Armazena um inteiro no array de variaveis locais no indice 2
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::istore_2(Frame * this_frame){
    Operand * op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    this_frame->local_variables.at(2) = op;
    this_frame->pc++;

 }

 /**
 * @brief Armazena um inteiro no array de variaveis locais no indice 3
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::istore_3(Frame * this_frame){
     Operand * op = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();
     this_frame->local_variables.at(3) = op;
     this_frame->pc++;

  }

 /**
 * @brief Desempilha 1 float da pilha de operandos e o converte para double
 * e empilha o resultado na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::f2d(Frame * this_frame){
    float float_value;
    double double_value;

    Operand * op_float = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    memcpy(&float_value, &(op_float->type_float), sizeof(float));

    double_value = (double) float_value;
    Operand * op_double = (Operand *) malloc(sizeof(Operand));
    op_double->tag = CONSTANT_Double;
    memcpy(&(op_double->type_double), &double_value, sizeof(uint64_t));
    this_frame->operand_stack.push(op_double);
    this_frame->pc++;

 }

 /**
 * @brief Desempilha 1 float da pilha de operandos e o converte para inteiro
 * e empilha o resultado na pilha de operandos
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::f2i(Frame * this_frame){
     float float_value;
     int int_value;

     Operand * op_float = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     memcpy(&float_value, &(op_float->type_float), sizeof(float));

     int_value = (int) float_value;
     Operand * op_int = (Operand *) malloc(sizeof(Operand));
     op_int->tag = CONSTANT_Integer;
     op_int->type_int = int_value;
     this_frame->operand_stack.push(op_int);
     this_frame->pc++;


 }

/**
*   @brief void l2f(Frame* this_frame)
*   @brief Função que desempilha um long, converte para um float e empilha novamente.
*   @param this_frame Ponteiro para frame.
*   @return
*/
void InstructionImpl::l2d(Frame * this_frame){
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   double converted_value = (double) op->type_long;
   memcpy(&op->type_double,&converted_value,sizeof(uint64_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;
}

/**
*   @brief void l2f(Frame* this_frame)
*   @brief Função que desempilha um long, converte para um float e empilha novamente.
*   @param this_frame Ponteiro para frame.
*   @return
*/
void InstructionImpl::l2f(Frame * this_frame){
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   float converted_value = (float) op->type_long;
   memcpy(&op->type_float,&converted_value,sizeof(uint32_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;
}

/**
*   @brief void l2i(Frame* this_frame)
*   @brief Função que desempilha um long, converte para inteiro e empilha novamente.
*   @param frame Ponteiro para frame.
*   @return
*/
void InstructionImpl::l2i(Frame * this_frame){
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   int converted_value = (long) op->type_long;
   memcpy(&op->type_int,&converted_value,sizeof(uint32_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;
}

/**
* @brief Converte de inteiro para float
* @param *this_frame ponteiro para o frame atual
* @return void
*/
void InstructionImpl::i2f(Frame * this_frame){
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   int converted_value = (int) op->type_int;
   memcpy(&op->type_short,&converted_value,sizeof(uint32_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;
}

/**
*   @brief void i2s(Frame* frame)
*   @brief Função que desempilha um inteiro, converte para short e empilha novamente.
*   @param frame Ponteiro para frame.
*   @return
*/
void InstructionImpl::i2s(Frame * this_frame){
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   short converted_value = (short) op->type_int;
   memcpy(&op->type_short,&converted_value,sizeof(uint32_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;

}

/**
* @brief Converte de double para float
* @param *this_frame ponteiro para o frame atual
* @return void
*/
void InstructionImpl::d2f(Frame * this_frame){
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   float converted_value = (float) op->type_double;
   memcpy(&op->type_float,&converted_value,sizeof(uint64_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;

}
 /*
 * @brief Desempilha 2 inteiros da pilha de operandos e empilha a soma deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
void InstructionImpl::iadd(Frame * this_frame){

   Operand *operand_1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand *operand_2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   Operand *result = (Operand *) malloc(sizeof(Operand));

   result->tag = CONSTANT_Integer;
   result->type_int = operand_1->type_int + operand_2->type_int;

   this_frame->operand_stack.push(result);
   this_frame->pc++;

 }

/**
*   @fn void iand(Frame* this_frame)
*   @brief Desempilha dois valores inteiros da pilha, realiza a operação lógica de AND entre os inteiros bit a bit.
*   @param this_frame Ponteiro para o frame corrente.
*   @return
*/
void InstructionImpl::iand(Frame * this_frame){

   Operand *op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand *op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   int result = op1->type_int & op2->type_int;
   memcpy(&op1->type_int,&result,sizeof(int));
   this_frame->operand_stack.push(op1);
   this_frame->pc++;
}

/**
* @brief Converte de double para long
* @param *this_frame ponteiro para o frame atual
* @return void
*/
void InstructionImpl::d2l(Frame * this_frame){
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   long converted_value = (long) op->type_double;
   memcpy(&op->type_long,&converted_value,sizeof(uint64_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;

}

/**
*   @brief void d2i(Frame* this_frame)
*   @brief Função que desempilha um double, converte para inteiro e empilha novamente.
*   @param this_frame Ponteiro para frame.
*   @return
*/
void InstructionImpl::d2i(Frame * this_frame){
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   int converted_value = (int) op->type_double;
   memcpy(&op->type_int,&converted_value,sizeof(uint32_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;
}

/**
* @brief Realiza a operacao de OR entre dois operandos e empilha o resultado
* @param *this_frame ponteiro para o frame atual
* @return void
*/
void InstructionImpl::ior(Frame * this_frame){
   auto op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   Operand * result = (Operand*)calloc(1,sizeof(Operand));

   result->type_int = op2->type_int | op1->type_int;

   this_frame->operand_stack.push(result);
   this_frame->pc++;

}
/**
 * @brief Realiza uma duplicação b, a -> a, b, a
 * @param *this_frame ponteiro para frame atual.
 * @return void
 */
void InstructionImpl::dup_x1(Frame * this_frame){

   MethodsArea methAux;

   this_frame->pc++;
   Operand *op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand *op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   if (op1->tag == CONSTANT_Double || op1->tag == CONSTANT_Long || op2->tag == CONSTANT_Double || op2->tag == CONSTANT_Long) {
       this_frame->operand_stack.push(op2);
       this_frame->operand_stack.push(op1);
   }
   else {
       Operand *tipo = methAux.copyOperand(op1);
       this_frame->operand_stack.push(tipo);
       this_frame->operand_stack.push(op2);
       this_frame->operand_stack.push(op1);
   }

}
void InstructionImpl::dup_x2(Frame * this_frame){

   MethodsArea methAux;

   this_frame->pc++;
   auto op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   if (op1->tag != CONSTANT_Double && op1->tag != CONSTANT_Long) {
       if (op2->tag == CONSTANT_Double || op2->tag == CONSTANT_Long) {
           auto copy = methAux.copyOperand(op1);
           this_frame->operand_stack.push(copy);
           this_frame->operand_stack.push(op2);
           this_frame->operand_stack.push(op1);

       }
       else {
           auto op3 = this_frame->operand_stack.top();
           this_frame->operand_stack.pop();
           if (op3->tag != CONSTANT_Double && op3->tag != CONSTANT_Long) {
               auto copy = methAux.copyOperand(op1);
               this_frame->operand_stack.push(copy);
               this_frame->operand_stack.push(op3);
               this_frame->operand_stack.push(op2);
               this_frame->operand_stack.push(op1);
           }
       }
   }

}

/**
* @brief Duplica um ou dois valores da pilha de operandos e insere os valores duplicados na ordem original
* @param *this_frame ponteiro para o frame atual
* @return void
*/
void InstructionImpl::dup2(Frame * this_frame){

   MethodsArea methAux;

   this_frame->pc++;
   auto op = this_frame->operand_stack.top();
   auto another = methAux.copyOperand(op);

   this_frame->operand_stack.pop();

   if(op->tag == CONSTANT_Double || op->tag == CONSTANT_Float) {

     this_frame->operand_stack.push(another);
     this_frame->operand_stack.push(op);
   }
   else {
     auto op2 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     auto another2 = methAux.copyOperand(op2);

     this_frame->operand_stack.push(another2);
     this_frame->operand_stack.push(another);
     this_frame->operand_stack.push(op2);
     this_frame->operand_stack.push(op);
   }

}

/**
*   @fn void dup2_x1(Frame* this_frame)
*   @brief Duplica os dois primeiro valores da pilha embaixo do penultimo valor da pilha
*   @param this_frame Ponteiro para frame corrente.
*   @return
*/
void InstructionImpl::dup2_x1(Frame * this_frame){
   Operand * op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * op3 = this_frame->operand_stack.top();
   this_frame->operand_stack.push(op2);
   this_frame->operand_stack.push(op1);
   this_frame->operand_stack.push(op3);
   this_frame->operand_stack.push(op2);
   this_frame->operand_stack.push(op1);
   this_frame->pc++;

}

/**
*   @fn void dup2_x2(Frame* this_frame)
*   @brief Duplica os dois primeiro valores da pilha embaixo do penultimo valor da pilha
*   @param this_frame Ponteiro para frame corrente.
*   @return
*/
void InstructionImpl::dup2_x2(Frame * this_frame){
   Operand * op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * op3 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * op4 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   this_frame->operand_stack.push(op2);
   this_frame->operand_stack.push(op1);
   this_frame->operand_stack.push(op4);
   this_frame->operand_stack.push(op3);
   this_frame->operand_stack.push(op2);
   this_frame->operand_stack.push(op1);
   this_frame->pc++;

}


 void InstructionImpl::f2l(Frame * this_frame){
     float float_value;
     long long_value;

     Operand * op_float = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     memcpy(&float_value, &(op_float->type_float), sizeof(float));

     long_value = (long) float_value;
     Operand * op_long = (Operand *) malloc(sizeof(Operand));
     op_long->tag = CONSTANT_Integer;
     op_long->type_int = long_value;
     this_frame->operand_stack.push(op_long);
     this_frame->pc++;

 }

 /**
 * @brief Soma do tipo long. Retira os dois operando do topo da pilha, soma-os e coloca o resultado
 * no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::ladd(Frame * this_frame){

    Operand * result = (Operand*)calloc(1,sizeof(Operand));
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

    Operand * result = (Operand*)calloc(1,sizeof(Operand));
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

    Operand * result = (Operand*)calloc(1,sizeof(Operand));
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

/**
 * @brief Empilha na pilha de operandos um elemento de um array de inteiros.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::iaload(Frame * this_frame){
   Operand * index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * array = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   Operand * op = array->array_type->array->at(index->type_int);

   this_frame->operand_stack.push(op);

   this_frame->pc++;
 }

/**
 * @brief Empilha na pilha de operandos um elemento de um array de float.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::faload(Frame * this_frame){
   Operand * index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * array = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   Operand * op = array->array_type->array->at(index->type_int);

   this_frame->operand_stack.push(op);

   this_frame->pc++;
}


/**
 * @brief Empilha na pilha de operandos um elemento de um array de float de dupla precisao.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
void InstructionImpl::daload(Frame * this_frame){
   Operand * index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * array = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   Operand * op = array->array_type->array->at(index->type_int);

   this_frame->operand_stack.push(op);

   this_frame->pc++;
}

/**
 * @brief Empilha na pilha de operandos um byte de um array de bytes.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
void InstructionImpl::baload(Frame * this_frame){
   Operand * index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * array = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   Operand * op = array->array_type->array->at(index->type_int);

   this_frame->operand_stack.push(op);

   this_frame->pc++;
}

/**
 * @brief Empilha na pilha de operandos um char de um array de chars.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::caload(Frame * this_frame){
   Operand * index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * array = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   Operand * op = array->array_type->array->at(index->type_int);

   this_frame->operand_stack.push(op);

   this_frame->pc++;

 }

 /**
 * @brief Empilha na pilha de operandos um char de um array de chars.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::saload(Frame * this_frame){
   Operand * index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * array = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   Operand * op = array->array_type->array->at(index->type_int);

   this_frame->operand_stack.push(op);

   this_frame->pc++;
 }

/**
*   @fn void lastore(Frame* this_frame)
*   @brief Desempilha a referencia para o array, o indice e o valor(long) e salva o valor no array.
*   @param this_frame Ponteiro para frame corrente.
*   @return
*/
 void InstructionImpl::lastore(Frame * this_frame){
    InstructionImpl::nop(this_frame);

 }

 /**
*   @fn void fastore(Frame* this_frame)
*   @brief Desempilha a referencia para o array, o indice e o valor(float) e salva o valor no array.
*   @param *this_frame Ponteiro para frame corrente.
*   @return
*/
 void InstructionImpl::fastore(Frame * this_frame){
   auto value = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto arrayRef = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   memcpy(&arrayRef->array_type->array->at(index->type_int), &value->type_float, sizeof(uint64_t));
 }


 void InstructionImpl::dastore(Frame * this_frame){
    InstructionImpl::nop(this_frame);

 }
/**
*   @fn void aastore(Frame* this_frame)
*   @brief Desempilha a referencia para o array, o indice e o valor(referencia) e salva o valor no array.
*   @param *this_frame Ponteiro para frame corrente.
*   @return
*/
void InstructionImpl::aastore(Frame * this_frame){
   auto value = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto arrayRef = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   memcpy(&arrayRef->array_type->array->at(index->type_int), &value, sizeof(uint64_t));
}

/**
*   @fn void bastore(Frame* this_frame)
*   @brief Desempilha a referencia para o array, o indice e o valor(byte) e salva o valor no array.
*   @param *this_frame Ponteiro para frame corrente.
*   @return
*/
void InstructionImpl::bastore(Frame * this_frame){
   auto value = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   uint8_t converted_value = (uint8_t) value->type_int;
   auto index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto arrayRef = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   memcpy(&arrayRef->array_type->array->at(index->type_int), &converted_value, sizeof(uint64_t));

}

/**
*   @fn void castore(Frame* this_frame)
*    @brief Desempilha a referencia para o array, o indice e o valor(char) e salva o valor no array. Trunca inteiro
*   de 32 bits da pilha para 16 bits
*   @param *this_frame Ponteiro para frame corrente.
*   @return
*/
void InstructionImpl::castore(Frame * this_frame){
   auto value = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   uint16_t converted_value = (uint16_t) value->type_int;
   auto index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto arrayRef = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   memcpy(&arrayRef->array_type->array->at(index->type_int), &converted_value, sizeof(uint64_t));

}

/**
*   @fn void sastore(Frame* this_frame)
*   @brief Desempilha a referencia para o array, o indice e o valor(short) e salva o valor no array. Trunca int do
*   stack para short de 16 bits.
*   @param *this_frame Ponteiro para frame corrente.
*   @return
*/
void InstructionImpl::sastore(Frame * this_frame){
   auto value = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   short converted_value = (short) value->type_int;
   auto index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto arrayRef = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   memcpy(&arrayRef->array_type->array->at(index->type_int), &converted_value, sizeof(uint64_t));

}

void InstructionImpl::pop(Frame * this_frame){
   this_frame->operand_stack.pop();
}

void InstructionImpl::pop2(Frame * this_frame){
   auto op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   if(op->tag != CONSTANT_Double && op->tag != CONSTANT_Long) this_frame->operand_stack.pop();
}

/*
* @brief Armazena uma referencia de objeto no array de variaveis locais no valor indicado
*   pelo indice.
* @param *this_frame Ponteiro para o frame atual
* @return void
*/
void InstructionImpl::astore(Frame * this_frame){
   this_frame->pc++;
   auto index = this_frame->method_code.code[this_frame->pc++];
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   this_frame->local_variables.at((int)index) = op;

 }

/**
 * @brief Guarda um objeto em uma na posição 0 do vetor de variáveis.
 * @param *this_frame ponteiro para o frame atual.
 * @void
 */
 void InstructionImpl::astore_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    this_frame->local_variables.at(0) = op;
 }

/**
 * @brief Realiza um salto condicional caso o segundo operando na pilha seja >= que o topo.
 * @param Frame *this_frame ponteiro que aponta para o frame atual
 * @return void
 */
 void InstructionImpl::if_icmpge(Frame * this_frame){

    int16_t offset;

    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if(op2->type_int >= op1->type_int) {
      offset = this_frame->method_code.code[this_frame->pc+1];
      offset = (offset << 8) | this_frame->method_code.code[this_frame->pc+2];
    }
    else offset = 3;
    this_frame->pc += offset;

 }

 /**
 * @brief Realiza um falto baseado em um offset.
 * @param *this_frame ponteiro para o frame atual.
 * @void
 */
 void InstructionImpl::ins_goto(Frame * this_frame){
    uint16_t offset = this_frame->method_code.code[this_frame->pc+1];
    offset = (offset << 8) + this_frame->method_code.code[this_frame->pc+2];
    this_frame->pc += offset;
 }

/**
*   @brief void i2l(Frame* this_frame)
*   @brief Função convert operando inteiro long e empilha novamente.
*   @param *this_frame ponteiro para frame atual.
*   @return
*/
 void InstructionImpl::i2l(Frame * this_frame){
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   uint64_t converted_value = (uint64_t) op->type_int;
   memcpy(&op->type_long,&converted_value,sizeof(uint64_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;
 }

/**
*   @brief void i2d(Frame* this_frame)
*   @brief Função que desempilha um inteiro, converte para double e empilha novamente.
*   @param this_frame Ponteiro para frame.
*   @return
*/
 void InstructionImpl::i2d(Frame * this_frame){
   Operand * op = this_frame->operand_stack.top();
   op->tag = CONSTANT_Double;

   this_frame->operand_stack.pop();
   double converted_value = (double) op->type_int;
   memcpy(&converted_value,&op->type_double,sizeof(uint64_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;


 }

 /*
 * @brief Invoca um método de classe.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::invokestatic(Frame * this_frame){
   CpAttributeInterface cpAttrAux;
   Interpreter auxInterpreter;
   std::vector<Operand*> instance_arguments;
   MethodsArea methodAreaAux;
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
   std::string method_desc = cpAttrAux.getUTF8(this_frame->cp_reference,name_and_type->NameAndType.name_index-1);

   /* Caso o metodo seja nativo, printa um erro */
   if (class_name == "java/lang/Object" && method_name == "registerNatives") {
      std::cout << "Metodo nativos nao suportados por esse projeto" << std::endl;
      return;
   }

   /* Calcula o numero de argumentos do metodo */
   int count_args = 0;
   uint8_t counter = 1;
   while (method_desc[counter] != ')') {
      char find_type = method_desc[counter];
      if (find_type == 'L') { // tipo é um objeto
         count_args++;
         while (method_desc[++counter] != ';');
      } else if (find_type == '[') { // tipo é um array
         count_args++;
         while (method_desc[++counter] == '[');
         if (method_desc[counter] == 'L')
               while(method_desc[++counter] != ';');
      } else count_args++;
      counter++;
   }

   if (class_name.find("Float") != std::string::npos && method_name.find("valueOf") != std::string::npos) {
      return;
   }
   else {
      for (int i = 0; i < count_args; ++i) {
         /* Para o numero de argumentos, desempilhamos um operando da pilha de operandos */
         Operand * instance_argument = this_frame->operand_stack.top();
         this_frame->operand_stack.pop();
         /* Inserimos no vetor de instancias */
         instance_arguments.insert(instance_arguments.begin(), instance_argument);
         /* Se a instancia for do tipo double ou do tipo long, criamos um operando do tipo empty space */
         if (instance_argument->tag == CONSTANT_Double || instance_argument->tag == CONSTANT_Long) {
            Operand * new_op = auxInterpreter.createType("P");
            instance_arguments.insert(instance_arguments.begin()+1,new_op);
         }
      }
   }

   Instance * reference_class = methodAreaAux.GLOBAL_staticClasses.at(class_name);

   MethodInfo * searched_method_info = auxInterpreter.findMethodByNameOrDescriptor(reference_class->classe, method_name, method_desc);
   Frame *new_frame = new Frame(reference_class->classe->getConstPool(),searched_method_info);

   for (int j = 0; (unsigned)j < instance_arguments.size(); ++j)
      new_frame->local_variables.at(j) = instance_arguments.at(j);

   auxInterpreter.frame_stack.push(new_frame);
 }

 void InstructionImpl::invokedynamic(Frame * this_frame) {
    InstructionImpl::nop(this_frame);

 }
  /**
 * @brief Retorna long de um método.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lreturn(Frame * this_frame){
    Interpreter AuxInter;

    this_frame->pc++;
    auto longVal = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    AuxInter.frame_stack.pop();
    AuxInter.frame_stack.top()->operand_stack.push(longVal);

 }
 void InstructionImpl::new_obj(Frame * this_frame){

   CpAttributeInterface cpAttrAux;
   Interpreter auxInterpreter;
   this_frame->pc++;
   uint8_t index1 = this_frame->method_code.code[this_frame->pc++];
   uint8_t index2 = this_frame->method_code.code[this_frame->pc++];
   uint16_t index = (index1 << 8) | index2;

   CpInfo * Cp = this_frame->cp_reference[index-1];
   std::string class_name = cpAttrAux.getUTF8(this_frame->cp_reference,Cp->Class.name_index-1);
   Operand * op = (Operand*)calloc(1,sizeof(Operand));
   if(class_name == "java/lang/StringBuilder") {
      op->tag = CONSTANT_String;
      op->type_string = new std::string("");
   }
   else {
      op = auxInterpreter.createType("L" + class_name);
   }
   this_frame->operand_stack.push(op);
 }

 /**
 * @brief Duplica um valor da pilha de operandos e insere os valores duplicados na ordem original
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dup(Frame * this_frame){

    MethodsArea methAux;

    this_frame->pc++;
    auto toCopy = this_frame->operand_stack.top();
    //auto copy = methAux.copyOperand(toCopy);
    auto copy = toCopy;
    this_frame->operand_stack.push(copy);
 }



/*
 * @brief Seta o campo em um objeto.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
void InstructionImpl::putfield(Frame * this_frame){
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

   Operand *var_operand = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand *class_instance = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   Operand *class_variable = class_instance->class_instance->references->at(field_name);

   switch (var_operand->tag) {
      case CONSTANT_Integer:
         class_variable->type_int = var_operand->type_int;
         break;
      case CONSTANT_Long:
         class_variable->type_long = var_operand->type_long;
         break;
      case CONSTANT_Boolean:
         class_variable->type_bool = var_operand->type_bool;
         break;
      case CONSTANT_Char:
         class_variable->type_char = var_operand->type_char;
         break;
      case CONSTANT_Short:
         class_variable->type_short = var_operand->type_short;
         break;
      case CONSTANT_Byte:
         class_variable->type_byte = var_operand->type_byte;
         break;
      case CONSTANT_Float:
         class_variable->type_float = var_operand->type_float;
         break;
      case CONSTANT_Double:
         class_variable->type_double = var_operand->type_double;
         break;
      case CONSTANT_String:
         class_variable->type_string = var_operand->type_string;
         break;
      case CONSTANT_Class:
         class_variable->class_instance = var_operand->class_instance;
         break;
      case CONSTANT_Array:
         class_variable->array_type = var_operand->array_type;
         break;
    }

}

 void InstructionImpl::putstatic(Frame * this_frame){
    InstructionImpl::nop(this_frame);

 }

 void InstructionImpl::ldc_w(Frame * this_frame){
    InstructionImpl::nop(this_frame);

 }
 void InstructionImpl::ldc2_w(Frame * this_frame){
    this_frame->pc++;

    Operand * op = (Operand*)calloc(1,sizeof(Operand));
    uint8_t index1 = this_frame->method_code.code[this_frame->pc++];
    uint8_t index2 = this_frame->method_code.code[this_frame->pc++];
    uint16_t index = (index1 << 8) + index2;

    CpInfo * Cp = this_frame->cp_reference[index-1];
    op->tag = Cp->tag;

    if (op->tag == CONSTANT_Double) {
        op->type_double = ((uint64_t)(Cp->Double.high_bytes) << 32);
        op->type_double = op->type_double + Cp->Double.low_bytes;
    }
    else {
        op->type_long = ((uint64_t)(Cp->Long.high_bytes) << 32);
        op->type_long = op->type_long + Cp->Long.low_bytes;

    }
    this_frame->operand_stack.push(op);
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

  /*
 * @brief Invoca um metodo declarado com a interface do java.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::invokeinterface(Frame * this_frame){
   CpAttributeInterface cpAttrAux;
   Interpreter auxInterpreter;
   std::vector<Operand*> instance_arguments;
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
   std::string method_desc = cpAttrAux.getUTF8(this_frame->cp_reference,name_and_type->NameAndType.name_index-1);

   if (class_name.find("java/") != std::string::npos) {
      std::cout << "Metodo de Classe em questao nao implementada" << std::endl;
      getchar();
      exit(1);
   }

   /* Calcula o numero de argumentos do metodo */
   int count_args = 0;
   uint8_t counter = 1;
   while (method_desc[counter] != ')') {
      char find_type = method_desc[counter];
      if (find_type == 'L') { // tipo é um objeto
         count_args++;
         while (method_desc[++counter] != ';');
      } else if (find_type == '[') { // tipo é um array
         count_args++;
         while (method_desc[++counter] == '[');
         if (method_desc[counter] == 'L')
               while(method_desc[++counter] != ';');
      } else count_args++;
      counter++;
   }

   for (int i = 0; i < count_args; ++i) {
      /* Para o numero de argumentos, desempilhamos um operando da pilha de operandos */
      Operand * instance_argument = this_frame->operand_stack.top();
      this_frame->operand_stack.pop();
      /* Inserimos no vetor de instancias */
      instance_arguments.insert(instance_arguments.begin(), instance_argument);
      /* Se a instancia for do tipo double ou do tipo long, criamos um operando do tipo empty space */
      if (instance_argument->tag == CONSTANT_Double || instance_argument->tag == CONSTANT_Long) {
         Operand * new_op = auxInterpreter.createType("P");
         instance_arguments.insert(instance_arguments.begin()+1,new_op);
      }
   }


 }

 /**
 * @brief retorna um objeto para o frame anterior.
 * @param *this_frame ponteiro para o frame atual.
 * @return void
 */
 void InstructionImpl::areturn(Frame * this_frame){
    Interpreter AuxInter;

    this_frame->pc++;
    auto object = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    AuxInter.frame_stack.pop();
    AuxInter.frame_stack.top()->operand_stack.push(object);

 }

 /**
 * @brief Retorna double de um método.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::dreturn(Frame * this_frame){
    Interpreter AuxInter;

    this_frame->pc++;
    auto dValue = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    AuxInter.frame_stack.pop();
    AuxInter.frame_stack.top()->operand_stack.push(dValue);
 }

 /**
 * @brief Retorna float de um método.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::freturn(Frame * this_frame){
    Interpreter AuxInter;

    this_frame->pc++;
    auto pontoFloat = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    AuxInter.frame_stack.pop();
    AuxInter.frame_stack.top()->operand_stack.push(pontoFloat);

 }
 /**
 * @brief Retorna int de um método.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::ireturn(Frame * this_frame){

    Interpreter AuxInter;

    this_frame->pc++;
    auto integer = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    AuxInter.frame_stack.pop();
    AuxInter.frame_stack.top()->operand_stack.push(integer);
 }

 /*
 * @brief Desempilha dois float da pilha e empilha adição deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fadd(Frame * this_frame){
     this_frame->pc++;

    float value_1, value_2;


    Operand * op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand * op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand * result = (Operand*) calloc(1,sizeof(Operand));

    std::memcpy(&value_1,&op1->type_float,sizeof(float));
    std::memcpy(&value_2,&op2->type_float,sizeof(float));
    value_1 += value_2;

    result->tag = CONSTANT_Float;
    std::memcpy(&result->type_float,&value_1,sizeof(float));
    this_frame->operand_stack.push(result);

 }
 /*
 * @brief Desempilha dois floats da pilha e empilha subtração deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fsub(Frame * this_frame){
     this_frame->pc++;

    float value_1, value_2;


    Operand * op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand * op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand * result = (Operand*) calloc(1,sizeof(Operand));

    std::memcpy(&value_1,&op1->type_float,sizeof(float));
    std::memcpy(&value_2,&op2->type_float,sizeof(float));
    value_2 -= value_1;

    result->tag = CONSTANT_Float;
    std::memcpy(&result->type_float,&value_2,sizeof(float));
    this_frame->operand_stack.push(result);

 }
 /*
 * @brief Desempilha dois floats da pilha e empilha a divisão deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fdiv(Frame * this_frame){
     this_frame->pc++;

    float value_1, value_2;


    Operand * op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand * op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand * result = (Operand*) calloc(1,sizeof(Operand));

    std::memcpy(&value_1,&op1->type_float,sizeof(float));
    std::memcpy(&value_2,&op2->type_float,sizeof(float));
    value_2 /= value_1;

    result->tag = CONSTANT_Float;
    std::memcpy(&result->type_float,&value_2,sizeof(float));
    this_frame->operand_stack.push(result);

 }
 /*
 * @brief Desempilha dois floats da pilha e empilha o produto deles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fmul(Frame * this_frame){
     this_frame->pc++;

    float value_1, value_2;


    Operand * op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand * op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand * result = (Operand*) calloc(1,sizeof(Operand));

    std::memcpy(&value_1,&op1->type_float,sizeof(float));
    std::memcpy(&value_2,&op2->type_float,sizeof(float));
    value_1 *= value_2;

    result->tag = CONSTANT_Float;
    std::memcpy(&result->type_float,&value_1,sizeof(float));
    this_frame->operand_stack.push(result);

 }
 /*
 * @brief Desempilha dois floats da pilha e empilha resto da divisão entre eles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::frem(Frame * this_frame){
     this_frame->pc++;

    float value_1, value_2;

    Operand * op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand * op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand * result = (Operand*) calloc(1,sizeof(Operand));

    std::memcpy(&value_1,&op1->type_float,sizeof(float));
    std::memcpy(&value_2,&op2->type_float,sizeof(float));
    value_1 = fmod(value_2, value_1);

    result->tag = CONSTANT_Float;
    std::memcpy(&result->type_float,&value_1,sizeof(float));
    this_frame->operand_stack.push(result);

 }
 /*
 * @brief Desempilha dois doubles da pilha e empilha resto da divisão entre eles.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::drem(Frame * this_frame){
     this_frame->pc++;
     
     double value_1, value_2, value_3;
     Operand *operand_1 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();
     Operand *operand_2 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     
     memcpy(&value_1,&operand_1->type_double, sizeof(double));
     memcpy(&value_2,&operand_2->type_double, sizeof(double));
     value_3 = fmod(value_2, value_1);
     Operand *result = (Operand *) malloc(sizeof(Operand));

     result->tag = CONSTANT_Double;
     memcpy(&result->type_double,&value_3, sizeof(uint64_t));

     this_frame->operand_stack.push(result);

 }
 /*
 * @brief Desempilha 1 doubles da pilha de operandos e empilha o valor negativo dele.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::fneg(Frame * this_frame){
     this_frame->pc++;

     Operand *operand_1 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     float value_1, value_2;
     memcpy(&value_1,&operand_1->type_float, sizeof(float));
     value_2 = -value_1;
     Operand *result = (Operand *) malloc(sizeof(Operand));

     result->tag = CONSTANT_Double;
     memcpy(&result->type_float,&value_2, sizeof(float));

     this_frame->operand_stack.push(result);

 }
 void InstructionImpl::lookupswitch(Frame * this_frame){
    InstructionImpl::nop(this_frame);

 }

/**
 * @brief Realiza um salto condicional caso o segundo operando na pilha seja != que o topo.
 * @param Frame *this_frame ponteiro que aponta para o frame atual
 * @return void
 */
 void InstructionImpl::if_icmpne(Frame * this_frame){
    int16_t offset;

    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if(op2->type_int != op1->type_int) {
      offset = this_frame->method_code.code[this_frame->pc+1];
      offset = (offset << 8) | this_frame->method_code.code[this_frame->pc+2];
    }
    else offset = 3;
    this_frame->pc += offset;

 }

 /**
 * @brief Realiza um salto condicional caso o segundo operando na pilha seja == que o topo.
 * @param Frame *this_frame ponteiro que aponta para o frame atual
 * @return void
 */
 void InstructionImpl::if_icmpeq(Frame * this_frame){
    int16_t offset;

    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if(op2->type_int == op1->type_int) {
      offset = this_frame->method_code.code[this_frame->pc+1];
      offset = (offset << 8) | this_frame->method_code.code[this_frame->pc+2];
    }
    else offset = 3;
    this_frame->pc += offset;

 }

 /**
  * @brief Instrução que realiza um salto condicional baseado se o topo da pilha tem valor ==0
  * @param *this_frame ponteiro para o frame atual.
  * @return void
  */
 void InstructionImpl::ifeq(Frame * this_frame){
    uint32_t offset;

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if((int)op->type_int == 0) {
      offset = this_frame->method_code.code[this_frame->pc + 1];
      offset = (offset << 8 ) | this_frame->method_code.code[this_frame->pc + 2];
    }
    else offset = 3;
    this_frame->pc+=offset;
 }

/**
  * @brief Instrução que realiza um salto condicional baseado se o topo da pilha tem valor !=0
  * @param *this_frame ponteiro para o frame atual.
  * @return void
  */
 void InstructionImpl::ifne(Frame * this_frame){
    uint32_t offset;

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if((int)op->type_int != 0) {
      offset = this_frame->method_code.code[this_frame->pc + 1];
      offset = (offset << 8 ) | this_frame->method_code.code[this_frame->pc + 2];
    }
    else offset = 3;
    this_frame->pc+=offset;

 }

 /**
  * @brief Instrução que realiza um salto condicional baseado se o topo da pilha tem valor <0
  * @param *this_frame ponteiro para o frame atual.
  * @return void
  */
 void InstructionImpl::iflt(Frame * this_frame){
    uint32_t offset;

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if((int)op->type_int < 0) {
      offset = this_frame->method_code.code[this_frame->pc + 1];
      offset = (offset << 8 ) | this_frame->method_code.code[this_frame->pc + 2];
    }
    else offset = 3;
    this_frame->pc+=offset;

 }

/**
  * @brief Instrução que realiza um salto condicional baseado se o topo da pilha tem valor >=0
  * @param *this_frame ponteiro para o frame atual.
  * @return void
  */
 void InstructionImpl::ifge(Frame * this_frame){
    uint32_t offset;

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if((int)op->type_int >= 0) {
      offset = this_frame->method_code.code[this_frame->pc + 1];
      offset = (offset << 8 ) | this_frame->method_code.code[this_frame->pc + 2];
    }
    else offset = 3;
    this_frame->pc+=offset;
 }

/**
  * @brief Instrução que realiza um salto condicional baseado se o topo da pilha tem valor >0
  * @param *this_frame ponteiro para o frame atual.
  * @return void
  */
 void InstructionImpl::ifgt(Frame * this_frame){
    uint32_t offset;

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if((int)op->type_int > 0) {
      offset = this_frame->method_code.code[this_frame->pc + 1];
      offset = (offset << 8 ) | this_frame->method_code.code[this_frame->pc + 2];
    }
    else offset = 3;
    this_frame->pc+=offset;
 }

/**
  * @brief Instrução que realiza um salto condicional baseado se o topo da pilha tem valor <=0
  * @param *this_frame ponteiro para o frame atual.
  * @return void
  */
 void InstructionImpl::ifle(Frame * this_frame){
    uint32_t offset;

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    int value = (int) op->type_int;

   if(value <= 0) {
      offset = this_frame->method_code.code[this_frame->pc + 1];
      offset = (offset << 8 ) | this_frame->method_code.code[this_frame->pc + 2];
    }
    else offset = 3;

    this_frame->pc+=offset;

 }

 /**
 * @brief Realiza um salto condicional caso o segundo operando na pilha seja < que o topo.
 * @param Frame *this_frame ponteiro que aponta para o frame atual
 * @return void
 */
 void InstructionImpl::if_icmplt(Frame * this_frame){
    int16_t offset;

    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if(op2->type_int < op1->type_int) {
      offset = this_frame->method_code.code[this_frame->pc+1];
      offset = (offset << 8) | this_frame->method_code.code[this_frame->pc+2];
    }
    else offset = 3;
    this_frame->pc += offset;

 }

 /**
 * @brief Realiza um salto condicional caso o segundo operando na pilha seja > que o topo.
 * @param Frame *this_frame ponteiro que aponta para o frame atual
 * @return void
 */
 void InstructionImpl::if_icmpgt(Frame * this_frame){
    int16_t offset;

    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if(int(op2->type_int) > int(op1->type_int)) {
      offset = this_frame->method_code.code[this_frame->pc+1];
      offset = (offset << 8) | this_frame->method_code.code[this_frame->pc+2];
    }
    else offset = 3;

    this_frame->pc += offset;

 }

 /**
 * @brief Realiza um salto condicional caso o segundo operando na pilha seja <= que o topo.
 * @param Frame *this_frame ponteiro que aponta para o frame atual
 * @return void
 */
 void InstructionImpl::if_icmple(Frame * this_frame){
    int16_t offset;

    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    if((int)(op2->type_int) <= (int)(op1->type_int)) {
      offset = this_frame->method_code.code[this_frame->pc+1];
      offset = (offset << 8) | this_frame->method_code.code[this_frame->pc+2];
    }
    else offset = 3;
    this_frame->pc += offset;

 }

 /**
  * @brief Compara dois objetos e realiza um salto condicional caso obj_1 == obj_2.
  * @param *this_frame ponteiro para o frame atual.
  * @return void.
  */
 void InstructionImpl::if_acmpeq(Frame * this_frame){

   uint16_t offset;

   auto op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   if(op1->class_instance == op2->class_instance) {
     offset = this_frame->method_code.code[this_frame->pc +1];
     offset = (offset << 8) | this_frame->method_code.code[this_frame->pc +2];
   }
   else offset = 3;
   this_frame->pc += offset;
 }

 /**
  * @brief Compara dois objetos e realiza um salto condicional caso obj_1 != obj_2.
  * @param *this_frame ponteiro para o frame atual.
  * @return void.
  */
 void InstructionImpl::if_acmpne(Frame * this_frame){
    uint16_t offset;

   auto op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   if(op1->class_instance != op2->class_instance) {
     offset = this_frame->method_code.code[this_frame->pc +1];
     offset = (offset << 8) | this_frame->method_code.code[this_frame->pc +2];
   }
   else offset = 3;
   this_frame->pc += offset;

 }

 void InstructionImpl::newarray(Frame * this_frame){
     Operand * count = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     uint32_t index = count->type_int;

     Interpreter AuxInter;
     auto result = AuxInter.createType("[");
     uint8_t type =  this_frame->method_code.code[this_frame->pc++];

     switch (type) {
         case T_BOOLEAN:
            for (int i = 0; i < (int)index; i++) {
            result->array_type->array->emplace_back(AuxInter.createType("Z"));
            }
            break;
         case T_CHAR:
            for (int i = 0; i < (int)index; i++) {
            result->array_type->array->emplace_back(AuxInter.createType("C"));
            }
            break;
         case T_FLOAT:
            for (int i = 0; i < (int)index; i++) {
            result->array_type->array->emplace_back(AuxInter.createType("F"));
            }
            break;
         case T_DOUBLE:
            for (int i = 0; i < (int)index; i++) {
            result->array_type->array->emplace_back(AuxInter.createType("D"));
            }
            break;
         case T_BYTE:
            for (int i = 0; i < (int)index; i++) {
            result->array_type->array->emplace_back(AuxInter.createType("B"));
            }
            break;
         case T_SHORT:
            for (int i = 0; i < (int)index; i++) {
            result->array_type->array->emplace_back(AuxInter.createType("S"));
            }
            break;
         case T_INT:
            for (int i = 0; i < (int)index; i++) {
            result->array_type->array->emplace_back(AuxInter.createType("I"));
            }
            break;
         case T_LONG:         
            for (int i = 0; i < (int)index; i++) {
            result->array_type->array->emplace_back(AuxInter.createType("J"));
            }
            break;
     }
     this_frame->operand_stack.push(result);
      std::cout << (int)result->tag;
      getchar();
 }
 void InstructionImpl::anewarray(Frame * this_frame){
    InstructionImpl::nop(this_frame);

 }
 void InstructionImpl::athrow(Frame * this_frame){
    InstructionImpl::nop(this_frame);

 }

/**
*   @fn void arraylength(Frame *this_frame)
*   @brief pega a referencia de um array, remove array, e empilha o seu tamanho no lugar
*	 @param this_frame Ponteiro para frame corrente
*   @return
*/
void InstructionImpl::arraylength(Frame * this_frame){

   Interpreter AuxInter;

   auto arrayRef = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   auto size      = AuxInter.createType("I");

   if(arrayRef->tag != CONSTANT_Array) size->type_int = 0;
   else size->type_int = arrayRef->array_type->array->size();
   
   this_frame->operand_stack.push(size);
   this_frame->pc++;

   std::cout << (int)size->type_int << std::endl;
   getchar();
}

/**
*   @fn void iastore(Frame* frame)
*   @brief Desempilha a referencia para o array, o indice e o valor(inteiro) e salva o valor no array.
*   @param frame Frame corrente.
*   @return
*/
void InstructionImpl::iastore(Frame * this_frame){

   auto value = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto index = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   auto arrayRef = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();

   std::cout << (int)index->tag << std::endl;
   getchar();
   arrayRef->array_type->array->at(index->type_int) = value;

   std::cout << this_frame->operand_stack.size() << std::endl;
   getchar();
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

    Operand * result = (Operand*)calloc(1,sizeof(Operand));
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
    result->type_int = floor(operand_2->type_int / operand_1->type_int);

    this_frame->operand_stack.push(result);

 }

 /**
 * @brief Calcula o modulo entre dois inteiros.
 * @param *this_frame Ponteiro para o frame atual
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
    this_frame->pc++;

    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    auto result = (Operand *)calloc(1,sizeof(Operand));
    result->type_long = op2->type_long % op1->type_long;
    result->tag = CONSTANT_Long;

    this_frame->operand_stack.push(result);

 }

 /**
 * @brief Calcula o valor negativo de int. Retira o operando do topo da pilha, nega o valor do
 * operando e o salva o resultado no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::ineg(Frame * this_frame){
    this_frame->pc++;

    Interpreter aux;

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    auto result  = aux.createType("I");
    result->type_int = -(uint32_t)op->type_int;

    this_frame->operand_stack.push(result);
 }
 /**
 * @brief Calcula o valor negativo de long. Retira o operando do topo da pilha, nega o valor do
 * operando e o salva o resultado no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lneg(Frame * this_frame){
    this_frame->pc++;

    Interpreter aux;

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    auto result  = aux.createType("J");
    result->type_int = -(uint64_t)op->type_int;

    this_frame->operand_stack.push(result);
 }

/*
* @brief Desempilha 1 doubles da pilha de operandos e empilha o valor negativo dele.
* @param *this_frame ponteiro para o frame atual
* @return void
*/
 void InstructionImpl::dneg(Frame * this_frame){
     this_frame->pc++;

     Operand *operand_1 = this_frame->operand_stack.top();
     this_frame->operand_stack.pop();

     double value_1, value_2;
     memcpy(&value_1,&operand_1->type_double, sizeof(double));
     value_2 = -value_1;
     Operand *result = (Operand *) malloc(sizeof(Operand));

     result->tag = CONSTANT_Double;
     memcpy(&result->type_double,&value_2, sizeof(uint64_t));

     this_frame->operand_stack.push(result);

 }

 /**
 * @brief Calcula o valor do shift left lógico para inteiro. Retira dois operandos do topo da pilha
 * e faz o shift left do primeiro operando por s posições, onde s são os 5 bits menos significativos
 * do segundo operando. O resultado é colocado no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::ishl(Frame * this_frame){
    this_frame->pc++;

    Interpreter aux;

    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    auto val1 = op1->type_int;
    auto val2 = op2->type_int;

    val2 &= 0x0000001f;

    auto valResult = val1 << val2;

    auto result = aux.createType("I");

    result->type_int = valResult;
    this_frame->operand_stack.push(result);
 }

 /**
 * @brief Calcula o valor do shift left lógico para long. Retira dois operandos do topo da pilha
 * e faz o shift left do primeiro operando por s posições, onde s são os 5 bits menos significativos
 * do segundo operando. O resultado é colocado no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lshl(Frame * this_frame){
    this_frame->pc++;

    Interpreter aux;

    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    auto val1 = op1->type_long;
    auto val2 = op2->type_long;

    val2 &= 0x0000003f;

    auto l_result = val1 << val2;

    auto result = aux.createType("J");

    result->type_long = l_result;
    this_frame->operand_stack.push(result);

 }

 /**
 * @brief Calcula o valor do shift right lógico para inteiro. Retira dois operandos do topo da pilha
 * e faz o shift right do primeiro operando por s posições, onde s são os 5 bits menos significativos
 * do segundo operando. O resultado é colocado no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::ishr(Frame * this_frame){

    Interpreter aux;
    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    auto opResult   = aux.createType("I");
    uint32_t result = (uint32_t)(op1->type_int/pow(2,op2->type_int));

    opResult->type_int = result;

    this_frame->operand_stack.push(opResult);
    this_frame->pc++;

 }

 /**
 * @brief Calcula o valor do shift right lógico para long. Retira dois operandos do topo da pilha
 * e faz o shift right do primeiro operando por s posições, onde s são os 5 bits menos significativos
 * do segundo operando. O resultado é colocado no topo da pilha.
 * @param *this_frame Ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::lshr(Frame * this_frame){
    this_frame->pc++;

    Interpreter aux;
    auto operand_1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    uint64_t l_result = (uint64_t) (operand_1->type_long / pow(2, op2->type_long & 0x3f));

    auto result = aux.createType("J");
    result->type_long = (uint64_t) l_result;

    this_frame->operand_stack.push(result);
 }
 /**
 *   @fn void iushr(Frame* this_frame)
 *   @brief Desempilha dois valores inteiros da pilha, realiza o shift para a direita do número de bits,indicados
 *   pelos 2 valor do stack e empilha o resultado.
 *   @param this_frame Ponteiro para frame corrente.
 *   @return
 */
 void InstructionImpl::iushr(Frame * this_frame){
    Operand *op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand *op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    int shiftedValue = op1->type_int << op2->type_int;

    memcpy(&op1->type_int,&shiftedValue,sizeof(int));
    this_frame->operand_stack.push(op1);
    this_frame->pc++;
 }

/**
 * @brief Realiza um shift a direita do valor v2.
 * @param *this_frame ponteiro para frame atual.
 * @return void
 */
 void InstructionImpl::lushr(Frame * this_frame){

    Interpreter auxInter;

    this_frame->pc++;

    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    auto v1 = op1->type_long;
    auto v2 = op2->type_long;

    v2 &= 0x0000003f;

    auto result = v1 >> v2;
    auto opResult = auxInter.createType("J");

    opResult->type_long = result;
    this_frame->operand_stack.push(opResult);
 }

/**
 * @brief Realiza um and bit a bit em um long.
 * @param *this_frame ponteiro para frame atual.
 * @return void
 */
 void InstructionImpl::land(Frame * this_frame){

    Interpreter auxInter;

    this_frame->pc++;
    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    uint64_t v1 = op1->type_long;
    uint64_t v2 = op2->type_long;

    auto result = auxInter.createType("J");
    result->type_long = (uint64_t)(v1 & v2);
    this_frame->operand_stack.push(result);
 }

/**
 * @class  InstructionImpl::ixor
 * @brief Boolean XOR inteiro
 * @param *this_frame ponteiro para o frame atual.
 * @void
 **/
 void InstructionImpl::ixor(Frame * this_frame){
    Operand *operand_1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    Operand *operand_2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    Operand *result = (Operand *) malloc(sizeof(Operand));

    result->tag = CONSTANT_Integer;
    result->type_int = operand_2->type_int ^ operand_1->type_int;

    this_frame->operand_stack.push(result);
    this_frame->pc++;

 }

/**
 *@class InstructionImpl::lor
 * @brief Boolean OR long
 * @param *this_frame ponteiro para o frame atual.
 * @void
 */
 void InstructionImpl::lor(Frame * this_frame){

    this_frame->pc++;

    Operand * result = (Operand*)calloc(1,sizeof(Operand));
    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    result->tag = CONSTANT_Long;
    result->type_long = op2->type_long | op1->type_long;

    this_frame->operand_stack.push(result);

 }

/**
 *@class InstructionImpl::lxor
 * @brief Boolean XOR long
 * @param *this_frame ponteiro para o frame atual.
 * @void
 */
 void InstructionImpl::lxor(Frame * this_frame){
    this_frame->pc++;

    Operand * result = (Operand*)calloc(1,sizeof(Operand));
    auto op1 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();
    auto op2 = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    result->tag = CONSTANT_Long;
    result->type_long = op2->type_long ^ op1->type_long;

    this_frame->operand_stack.push(result);

 }

/**
 * @brief Verifica se é null, caso sim, realiza um salto baseado em um offset.
 * @param *this_frame ponteiro para o frame atual.
 * @void
 */
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

/**
 * @brief Verifica se é null, caso não, realiza um salto baseado em um offset.
 * @param *this_frame ponteiro para o frame atual.
 * @void
 */
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

/**
*   @brief void i2b(Frame* this_frame)
*   @brief Função convert um inteiro  para byte.
*   @param *this_frame Ponteiro para Ponteiro para frame.
*   @return
*/
void InstructionImpl::i2b(Frame * this_frame) {
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   uint32_t converted_value = (uint32_t) op->type_int;
   memcpy(&op->type_byte,&converted_value,sizeof(uint32_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;

}

/**
*   @brief void i2c(Frame* frame)
*   @brief Função que desempilha um inteiro, converte para char e empilha novamente.
*   @param frame Ponteiro para frame.
*   @return
*/
void InstructionImpl::i2c(Frame * this_frame) {
   Operand * op = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   char converted_value = (char) op->type_int;
   memcpy(&op->type_char,&converted_value,sizeof(uint32_t));
   this_frame->operand_stack.push(op);
   this_frame->pc++;
}

/**
*   @brief void lcmp(Frame* this_frame)
*   @brief Função desempilha dois longs, compara os memos e empilha o resultado da comparação. 0 se forem iguais,
*   se o segund número for maior empilha 1, caso contrário empilha -1.
*   @param this_frame Ponteiro para frame.
*   @return
*/
void InstructionImpl::lcmp(Frame * this_frame) {
   Operand * op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   int result = 0;
   if (op1->type_long < op2->type_long) {
      result = 1;
   } else if (op1->type_long > op2->type_long) {
      result = -1;
   }
   memcpy(&op1->type_int,&result,sizeof(uint32_t));
   this_frame->operand_stack.push(op1);
   this_frame->pc++;
}

/**
*   @brief void fcmpl(Frame* this_frame)
*   @brief Função que desempilha dois floats, compara ambos e empilha o resultado da comparação. 0 se forem iguais,
*   se o segundo número for maior empilha 1, caso contrário empilha -1. Caso algum dos números seja NaN, empilha 1.
*   @param *this_frame Frame corrente.
*   @return
*/
void InstructionImpl::fcmpl(Frame * this_frame) {
   Operand * op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   int result = 0;
   if (isnan(op1->type_float) || isnan(op2->type_float)) {
   		result = 1;
   } else if ((int)op1->type_float < (int)op2->type_float) {
      result = 1;
   } else if ((int)op1->type_float > (int)op2->type_float) {
      result = -1;
   }
   memcpy(&op1->type_int,&result,sizeof(uint32_t));
   this_frame->operand_stack.push(op1);
   this_frame->pc++;

}

/**
*   @brief void fcmpg(Frame* this_frame)
*   @brief Função desempilha dois floats, compara ambos e empilha o resultado da comparação. Se são iguais empilha 0,
*   se o segundo número for maior empilha 1, caso contrário empilha -1. Caso algum dos números seja NaN, empilha 1.
*   @param *this_frame Ponteiro para frame
*   @return
*/
void InstructionImpl::fcmpg(Frame * this_frame) {
   Operand * op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   int result = 0;

   if (isnan(op1->type_float) || isnan(op2->type_float)) {
      result = 1;
   } else if (op1->type_float < op2->type_float) {
      result = 1;
   } else if (op1->type_float > op2->type_float) {
      result = -1;
   }
   memcpy(&op1->type_int,&result,sizeof(uint32_t));
   this_frame->operand_stack.push(op1);
   this_frame->pc++;

}

/**
*   @brief void dcmpl(Frame* this_frame)
*   @brief Função desempilha dois doubles, compara os memos e empilha o resultado da comparação. 0 se forem iguais,
*   se o segundo número for maior empilha 1, caso contrário empilha -1. Caso algum dos números seja NaN, empilha 0.
*   @param this_frame Ponteiro para frame.
*   @return
*/
void InstructionImpl::dcmpl(Frame * this_frame){
   Operand * op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   int result = 0;

   if (isnan(op1->type_double) || isnan(op2->type_double)) {
      result = 1;
   } else if ((double)op1->type_double < (double)op2->type_double) {
      result = 1;
   } else if ((double)op1->type_double > (double)op2->type_double) {
      result = -1;
   }
   memcpy(&op1->type_int,&result,sizeof(uint32_t));
   this_frame->operand_stack.push(op1);
   this_frame->pc++;

}

/**
*   @brief void dcmpg(Frame* this_frame)
*   @brief Função desempilha dois doubles, compara os memos e empilha o resultado da comparação. 0 se forem iguais,
*   se o segundo número for maior empilha 1, caso contrário empilha -1. Caso algum dos números seja NaN, empilha 1.
*   @param this_frame Ponteiro para frame.
*   @return
*/
void InstructionImpl::dcmpg(Frame * this_frame){
   Operand * op1 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   Operand * op2 = this_frame->operand_stack.top();
   this_frame->operand_stack.pop();
   int result = 0;

   if (isnan(op1->type_double) || isnan(op2->type_double)) {
      result = 1;
   } else if ((double)op1->type_double < (double)op2->type_double) {
      result = 1;
   } else if ((double)op1->type_double > (double)op2->type_double) {
      result = -1;
   }
   memcpy(&op1->type_int,&result,sizeof(uint32_t));
   this_frame->operand_stack.push(op1);
   this_frame->pc++;

}

/*
 * @brief Instruçao que executa o desvio para determinado endereco.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::i_goto(Frame * this_frame){
   int16_t branchbyte1 = this_frame->method_code.code[this_frame->pc+1];
   int16_t branchbyte2 = this_frame->method_code.code[this_frame->pc+2];
   int16_t offset = (branchbyte1 << 8) | branchbyte2;
   this_frame->pc += offset;

 }

 /*
 * @brief Instruçao que executa o desvio para determinado endereco com mais bytes.
 * @param *this_frame ponteiro para o frame atual
 * @return void
 */
 void InstructionImpl::goto_w(Frame * this_frame){
   uint32_t branchbyte1 = this_frame->method_code.code[this_frame->pc+1];
   uint32_t branchbyte2 = this_frame->method_code.code[this_frame->pc+2];
   uint32_t branchbyte3 = this_frame->method_code.code[this_frame->pc+3];
   uint32_t branchbyte4 = this_frame->method_code.code[this_frame->pc+4];
   uint32_t offset = (branchbyte1 << 24) | (branchbyte2 << 16) | (branchbyte3 << 8) | branchbyte4;
   this_frame->pc +=offset;

 }
 void InstructionImpl::instanceof(Frame * this_frame){

    /*CpAttributeInterface cpAt;

    uint16_t index = this_frame->method_code.code[this_frame->pc+1];
    index = this_frame->method_code.code[this_frame->pc+2];

    auto op = this_frame->operand_stack.top();
    this_frame->operand_stack.pop();

    auto name = cpAt.getUTF8(this_frame->cp_reference,index-1);
    std::cout << name.substr(name.find_last_of('/')+1,name.size()-1) << std::endl;
    getchar();

    switch(op->tag) {
      case CONSTANT_Boolean: break;
      case CONSTANT_Byte: break;
      case CONSTANT_Char: break;
      case CONSTANT_Double: break;
      case CONSTANT_Fieldref: break;
      case CONSTANT_Float: break;
      case CONSTANT_Integer: break;
      case CONSTANT_InterfaceMethodref: break;
      case CONSTANT_Long: break;
      case CONSTANT_Utf8: break;
      case CONSTANT_String: break;
      case CONSTANT_Short: break;
      case CONSTANT_NameAndType: break;
      case CONSTANT_Methodref: break;
      case CONSTANT_Empty: break;
      case CONSTANT_Class: break;

    }

    auto toPush = (Operand*)calloc(1,sizeof(Operand));
    toPush->tag = CONSTANT_Integer;
    toPush->type_int = 1;
    this_frame->operand_stack.push(toPush);*/
    for(int i = 0; i < 3; i++) this_frame->pc++;

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
/*
* @brief Instruçao que executa o desvio salvando o endereco de retorno na pilha de operandos.
* @param *this_frame ponteiro para o frame atual
* @return void
*/
void InstructionImpl::jsr(Frame * this_frame){
   Operand * op = (Operand*)calloc(1,sizeof(Operand));
   op->type_byte = this_frame->pc;
   this_frame->operand_stack.push(op);
   uint16_t branchbyte1 = this_frame->method_code.code[this_frame->pc+1];
   uint16_t branchbyte2 = this_frame->method_code.code[this_frame->pc+2];
   uint16_t offset = (branchbyte1 << 8) | (branchbyte2);

   this_frame->pc += offset;
}

/*
* @brief Instruçao que executa o desvio salvando o endereco de retorno na pilha de operandos.
* @param *this_frame ponteiro para o frame atual
* @return void
*/
void InstructionImpl::jsr_w(Frame * this_frame){
   Operand * op = (Operand*)calloc(1,sizeof(Operand));
   op->type_byte = this_frame->pc;
   this_frame->operand_stack.push(op);
   uint32_t branchbyte1 = this_frame->method_code.code[this_frame->pc+1];
   uint32_t branchbyte2 = this_frame->method_code.code[this_frame->pc+2];
   uint32_t branchbyte3 = this_frame->method_code.code[this_frame->pc+3];
   uint32_t branchbyte4 = this_frame->method_code.code[this_frame->pc+4];
   uint32_t offset = (branchbyte1 << 24) | (branchbyte2 << 16) | (branchbyte3 << 8) | branchbyte4;

   this_frame->pc += offset;
}

 void InstructionImpl::checkcast(Frame * this_frame){
    InstructionImpl::nop(this_frame);

 }

void InstructionImpl::impdep1(Frame * this_frame) {
   this_frame->pc++;
   std::cout << "impdep1 é reservada do Java" << std::endl;
   exit(-1);
}
void InstructionImpl::impdep2(Frame * this_frame) {
   this_frame->pc++;
   std::cout << "impdep2 é reservada do Java" << std::endl;
   exit(-1);
}
void InstructionImpl::breakpoint(Frame * this_frame) {
   this_frame->pc++;
   std::cout << "breakpoint é reservada do Java" << std::endl;
   exit(-1);
}