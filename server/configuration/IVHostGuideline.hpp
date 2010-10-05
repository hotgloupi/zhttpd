
#ifndef __VHOSTGUIDELINE_HPP__
# define __VHOSTGUIDELINE_HPP__

# include "api/IRequest.hpp"

namespace ZHTTPD
{
    class IVHostGuideline
    {
    public:
        virtual ~IVHostGuideline() {}
        virtual bool match(API::IRequest& request) const = 0;
    };
}

#endif // VHOSTGUIDELINE_HPP__

