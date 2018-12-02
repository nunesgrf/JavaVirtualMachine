/** @file Instance.hpp
 *  @brief Declaração da struct Instance.
 *  @bug No known bugs.
 */
#ifndef INSTANCE_H
#define INSTANCE_H

#include <map>
#include <string>
#include "ClassLoader.hpp"
#include "Frame.hpp"//Arrumar

struct Operand;

/**
*	@struct Instance
*	@brief tipo que determinará	o nome e o tipo de operando através do Operand class;
*		Álém contém o método Instance::Instance	
*/
struct Instance {
    std::string name;
    std::map< std::string, Operand* >  references;
    ClassLoader * classe;

    Instance(ClassLoader*);
};

#endif