#include "../main/cpp/JavaClass.cpp"
#include "../main/cpp/ByteReader.cpp"
#include "../main/cpp/CpInfo.cpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;
int main() {

    FILE * fp = fopen("HelloWorld.class","r");
    
    JavaClass javaclass(fp);

    cout << "MagicNumber  : " << hex << javaclass.getMagic() << endl;
    cout << "MinorVersion : " << dec << javaclass.getMinor() << endl;
    cout << "MajorVersion : " << dec << javaclass.getMajor() << endl;
    cout << "PoolCounter  : " << dec << javaclass.getConstCount() << endl;
    std::vector<CpInfo*> a = javaclass.getConstPool();
    cout << "top" << endl;
    cout << a.size() << endl;
    //for(int i = 0; i < a.size(); i++) {
      //  cout <<setw(2) << setfill('0') << i << " : " << setw(2) << setfill('0') << a[i]->UTF8.length << " : " << setw(2) << setfill('0') << a[i]->UTF8.bytes.size() << endl;

    //}
    fclose(fp);
}