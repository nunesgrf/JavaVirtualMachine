#ifndef ___JAVA_H___
#define ___JAVA_H___

#define MAGIC_NUMBER 0xCAFEBABE;

#include <cstdint>
#include <iostream>
#include "ByteReader.cpp"

class JavaClass {
    private: 
        uint32_t magicNumber;
        uint16_t majorVersion;
        uint16_t minorVersion;

        bool verify();

    public:
        void setMagic(FILE * fp);
        void setMajor(FILE * fp);
        void setMinor(FILE * fp);

        typeof(magicNumber) getMagic() {
            return magicNumber;
        }
        typeof(majorVersion) getMajor() {
            return majorVersion;
        }
        typeof(minorVersion) getMinor() {
            return minorVersion;
        }
};

bool JavaClass::verify() {
    return true;
}

void JavaClass::setMagic(FILE * fp) {
    ByteReader<typeof(magicNumber)> bReader;
    magicNumber = bReader.byteCatch(fp);
}

void JavaClass::setMajor(FILE * fp) {
    ByteReader<typeof(majorVersion)> bReader;
    majorVersion = bReader.byteCatch(fp);
}

void JavaClass::setMinor(FILE * fp) {
    ByteReader<typeof(minorVersion)> bReader;
    minorVersion = bReader.byteCatch(fp);
}

#endif // ___JAVA_H___