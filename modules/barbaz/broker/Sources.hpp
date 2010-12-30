
#ifndef __SOURCESBROKER_HPP__
# define __SOURCESBROKER_HPP__

# include "types/Source.hpp"
# include "db/ItemCreator.hpp"
# include "db/ItemList.hpp"
# include "db/IConnection.hpp"

namespace broker
{
    struct Sources
    {
        static const db::ItemCreator<types::Source> creator;
        static db::ItemList* getUserSources(db::IConnection& conn, zhttpd::api::uint64_t user_id);
    };
}

#endif /* !__SOURCES_HPP__ */

