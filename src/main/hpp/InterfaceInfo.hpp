#ifndef ___INTERFACEINFO_H___
#define ___INTERFACEINFO_H___

#include "ClassLoader.hpp"
#include "../cpp/ByteReader.cpp"

class InterfaceInfo {

    public:
        uint16_t interface_table;
        void setInterfaceInfo(FILE*);

};

#endif