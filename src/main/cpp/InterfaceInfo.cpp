/** @file InterfaceInfo.hpp
 *  @brief Implementações das funções e atributos de InterfaceInfo.
 *  @bug No known bugs.
 */
#include "../hpp/InterfaceInfo.hpp"

InterfaceInfo::~InterfaceInfo() {

}

void InterfaceInfo::setInterfaceInfo(FILE *fp) {
    ByteReader<uint16_t> TwoByte;
    interface_table = TwoByte.byteCatch(fp);
}
