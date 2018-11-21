#ifndef ___INTERFACEINFO_H___
#define ___INTERFACEINFO_H___

#include <fstream>
#include <cstdint>

#include "../cpp/ByteReader.cpp"

class InterfaceInfo {

    public:
        uint16_t interface_table;

        ~InterfaceInfo();
        void setInterfaceInfo(FILE*);

};

#endif