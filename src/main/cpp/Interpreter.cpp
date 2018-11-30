#include "../hpp/Interpreter.hpp"
#include "../hpp/CpAttributeInterface.hpp"
#include "GLOBAL_file.hpp"
#include <stdio.h>
#include <iostream>
#include <string>

/** @brief Carrega o classLoader na memória e na stack e executa-o.
 * @param *classloader ponteiro para ClassLoader
 * @return void
 */
void Interpreter::execute(ClassLoader * classloader) {

    Interpreter::loadInMemo(classloader);
    
    Frame toRun(classloader->getConstPool(),this->mainFinder(classloader));
    this->frame_stack.push(&toRun);

    while(!this->frame_stack.empty()) {
        this->frame_stack.top()->run();
        this->frame_stack.pop();
    }
}

/** @brief Acessa a memória em busca de uma classe, caso inexistente, carrega a classe buscada em memória.
 * @param className nome da classe buscada.
 * @return void
 */
ClassLoader * Interpreter::getClassInfo(std::string className) {

    std::cout << "Interpreter::getClassInfo BEGIN" <<std::endl;
    
    MethodsArea container;
    Instance * instance = container.GLOBAL_staticClasses[className];
    
    if(instance == NULL) {
        FILE * fp = fopen((container.path + className + ".class").c_str(),"r");
        ClassLoader new_class(fp);
        instance = Interpreter::loadInMemo(&new_class);
        fclose(fp);
    }

    std::cout << "Interpreter::getClassInfo END" <<std::endl;

    return instance->classe;
}

/** @brief Cria um operando settado em um tipo específico.
 * @param type string contendo a informação de qual novo tipo.
 * @return Operand*
 */
Operand * Interpreter::createType(std::string type) {

    char toSwitch      = type[0];
    Operand * toReturn = (Operand*)calloc(1,sizeof(toReturn));

    switch(toSwitch) {
        case 'I':
            toReturn->tag = CONSTANT_Integer; break;
        case 'F':
            toReturn->tag = CONSTANT_Float; break;
        case 'J':
            toReturn->tag = CONSTANT_Long; break;
        case 'D':
            toReturn->tag = CONSTANT_Double; break;      
        case 'P':
            toReturn->tag = CONSTANT_Empty; break;
        //case CONSTANT_String:
        //    toReturn->tag = CONSTANT_String;
        //    toReturn->type_string = "";

        //IMPLEMENTAR DEMAIS MÉTODOS.
    }

    return toReturn;
}

/** @brief Carrega em memórias as variáveis de uma classe.
 * @param *instance ponteiro para Instance.
 * @return void
 */
void Interpreter::loadVariables(Instance * instance) {

    CpAttributeInterface cpAt;
    auto currClass = instance->classe;
    auto superInfo = currClass->getConstPool()[currClass->getSuper()-1];
    auto superClassName = cpAt.getUTF8(currClass->getConstPool(),superInfo->Class.name_index-1);

    while(superClassName != "java/lang/Object") {

        superClassName = cpAt.getUTF8(currClass->getConstPool(),currClass->getThisClass()-1);
        //
        for(auto fpointer : currClass->getFields()) {
            std::string nameField = cpAt.getUTF8(currClass->getConstPool(),fpointer->name_index-1);
            std::string typeVariable = cpAt.getUTF8(currClass->getConstPool(),fpointer->descriptor_index-1);
            instance->references[nameField] = Interpreter::createType(typeVariable); 
        }

        if(superClassName != "java/lang/Object" && superClassName != "") currClass = Interpreter::getClassInfo(superClassName); //IMPLEMENTAR O MÉTODO getClassInfo();

    } 
}

/** @brief Realiza o carregamento do ClassLoader em memória e retorna a instância.
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

/** @brief Busca a main de uma javaclass e retorna o método.
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
