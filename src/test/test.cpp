#include "../main/cpp/ClassLoader.cpp"
#include "../main/cpp/ByteReader.cpp"
#include "../main/cpp/CpInfo.cpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;
int main() {

    FILE * fp = fopen("HelloWorld.class","r");
    
    ClassLoader classloader(fp);
    cout << "MagicNumber  : " << hex << classloader.getMagic() << endl;
    cout << "MinorVersion : " << dec << classloader.getMinor() << endl;
    cout << "MajorVersion : " << dec << classloader.getMajor() << endl;
    cout << "PoolCounter  : " << dec << classloader.getConstCount() << endl;
    cout << "AcessFlag    : " << dec << classloader.getFlag() << endl;
    cout << "ThisClass    : " << dec << classloader.getThisClass() << endl;
    cout << "SuperClass   : " << dec << classloader.getSuper() << endl;
    cout << "InterfaceCou : " << dec << classloader.getInterCounter() << endl;
    cout << "FieldsCount  : " << dec << classloader.getFieldCount() << dec << endl;
    cout << "MethodCount  : " << dec << classloader.getMethoCount() << dec << endl;
    cout << "AtributeCoun : " << dec << classloader.getAttriCount() << endl;

    /* std::vector<CpInfo*> a = classloader.getConstPool();
    cout << "top" << endl;
    cout << a.size() << endl;
    for(int i = 0; i < a.size(); i++) {
      cout <<setw(2) << setfill('0') << i << " : " << setw(2) << setfill('0') << a[i]->UTF8.length << " : " << setw(2) << setfill('0') << a[i]->UTF8.bytes.size() << endl;

    } */
    fclose(fp);
}