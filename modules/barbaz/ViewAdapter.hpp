/**
 * @file ViewAdapter.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __VIEWADAPTER_HPP__
# define __VIEWADAPTER_HPP__

# include <map>
# include <cassert>

# include "IViewAdapter.hpp"
# include "IViewable.hpp"
# include "server/utils/Logger.hpp"

template<typename View>
class ViewAdapter : IViewAdapter
{
public:
    ViewAdapter(IViewable const& type)
    {
        View::registerAdapter(type.getViewableTypeId(), *this);
    }
};

template<typename T>
class MakeViewAdapter
{
protected:
    static IViewAdapter* _map[viewable_types::MAX_TYPES];

public:
    static void registerAdapter(viewable_types::Type type_id, IViewAdapter& view_adapter)
    {
        assert(MakeViewAdapter::_map[type_id] == 0);
        LOG_DEBUG("BB: register view adapter for " + zhttpd::Logger::toString(type_id));
        MakeViewAdapter::_map[type_id] = &view_adapter;
    }

    static IViewAdapter* getAdapter(viewable_types::Type type_id)
    {
        if (type_id < viewable_types::MAX_TYPES)
            return MakeViewAdapter::_map[type_id];
        return 0;
    }
};

template<class T> IViewAdapter* MakeViewAdapter<T>::_map[viewable_types::MAX_TYPES] = {0};

#endif /* !__VIEWADAPTER_HPP__ */

