#include "../main/ByteReader.cpp"

using namespace std;
int main() {
    ByteReader<uint32_t> type_char;
    FILE * fp = fopen("HelloWorld.class","r");
    cout << type_char.byteCatch(fp) << endl;
}