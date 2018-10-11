#ifndef ___BYTEREADER_H___
#define ___BYTEREADER_H___


#define NEUTRAL_BYTE_FOR_OR 0x00;

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
    T toReturn = NEUTRAL_BYTE_FOR_OR;

    for(int i = 0; i < num_of_bytes; i++) {
        toReturn = ((toReturn << 8) | getc(fp));
    }
    return toReturn;
}

#endif // ___BYTEREADER_H___
