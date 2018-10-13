#include "../hpp/InterfaceInfo.hpp"

void InterfaceInfo::read(ClassLoader* classloader, FILE* fp){
    int i = 0;

    for (i = 0; i < classloader.interfaceCounter; i++){
        classloader.interfaces[i] = this->getInterfaceInfo(classloader,fp,classloader.interface[i]);
    }
}

InterfaceInfo InterfaceInfo::getInterfaceInfo(ClassLoader *classloader, FILE *fp, InterfaceInfo interface_info) {
    ByteReader<uint16_t> TwoByte;
    
    interface_info.interface_table = TwoByte.byteCatch(fp);
    return classloader.interface_info;

}