/** @file MethodInfo.hpp
 *  @brief Declarações das funções do MethodInfo estrutura responsável por guardas as informaçoes dos metodos do arquivo .class
 *  @bug No known bugs.
 */
#ifndef ___METHOD_H___
#define ___METHOD_H___

#include <cstdint>
#include "AttributeInfo.hpp"
#include "CpInfo.hpp"

struct MethodInfo
{
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    AttributeInfo *attributes;

    void read(FILE *fp,std::vector<CpInfo*>);
    ~MethodInfo();
};

#endif // ___METHOD_H___