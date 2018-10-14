#include "../main/cpp/ClassLoader.cpp"
#include "../main/cpp/ByteReader.cpp"
#include "../main/cpp/CpInfo.cpp"
#include "../main/cpp/CpAttributeInterface.cpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;
int main() {

    FILE * fp = fopen("HelloWorld.class","r");
    
    ClassLoader classloader(fp);
    CpAttributeInterface x;

    cout << "MagicNumber  : " << hex << classloader.getMagic() << endl;
    cout << "MinorVersion : " << dec << classloader.getMinor() << endl;
    cout << "MajorVersion : " << dec << classloader.getMajor() << endl;
    cout << "PoolCounter  : " << dec << classloader.getConstCount() << endl;
    cout << "AcessFlag    : " << hex << classloader.getFlag() << endl;
    cout << "ThisClass    : " << dec << classloader.getThisClass() << endl;
    cout << "SuperClass   : " << dec << classloader.getSuper() << endl;
    cout << "InterfaceCou : " << dec << classloader.getInterCounter() << endl;
    cout << "FieldsCount  : " << dec << classloader.getFieldCount() << dec << endl;
    cout << "MethodCount  : " << dec << classloader.getMethoCount() << dec << endl;
    cout << "AtributeCoun : " << dec << classloader.getAttriCount() << endl;

    for(int i = 0; i < classloader.getConstCount()-1; i++) {
        cout << x.getUTF8(classloader.getConstPool(),i) << endl;
    }
    fclose(fp);
}