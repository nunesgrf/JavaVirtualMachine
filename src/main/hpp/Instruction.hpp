/** @file Instruction.hpp
 *  @brief Declarações das instruçoes referentes ao interpretador da JVM.
 *  @bug No known bugs.
 */
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include "InstructionImpl.hpp"

#define c_newarray 188
#define c_anewarray 189
#define c_multianewarray 197
#define c_checkcast 192
#define c_getfield 180
#define c_getstatic 178
#define c_instanceof 193
#define c_invokedynamic 186
#define c_invokeinterface 185
#define c_invokespecial 183
#define c_invokestatic 184
#define c_invokevirtual 182
#define c_ldc 18
#define c_ldc_w 19
#define c_ldc2_w 20
#define c_NEW 187
#define c_putfield 181
#define c_putstatic 179
#define c_GOTO 167
#define c_if_acmpeq 165
#define c_if_acmpne 166
#define c_if_icmpeq 159
#define c_if_icmpne 160
#define c_if_icmplt 161
#define c_if_icmpge 162
#define c_if_icmpgt 163
#define c_if_icmple 164
#define c_iifeq 153
#define c_ifne 154
#define c_iflt 155
#define c_ifge 156
#define c_ifgt 157
#define c_ifle 158
#define c_ifnonull 199
#define c_ifnull 198
#define c_jsr 168

#define c_aload 25
#define c_aload_0 42
#define c_aload_1 43
#define c_aload_2 44
#define c_aload_3 45
#define c_areturn 176
#define c_return_original 177

struct Frame;
/**
*	@class Instruction
*	@brief Determina a instrução de acordo com o interpretador;	
*/
class Instruction {
    public:
        std::string name;
        uint32_t bytes;
        void (* func)(Frame * this_frame);
        static void init(Instruction *, int);
};

#endif
