#include "../hpp/CpAttributeInterface.hpp"

#define CONSTANT_Utf8 1
#define CONSTANT_Integer 3
#define CONSTANT_Float 4
#define CONSTANT_Long 5
#define CONSTANT_Double 6
#define CONSTANT_Class 7
#define CONSTANT_String 8
#define CONSTANT_Fieldref 9
#define CONSTANT_Methodref 10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_NameAndType 12
#define CONSTANT_Empty 0

std::string CpAttributeInterface::getUTF8(std::vector<CpInfo*> alpha, uint16_t beta) {

    if(alpha[beta]->tag == 1) return (char*)alpha[beta]->UTF8.bytes;

    switch(alpha[beta]->tag) {
        
        case CONSTANT_Class:
            return getUTF8(alpha, alpha[beta]->Class.name_index -1);
            break;

        case CONSTANT_Fieldref:
            return getUTF8(alpha, alpha[beta]->Fieldref.class_index -1);
            return getUTF8(alpha, alpha[beta]->Fieldref.name_and_type_index -1);
            break;

        case CONSTANT_Methodref:
            return getUTF8(alpha, alpha[beta]->Methodref.class_index -1);
            return getUTF8(alpha, alpha[beta]->Methodref.name_and_type_index -1);
            break;

        case CONSTANT_NameAndType:
            return getUTF8(alpha, alpha[beta]->NameAndType.name_index -1);
            return getUTF8(alpha, alpha[beta]->NameAndType.descriptor_index -1);
            break;

        case CONSTANT_InterfaceMethodref:
            return getUTF8(alpha, alpha[beta]->InterfaceMethodref.class_index -1);
            return getUTF8(alpha, alpha[beta]->InterfaceMethodref.name_and_type_index -1);
            break;

        case CONSTANT_String:
            return getUTF8(alpha, alpha[beta]->String.string_index -1);
            break;
        default:
             break; /* Must Change default */
    }
} 
