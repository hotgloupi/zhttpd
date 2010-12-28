
#include "ItemListJsonView.hpp"
#include "db/ItemList.hpp"
#include "db/Attributes.hpp"
#include "server/utils/Logger.hpp"
#include "json_spirit.h"

using namespace view;

static ItemListJsonView dummy; //registration at load time

ItemListJsonView::ItemListJsonView() : ViewAdaptor<IJsonView>(db::ItemList()) {}

namespace {
struct JsonVisitor : public db::IVisitor
{
    json_spirit::Object& map;

    JsonVisitor(json_spirit::Object& map) : map(map) {}
    void visitInt(db::AttributeInt const& attr, db::IItem const& i)
    { this->map.push_back(json_spirit::Pair(attr.getName(), attr.getValue(i))); }
    void visitInt64(db::AttributeInt64 const& attr, db::IItem const& i)
    { this->map.push_back(json_spirit::Pair(attr.getName(), attr.getValue(i))); }
    void visitUint64(db::AttributeUint64 const& attr, db::IItem const& i)
    { this->map.push_back(json_spirit::Pair(attr.getName(), attr.getValue(i))); }
    void visitDouble(db::AttributeDouble const& attr, db::IItem const& i)
    { this->map.push_back(json_spirit::Pair(attr.getName(), attr.getValue(i))); }
    void visitFloat(db::AttributeFloat const& attr, db::IItem const& i)
    { this->map.push_back(json_spirit::Pair(attr.getName(), attr.getValue(i))); }
    void visitString(db::AttributeString const& attr, db::IItem const& i)
    { this->map.push_back(json_spirit::Pair(attr.getName(), attr.getValue(i))); }
    void visitBool(db::AttributeBool const& attr, db::IItem const& i)
    { this->map.push_back(json_spirit::Pair(attr.getName(), attr.getValue(i))); }
};
} // end anonymous namespace

zhttpd::api::IBuffer* ItemListJsonView::convert(IViewable const& object,
                                              zhttpd::api::IBufferManager& manager) const
{
    db::ItemList const& items = dynamic_cast<db::ItemList const&>(object);
    db::ItemList::const_iterator it = items.begin(), end = items.end();
    json_spirit::Array res;
    for (; it != end; ++it)
    {
        json_spirit::Object item;
        JsonVisitor visitor(item);
        static_cast<db::IItem const*>((*it))->visitAll(visitor);
        res.push_back(item);
    }
    return manager.allocate(json_spirit::write(res));
}

