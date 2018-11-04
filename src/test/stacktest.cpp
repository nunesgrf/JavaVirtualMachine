#include "../main/cpp/Stack.cpp"
#include <iostream>

using namespace std;

int main() {
    Stack<int> sp;


    
    cout << "Stack.push(0)" << endl;
    sp.push(0);
    cout << "Stack.push(1)" << endl;
    sp.push(1);
    cout << "Stack.push(2)" << endl;
    sp.push(2);
    cout << "Stack.push(3)" << endl;
    sp.push(3);
    
    sp.dup2_x2();

    while(sp.size() != 0) {
        cout << " | " << sp.pop() << " | " << endl;
    }

    
    
}