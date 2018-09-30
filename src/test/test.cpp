#include "../main/cpp/JavaClass.cpp"
#include "../main/cpp/ByteReader.cpp"
#include <stdio.h>
#include <iostream>
using namespace std;
int main() {

    FILE * fp = fopen("HelloWorld.class","r");
    
    JavaClass javaclass;

    cout << "Teste!" << endl;
    
    javaclass.setMagic(fp);
    javaclass.setMinor(fp);
    javaclass.setMajor(fp);

    cout << "MagicNumber  : " << hex << javaclass.getMagic() << endl;
    cout << "MinorVersion : " << dec << javaclass.getMinor() << endl;
    cout << "MajorVersion : " << dec << javaclass.getMajor() << endl;
}