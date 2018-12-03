/** @file Instance.hpp
 *  @brief Declaração da classe Instance.
 *  @bug No known bugs.
 */
#ifndef INSTANCE_H
#define INSTANCE_H

#include <map>
#include <string>
#include "ClassLoader.hpp"
#include "Frame.hpp"//Arrumar

struct Operand;

struct Instance {
    std::string name;
    std::map< std::string, Operand* > references;
    ClassLoader * classe;

    Instance(ClassLoader*);
};

#endif