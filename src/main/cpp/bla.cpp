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
    Interpreter x;

    x.execute(&cl);
    free(fp);
    return 0;
}