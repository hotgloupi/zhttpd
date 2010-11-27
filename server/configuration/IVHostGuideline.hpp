
#ifndef __VHOSTGUIDELINE_HPP__
# define __VHOSTGUIDELINE_HPP__

# include "api/IRequest.hpp"

namespace zhttpd
{
    class IVHostGuideline
    {
    public:
        virtual ~IVHostGuideline() {}
        virtual bool match(api::IRequest& request) const = 0;
    };
}

#endif // VHOSTGUIDELINE_HPP__

