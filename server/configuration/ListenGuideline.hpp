
#ifndef __LISTENGUIDELINE_HPP__
# define __LISTENGUIDELINE_HPP__

# include "IVHostGuideline.hpp"

namespace ZHTTPD
{
    class ListenGuideline : public IVHostGuideline
    {
    private:
        API::uint16_t _port;
        bool _deny;

    public:
        ListenGuideline(API::uint16_t port, bool deny = false);
        virtual ~ListenGuideline();

        bool match(API::IRequest& request) const;
    };
}

#endif // __LISTENGUIDELINE_HPP__
