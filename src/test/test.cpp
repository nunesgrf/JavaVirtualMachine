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

    std::vector<CpInfo*> a = classloader.getConstPool();
    cout << "top" << endl << endl;
    cout << a.size() << endl;
    for(int i = 0; i < a.size(); i++) {
      switch (a[i]->tag){
        case CONSTANT_Fieldref: 
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = constantpool["  << a[i]->Fieldref.class_index <<"]" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Name and Type = constantpool["  << a[i]->Fieldref.name_and_type_index<<"]"  << endl <<endl;
          break;

        case CONSTANT_Methodref:
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = constantpool["  << a[i]->Methodref.class_index  << "]"<< endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0')  <<"Name and Type = contantpool[" << a[i]->Methodref.name_and_type_index  << "]"<< endl << endl;
          break;

        case CONSTANT_NameAndType:
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Name = contantpool["  << a[i]->NameAndType.name_index <<"]" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Descriptor = contantpool["  << a[i]->NameAndType.descriptor_index <<"]" << endl << endl;
          break;
        case CONSTANT_String:
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = contantpool["  << a[i]->String.string_index << "]" << endl<<endl;
          break;

        case CONSTANT_Utf8:
          cout << "UTF8" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Length of byte array = "  << a[i]->UTF8.length <<endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = "  << a[i]->UTF8.bytes << endl;
          // cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = "  << a[i]->UTF8.length << endl<<endl;
            
        default :
          cout << "fodase" << endl;

      }
      
    }
    fclose(fp);
}