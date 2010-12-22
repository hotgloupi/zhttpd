
#include "DbItemJsonView.hpp"
#include "db/IItem.hpp"
#include "db/Attributes.hpp"
#include "server/utils/Logger.hpp"
#include "json_spirit.h"

static DbItemJsonView dummy; //registration at load time

DbItemJsonView::DbItemJsonView() : ViewAdaptor<IJsonView>(db::IItem()) {}

struct JsonVisitor : public db::IVisitor
{
    json_spirit::mObject& map;

    JsonVisitor(json_spirit::mObject& map) : map(map) {}
    void visitInt(db::AttributeInt const& attr, db::IItem const& i)       { this->map[attr.getName()] = attr.getValue(i); }
    void visitInt64(db::AttributeInt64 const& attr, db::IItem const& i)   { this->map[attr.getName()] = attr.getValue(i); }
    void visitUint64(db::AttributeUint64 const& attr, db::IItem const& i) { this->map[attr.getName()] = attr.getValue(i); }
    void visitDouble(db::AttributeDouble const& attr, db::IItem const& i) { this->map[attr.getName()] = attr.getValue(i); }
    void visitFloat(db::AttributeFloat const& attr, db::IItem const& i)   { this->map[attr.getName()] = attr.getValue(i); }
    void visitString(db::AttributeString const& attr, db::IItem const& i) { this->map[attr.getName()] = attr.getValue(i); }
    void visitBool(db::AttributeBool const& attr, db::IItem const& i)     { this->map[attr.getName()] = attr.getValue(i); }
};

zhttpd::api::IBuffer* DbItemJsonView::convert(IViewable const& object,
                                              zhttpd::api::IBufferManager& manager) const
{
    db::IItem const& item = dynamic_cast<db::IItem const&>(object);
    json_spirit::mObject res;
    JsonVisitor visitor(res);
    item.visitAll(visitor);
    return manager.allocate(json_spirit::write(res));
}

