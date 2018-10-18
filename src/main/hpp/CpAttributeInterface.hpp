#ifndef _CP_ATR_H_
#define _CP_ATR_H_

#include <string>
#include <vector>
#include "CpInfo.hpp"

struct CpAttributeInterface {

    std::string getUTF8(std::vector<CpInfo*>, uint16_t);
};

#endif // _CP_ATR_H_