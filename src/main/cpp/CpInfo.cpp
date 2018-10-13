#include "../hpp/CpInfo.hpp"



std::string getUTF8CP(CpInfo* constant_pool, uint16_t position){

    std::string utf8_const;
    uint16_t tag = constant_pool[position].tag;

    switch(tag) {
        case CONSTANT_Utf8:
            utf8_const = (char*) constant_pool[position].UTF8.bytes;
            break;

        case CONSTANT_Class:
            utf8_const = getUTF8CP(constant_pool,constant_pool[position].Class.name_index - 1);
            break;

        case CONSTANT_Fieldref:
            utf8_const = getUTF8CP(constant_pool, constant_pool[position].Fieldref.class_index - 1);
            utf8_const += getUTF8CP(constant_pool, constant_pool[position].Fieldref.name_and_type_index - 1);
            break;

        case CONSTANT_Methodref:
            utf8_const = getUTF8CP(constant_pool, constant_pool[position].Methodref.class_index - 1);
            utf8_const += getUTF8CP(constant_pool, constant_pool[position].Methodref.name_and_type_index - 1);
            break;

        case CONSTANT_NameAndType:
            utf8_const = getUTF8CP(constant_pool, constant_pool[position].NameAndType.name_index - 1);
            utf8_const += getUTF8CP(constant_pool, constant_pool[position].NameAndType.descriptor_index - 1);
            break;

        case CONSTANT_InterfaceMethodref:
            utf8_const = getUTF8CP(constant_pool, constant_pool[position].InterfaceMethodref.class_index - 1);
            utf8_const += getUTF8CP(constant_pool, constant_pool[position].InterfaceMethodref.name_and_type_index - 1);
            break;

        case CONSTANT_String:
            utf8_const = getUTF8CP(constant_pool, constant_pool[position].String.string_index - 1);
            break;
        default:
             break; /* Must Change default */
    }

    return utf8_const;
}