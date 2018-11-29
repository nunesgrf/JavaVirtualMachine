#include "../hpp/GLOBAL_file.hpp"

std::map<std::string, Instance*> MethodsArea::GLOBAL_staticClasses;
std::map<std::string, Instance*> MethodsArea::GLOBAL_loadedClasses;
std::string MethodsArea::path;
/** @brief Retorna uma variável de uma classe especifica.
 * @param className nome da classe @param varName nome da variável.
 * @return Operand*
 */
Operand * MethodsArea::getStaticfield(std::string className, std::string varName) {
    auto staticField = GLOBAL_staticClasses.at(className)->references.at(varName);
    return staticField;
}