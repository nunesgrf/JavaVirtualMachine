#include "../hpp/MethodInfo.hpp"
#include "../hpp/ByteReader.hpp"

/** @brief Destrutor da classe MethodInfo.
 * @param sem parâmetros.
 * @return destructor.
 */
MethodInfo::~MethodInfo()
{
    free(this->attributes);
}

/** @brief Serve para preencher os valores de uma instância de MethodInfo.
 * @param *fp ponteiro para arquivo @param trueCpInfo vetor de CpInfo*
 * @return void
 */
void MethodInfo::read(FILE *fp, std::vector<CpInfo*> trueCpInfo)
{
    int i = 0;
    ByteReader<uint16_t> reader;

    this->access_flags = reader.byteCatch(fp);
    this->name_index = reader.byteCatch(fp);
    this->descriptor_index = reader.byteCatch(fp);
    this->attributes_count = reader.byteCatch(fp);

    
    this->attributes = (AttributeInfo *)calloc(attributes_count, sizeof(AttributeInfo));
    for(i = 0; i < this->attributes_count; i++){
        this->attributes[i].read(fp,trueCpInfo);
    }
}
