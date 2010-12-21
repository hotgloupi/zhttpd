/**
 * @file ViewAdaptor.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __VIEWADAPTOR_HPP__
# define __VIEWADAPTOR_HPP__

# include <map>
# include <cassert>

# include "IViewAdaptor.hpp"
# include "IViewable.hpp"
# include "server/utils/Logger.hpp"

template<typename View>
class ViewAdaptor : IViewAdaptor
{
public:
    ViewAdaptor(IViewable const& type)
    {
        View::registerAdaptor(type.getViewableTypeId(), *this);
    }
};

template<typename T>
class MakeViewAdaptor
{
protected:
    static IViewAdaptor* _map[viewable_types::MAX_TYPES];

public:
    static void registerAdaptor(viewable_types::Type type_id, IViewAdaptor& view_Adaptor)
    {
        assert(MakeViewAdaptor::_map[type_id] == 0);
        LOG_DEBUG("BB: register view Adaptor for " + zhttpd::Logger::toString(type_id));
        MakeViewAdaptor::_map[type_id] = &view_Adaptor;
    }

    static IViewAdaptor* getAdaptor(viewable_types::Type type_id)
    {
        if (type_id < viewable_types::MAX_TYPES)
            return MakeViewAdaptor::_map[type_id];
        return 0;
    }
};

template<class T> IViewAdaptor* MakeViewAdaptor<T>::_map[viewable_types::MAX_TYPES] = {0};

#endif /* !__VIEWADAPTOR_HPP__ */

