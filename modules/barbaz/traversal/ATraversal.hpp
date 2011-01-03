
#ifndef __ATRAVERSAL_HPP__
# define __ATRAVERSAL_HPP__

# include <map>
# include <string>

# include "server/utils/NonCopyable.hpp"

# include "ITraversal.hpp"

namespace traversal
{
    template<typename SelfClass>
    class ATraversal : public ITraversal, public zhttpd::NonCopyable
    {
    public:
        typedef view::IViewable* (SelfClass::* method_t)(zhttpd::api::IRequest&, zhttpd::mod::PostData&);

    private:
        typedef typename std::map<std::string, method_t> views_t;
    private:
        views_t _views;
        SelfClass* _self;

    public:
        ATraversal(SelfClass* self) : _self(self)
        {
            assert(this->_self != 0 && "Where am I ?!");
        }
        virtual ~ATraversal() {}
        virtual view::IViewable* index(std::list<std::string>&,
                                       zhttpd::api::IRequest&,
                                       zhttpd::mod::PostData&)
        {
            return 0;
        }

        virtual view::IViewable* traverse(std::list<std::string>& path,
                                          zhttpd::api::IRequest& r,
                                          zhttpd::mod::PostData& d)
        {
            if (path.size() > 0)
            {
                std::string method = path.front().substr(0, path.front().find_first_of(".?#/"));
                LOG_DEBUG("Traverse with " + path.front() + " => " + method);
                typename views_t::iterator view = this->_views.find(method);
                if (view != this->_views.end())
                {
                    return ((this->_self)->*((*view).second))(r, d);
                }
            }
            return this->index(path, r, d);
        }

    protected:
        void registerMethod(char const* path, method_t method)
        {
            assert(this->_views.find(path) == this->_views.end() && "path registered twice");
            this->_views[path] = method;
        }
    };
}

#endif /* !__ATRAVERSAL_HPP__ */

