#pragma once

#include <map>
#include <string>

namespace dynser
{

/**
 * \brief Named parts of string, what can be converted and projected into type fields.
 */
using Fields = std::map<std::string, std::string>;

}    // namespace dyn_ser
