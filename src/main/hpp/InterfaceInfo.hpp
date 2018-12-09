/** @file InterfaceInfo.hpp
 *  @brief Declarações das funções e atributos de InterfaceInfo.
 *  @bug No known bugs.
 */
#ifndef ___INTERFACEINFO_H___
#define ___INTERFACEINFO_H___

#include <fstream>
#include <cstdint>

#include "ByteReader.hpp"

/**
*   @class InterfaceInfo
*   @brief classe contém interface_table(uint16)
*			Além de um destructor e um método para setar as informações da interface;    
*/

class InterfaceInfo {

    public:
        uint16_t interface_table;

        ~InterfaceInfo();
        void setInterfaceInfo(FILE*);

};

#endif