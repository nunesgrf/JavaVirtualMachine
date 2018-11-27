#include "../hpp/Interpreter.hpp"
#include "../hpp/CpAttributeInterface.hpp"
#include <stdio.h>
#include <iostream>
#include <string>

void Interpreter::execute(ClassLoader * classloader) {

    this->loadInMemo(classloader);
    
    Frame toRun(classloader->getConstPool(),this->mainFinder(classloader));
    this->frame_stack.push(&toRun);

    while(!this->frame_stack.empty()) {
        this->frame_stack.top()->run();
        this->frame_stack.pop();
    }
}

Operand * Interpreter::getStaticfield(std::string className, std::string varName) {
    auto staticField = GLOBAL_staticClasses.at(className)->references.at(varName);
    return staticField;
}

ClassLoader * Interpreter::getClassInfo(std::string className) {

    std::cout << "Interpreter::getClassInfo BEGIN" <<std::endl;
    
    Instance * instance = GLOBAL_staticClasses[className];
    
    if(instance == NULL) {
        //mudar esse bereguejhonson
        FILE * fp = fopen("/home/nunesgrf/Documentos/JavaVirtualMachine/src/test/IntegerDemo.class","r");
        ClassLoader cl(fp);
        instance = this->loadInMemo(&cl);
        fclose(fp);
    }

    std::cout << "Interpreter::getClassInfo END" <<std::endl;

    return instance->classe;

    
}

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
void Interpreter::loadVariables(Instance * instance) {

    std::cout << "Interpreter::loadVariables BEGIN" << std::endl;

    CpAttributeInterface cpAt;
    auto currClass = instance->classe;
    auto superInfo = currClass->getConstPool()[currClass->getSuper()-1];
    auto superClassName = cpAt.getUTF8(currClass->getConstPool(),superInfo->Class.name_index-1);

    while(superClassName != "java/lang/Object") {

        superClassName = cpAt.getUTF8(currClass->getConstPool(),currClass->getThisClass()-1);
        
        for(auto fpointer : currClass->getFields()) {
            std::string nameField = cpAt.getUTF8(currClass->getConstPool(),fpointer->name_index-1);
            std::string typeVariable = cpAt.getUTF8(currClass->getConstPool(),fpointer->descriptor_index-1);
            instance->references[nameField] = this->createType(typeVariable); 
        }

        if(superClassName != "java/lang/Object" && superClassName != "") currClass = this->getClassInfo(superClassName); //IMPLEMENTAR O MÉTODO getClassInfo();

    } 
    std::cout << "Interpreter::loadVariables END" << std::endl;

}

Instance * Interpreter::loadInMemo(ClassLoader * javaclass) {

    std::cout << "Interpreter::loadInMemo BEGIN" << std::endl;
        
    Instance * inst_LC = new Instance(javaclass);  
    Instance * inst_SC = new Instance(javaclass);

    GLOBAL_loadedClasses.insert(std::pair<std::string, Instance*>(inst_LC->name,inst_LC));
    GLOBAL_staticClasses.insert(std::pair<std::string, Instance*>(inst_SC->name,inst_SC)); 

    this->loadVariables(inst_LC);

    std::cout << "Interpreter::loadInMemo END" << std::endl;
    
    return inst_LC;
}


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
    return toReturn;
    // ARRUMAR ESTA SAIDA;
}

/*Instance * Interpreter::loadInMemo(ClassLoader * classloader) {

    std::string name;
    CpAttributeInterface cpAt;
    Instance * instance = (Instance*)calloc(1,sizeof(Instance));

    instance->classe = classloader;
    name = cpAt.getUTF8(classloader->getConstPool(),classloader->getThisClass()-1);
    instance->name = name;

    std::cout << "Classe estática " << name << " carregada na memória...!" << std::endl;
    
}*/