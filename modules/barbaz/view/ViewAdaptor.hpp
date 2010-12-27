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
# include <stdexcept>

# include "IViewAdaptor.hpp"
# include "IViewable.hpp"
# include "server/utils/Logger.hpp"

namespace view
{

    template<typename View>
    class ViewAdaptor : public IViewAdaptor
    {
    public:
        ViewAdaptor(IViewable const& type)
        {
            View::registerAdaptor(type.getViewableTypeId(), *this);
        }
        virtual std::string const& getContentType() const
        {
            throw std::runtime_error("getContentType should not be called on adaptors");
        }
    };

    template<typename T>
    class MakeViewAdaptor : public IViewAdaptor
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
}

#endif /* !__VIEWADAPTOR_HPP__ */

