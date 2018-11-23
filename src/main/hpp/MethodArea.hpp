/** @file MethodArea.hpp
 *  @brief Declarações do MethodArea, estrutura responsável por guardar todas as estruturas ClassLoader.
 *  @bug No known bugs.
 */
#ifndef METHOD_AREA
#define METHOD_AREA

#include <vector>
#include "ClassLoader.hpp"

class MethodArea {
  public: 
    std::vector<ClassLoader*> loaded_classes;
    void add_class(ClassLoader*);
};

#endif 