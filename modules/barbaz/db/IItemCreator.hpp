/**
 * @file IItemCreator.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IITEMCREATOR_HPP__
# define __IITEMCREATOR_HPP__


namespace db
{
    class IItem;

    class IItemCreator
    {
    public:
        virtual IItem* create() const = 0;
        virtual ~IItemCreator() {}
    };
}

#endif /* !__IITEMCREATOR_HPP__ */

