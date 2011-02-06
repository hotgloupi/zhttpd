
#include "ItemList.hpp"

using namespace db;


IItem& ItemList::operator[](unsigned int i)
{
    return *this->_items[i];
}

zhttpd::api::size_t ItemList::size() const
{
    return this->_items.size();
}
void ItemList::push_back(IItem& i)
{
    this->_items.push_back(&i);
}

ItemList::~ItemList()
{
    if (this->_items.size() > 0)
    {
        items_t::iterator it = this->_items.begin(),
                          end = this->_items.end();
        for (; it != end; ++it)
            delete (*it);
        this->_items.clear();
    }
}

ItemList::iterator ItemList::begin()
{
    return this->_items.begin();
}

ItemList::const_iterator ItemList::begin() const
{
    return this->_items.begin();
}

ItemList::iterator ItemList::end()
{
    return this->_items.end();
}

ItemList::const_iterator ItemList::end() const
{
    return this->_items.end();
}

viewable_types::Type ItemList::getViewableTypeId() const
{
    return viewable_types::DB_ITEMS;
}

