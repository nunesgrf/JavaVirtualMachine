#include "../hpp/Interpreter.hpp"
#include "../hpp/CpAttributeInterface.hpp"

void Interpreter::execute(ClassLoader * javaclass) {

    std::cout << "Interpreter::execute begin" << std::endl;
    Frame toRun(javaclass->getConstPool(),this->mainFinder(*javaclass));
    
    this->frame_stack.push(&toRun);
    std::cout << this->frame_stack.empty() << std::endl;
    
    while(!this->frame_stack.empty()) {
        this->frame_stack.top()->run();
        this->frame_stack.pop();
    }

    toRun.~Frame();
    std::cout << "Interpreter::execute end" << std::endl;
}

void Interpreter::loadClasses(ClassLoader * javaclass) {

    Instance * inst = new Instance(javaclass);  
    this->loaded_classes.push_back(inst);
}

MethodInfo * Interpreter::mainFinder(ClassLoader javaclass) {
    
    
    auto methods = javaclass.getMethods();
    CpAttributeInterface CpAtAux;

    for(auto m : methods) {
        
        std::string descriptor = "";
        std::string name = CpAtAux.getUTF8(javaclass.getConstPool(),m->name_index-1);
        
        if(name == "main") descriptor = CpAtAux.getUTF8(javaclass.getConstPool(),m->descriptor_index-1);
        if(descriptor == "([Ljava/lang/String;)V") return m;
    }
    std::cout << "Arquivo não possui main " << std::endl;
    // ARRUMAR ESTA SAIDA;
}