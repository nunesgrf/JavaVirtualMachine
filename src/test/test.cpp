#ifndef TEST

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
    /* Print de infomações genericas do .class */

    cout << "General information \n\n\n";
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
    /*Fim do Print de infomações genericas do .class */
    /* Print do vetor de constant pool */
    vector<CpInfo*> a = classloader.getConstPool();
    cout << "top" << endl << endl;
    cout << a.size() << endl;
    for(int i = 0; i < a.size(); i++) {
      switch (a[i]->tag){
        case CONSTANT_Fieldref:
          cout << "Fieldref" <<endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = constantpool["  << a[i]->Fieldref.class_index <<"] ";
          cout <<"<" << a[a[a[i]->Fieldref.class_index-1]->Class.name_index -1]->UTF8.bytes << ">" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Name and Type = constantpool["  << a[i]->Fieldref.name_and_type_index<<"] "; 
          cout << "<" << a[a[a[i]->Fieldref.name_and_type_index-1]->NameAndType.name_index -1]->UTF8.bytes << " : " << a[a[a[i]->Fieldref.name_and_type_index-1]->NameAndType.descriptor_index -1]->UTF8.bytes << ">" << endl << endl;
          break;

        case CONSTANT_Methodref:
          cout << "Methodref" <<endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = constantpool["  << a[i]->Methodref.class_index  << "] ";
          cout <<"<" << a[a[a[i]->Methodref.class_index-1]->Class.name_index -1]->UTF8.bytes << ">" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0')  <<"Name and Type = constantpool[" << a[i]->Methodref.name_and_type_index  << "] ";
          cout << "<" << a[a[a[i]->Methodref.name_and_type_index-1]->NameAndType.name_index -1]->UTF8.bytes << " : " << a[a[a[i]->Methodref.name_and_type_index-1]->NameAndType.descriptor_index -1]->UTF8.bytes << ">" << endl << endl;
          break;

        case CONSTANT_NameAndType:
          cout << "NameAndType" <<endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Name = constantpool["  << a[i]->NameAndType.name_index <<"] " ;
          cout << "<"<< a[a[i]->NameAndType.name_index-1]->UTF8.bytes <<  ">"  <<endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Descriptor = constantpool["  << a[i]->NameAndType.descriptor_index <<"] " ;
          cout << "<"<< a[a[i]->NameAndType.descriptor_index-1]->UTF8.bytes <<  ">" << endl << endl;
          break;

        case CONSTANT_String:
          cout << "String" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = constantpool["  << a[i]->String.string_index << "] ";
          cout << "<"<< a[a[i]->String.string_index-1]->UTF8.bytes <<  ">"  <<endl << endl;
          break;

        case CONSTANT_Utf8:
          cout << "UTF8" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Length of byte array = "  << a[i]->UTF8.length <<endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = "  << a[i]->UTF8.bytes << endl << endl;
          // cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = "  << a[i]->UTF8.length << endl<<endl;
          break;

        case CONSTANT_Class:
          cout << "Class" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = "  << a[i]->Class.name_index ;
          cout << " <"<< a[a[i]->Class.name_index-1]->UTF8.bytes <<  ">"  <<endl << endl;
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
    /* Fim do Print do vetor de constant pool */
    /* Print do vetor de interfaces */
    std::vector<InterfaceInfo*> interfaces = classloader.getInterfaces();
    cout << "Interfaces" << endl << endl;
    for (int j = 0; j < interfaces.size(); j++) {
        cout << x.getUTF8(classloader.getConstPool(), interfaces[j]->interface_table);

    }
    /* Fim do Print do vetor de interfaces */
    /* Print do vetor de atributos */
    vector<AttributeInfo *> attributes = classloader.getAttributes();
    cout << "Attributes" << endl << endl;
    for (int k = 0 ; k < attributes.size(); k++) {

    }

    /* Printar o method */
    cout << "Method" << endl << endl;
    vector<MethodInfo *> methods = classloader.getMethods();
    int countMethod = methods.size();
    for(int i=0;i<countMethod;i++){
      cout << "[" << i << "]" << endl;
      cout <<"Name = constantpool[" << methods[i]->name_index<<"] " <<"<"<<a[methods[i]->name_index-1]->UTF8.bytes<<">"<< endl;
      cout <<"Descriptor = constantpool[" << methods[i]->descriptor_index<<"] "<< "<"<< a[methods[i]->descriptor_index-1]->UTF8.bytes<<">"<<endl;
      cout <<"Access flag = " << "0x" << setw(4) << setfill('0') << methods[i]->access_flags <<endl;
      //cout << "atributos = " << attributes[i] << endl; 
    }
     

    /* Fim do print method */
    fclose(fp);
}

#endif 