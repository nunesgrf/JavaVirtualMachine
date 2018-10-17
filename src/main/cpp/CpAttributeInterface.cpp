#ifndef CPP_INTERACP
#define CPP_INTERACP

#include "../hpp/CpAttributeInterface.hpp"

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

#endif