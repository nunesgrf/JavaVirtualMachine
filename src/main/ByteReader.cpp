// TODO: Dividir este arquivo em .cpp e .h e definir macros para melhorar a leitura.

/**
 * @file ByteReader.cpp
 * @brief Uma classe que contém métodos de seleção e retorno de bytes.
 * @bug ...
 */
#include <iostream>
using namespace std;

/**
 * @brief Classe ByteReader. No momento da instanciação define-se qual tipo deseja-se buscar
 * E assim quando acionado o método byteCatch(FILE * fp) o arquivo busca o binário correspondente.
 */
template <class T>
class ByteReader {   
    public:
    T byteCatch(FILE * fp);
};

/**
 * @brief byteCatch(FILE * fp) busca o binário correspondendo ao tipo T.
 * @param FILE * fp : arquivo de acesso.
 */
template <class T>
T ByteReader<T>::byteCatch(FILE * fp) {
    int num_of_bytes = sizeof(T); 
    T toReturn = 0x00;

    for(int i = 0; i < num_of_bytes; i++) {
        toReturn = ((toReturn << 8) | getc(fp));
    }
    return toReturn;
}
