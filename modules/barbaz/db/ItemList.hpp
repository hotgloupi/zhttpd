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

    protected:
        items_t _items;

    public:
        virtual IItem& operator[](unsigned int i)
        {
            return *this->_items[i];
        }

        virtual zhttpd::api::size_t size() const
        {
            return this->_items.size();
        }
        virtual void push_back(IItem& i)
        {
            this->_items.push_back(&i);
        }

        template<typename T>
        void push_back(T const& i)
        {
            db::IItem* item = new T(i);
            this->_items.push_back(item);
        }

        virtual ~ItemList()
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
        virtual iterator begin()
        {
            return this->_items.begin();
        }
        virtual const_iterator begin() const
        {
            return this->_items.begin();
        }
        virtual iterator end()
        {
            return this->_items.end();
        }
        virtual const_iterator end() const
        {
            return this->_items.end();
        }

        virtual viewable_types::Type getViewableTypeId() const
        {
            return viewable_types::DB_ITEMS;
        }
    };
}

#endif /* !__ITEMLIST_HPP__ */

