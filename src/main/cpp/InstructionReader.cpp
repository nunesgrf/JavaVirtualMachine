#ifndef INSTRUCTION_READER
#define INSTRUCTION_READER


#include "../hpp/InstructionReader.hpp"

void InstructionReader::print(int code)
{
    switch(code){
        case newarray : cout << "newarray" <<endl; break;
        case anewarray :cout << "anewarray" <<endl; break;
        case multianewarray :cout << "multianewarray" <<endl; break;
        case checkcast :cout << "checkcast" <<endl; break;
        case getfield :cout << "getfield" <<endl; break;
        case getstatic :cout << "getstatic" <<endl; break;
        case instanceof :cout << "instanceof" <<endl; break;
        case invokedynamic :cout << "invokedynamic" <<endl; break;
        case invokeinterface :cout << "invokeinterface" <<endl; break;
        case invokespecial :cout << "invokespecial" <<endl; break;
        case invokestatic :cout << "invokestatic" <<endl; break;
        case invokevirtual :cout << "invokevirtual" <<endl; break;
        case ldc :         cout << "ldc" <<endl; break;    
        case ldc_w :       cout << "ldc_w" <<endl; break; 
        case ldc2_w:cout << "ldc2_w" <<endl; break;
        case NEW :cout << "NEW" <<endl; break;
        case putfield :       cout << "putfield" <<endl; break;
        case putstatic : cout << "putstatic" <<endl; break;
        case GOTO :cout << "GOTO" <<endl; break;
        case if_acmpeq :cout << "if_acmpeq" <<endl; break;
        case if_acmpne :cout << "if_acmpne" <<endl; break;
        case if_icmpeq :cout << "if_icmpeq" <<endl; break;
        case if_icmpne :cout << "if_icmpne" <<endl; break;
        case if_icmplt :       cout << "if_icmplt" <<endl; break;
        case if_icmpge :cout << "if_icmpge" <<endl; break;
        case if_icmpgt :cout << "if_icmpgt" <<endl; break;
        case if_icmple :cout << "if_icmple" <<endl; break;
        case iifeq :cout << "iifeq" <<endl; break;
        case ifne :cout << "ifne" <<endl; break;
        case iflt :cout << "iflt" <<endl; break;
        case ifge :cout << "ifge" <<endl; break;
        case ifgt :cout << "ifgt" <<endl; break;
        case ifle :cout << "ifle" <<endl; break;
        case ifnonull :cout << "ifnonull" <<endl; break;
        case ifnull :cout << "ifnull" <<endl; break;
        case jsr :cout << "jsr" <<endl; break;
        default : break;
    }

}

#endif