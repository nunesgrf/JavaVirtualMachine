#include "../main/hpp/Frame.hpp"
#include <iostream>
#include <vector>

using namespace std;
int main() {
    
    vector< CpInfo* > cp;
    MethodInfo * m = (MethodInfo*)calloc(1,sizeof(m));

    Frame frame(cp,m);

    Operand * op = (Operand*)calloc(1,sizeof(op));
    op->type_int = 10;

    cout << "Stack Size: " << frame.operand_stack.size() << endl;
    frame.operand_stack.push(op);
    cout << "Stack Size: " << frame.operand_stack.size() << endl;
    
    auto a = frame.operand_stack.top();
    cout << "Stack top: " << a->type_int << endl;


    
}