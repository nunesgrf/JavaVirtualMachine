#include "../hpp/FieldInfo.hpp"
#include "../hpp/ByteReader.hpp"

/** @brief Destrutor de FieldInfo.
 * @param sem parâmetros.
 * @return void
 */
FieldInfo::~FieldInfo() {
    
    for(auto a : this->attributes) {
        a->~AttributeInfo();
        free(a);
    }
}

/** @brief setting inicial do FieldInfo a partir de um arquivo.
 * @param *fp ponteiro de arquivo @param trueCpInfo vetor de cpInfo.
 * @return void
 */
void FieldInfo::read(FILE *fp, std::vector<CpInfo *> trueCpInfo)
{
    ByteReader<uint16_t> TwoByte;
    access_flags = TwoByte.byteCatch(fp);
    name_index = TwoByte.byteCatch(fp);
    descriptor_index = TwoByte.byteCatch(fp);
    attributes_count = TwoByte.byteCatch(fp);
    
    for(int i = 0; i < attributes_count; i++)
    {
        /* Allocate a attribute */
        AttributeInfo * attribute = (AttributeInfo *)calloc(1, sizeof(*attribute));
        attribute[i].read(fp,trueCpInfo);
        /* Puts into the vector of attributes  */
        attributes.push_back(attribute);
    }
    

}
