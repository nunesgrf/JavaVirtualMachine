/** @file FieldInfo.hpp
 *  @brief Declarações das funções do FieldInfo para tratamento dos fields do arquivo .class.
 *  @bug No known bugs.
 */
#ifndef ___FIELDINFO_H___
#define ___FIELDINFO_H___

#include "CpInfo.hpp"
#include "AttributeInfo.hpp"
#include <cstdint>

class FieldInfo {
    public:
        uint16_t access_flags;
        uint16_t name_index;
        uint16_t descriptor_index;
        uint16_t attributes_count;
        std::vector<AttributeInfo *> attributes;

        ~FieldInfo();
        void read(FILE *,std::vector<CpInfo*>);
};

#endif