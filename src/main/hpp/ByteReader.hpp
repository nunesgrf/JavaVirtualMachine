/** @file ByteReader.hpp
 *  @brief Declarações das funções do ByteReader para leitura dos bytes no arquivo .class.
 *  @bug No known bugs.
 */
#ifndef ___BYTEREADER_H___
#define ___BYTEREADER_H___

#include <fstream>

template <class T>
/**	@class ByteReader
*	@brief Contém o método byteCatch - busca fazer a leitura dos bytes do .class;
*
*/
class ByteReader {   
    public:
    T byteCatch(FILE * fp);
};


#endif // ___BYTEREADER_H___