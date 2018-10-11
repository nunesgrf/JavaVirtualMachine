#include "CpInfo.hpp"

ByteReader<uint8_t> OneByte;
ByteReader<uint16_t> TwoByte;
ByteReader<uint32_t> FourByte;

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
            utf8_const = getUTF8CP(constant_pool, constant_pool[position].Class.name_index - 1);
            break;
        default:
            
    }
}