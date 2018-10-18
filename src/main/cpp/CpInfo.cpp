#ifndef CPP_CPINFO
#define CPP_CPINFO

#include "../hpp/CpInfo.hpp"

CpInfo::~CpInfo() {
     if(this->tag == 1) free(this->UTF8.bytes);
}

void CpInfo::read(FILE * fp) {

    ByteReader<uint8_t>  OneByte;
    ByteReader<uint16_t> TwoByte;
    ByteReader<uint32_t> FourByte;

    this->tag = OneByte.byteCatch(fp);

    switch(this->tag) {

            case CONSTANT_Utf8:

                this->UTF8.length = TwoByte.byteCatch(fp); /* It reads two bytes from the file */
                /**
                 * A alocação via calloc é feita aqui com o entuito de settar os valores como 0
                 * inicialmente. Alocala-se um total de lenght+1 posições para que ao final possa se inserir
                 * o caractere '\0' sem acessar uma região de memória não garantida para nosso
                 * programa.
                 */
                this->UTF8.bytes = (uint8_t *) calloc(this->UTF8.length+1,sizeof(uint8_t));

                for (int j = 0; j < this->UTF8.length; j++) {

                    uint8_t byte = OneByte.byteCatch(fp); /* Reads one byte from file */
                    this->UTF8.bytes[j] = byte; /* It pushes into the UTF8 array */

                }
                this->UTF8.bytes[this->UTF8.length] = '\0'; /* Concatenates \0 for string last char */
                break;

            case CONSTANT_Integer:

                this->Integer.bytes = FourByte.byteCatch(fp) /* Reads 4 bytes of the file */;
                break;

            case CONSTANT_Float:

                this->Float.bytes = FourByte.byteCatch(fp);  /* Reads 4 bytes of the file */
                break;

            case CONSTANT_Long:

                this->Long.high_bytes = FourByte.byteCatch(fp); /* Reads 4 bytes of the file */
                this->Long.low_bytes  = FourByte.byteCatch(fp); /* Reads 4 bytes of the file */
                break;

            case CONSTANT_Double:

                this->Double.high_bytes = FourByte.byteCatch(fp); /* Reads 4 bytes of the file */
                this->Double.low_bytes  = FourByte.byteCatch(fp); /* Reads 4 bytes of the file */
                break;

            case CONSTANT_Class:

                this->Class.name_index = TwoByte.byteCatch(fp); /* Reads 2 bytes of the file */
                break;

            case CONSTANT_String:

                this->String.string_index = TwoByte.byteCatch(fp); /* Reads 2 bytes of the file */
                break;

            case CONSTANT_Fieldref:

                this->Fieldref.class_index = TwoByte.byteCatch(fp); /* Reads 2 bytes of the file */
                this->Fieldref.name_and_type_index = TwoByte.byteCatch(fp); /* Reads 2 bytes of the file */
                break;

            case CONSTANT_Methodref:

                this->Methodref.class_index = TwoByte.byteCatch(fp); /* Reads 2 bytes of the file */
                this->Methodref.name_and_type_index = TwoByte.byteCatch(fp); /* Reads 2 bytes of the file */
                break;

            case CONSTANT_InterfaceMethodref:

                this->InterfaceMethodref.class_index = TwoByte.byteCatch(fp); /* Reads 2 bytes of the file */
                this->InterfaceMethodref.name_and_type_index = TwoByte.byteCatch(fp); /* Reads 2 bytes of the file */
                break;

            case CONSTANT_NameAndType:

                this->NameAndType.name_index = TwoByte.byteCatch(fp); /* Reads 2 bytes of the file */
                this->NameAndType.descriptor_index = TwoByte.byteCatch(fp); /* Reads 2 bytes of the file */
                break;

            default:
                if(true) std::cout << "A invalid tag was detected" << std::endl;
        }
}


#endif
