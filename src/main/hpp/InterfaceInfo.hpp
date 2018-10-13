#ifndef ___INTERFACEINFO_H___
#define ___INTERFACEINFO_H___

#include "ClassLoader.hpp"
#include "../cpp/ByteReader.cpp"

class InterfaceInfo {

    private:
        uint16_t interface_table;

    public:
        uint16_t getInterfaceInfo() {
            return this->interface_table;
        }
        void setInterfaceInfo(FILE*);

};

#endif