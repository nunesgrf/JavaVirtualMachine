#include "../hpp/Interpreter.hpp"
#include "../hpp/CpAttributeInterface.hpp"

void Interpreter::execute(ClassLoader * classloader) {

    Interpreter::loadInMemo(classloader);
    //std::cout << "Interpreter::execute begin" << std::endl;
    Frame toRun(classloader->getConstPool(),this->mainFinder(classloader));
    this->frame_stack.push(&toRun);

    while(!this->frame_stack.empty()) {
        this->frame_stack.top()->run();
        this->frame_stack.pop();
    }
    //std::cout << "Interpreter::execute end" << std::endl;
}

void Interpreter::loadClasses(ClassLoader * javaclass) {

    Instance * inst = new Instance(javaclass);  
    this->loaded_classes.push_back(inst);
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
    // ARRUMAR ESTA SAIDA;
}

Instance * Interpreter::loadInMemo(ClassLoader * classloader) {

    std::string name;
    CpAttributeInterface cpAt;
    Instance * instance = (Instance*)calloc(1,sizeof(Instance));

    instance->classe = classloader;
    name = cpAt.getUTF8(classloader->getConstPool(),classloader->getThisClass()-1);
    instance->name = name;

    std::cout << "Classe estática " << name << " carregada na memória...!" << std::endl;
    
}