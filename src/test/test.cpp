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
          cout << "Fieldref" <<endl;   
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = constantpool["  << a[i]->Fieldref.class_index <<"]" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Name and Type = constantpool["  << a[i]->Fieldref.name_and_type_index<<"]"  << endl <<endl;
          break;

        case CONSTANT_Methodref:
          cout << "Methodref" <<endl;   
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = constantpool["  << a[i]->Methodref.class_index  << "]"<< endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0')  <<"Name and Type = constantpool[" << a[i]->Methodref.name_and_type_index  << "]"<< endl << endl;
          break;

        case CONSTANT_NameAndType:
          cout << "NameAndType" <<endl;   
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Name = constantpool["  << a[i]->NameAndType.name_index <<"]" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Descriptor = constantpool["  << a[i]->NameAndType.descriptor_index <<"]" << endl << endl;
          break;
        case CONSTANT_String:
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = constantpool["  << a[i]->String.string_index << "]" << endl<<endl;
          break;

        case CONSTANT_Utf8:
          cout << "UTF8" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Length of byte array = "  << a[i]->UTF8.length <<endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = "  << a[i]->UTF8.bytes << endl << endl;
          // cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = "  << a[i]->UTF8.length << endl<<endl;
          break;

        case CONSTANT_Class:
          cout << "Class" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = "  << a[i]->Class.name_index << endl << endl;
          break;
        
        case CONSTANT_InterfaceMethodref:
          cout <<  "InterfaceMethodref" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = "  << a[i]->InterfaceMethodref.class_index << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Name and Type = "  << a[i]->InterfaceMethodref.name_and_type_index << endl << endl;
          break;

        case CONSTANT_Integer:
          cout <<  "Integer" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Bytes = "  << a[i]->Integer.bytes << endl << endl;
          break;

        case CONSTANT_Float:
          cout << "Float" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Bytes = "  << a[i]->Float.bytes << endl << endl;
          break;
        
        case CONSTANT_Long:
          cout << "Long" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "High Bytes = "  << a[i]->Long.high_bytes << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Low Bytes = "  << a[i]->Long.low_bytes << endl << endl;
          break;

        case CONSTANT_Double:
          cout << "Double" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "High Bytes = "  << a[i]->Double.high_bytes << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Low Bytes = "  << a[i]->Double.low_bytes << endl << endl;
          break;  

        default :
          cout << "Non-type" << endl;
      }
      
    }
    fclose(fp);
}