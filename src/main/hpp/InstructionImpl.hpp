/** @file InstructionImpl.hpp
 *  @brief Declarações das funções do ClassLoader para salvar todos os bytes de .class corretamente.
 *  @bug No known bugs.
 */
#ifndef INTRUCTION_IMPL_H
#define INTRUCTION_IMPL_H


class InstructionImpl {

public:
    static void nop( );
    static void ldc( );
    static void invokespecial( );
    static void invokevirtual( );
    static void astore_1( );
    static void astore_2( );
    static void astore_3( );
    static void getstatic( );
    static void aaload( );
    static void aconst_null( );
    static void iconst_m1( );
    static void iconst_0( );
    static void iconst_1( );
    static void iconst_2( );
    static void iconst_3( );
    static void iconst_4( );
    static void iconst_5( );
    static void getfield( );
    static void aload( );
    static void aload_0( );
    static void aload_1( );
    static void aload_2( );
    static void aload_3( );
    static void void_return( );
    static void iinc( );
    static void iload( );
    static void iload_0( );
    static void iload_1( );
    static void iload_2( );
    static void iload_3( );
    static void lconst_0();
    static void lconst_1();
    static void fconst_0();
    static void fconst_1();
    static void fconst_2();
    static void dconst_0();
    static void dconst_1();
    static void bipush();
    static void sipush();
    static void lload_0();
    static void lload_1();
    static void lload_2();
    static void lload_3();
    static void fload_0();
    static void fload_1();
    static void fload_2();
    static void fload_3();
    static void lload();
    static void dload();
    static void dload_0();
    static void dload_1();
    static void dload_2();
    static void dload_3();
    static void dstore();
    static void dstore_0();
    static void dadd();
    static void dsub();
    static void dmul();
    static void ddiv();
    static void dstore_1();
    static void dstore_2();
    static void dstore_3();
    static void fstore();
    static void fstore_0();
    static void fstore_1();
    static void fstore_2();
    static void fstore_3();
    static void lstore();
    static void lstore_0();
    static void lstore_1();
    static void lstore_2();
    static void lstore_3();
    static void fload();
    static void istore( );
    static void istore_0( );
    static void istore_1( );
    static void istore_2( );
    static void istore_3( );
    static void f2d( );
    static void f2i( );
    static void l2d( );
    static void l2f( );
    static void l2i( );
    static void i2f( );
    static void i2s( );
    static void d2f( );
    static void iadd( );
    static void iand( );
    static void d2l( );
    static void d2i( );
    static void ior( );
    static void dup_x1( );
    static void dup_x2( );
    static void dup2( );
    static void dup2_x1( );
    static void dup2_x2( );
    static void f2l( );
    static void ladd( );
    static void lsub( );
    static void ldiv( );
    static void laload( );
    static void iaload( );
    static void faload( );
    static void daload( );
    static void baload( );
    static void caload( );
    static void saload( );

    static void lastore( );
    static void fastore( );
    static void dastore( );
    static void aastore( );
    static void bastore( );
    static void castore( );
    static void sastore( );

    static void pop( );
    static void pop2( );

    static void astore( );
    static void astore_0( );

    static void if_icmpge();
    static void ins_goto();
    static void i2l();
    static void i2d();
    static void invokestatic( );
    static void lreturn( );
    static void new_obj( );
    static void dup( );

    static void putfield( );

    static void ldc_w( );
    static void ldc2_w( );

    static void isub( );
    static void invokeinterface( );
    static void invokedynamic( );
    static void areturn( );
    static void dreturn( );
    static void freturn( );
    static void ireturn( );

    static void fadd( );
    static void fsub( );
    static void fdiv( );
    static void fmul( );
    static void frem( );
    static void drem( );
    static void fneg( );
    static void lookupswitch( );

    static void if_icmpne( );
    static void if_icmpeq( );
    static void ifeq( );
    static void ifne( );
    static void iflt( );
    static void ifge( );
    static void ifgt( );
    static void ifle( );
    static void if_icmplt( );
    static void if_icmpgt( );
    static void if_icmple( );
    static void if_acmpeq( );
    static void if_acmpne( );

    static void newarray();
    static void anewarray();
    static void arraylength( );
    static void iastore();

    static void swap( );
    static void imul( );
    static void lmul( );
    static void idiv( );
    static void irem( );
    static void lrem( );
    static void ineg( );
    static void lneg( );
    static void dneg( );
    static void ishl( );
    static void lshl( );
    static void ishr( );
    static void lshr( );
    static void iushr( );
    static void lushr( );
    static void land( );
    static void lor( );
    static void ixor( );
    static void lxor( );


    static void ifnull( );
    static void ifnonnull( );

    static void ret( );
    static void i2b( );
    static void i2c( );
    static void lcmp( );
    static void fcmpl( );
    static void fcmpg( );
    static void dcmpl( );
    static void dcmpg( );
    static void i_goto( );
    static void jsr( );
    static void jsr_w( );
    static void putstatic( );
    static void athrow();
    static void checkcast();
    static void goto_w( );
    static void monitorenter( );
    static void monitorexit( );
    static void instanceof( );
    static void wide( );
    static void multianewarray( );

    static void tableswitch( );
};

#endif
