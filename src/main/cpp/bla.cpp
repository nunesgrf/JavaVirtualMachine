#include "../hpp/Interpreter.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
int main() {
    
    FILE * fp = fopen("../../test/HelloWorld.class","r");
    ClassLoader cl(fp);
    Interpreter * x = (Interpreter*)calloc(1,sizeof(x));

    std::cout << x->loaded_classes.size() << std::endl;
    auto m = x->mainFinder(cl);

}