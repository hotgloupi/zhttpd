/**
 * @file ItemCreator.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ITEMCREATOR_HPP__
# define __ITEMCREATOR_HPP__

# include "IItemCreator.hpp"

namespace db
{
    template<typename ItemType>
    class ItemCreator : public IItemCreator
    {
    public:
        virtual IItem* create() const
        {
            return new ItemType();
        }
    };
}

#endif /* !__ITEMCREATOR_HPP__ */

