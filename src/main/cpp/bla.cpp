#include "../hpp/Interpreter.hpp"
#include "../hpp/Frame.hpp"
#include "../hpp/MethodInfo.hpp"
#include "../hpp/CpInfo.hpp"
#include "../hpp/Instruction.hpp"
#include "../hpp/GLOBAL_file.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;



int main() {
    
    FILE * fp = fopen("../../test/HelloWorld.class","r");
    FILE * fp2 = fopen("../../test/IntegerDemo.class","r");
    
    ClassLoader cl(fp), bl(fp2);
    MethodsArea test, blabla;
    Instruction x, *vec = (Instruction*)calloc(256,sizeof(Instruction));
    MethodInfo * m = (MethodInfo*)calloc(1,sizeof(m));
    std::vector<CpInfo*> cp;

    Operand * op1 = (Operand*)calloc(1,sizeof(op1));
    Operand * op2 = (Operand*)calloc(1,sizeof(op2));

    op1->type_int = 3;
    op2->type_int = 2;

    Frame f(cp,m);
    f.operand_stack.push(op1);  
    f.operand_stack.push(op2);   
    
    x.init(vec);
    vec[100].func(&f);
    
    auto a = f.operand_stack.top();

    Instance instance1(&cl);
    Instance instance2(&bl);

    test.GLOBAL_staticClasses.insert((std::pair<std::string, Instance*>("six",&instance1)));
    auto y = blabla.GLOBAL_staticClasses.at("six");
    

    std::cout << "string = " << y->name << endl;
    std::cout << std::endl << "Encerrando execução de bla.cpp" << std::endl;
}