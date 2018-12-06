/** @file Stack.hpp
 *  @brief Simular uma pilha com todos os métodos provenientes para seu funcionamento.
 *  @bug No known bugs.
 */
#ifndef _STACK_H_
#define _STACK_H_

#include <vector>

template <class T>

/**
*   @class Stack
*   @brief Serve para simular uma pilha, assim, contendo todos os métodos necessários para manipular a mesma.
*/
class Stack {

    private:
        std::vector<T> stack_pointer;
        int stack_size = 0;  

    public:
        T pop();
        T peek();
        int  size();
        void push(T);
        void dup();
        void dup_x1();
        void dup_x2();
        void dup2();
        void dup2_x1();
        void dup2_x2();
        void swap();

             
};

#endif 