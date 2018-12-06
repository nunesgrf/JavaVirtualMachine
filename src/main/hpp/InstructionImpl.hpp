/** @file InstructionImpl.hpp
 *  @brief Declarações das funções do ClassLoader para salvar todos os bytes de .class corretamente.
 *  @bug No known bugs.
 */
#ifndef INTRUCTION_IMPL_H
#define INTRUCTION_IMPL_H
#define T_BOOLEAN 4
#define T_CHAR 	5
#define T_FLOAT 6
#define T_DOUBLE 7
#define T_BYTE 	8
#define T_SHORT 9
#define T_INT 10
#define T_LONG 11

#include "Frame.hpp"
struct Frame;

/**
*   @class InstructionImpl
*   @brief Objetivo incluir todos os metodos para a ação de cada instrução achada pelo interpretador;    
*/

class InstructionImpl {

public:
    static void nop(Frame *);
    static void ldc(Frame *);
    static void invokespecial(Frame *);
    static void invokevirtual(Frame *);
    static void astore_1(Frame *);
    static void astore_2(Frame *);
    static void astore_3(Frame *);
    static void getstatic(Frame *);
    static void aaload(Frame *);
    static void aconst_null(Frame *);
    static void iconst_m1(Frame *);
    static void iconst_0(Frame *);
    static void iconst_1(Frame *);
    static void iconst_2(Frame *);
    static void iconst_3(Frame *);
    static void iconst_4(Frame *);
    static void iconst_5(Frame *);
    static void getfield(Frame *);
    static void aload(Frame *);
    static void aload_0(Frame *);
    static void aload_1(Frame *);
    static void aload_2(Frame *);
    static void aload_3(Frame *);
    static void void_return(Frame *);
    static void iinc(Frame *);
    static void iload(Frame *);
    static void iload_0(Frame *);
    static void iload_1(Frame *);
    static void iload_2(Frame *);
    static void iload_3(Frame *);
    static void lconst_0(Frame *);
    static void lconst_1(Frame *);
    static void fconst_0(Frame *);
    static void fconst_1(Frame *);
    static void fconst_2(Frame *);
    static void dconst_0(Frame *);
    static void dconst_1(Frame *);
    static void bipush(Frame *);
    static void sipush(Frame *);
    static void lload_0(Frame *);
    static void lload_1(Frame *);
    static void lload_2(Frame *);
    static void lload_3(Frame *);
    static void fload_0(Frame *);
    static void fload_1(Frame *);
    static void fload_2(Frame *);
    static void fload_3(Frame *);
    static void lload(Frame *);
    static void dload(Frame *);
    static void dload_0(Frame *);
    static void dload_1(Frame *);
    static void dload_2(Frame *);
    static void dload_3(Frame *);
    static void dstore(Frame *);
    static void dstore_0(Frame *);
    static void dadd(Frame *);
    static void dsub(Frame *);
    static void dmul(Frame *);
    static void ddiv(Frame *);
    static void dstore_1(Frame *);
    static void dstore_2(Frame *);
    static void dstore_3(Frame *);
    static void fstore(Frame *);
    static void fstore_0(Frame *);
    static void fstore_1(Frame *);
    static void fstore_2(Frame *);
    static void fstore_3(Frame *);
    static void lstore(Frame *);
    static void lstore_0(Frame *);
    static void lstore_1(Frame *);
    static void lstore_2(Frame *);
    static void lstore_3(Frame *);
    static void fload(Frame *);
    static void istore(Frame *);
    static void istore_0(Frame *);
    static void istore_1(Frame *);
    static void istore_2(Frame *);
    static void istore_3(Frame *);
    static void f2d(Frame *);
    static void f2i(Frame *);
    static void l2d(Frame *);
    static void l2f(Frame *);
    static void l2i(Frame *);
    static void i2f(Frame *);
    static void i2s(Frame *);
    static void d2f(Frame *);
    static void iadd(Frame *);
    static void iand(Frame *);
    static void d2l(Frame *);
    static void d2i(Frame *);
    static void ior(Frame *);
    static void dup_x1(Frame *);
    static void dup_x2(Frame *);
    static void dup2(Frame *);
    static void dup2_x1(Frame *);
    static void dup2_x2(Frame *);
    static void f2l(Frame *);
    static void ladd(Frame *);
    static void lsub(Frame *);
    static void ldiv(Frame *);
    static void laload(Frame *);
    static void iaload(Frame *);
    static void faload(Frame *);
    static void daload(Frame *);
    static void baload(Frame *);
    static void caload(Frame *);
    static void saload(Frame *);

