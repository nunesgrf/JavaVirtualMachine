#ifndef ___BYTEREADER_H___
#define ___BYTEREADER_H___

#include <fstream>

/**
 * @brief Classe ByteReader. No momento da instanciação define-se qual tipo deseja-se buscar
 * E assim quando acionado o método byteCatch(FILE * fp) o arquivo busca o binário correspondente.
 */
template <class T>
class ByteReader {   
    public:
    T byteCatch(FILE * fp);
};


#endif // ___BYTEREADER_H___