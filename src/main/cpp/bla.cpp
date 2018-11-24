#include "../hpp/Interpreter.hpp"
#include "../hpp/Frame.hpp"
#include "../hpp/MethodInfo.hpp"
#include "../hpp/CpInfo.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
int main() {
    
    FILE * fp = fopen("../../test/HelloWorld.class","r");
    ClassLoader cl(fp);
    Interpreter * x = (Interpreter*)calloc(1,sizeof(x));
    MethodInfo * m = (MethodInfo*)calloc(1,sizeof(m));
    std::vector<CpInfo*> cp;
    Frame frame(cp,m);

    frame.instructions[0].func;

    cout << "teste" << endl;
}