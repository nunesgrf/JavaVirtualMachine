#include "../hpp/Instance.hpp"
#include "../hpp/CpAttributeInterface.hpp"

/** @brief Carrega as informações de um ClassLoader na Instancia.
 * @param *toLoad ponteiro para ClassLoader.
 * @return constructor
 */
Instance::Instance(ClassLoader * toLoad) {
    
    CpAttributeInterface cpAtAux;
    
    this->name   = cpAtAux.getUTF8(toLoad->getConstPool(),toLoad->getThisClass()-1);
    this->classe = toLoad;   
    //this->references = new std::map< std::string, Operand* >();
}