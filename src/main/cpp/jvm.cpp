#include <iostream>
#include <iomanip>
#include <fstream>

#include "../hpp/ClassLoader.hpp"
#include "../hpp/AttributeInfo.hpp"
#include "../hpp/CpInfo.hpp"
#include "../hpp/CpAttributeInterface.hpp"
#include "../hpp/Interpreter.hpp"
#include "../hpp/GLOBAL_file.hpp"

#define ERROR -1
#define ERROR_MESSAGE " Mensagem de erro: "

/** @brief Chama o interpretador para classloader.
 * @param classloader Instancia da classe ClassLoader.
 * @return void
 */
void classInterpreter(ClassLoader classloader) {
    Interpreter engine;
    engine.execute(&classloader);
}

/** @brief Exibidor de ClassLoader
 * @param classloader Instancia da classe ClassLoader
 * @return void
 */
void classReader(ClassLoader classloader) {
    CpAttributeInterface x;
    vector<CpInfo*> a = classloader.getConstPool();
    /* Print de infomações genericas do .class */

    std::cout << "------------------------------General information------------------------------ \n\n\n";
    std::cout << "MagicNumber  : " << hex << classloader.getMagic() << endl;
    std::cout << "MinorVersion : " << dec << classloader.getMinor() << endl;
    std::cout << "MajorVersion : " << dec << classloader.getMajor() << endl;
    std::cout << "PoolCounter  : " << dec << classloader.getConstCount() << endl;
    std::cout << "AcessFlag    : "<< "0x" << setw(4) << setfill('0') << hex << classloader.getFlag() << endl;
    std::cout << "ThisClass    : " <<"constantpool[" << dec << classloader.getThisClass()<<"] " <<"<"<<x.getUTF8(classloader.getConstPool(),classloader.getThisClass()-1)<<">"<< endl;
    std::cout << "SuperClass   : " <<"constantpool[" << dec << classloader.getSuper()<<"] " <<"<"<<x.getUTF8(classloader.getConstPool(),classloader.getSuper()-1)<<">"<< endl;
    std::cout << "InterfaceCou : " << dec << classloader.getInterCounter() << endl;
    std::cout << "FieldsCount  : " << dec << classloader.getFieldCount() << dec << endl;
    std::cout << "MethodCount  : " << dec << classloader.getMethoCount() << dec << endl;
    std::cout << "AtributeCoun : " << dec << classloader.getAttriCount() << endl;
    /*Fim do Print de infomações genericas do .class */
    /* Print do vetor de constant pool */
    std::cout << "------------------------------ConstantPool------------------------------ \n\n\n";
    for(int i = 0; i < a.size(); i++) {
      switch (a[i]->tag){
        case CONSTANT_Fieldref:
          std::cout << "Fieldref" <<endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = constantpool["  << a[i]->Fieldref.class_index <<"] ";
          std::cout <<"<" << a[a[a[i]->Fieldref.class_index-1]->Class.name_index -1]->UTF8.bytes << ">" << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Name and Type = constantpool["  << a[i]->Fieldref.name_and_type_index<<"] ";
          std::cout << "<" << a[a[a[i]->Fieldref.name_and_type_index-1]->NameAndType.name_index -1]->UTF8.bytes << " : " << a[a[a[i]->Fieldref.name_and_type_index-1]->NameAndType.descriptor_index -1]->UTF8.bytes << ">" << endl << endl;
          break;

        case CONSTANT_Methodref:
          std::cout << "Methodref" <<endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = constantpool["  << a[i]->Methodref.class_index  << "] ";
          std::cout <<"<" << a[a[a[i]->Methodref.class_index-1]->Class.name_index -1]->UTF8.bytes << ">" << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0')  <<"Name and Type = constantpool[" << a[i]->Methodref.name_and_type_index  << "] ";
          std::cout << "<" << a[a[a[i]->Methodref.name_and_type_index-1]->NameAndType.name_index -1]->UTF8.bytes << " : " << a[a[a[i]->Methodref.name_and_type_index-1]->NameAndType.descriptor_index -1]->UTF8.bytes << ">" << endl << endl;
          break;

        case CONSTANT_NameAndType:
          std::cout << "NameAndType" <<endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Name = constantpool["  << a[i]->NameAndType.name_index <<"] " ;
          std::cout << "<"<< a[a[i]->NameAndType.name_index-1]->UTF8.bytes <<  ">"  <<endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Descriptor = constantpool["  << a[i]->NameAndType.descriptor_index <<"] " ;
          std::cout << "<"<< a[a[i]->NameAndType.descriptor_index-1]->UTF8.bytes <<  ">" << endl << endl;
          break;

        case CONSTANT_String:
          std::cout << "String" << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = constantpool["  << a[i]->String.string_index << "] ";
          std::cout << "<"<< a[a[i]->String.string_index-1]->UTF8.bytes <<  ">"  <<endl << endl;
          break;

        case CONSTANT_Utf8:
          std::cout << "UTF8" << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Length of byte array = "  << a[i]->UTF8.length <<endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = "  << a[i]->UTF8.bytes << endl << endl;
          // std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = "  << a[i]->UTF8.length << endl<<endl;
          break;

        case CONSTANT_Class:
          std::cout << "Class" << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = "  << a[i]->Class.name_index ;
          std::cout << " <"<< a[a[i]->Class.name_index-1]->UTF8.bytes <<  ">"  <<endl << endl;
          break;

        case CONSTANT_InterfaceMethodref:
          std::cout <<  "InterfaceMethodref" << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Class name = "  << a[i]->InterfaceMethodref.class_index << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Name and Type = "  << a[i]->InterfaceMethodref.name_and_type_index << endl << endl;
          break;

        case CONSTANT_Integer:
          std::cout <<  "Integer" << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Bytes = "  << a[i]->Integer.bytes << endl << endl;
          break;

        case CONSTANT_Float:
          std::cout << "Float" << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Bytes = "  << a[i]->Float.bytes << endl << endl;
          break;

        case CONSTANT_Long:
          std::cout << "Long" << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "High Bytes = "  << a[i]->Long.high_bytes << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Low Bytes = "  << a[i]->Long.low_bytes << endl << endl;
          break;

        case CONSTANT_Double:
          double print_double; /* Transforma os bytes para double */
          uint64_t aux;
          aux = ((uint64_t)a[i]->Double.high_bytes << 32) | a[i]->Double.low_bytes;
          memcpy(&print_double, &aux, sizeof(double));
          std::cout << "Double" << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "High Bytes = "  << a[i]->Double.high_bytes << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Low Bytes = "  << a[i]->Double.low_bytes << endl;
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "Double = "  << print_double << endl << endl;
          break;

        case CONSTANT_Empty:
          std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "(large numeric continued)" << endl << endl;
          break;
        default :
          std::cout << "Non-type" << endl;
      }
    }
    /* Fim do Print do vetor de constant pool */
    std::cout << "------------------------------Fields------------------------------" << endl << endl;

    vector<FieldInfo *> fieldInfo = classloader.getFields();
    int countFields = fieldInfo.size();
    for(int i=0;i<countFields;i++){
      std::cout << "[" << i << "]" << endl;
      std::cout <<"Name = constantpool[" << fieldInfo[i]->name_index<<"] " <<"<"<<a[fieldInfo[i]->name_index-1]->UTF8.bytes<<">"<< endl;
      std::cout <<"Descriptor = constantpool[" << fieldInfo[i]->descriptor_index<<"] "<< "<"<< a[fieldInfo[i]->descriptor_index-1]->UTF8.bytes<<">"<<endl;
      std::cout <<"Access flag = " << "0x" << setw(4) << setfill('0') << fieldInfo[i]->access_flags <<endl << endl;
    }
    if(countFields == 0 ){
      std::cout << "EMPTY" << endl;
    }


    /* Print do vetor de interfaces */
    std::vector<InterfaceInfo*> interfaces = classloader.getInterfaces();
    std::cout << "------------------------------Interfaces------------------------------" << endl << endl;
    for (int j = 0; j < interfaces.size(); j++) {
        std::cout << x.getUTF8(classloader.getConstPool(), interfaces[j]->interface_table -1);
    }
    std::cout << "\nEMPTY" << endl;
    /* Fim do Print do vetor de interfaces */

    /* Printar o method */
    std::cout << "------------------------------Method------------------------------" << endl << endl;
    vector<MethodInfo *> methods = classloader.getMethods();
    int countMethod = methods.size();
    for(int i=0;i<countMethod;i++){
      std::cout << "[" << i << "]" << endl;
      std::cout <<"Name = constantpool[" << methods[i]->name_index<<"] " <<"<"<<a[methods[i]->name_index-1]->UTF8.bytes<<">"<< endl;
      std::cout <<"Descriptor = constantpool[" << methods[i]->descriptor_index<<"] "<< "<"<< a[methods[i]->descriptor_index-1]->UTF8.bytes<<">"<<endl;
      std::cout <<"Access flag = " << "0x" << setw(4) << setfill('0') << methods[i]->access_flags <<endl << endl;
      for(int j=0;j<methods[i]->attributes_count; j++){
        methods[i]->attributes[j].print(a);
      }
    }


    /* Fim do print method */
    /* Print do vetor de attributes */
    vector<AttributeInfo *> attributes = classloader.getAttributes();
    std::cout << "\n------------------------------Attributes------------------------------" << endl << endl;
    for (int k = 0 ; k < attributes.size(); k++) {
        std::cout << "[" << k << "]" << endl;
        attributes[k]->print(a);
    }

    /* Fim do print attributes */
    //fclose(fp);
}

/**
 * @brief isola o path de entrada e armazena em uma classe estática para que possa ser acessado posteriormente.
 * @param *toConvert ponteiro para string.
 * @return void
 */
void getPath(char * toConvert) {

    MethodsArea dump;
    std::string toEdit(toConvert);

    auto pos  = toEdit.find_last_of('/');
    auto path = toEdit.substr(0,pos+1);
    
    dump.path = path;
}
/** @brief Função main
 * @param argc contador de argumentos @param *argv[] argumentos do tipo texto
 * @return void
 */
int main(int argc, char * argv[]) {
 
    if(argc != 3) {
        std::cout << ERROR_MESSAGE << "Necerrário(s) 3 argumentos." << std::endl;
        return ERROR;
    }

    FILE * fp = fopen(argv[2],"r");
    ClassLoader classloader(fp);
    
    getPath(argv[2]);
    fclose(fp);

    switch(*argv[1]) {
        case 'i': classInterpreter(classloader); break;
        case 'e': classReader(classloader);      break;
    }
}