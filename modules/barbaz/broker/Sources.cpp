
# include "Sources.hpp"
# include "db/ICursor.hpp"

using namespace broker;

db::ItemCreator<types::Source> const Sources::creator;

db::ItemList* Sources::getUserSources(db::IConnection& conn, zhttpd::api::uint64_t user_id)
{
    db::ICursor& curs = conn.cursor();
    curs.execute(
        "SELECT * FROM sources"
        "   JOIN sources_auth ON sources_auth.source_id = sources.id"
        "   WHERE sources_auth.user_id = 0 OR sources_auth.user_id = ?"
    ).bind(user_id);
    db::ItemList* res = new db::ItemList();
    curs.fillItems(*res, Sources::creator);
    return res;
}
