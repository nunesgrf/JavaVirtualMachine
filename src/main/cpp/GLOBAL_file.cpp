#include "../hpp/GLOBAL_file.hpp"
#include "../hpp/CpAttributeInterface.hpp"

std::map<std::string, Instance*> MethodsArea::GLOBAL_staticClasses;
std::map<std::string, Instance*> MethodsArea::GLOBAL_loadedClasses;
std::string MethodsArea::path;
/** @brief Retorna uma variável de uma classe especifica.
 * @param className nome da classe @param varName nome da variável.
 * @return Operand*
 */
Operand * MethodsArea::getStaticfield(std::string className, std::string varName) {
    auto staticField = GLOBAL_staticClasses.at(className)->references->at(varName);
    return staticField;
}

MethodInfo * MethodsArea::findMethodByNameOrDeor(ClassLoader* classloader,std::string method_name,std::string method_desc){
    
    std::vector<MethodInfo*> methods = classloader->getMethods();
    std::vector<CpInfo*> constantPool = classloader->getConstPool();
    CpAttributeInterface cpAttrAux;
    for (int i = 0; i < classloader->getMethoCount(); i++) {
        std::string searched_method_name = cpAttrAux.getUTF8(constantPool,methods.at(i)->name_index-1);

        if (searched_method_name == method_name) {
            std::string searched_desc_name = cpAttrAux.getUTF8(constantPool,methods.at(i)->descriptor_index-1);
            if (searched_desc_name == method_desc) return methods.at(i);
        }
    }
  std::cout << "Método nao encontrado, ponteiro retornado nulo";
  getchar();
  return NULL;
}

Operand * MethodsArea::copyOperand(Operand * toCopy) {

    Operand * toReturn = (Operand*)calloc(1,sizeof(Operand));

    std::cout << "copyOperand : " << toCopy->class_instance->classe->getConstPool().size() << std::endl;
    toReturn->tag = toCopy->tag;
    
    switch((int)toCopy->tag) {
        case CONSTANT_Byte:
            toReturn->type_byte = toCopy->type_byte; break;
        case CONSTANT_Char:
            toReturn->type_char = toCopy->type_char; break;
        case CONSTANT_Short:
            toReturn->type_short = toCopy->type_short; break;
        case CONSTANT_Integer:
            toReturn->type_int = toCopy->type_int; break;
        case CONSTANT_Float:
            toReturn->type_float = toCopy->type_float; break;
        case CONSTANT_Long:
            toReturn->type_long = toCopy->type_long; break;
        case CONSTANT_Double:
            toReturn->type_double = toCopy->type_double; break;
        case CONSTANT_String:
            toReturn->type_string = new std::string(*toCopy->type_string); break;
        case CONSTANT_Class:
            
            toReturn->class_instance = (Instance*)calloc(1,sizeof(Instance));
            toReturn->class_instance->name = toCopy->class_instance->name;
            
            //toReturn->class_instance->classe = (ClassLoader*)calloc(1,sizeof(ClassLoader));
            //toReturn->class_instance->classe = new ClassLoader()

            toReturn->class_instance->classe = toCopy->class_instance->classe;
            std::cout << toReturn->class_instance->classe->getConstPool().size() << std::endl;
            getchar();
            
            toReturn->class_instance->references = new std::map<std::string, Operand*>();
            toReturn->class_instance->references = toCopy->class_instance->references;
            std::cout << "Entrei aqui" << std::endl;
            getchar();
            break;

        case CONSTANT_Array:

            toReturn->array_type = (ArrayType*)malloc(sizeof(ArrayType));
            toReturn->array_type->array = new std::vector<Operand*>();
            for (int i=0; (unsigned)i < toCopy->array_type->array->size(); i++) {
                Operand* aux = toCopy->array_type->array->at(i);
                Operand *value = MethodsArea::copyOperand(aux);
                toReturn->array_type->array->emplace_back(value);
            }
            break;
    }
    return toReturn;
}