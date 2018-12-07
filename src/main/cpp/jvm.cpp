/** @file jvm.cpp
    @brief Arquivo inicial em que é selecionado ou a exibição do leitor ou do interpretador dependendo da chamada de argumentos;

*/

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

/**
* @brief Verifica qual o access_flag
* @param flag_byte É enviado o valor da flag
* @param parametro verificar se a flag 32 é classloader ou method
* @return flag_name É retornado o nome para a flag setada
*/

string Flag_names(int flag_byte, int parametro){
  string flag_name;
  switch(flag_byte) {
    case 0: flag_name = "";
      break;
    case 1: flag_name = "[public]";
      break;
    case 33: flag_name = "[public]";
      break;
    case 9: flag_name = "[public static]";
      break;
    case 2: flag_name = "[private]";
      break;
    case 4: flag_name = "[protect]";
      break;
    case 8: flag_name = "[static]";
      break;
    case 16: flag_name = "[final]";
      break;
    case 32: 
      if(parametro == 1){
        flag_name = "[super]";
      }
      else{
        flag_name = "[synchronized]";
      }
      break;
    case 512: flag_name = "[interface]";
      break;
    case 1024: flag_name = "[abstract]";
      break;
    case 64:  flag_name = "[bridge]";
      break;
    case 128: flag_name = "[varargs]";
      break;
    case 256: flag_name = "[native]";
      break;
    case 2048: flag_name = "[strict]";
      break;
    case 4096: flag_name = "[synthetic]";
      break;      
  }
  return flag_name;
}

