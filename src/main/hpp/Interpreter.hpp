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
  
    void execute(ClassLoader);
};

#endif