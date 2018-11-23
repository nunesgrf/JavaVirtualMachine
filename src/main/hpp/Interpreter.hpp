/** @file Interpreter.hpp
 *  @brief Declarações das funções e atributos do Interpretador para interpretar o que foi lido do .class.
 *  @bug No known bugs.
 */
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ClassLoader.hpp"
#include "Frame.hpp"
#include <cstring>

using namespace std;

class Interpreter {

  public:
    std::string current_path_folder;
    std::stack<Frame*> frame_stack;
    std::vector<Instance*> loaded_classes;
  
    void execute(ClassLoader*);
    void loadClasses(ClassLoader*);
    MethodInfo * mainFinder(ClassLoader);
};

#endif