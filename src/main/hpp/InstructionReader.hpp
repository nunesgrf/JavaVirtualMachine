#ifndef INSTRUCTION_READER2
#define INSTRUCTION_READER2

#define newarray 188
#define anewarray 189
#define multianewarray 197
#define checkcast 192
#define getfield 180
#define getstatic 178
#define instanceof 193
#define invokedynamic 186
#define invokeinterface 185
#define invokespecial 183
#define invokestatic 184
#define invokevirtual 182
#define ldc 18
#define ldc_w 19
#define ldc2_w 20
#define NEW 187
#define putfield 181
#define putstatic 179
#define GOTO 167
#define if_acmpeq 165
#define if_acmpne 166
#define if_icmpeq 159
#define if_icmpne 160
#define if_icmplt 161
#define if_icmpge 162
#define if_icmpgt 163
#define if_icmple 164
#define iifeq 153
#define ifne 154
#define iflt 155
#define ifge 156
#define ifgt 157
#define ifle 158
#define ifnonull 199
#define ifnull 198
#define jsr 168

#define aload 25
#define aload_0 42
#define aload_1 43
#define aload_2 44
#define aload_3 45
#define areturn 176
#define return_original 177


class InstructionReader
{
    public:

    void print(int);
};

#endif