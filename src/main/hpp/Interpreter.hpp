/** @file Interpreter.hpp
 *  @brief Declarações das funções e atributos do Interpretador para interpretar o que foi lido do .class.
 *  @bug No known bugs.
 */
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ClassLoader.hpp"
#include "Frame.hpp"
#include "Instance.hpp"
#include <cstring>
#include <map>
#include <string>

using namespace std;

class Interpreter {

  public:
    std::string current_path_folder;
    static std::stack<Frame*> frame_stack;
    
    void execute(ClassLoader*);
    static void loadVariables(Instance*);
    static ClassLoader * getClassInfo(std::string);
    MethodInfo * mainFinder(ClassLoader*);
    MethodInfo * findMethodByNameOrDescriptor(ClassLoader*,std::string,std::string);
    static Instance    * loadInMemo(ClassLoader*);
    static Operand     * createType(std::string);
};

#endif