
#include "Source.hpp"

using namespace types;

DEFINE_ATTRIBUTE_CLASS(Source,
    NEW_FIELD(Source, id, zhttpd::api::uint64_t),
    NEW_FIELD(Source, name, std::string),
    NEW_FIELD(Source, type, std::string),
);

