#ifndef TEST

#include "../main/hpp/ClassLoader.hpp"
#include "../main/hpp/ByteReader.hpp"
#include "../main/hpp/CpInfo.hpp"
#include "../main/hpp/CpAttributeInterface.hpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>

using namespace std;


int main() {
    FILE *fp;

    int file_wish=0;
    while(file_wish ==0 ){

      cout << "------------------------------MENU------------------------------ \n\n\n";

      cout << "Escolha o arquivo que deseja : " << endl << endl;
      cout << "1 - HelloWorld.class" << endl;
      cout << "2 - Fibonacci.class" << endl;
      cout << "3 - AttributeInstDemo.class" << endl;
      cout << "4 - HarmonicSeries.class" << endl;
      cin >> file_wish ;

      switch(file_wish){
        case 1 :   fp  =fopen("HelloWorld.class","r"); break;
        case 2 :   fp =fopen("Fibonacci.class","r"); break;
        case 3 :   fp  =fopen("AttributeInstDemo.class","r"); break;
        case 4 :   fp = fopen("HarmonicSeries.class","r"); break;
        default : file_wish = 0 ; break;
      }
    }

    ClassLoader classloader(fp);
    CpAttributeInterface x;
    vector<CpInfo*> a = classloader.getConstPool();
    /* Print de infomações genericas do .class */

    cout << "------------------------------General information------------------------------ \n\n\n";
    cout << "MagicNumber  : " << hex << classloader.getMagic() << endl;
    cout << "MinorVersion : " << dec << classloader.getMinor() << endl;
    cout << "MajorVersion : " << dec << classloader.getMajor() << endl;
    cout << "PoolCounter  : " << dec << classloader.getConstCount() << endl;
    cout << "AcessFlag    : "<< "0x" << setw(4) << setfill('0') << hex << classloader.getFlag() << endl;
    cout << "ThisClass    : " <<"constantpool[" << dec << classloader.getThisClass()<<"] " <<"<"<<x.getUTF8(classloader.getConstPool(),classloader.getThisClass()-1)<<">"<< endl;
    cout << "SuperClass   : " <<"constantpool[" << dec << classloader.getSuper()<<"] " <<"<"<<x.getUTF8(classloader.getConstPool(),classloader.getSuper()-1)<<">"<< endl;
    cout << "InterfaceCou : " << dec << classloader.getInterCounter() << endl;
    cout << "FieldsCount  : " << dec << classloader.getFieldCount() << dec << endl;
    cout << "MethodCount  : " << dec << classloader.getMethoCount() << dec << endl;
    cout << "AtributeCoun : " << dec << classloader.getAttriCount() << endl;
    /*Fim do Print de infomações genericas do .class */
    /* Print do vetor de constant pool */
    cout << "------------------------------ConstantPool------------------------------ \n\n\n";
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
          double print_double; /* Transforma os bytes para double */
          uint64_t aux;
          aux = ((uint64_t)a[i]->Double.high_bytes << 32) | a[i]->Double.low_bytes;
          memcpy(&print_double, &aux, sizeof(double));
          cout << "Double" << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "High Bytes = "  << a[i]->Double.high_bytes << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Low Bytes = "  << a[i]->Double.low_bytes << endl;
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Double = "  << print_double << endl << endl;
          break;

        case CONSTANT_Empty:
          cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "(large numeric continued)" << endl << endl;
          break;
        default :
          cout << "Non-type" << endl;
      }
    }
    /* Fim do Print do vetor de constant pool */
    cout << "------------------------------Fields------------------------------" << endl << endl;

    vector<FieldInfo *> fieldInfo = classloader.getFields();
    int countFields = fieldInfo.size();
    for(int i=0;i<countFields;i++){
      cout << "[" << i << "]" << endl;
      cout <<"Name = constantpool[" << fieldInfo[i]->name_index<<"] " <<"<"<<a[fieldInfo[i]->name_index-1]->UTF8.bytes<<">"<< endl;
      cout <<"Descriptor = constantpool[" << fieldInfo[i]->descriptor_index<<"] "<< "<"<< a[fieldInfo[i]->descriptor_index-1]->UTF8.bytes<<">"<<endl;
      cout <<"Access flag = " << "0x" << setw(4) << setfill('0') << fieldInfo[i]->access_flags <<endl << endl;
    }
    if(countFields == 0 ){
      cout << "EMPTY" << endl;
    }


    /* Print do vetor de interfaces */
    std::vector<InterfaceInfo*> interfaces = classloader.getInterfaces();
    cout << "------------------------------Interfaces------------------------------" << endl << endl;
    for (int j = 0; j < interfaces.size(); j++) {
        cout << x.getUTF8(classloader.getConstPool(), interfaces[j]->interface_table -1);
    }
    cout << "\nEMPTY" << endl;
    /* Fim do Print do vetor de interfaces */

    /* Printar o method */
    cout << "------------------------------Method------------------------------" << endl << endl;
    vector<MethodInfo *> methods = classloader.getMethods();
    int countMethod = methods.size();
    for(int i=0;i<countMethod;i++){
      cout << "[" << i << "]" << endl;
      cout <<"Name = constantpool[" << methods[i]->name_index<<"] " <<"<"<<a[methods[i]->name_index-1]->UTF8.bytes<<">"<< endl;
      cout <<"Descriptor = constantpool[" << methods[i]->descriptor_index<<"] "<< "<"<< a[methods[i]->descriptor_index-1]->UTF8.bytes<<">"<<endl;
      cout <<"Access flag = " << "0x" << setw(4) << setfill('0') << methods[i]->access_flags <<endl << endl;
      for(int j=0;j<methods[i]->attributes_count; j++){
        methods[i]->attributes[j].print(a);
      }
    }


    /* Fim do print method */
    /* Print do vetor de attributes */
    vector<AttributeInfo *> attributes = classloader.getAttributes();
    cout << "\n------------------------------Attributes------------------------------" << endl << endl;
    for (int k = 0 ; k < attributes.size(); k++) {
        cout << "[" << k << "]" << endl;
        attributes[k]->print(a);
    }

    /* Fim do print attributes */
    fclose(fp);
}

#endif