    static void lastore(Frame *);
    static void fastore(Frame *);
    static void dastore(Frame *);
    static void aastore(Frame *);
    static void bastore(Frame *);
    static void castore(Frame *);
    static void sastore(Frame *);

    static void pop(Frame *);
    static void pop2(Frame *);

    static void astore(Frame *);
    static void astore_0(Frame *);

    static void if_icmpge(Frame *);
    static void ins_goto(Frame *);
    static void i2l(Frame *);
    static void i2d(Frame *);
    static void invokestatic(Frame *);
    static void lreturn(Frame *);
    static void new_obj(Frame *);
    static void dup(Frame *);

    static void putfield(Frame *);

    static void ldc_w(Frame *);
    static void ldc2_w(Frame *);

    static void isub(Frame *);
    static void invokeinterface(Frame *);
    static void invokedynamic(Frame *);
    static void areturn(Frame *);
    static void dreturn(Frame *);
    static void freturn(Frame *);
    static void ireturn(Frame *);

    static void fadd(Frame *);
    static void fsub(Frame *);
    static void fdiv(Frame *);
    static void fmul(Frame *);
    static void frem(Frame *);
    static void drem(Frame *);
    static void fneg(Frame *);
    static void lookupswitch(Frame *);

    static void if_icmpne(Frame *);
    static void if_icmpeq(Frame *);
    static void ifeq(Frame *);
    static void ifne(Frame *);
    static void iflt(Frame *);
    static void ifge(Frame *);
    static void ifgt(Frame *);
    static void ifle(Frame *);
    static void if_icmplt(Frame *);
    static void if_icmpgt(Frame *);
    static void if_icmple(Frame *);
    static void if_acmpeq(Frame *);
    static void if_acmpne(Frame *);

    static void newarray(Frame *);
    static void anewarray(Frame *);
    static void arraylength(Frame *);
    static void iastore(Frame *);

    static void swap(Frame *);
    static void imul(Frame *);
    static void lmul(Frame *);
    static void idiv(Frame *);
    static void irem(Frame *);
    static void lrem(Frame *);
    static void ineg(Frame *);
    static void lneg(Frame *);
    static void dneg(Frame *);
    static void ishl(Frame *);
    static void lshl(Frame *);
    static void ishr(Frame *);
    static void lshr(Frame *);
    static void iushr(Frame *);
    static void lushr(Frame *);
    static void land(Frame *);
    static void lor(Frame *);
    static void ixor(Frame *);
    static void lxor(Frame *);


    static void ifnull(Frame *);
    static void ifnonnull(Frame *);

    static void ret(Frame *);
    static void i2b(Frame *);
    static void i2c(Frame *);
    static void lcmp(Frame *);
    static void fcmpl(Frame *);
    static void fcmpg(Frame *);
    static void dcmpl(Frame *);
    static void dcmpg(Frame *);
    static void i_goto(Frame *);
    static void jsr(Frame *);
    static void jsr_w(Frame *);
    static void putstatic(Frame *);
    static void athrow(Frame *);
    static void checkcast(Frame *);
    static void goto_w(Frame *);
    static void monitorenter(Frame *);
    static void monitorexit(Frame *);
    static void instanceof(Frame *);
    static void wide(Frame *);
    static void multianewarray(Frame *);

    static void tableswitch(Frame *);
};

#endif
