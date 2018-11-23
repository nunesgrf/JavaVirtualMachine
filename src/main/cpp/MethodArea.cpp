#include "../hpp/MethodArea.hpp"

void MethodArea::add_class(ClassLoader* classloader) {
  this->loaded_classes.push_back(classloader);
}