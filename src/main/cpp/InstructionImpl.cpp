#include "../hpp/Frame.hpp"
#include "../hpp/CpAttributeInterface.hpp"
#include "../hpp/GLOBAL_file.hpp"
#include <iostream>

void InstructionImpl::nop(Frame * this_frame) {
    this_frame->pc++;
}

 void InstructionImpl::ldc(Frame * this_frame){
    InstructionImpl::nop(this_frame);
    this_frame->pc++;
    
 }
 void InstructionImpl::invokespecial(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::invokevirtual(Frame * this_frame){
    InstructionImpl::nop(this_frame);
    this_frame->pc++;
    this_frame->pc++;
     

 }
 void InstructionImpl::astore_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::astore_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::astore_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::getstatic(Frame * this_frame){

    InstructionImpl::nop(this_frame);
    MethodsArea container;

    CpAttributeInterface cpAtAux;
    uint16_t pos = this_frame->method_code.code[this_frame->pc++]; 
    pos = (pos << 8) + this_frame->method_code.code[this_frame->pc++];

    CpInfo * field = this_frame->cp_reference[pos-1];
    CpInfo * name_n_type = this_frame->cp_reference[field->Fieldref.name_and_type_index-1];

    std::string className = cpAtAux.getUTF8(this_frame->cp_reference,field->Fieldref.name_and_type_index-1);

    if(className == "java/lang/System") return;

    //TERMINAR ISSO DAQUI
 }

 void InstructionImpl::aaload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::aconst_null(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iconst_m1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::iconst_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::iconst_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::iconst_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iconst_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);

    Operand * op = (Operand*)calloc(1,sizeof(op));
    op->tag = CONSTANT_Integer;
    op->type_int = 3;
    this_frame->operand_stack.push(op);
     
 }
 void InstructionImpl::iconst_4(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iconst_5(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::getfield(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
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
 void InstructionImpl::iinc(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iload_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iload_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iload_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::iload_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lconst_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lconst_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::fconst_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fconst_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fconst_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dconst_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dconst_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::bipush(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::sipush(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lload_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lload_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::lload_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::lload_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::fload_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fload_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fload_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fload_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dload_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dload_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dload_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dload_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dstore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dstore_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
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
 void InstructionImpl::dstore_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dstore_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::dstore_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fstore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fstore_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fstore_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fstore_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fstore_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lstore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lstore_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lstore_1(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lstore_2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     

 }
 void InstructionImpl::lstore_3(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::fload(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
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
 void InstructionImpl::ladd(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::lsub(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ldiv(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
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
     

 }
 void InstructionImpl::pop2(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }

 void InstructionImpl::astore(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::astore_0(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
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

 void InstructionImpl::swap(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
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
 void InstructionImpl::lmul(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
 }
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
 void InstructionImpl::irem(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
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
    InstructionImpl::nop(this_frame);
     
 }
 void InstructionImpl::ifnonnull(Frame * this_frame){
    InstructionImpl::nop(this_frame);
     
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

