#ifndef cuprum_common_h
#define cuprum_common_h

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>

template <typename Target, typename Source>
inline bool instanceof(const Source* ptr) {
    return dynamic_cast<const Target*>(ptr) != nullptr;
}

#endif