#ifndef CPP_INTER
#define CPP_INTER

#include "../hpp/InterfaceInfo.hpp"

InterfaceInfo::~InterfaceInfo() {

}

void InterfaceInfo::setInterfaceInfo(FILE *fp) {
    ByteReader<uint16_t> TwoByte;
    interface_table = TwoByte.byteCatch(fp);
}

#endif 