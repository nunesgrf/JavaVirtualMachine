/** @file Interpreter.cpp
    @brief Métodos que fazem a atuação do interpretador;

*/
#include "../hpp/Interpreter.hpp"
#include "../hpp/CpAttributeInterface.hpp"
#include "GLOBAL_file.hpp"
#include <stdio.h>
#include <iostream>
#include <string>

/** @class Interpreter::execute
 *   @brief Carrega o classLoader na memória e na stack e executa-o.
 * @param *classloader ponteiro para ClassLoader
 * @return void
 */

std::stack<Frame*> Interpreter::frame_stack;

void Interpreter::execute(ClassLoader * classloader) {

    Interpreter::loadInMemo(classloader);
    Frame toRun(classloader->getConstPool(),this->mainFinder(classloader));
    Interpreter::frame_stack.push(&toRun);

    while(!Interpreter::frame_stack.empty()) {

        
        Interpreter::frame_stack.top()->run();
    }
}

/** @class Interpreter::getClassInfo
 *   @brief Acessa a memória em busca de uma classe, caso inexistente, carrega a classe buscada em memória.
 * @param className nome da classe buscada.
 * @return void
 */
ClassLoader * Interpreter::getClassInfo(std::string className) {

    MethodsArea container;

    
    Instance * instance = container.GLOBAL_staticClasses[className];

    if(instance == NULL) {
        
        FILE * fp = fopen((container.path + className + ".class").c_str(),"r");
        ClassLoader new_class(fp);
        
        instance = Interpreter::loadInMemo(&new_class);
        
        fclose(fp);
    }
    
    ;
    return instance->classe;
}

/** @class Interpreter::createType
  *  @brief Cria um operando settado em um tipo específico.
 * @param type string contendo a informação de qual novo tipo.
 * @return Operand*
 */
Operand * Interpreter::createType(std::string type) {

    char toSwitch      = type[0];
    Operand * toReturn = (Operand*)calloc(1,sizeof(Operand));

    switch(toSwitch) {
        case 'I':
            toReturn->tag = CONSTANT_Integer; break;
        case 'F':
            toReturn->tag = CONSTANT_Float; break;
        case 'J':
            toReturn->tag = CONSTANT_Long; break;
        case 'D':
            toReturn->tag = CONSTANT_Double; break;
        case 'Z':
            toReturn->tag = CONSTANT_Boolean; break;
        case 'B':
            toReturn->tag = CONSTANT_Byte; break;
        case 'C':
            toReturn->tag = CONSTANT_Char; break;
        case 'S':
            toReturn->tag = CONSTANT_Short; break;
        case '[':
            toReturn->tag = CONSTANT_Array;
            toReturn->array_type = (ArrayType*)calloc(1,sizeof(ArrayType));
            toReturn->array_type->array = new std::vector<Operand*>(); break;
        case 'P':
            toReturn->tag = CONSTANT_Empty; break;
        case CONSTANT_String:
            toReturn->tag = CONSTANT_String;
            toReturn->type_string = new std::string(""); break;
        case 'L':
            if(type == "Ljava/lang/String;") {
                toReturn->tag = CONSTANT_String;
                toReturn->type_string = new std::string("");
            }
            else {
                toReturn->tag = CONSTANT_Class;
                toReturn->class_instance = (Instance*)calloc(1,sizeof(Instance));

                std::string className  = type.substr(1,type.size());
                
                
                ClassLoader * newClass = Interpreter::getClassInfo(className);
                
                toReturn->class_instance->classe = newClass;
                toReturn->class_instance->name   = className;

                //Interpreter::loadVariables(toReturn->class_instance); // Essa linha miserável.
            }
            break;
    }
    return toReturn;
}

/** @class Interpreter::loadVariables
  *  @brief Carrega em memórias as variáveis de uma classe.
 * @param *instance ponteiro para Instance.
 * @return void
 */
void Interpreter::loadVariables(Instance * instance) {
    
    instance->references = new std::map<std::string, Operand*>();

    CpAttributeInterface cpAt;
    auto currClass = instance->classe;
    auto superInfo = currClass->getConstPool()[currClass->getSuper()-1];
    auto superClassName = cpAt.getUTF8(currClass->getConstPool(),superInfo->Class.name_index-1);

    do {

        superClassName = cpAt.getUTF8(currClass->getConstPool(),currClass->getSuper()-1);

        for(auto fpointer : currClass->getFields()) {
            std::string nameField = cpAt.getUTF8(currClass->getConstPool(),fpointer->name_index-1);
            
            std::string typeVariable = cpAt.getUTF8(currClass->getConstPool(),fpointer->descriptor_index-1);
            instance->references->operator[](nameField) = Interpreter::createType(typeVariable);
        }
            
        if(superClassName != "java/lang/Object" && superClassName != "") currClass = Interpreter::getClassInfo(superClassName); //IMPLEMENTAR O MÉTODO getClassInfo();

                

    } while(superClassName != "java/lang/Object");
    
}

/** @class Interpreter::loadInMemo
 * @brief Realiza o carregamento do ClassLoader em memória e retorna a instância.
 * @param *javaclass ponteiro de ClassLoader
 * @return Instance*
 */
Instance * Interpreter::loadInMemo(ClassLoader * javaclass) {

    MethodsArea dump;
    Instance * inst_LC = new Instance(javaclass);
    Instance * inst_SC = new Instance(javaclass);

    dump.GLOBAL_loadedClasses.insert(std::pair<std::string, Instance*>(inst_LC->name,inst_LC));
    dump.GLOBAL_staticClasses.insert(std::pair<std::string, Instance*>(inst_SC->name,inst_SC));

    
    Interpreter::loadVariables(inst_LC);
    
    return inst_LC;
}

/** @class Interpreter::mainFinder
 * @brief Busca a main de uma javaclass e retorna o método.
 * @param *javaclass ponteiro para ClassLoader.
 * @return MethodInfo*
 */
MethodInfo * Interpreter::mainFinder(ClassLoader * javaclass) {


    auto methods = javaclass->getMethods();
    CpAttributeInterface CpAtAux;

    for(auto m : methods) {

        std::string descriptor = "";
        std::string name = CpAtAux.getUTF8(javaclass->getConstPool(),m->name_index-1);

        if(name == "main") descriptor = CpAtAux.getUTF8(javaclass->getConstPool(),m->descriptor_index-1);
        if(descriptor == "([Ljava/lang/String;)V") return m;
    }
    std::cout << "Arquivo não possui main " << std::endl;

    MethodInfo * toReturn = (MethodInfo*)calloc(1,sizeof(toReturn));
    return toReturn; // ARRUMAR ESTA SAIDA;
}

/** @brief Busca um metodo pelo seu descritor ou nome e o retorna.
 * @param *javaclass ponteiro para ClassLoader.
 * @param string que representa o nome do metodo.
 * @param string que representa o descritor do metodo.
 * @return MethodInfo*
 */
MethodInfo * Interpreter::findMethodByNameOrDescriptor(ClassLoader* classloader,std::string method_name,std::string method_desc){
    
    
    auto methods = classloader->getMethods();

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
