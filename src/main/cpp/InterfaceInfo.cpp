/** @file InterfaceInfo.cpp
    @brief Método para carregar informações para a InterfaceInfo;

*/
#include "../hpp/InterfaceInfo.hpp"

InterfaceInfo::~InterfaceInfo() {

}

/** @brief Carrega as informações de um arquivo na instância de InterfaceInfo.
 * @param *fp ponteiro de arquivo
 * @return void
 */
void InterfaceInfo::setInterfaceInfo(FILE *fp) {
    ByteReader<uint16_t> TwoByte;
    interface_table = TwoByte.byteCatch(fp);
}
