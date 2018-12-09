/** @file Stack.cpp
    @brief Métodos que simulam a estrura de dados "pilha";

*/

#include "../hpp/Stack.hpp"

template <class T>
T Stack<T>::pop() {
    if(!this->stack_pointer.empty()) {
        T toReturn = this->stack_pointer.back();
        stack_pointer.pop_back();
        return toReturn;
    }
    else throw; //Colocar invalid_argument("") aqui.
}

template <class T>
T Stack<T>::peek() {
    T toReturn = this->stack_pointer.back();
    return toReturn;
}

template <class T>
int Stack<T>::size() {
    return this->stack_pointer.size();
}

template <class T>
void Stack<T>::push(T value) {
    this->stack_pointer.push_back(value);
}

template <class T>
void Stack<T>::dup() {
    if(!this->stack_pointer.empty()) {
        T toPush = this->peek();
        this->push(toPush);
    }
    else throw; //Colocar invalid_argument("") aqui.
}

template <class T>
void Stack<T>::dup_x1() {
    if(this->size() >= 2) {
        T toPush1 = this->pop();
        T toPush2 = this->pop();
        this->push(toPush1);
        this->push(toPush2);
        this->push(toPush1);
    }
    else throw;
}

template <class T>
void Stack<T>::dup_x2() {
    if(this->size() >= 3) {
        T toPush1 = this->pop();
        T toPush2 = this->pop();
        T toPush3 = this->pop();
        this->push(toPush1);
        this->push(toPush3);
        this->push(toPush2);
        this->push(toPush1);
    }
}

template <class T>
void Stack<T>::dup2() {
    if(this->size() >= 2) {
        T toPush1 = this->pop();
        T toPush2 = this->pop();
        this->push(toPush2);
        this->push(toPush1);
        this->push(toPush2);
        this->push(toPush1);
    } 
}

template <class T>
void Stack<T>::dup2_x1() {
    if(this->size() >= 3) {
        T toPush1 = this->pop();
        T toPush2 = this->pop();
        T toPush3 = this->pop();
        this->push(toPush2);
        this->push(toPush1);
        this->push(toPush3);
        this->push(toPush2);
        this->push(toPush1);
    }
}

template <class T>
void Stack<T>::dup2_x2() {
    if(this->size() >= 4) {
        T toPush1 = this->pop();
        T toPush2 = this->pop();
        T toPush3 = this->pop();
        T toPush4 = this->pop();
        this->push(toPush2);
        this->push(toPush1);
        this->push(toPush4);
        this->push(toPush3);
        this->push(toPush2);
        this->push(toPush1);
    }
}

template <class T>
void Stack<T>::swap() {
    if(this->size() >= 2) {
        T toSwap1 = this->pop();
        T toSwap2 = this->pop();
        this->push(toSwap1);
        this->push(toSwap2);
    }
    else throw; //Colocar invalid_argument("") aqui.
}