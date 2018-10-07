#ifndef ___ATTRIBUTEINFO_H___
#define ___ATTRIBUTEINFO_H___

#include <cstdint>
#include <vector>

class ConstantValue {
    uint16_t name_index;
    uint32_t length;
    uint16_t constvalue_index;
};

class CodeAttribute {
    uint16_t name_index;
    uint16_t max_stacks;
    uint16_t max_locals;
    /* Attribute length */
    uint32_t length;
    /* Code length */
    uint32_t code_length;
    uint16_t exception_length;
    /* TO DO: Criar a lista de exceçao*/
};

class AttributeInfo{
    public: 
        uint16_t name_index;
        uint32_t length
        std::vector<uint8_t> info;

};