/** 
 * @brief Chama o exibidor de ClassLoader
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
    std::cout << "AcessFlag    : "<< "0x" << setw(4) << setfill('0') << hex << classloader.getFlag() << Flag_names(classloader.getFlag(), 1) << endl;
    std::cout << "ThisClass    : " <<"cp info #" << dec << classloader.getThisClass() <<" <"<<x.getUTF8(classloader.getConstPool(),classloader.getThisClass()-1)<<">"<< endl;
    std::cout << "SuperClass   : " <<"cp info #" << dec << classloader.getSuper() <<" <"<<x.getUTF8(classloader.getConstPool(),classloader.getSuper()-1)<<">"<< endl;
    std::cout << "InterfaceCou : " << dec << classloader.getInterCounter() << endl;
    std::cout << "FieldsCount  : " << dec << classloader.getFieldCount() << dec << endl;
    std::cout << "MethodCount  : " << dec << classloader.getMethoCount() << dec << endl;
    std::cout << "AtributeCoun : " << dec << classloader.getAttriCount() << endl;
    /*Fim do Print de infomações genericas do .class */
    /* Print do vetor de constant pool */
    
    std::cout << "------------------------------ConstantPool------------------------------ \n\n\n";
    for(unsigned i = 0; i < a.size(); i++) {
      switch (a[i]->tag){
        case CONSTANT_Fieldref:
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] " << "CONSTANT_Fieldref_info" <<endl;
          std::cout << "\tClass name: cp info #"  << a[i]->Fieldref.class_index;
          std::cout <<" <" << a[a[a[i]->Fieldref.class_index-1]->Class.name_index -1]->UTF8.bytes << ">" << endl;
          std::cout <<"\tName and Type: cp info #"  << a[i]->Fieldref.name_and_type_index;
          std::cout << " <" << a[a[a[i]->Fieldref.name_and_type_index-1]->NameAndType.name_index -1]->UTF8.bytes << " : " << a[a[a[i]->Fieldref.name_and_type_index-1]->NameAndType.descriptor_index -1]->UTF8.bytes << ">" << endl << endl;
          break;

        case CONSTANT_Methodref:
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] " << "CONSTANT_Methodref_info" <<endl;
          std::cout << "\tClass name: cp info #"  << a[i]->Methodref.class_index ;
          std::cout <<" <" << a[a[a[i]->Methodref.class_index-1]->Class.name_index -1]->UTF8.bytes << ">" << endl;
          std::cout <<"\tName and Type: cp info #" << a[i]->Methodref.name_and_type_index ;
          std::cout << " <" << a[a[a[i]->Methodref.name_and_type_index-1]->NameAndType.name_index -1]->UTF8.bytes << " : " << a[a[a[i]->Methodref.name_and_type_index-1]->NameAndType.descriptor_index -1]->UTF8.bytes << ">" << endl << endl;
          break;

        case CONSTANT_NameAndType:
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] " << "CONSTANT_NameAndType_info" <<endl;
          std::cout << "\tName: cp info #"  << a[i]->NameAndType.name_index ;
          std::cout << " <"<< a[a[i]->NameAndType.name_index-1]->UTF8.bytes <<  ">"  <<endl;
          std::cout << "\tDescriptor: cp info #"  << a[i]->NameAndType.descriptor_index ;
          std::cout << " <"<< a[a[i]->NameAndType.descriptor_index-1]->UTF8.bytes <<  ">" << endl << endl;
          break;

        case CONSTANT_String:
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] "<< "CONSTANT_String_info" << endl;
          std::cout << "\tString: cp info #"  << a[i]->String.string_index;
          std::cout << " <"<< a[a[i]->String.string_index-1]->UTF8.bytes <<  ">"  <<endl << endl;
          break;

        case CONSTANT_Utf8:
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] "<< "CONSTANT_UTF8_info" << endl;
          std::cout << "\tLength of byte array: "  << a[i]->UTF8.length <<endl;
          std::cout << "\tLength of string: "  << a[i]->UTF8.length <<endl;
          std::cout << "\tString: "  << a[i]->UTF8.bytes << endl << endl;
          // std::cout << setw(2) << setfill('0') << i+1 << " : " << setw(2) << setfill('0') << "String = "  << a[i]->UTF8.length << endl<<endl;
          break;

        case CONSTANT_Class:
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] "<< "CONSTANT_Class_info" << endl;
          std::cout << "\tClass name: cp info #"  << a[i]->Class.name_index ;
          std::cout << " <"<< a[a[i]->Class.name_index-1]->UTF8.bytes <<  ">"  <<endl << endl;
          break;

        case CONSTANT_InterfaceMethodref:
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] " <<  "CONSTANT_InterfaceMethodref_info" << endl;
          std::cout << "\tClass name: "  << a[i]->InterfaceMethodref.class_index << endl;
          std::cout << "\tName and Type: "  << a[i]->InterfaceMethodref.name_and_type_index << endl << endl;
          break;

        case CONSTANT_Integer:
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] " <<  "CONSTANT_Integer_info" << endl;
          std::cout << "\tBytes: "  << a[i]->Integer.bytes << endl << endl;
          break;

        case CONSTANT_Float:
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] " << "CONTANT_Float_info" << endl;
          std::cout << "\tBytes: "  << a[i]->Float.bytes << endl << endl;
          break;

        case CONSTANT_Long:
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] " << "CONSTANT_Long_info" << endl;
          std::cout << "\tHigh Bytes: "  << a[i]->Long.high_bytes << endl;
          std::cout << "\tLow Bytes: "  << a[i]->Long.low_bytes << endl << endl;
          break;

        case CONSTANT_Double:
          double print_double; /* Transforma os bytes para double */
          uint64_t aux;
          aux = ((uint64_t)a[i]->Double.high_bytes << 32) | a[i]->Double.low_bytes;
          memcpy(&print_double, &aux, sizeof(double));
          std::cout << "["<< setw(2) << setfill('0') << i+1 << "] " << "CONSTANT_Double_info" << endl;
          std::cout << "\tHigh Bytes: 0x";
          printf("%x\n",a[i]->Double.high_bytes );
           
          std::cout << "\tLow Bytes: 0x";
          printf("%x\n",a[i]->Double.low_bytes );
          std::cout << "\tDouble: "  << print_double << endl << endl;
          break;

        case CONSTANT_Empty:
          std::cout <<"[" <<setw(2) << setfill('0') << i+1 << "] (large numeric continued)" << endl << endl;
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
      std::cout <<"Access flag = " << "0x" << setw(4) << setfill('0') << fieldInfo[i]->access_flags  << Flag_names(fieldInfo[i]->access_flags, 0) <<endl << endl;
    }
    if(countFields == 0 ){
      std::cout << "EMPTY" << endl;
    }


    /* Print do vetor de interfaces */
    std::vector<InterfaceInfo*> interfaces = classloader.getInterfaces();
    std::cout << "------------------------------Interfaces------------------------------" << endl << endl;
    for (unsigned j = 0; j < interfaces.size(); j++) {
        std::cout << x.getUTF8(classloader.getConstPool(), interfaces[j]->interface_table -1);
    }
    std::cout << "\nEMPTY" << endl;
    /* Fim do Print do vetor de interfaces */

    /* Printar o method */
    std::cout << "------------------------------Method------------------------------" << endl << endl;
    vector<MethodInfo *> methods = classloader.getMethods();
    int countMethod = methods.size();
    for(int i=0;i<countMethod;i++){
      std::cout << "\n[" << i << "]" << a[methods[i]->name_index-1]->UTF8.bytes << endl;
      std::cout <<"Name: cp info #" << methods[i]->name_index <<" <"<<a[methods[i]->name_index-1]->UTF8.bytes<<">"<< endl;
      std::cout <<"Descriptor: cp info #" << methods[i]->descriptor_index<<" <"<< a[methods[i]->descriptor_index-1]->UTF8.bytes<<">"<<endl;
      std::cout <<"Access flag: " << "0x" << setw(4) << setfill('0') << methods[i]->access_flags<< Flag_names(methods[i]->access_flags, 0) <<endl  << endl;
      for(int j=0;j<methods[i]->attributes_count; j++){
        methods[i]->attributes[j].print(a);
      }
    }


    /* Fim do print method */
    /* Print do vetor de attributes */
    vector<AttributeInfo *> attributes = classloader.getAttributes();
    std::cout << "\n------------------------------Attributes------------------------------" << endl << endl;
    for (unsigned k = 0 ; k < attributes.size(); k++) {
        std::cout << "[" << k << "]" ;
        attributes[k]->print(a);
    }

    /* Fim do print attributes */
    //fclose(fp);
  /**Finalizar o leitor*/
  exit(0);
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
        std::cout << ERROR_MESSAGE << "Necerrário(s) argumentos.\n" << std::endl;
        std::cout << "Ex leitor: ./jvm e path_to_class_file " << std::endl;
        std::cout << "Ex interpretador: ./jvm i path_to_class_file" << std::endl;
        return ERROR;
    }

    FILE * fp = fopen(argv[2],"rb");
    if (fp != NULL) {
    	
    	cout << endl << "Arquivo aberto" << endl;
    	ClassLoader classloader(fp);
    	getPath(argv[2]);
    	fclose(fp);
    	switch(*argv[1]) {
        	case 'i': classInterpreter(classloader); break;
        	case 'e': classReader(classloader);      break;
    	}
    	exit(0);
    	
	} else {
		cout << "Nao foi possivel abrir o arquivo" << endl;
		return -1;
	}
   
	exit(0);
}
