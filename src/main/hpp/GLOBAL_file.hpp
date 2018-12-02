/** @file GLOBAL_file.hpp
 *  @brief Armazenas classes estáticas para serem usadas por todo o código.
 *  @bug No known bugs.
 */
#ifndef GLOBAL_H
#define GLOBAL_H

#include "Instance.hpp"
/**
*   @class MethodsArea
*   @brief Armazenas classes estáticas para serem usadas por todo o código ; 
*/

class MethodsArea {
    public:
    
    static std::map<std::string, Instance*> GLOBAL_loadedClasses;
    static std::map<std::string, Instance*> GLOBAL_staticClasses;
    static std::string path;
        
    static Operand * getStaticfield(std::string className, std::string varName);
};

#endif