/** @file InterfaceInfo.hpp
 *  @brief Declarações das funções e atributos de InterfaceInfo.
 *  @bug No known bugs.
 */
#ifndef ___INTERFACEINFO_H___
#define ___INTERFACEINFO_H___

#include <fstream>
#include <cstdint>

#include "ByteReader.hpp"

class InterfaceInfo {

    public:
        uint16_t interface_table;

        ~InterfaceInfo();
        void setInterfaceInfo(FILE*);

};

#endif