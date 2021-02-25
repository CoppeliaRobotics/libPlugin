#include "Handle.h"

namespace sim
{
    std::map<int, std::map<int, std::set<const void*>>> HandleStorage::handles;
    std::map<const void*, std::map<int, int>> HandleStorage::handlesr;
}
