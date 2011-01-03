/**
 * @file ItemList.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ITEMLIST_HPP__
# define __ITEMLIST_HPP__

# include <vector>

# include "server/utils/static_assert.hpp"
# include "view/IViewable.hpp"
# include "IItem.hpp"

namespace db
{
    class ItemList : public view::IViewable
    {
    public:
        typedef std::vector<db::IItem*> items_t;
        typedef items_t::iterator iterator;
        typedef items_t::const_iterator const_iterator;

    private:
        template<typename T> struct IsIItem {static const bool res = false;};

    protected:
        items_t _items;

    public:
        virtual IItem& operator[](unsigned int i);
        virtual zhttpd::api::size_t size() const;
        void push_back(IItem& i);
        template<typename T> void push_back(T const& i)
        {
            static_assert(IsIItem<T>::res == false);
            T* item = new T(i);
            this->_items.push_back(item);
        }
        virtual ~ItemList();
        virtual iterator begin();
        virtual const_iterator begin() const;
        virtual iterator end();
        virtual const_iterator end() const;
        virtual viewable_types::Type getViewableTypeId() const;
    };

    template<> struct ItemList::IsIItem<IItem>{static const bool res = true;};

}

#endif /* !__ITEMLIST_HPP__ */

