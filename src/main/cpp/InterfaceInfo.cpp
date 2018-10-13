#include "../hpp/InterfaceInfo.hpp"


void InterfaceInfo::setInterfaceInfo(FILE *fp) {
    ByteReader<uint16_t> TwoByte;
    interface_table = TwoByte.byteCatch(fp);
}