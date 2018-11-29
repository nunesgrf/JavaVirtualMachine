#include "../hpp/ByteReader.hpp"

#define NEUTRAL_BYTE_FOR_OR 0x00;
/**
 * @brief byteCatch(FILE * fp) busca o binário correspondendo ao tipo T.
 * @param FILE * fp : arquivo de acesso.
 * @return T
 */
template <class T>
T ByteReader<T>::byteCatch(FILE * fp) {
    int num_of_bytes = sizeof(T); 
    T toReturn = NEUTRAL_BYTE_FOR_OR;

    for(int i = 0; i < num_of_bytes; i++) {
        toReturn = ((toReturn << 8) | getc(fp));
    }
    return toReturn;
}


