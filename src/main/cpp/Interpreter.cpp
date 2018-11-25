#include "../hpp/Interpreter.hpp"
#include "../hpp/CpAttributeInterface.hpp"

void Interpreter::execute(ClassLoader * javaclass) {
    Frame * frame = new Frame(javaclass->getConstPool(),this->mainFinder(*javaclass));

    frame->run();
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