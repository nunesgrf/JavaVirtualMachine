
/** @file MethodArea.cpp
    @brief Métodos que simulam a funcionalidade do MethodArea ;

*/

#include "../hpp/MethodArea.hpp"

/**
* @brief Adiciona na pilha a classe vinda pelo parametro;
* @param classloader ONde contém o method para ser salvo na pilha;
* @return void;
*/
void MethodArea::add_class(ClassLoader* classloader) {
  this->loaded_classes.push_back(classloader);
}