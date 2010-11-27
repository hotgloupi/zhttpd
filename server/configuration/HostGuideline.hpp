
#ifndef HOSTGUIDELINE_HPP__
# define HOSTGUIDELINE_HPP__

#include <string>
#include "IVHostGuideline.hpp"

namespace zhttpd
{
    class HostGuideline : public IVHostGuideline
    {
    private:
        std::string _match;
        bool _deny;

    public:
        HostGuideline(std::string const& match, bool deny = false);
        virtual ~HostGuideline();

        bool match(api::IRequest& request) const;
    };
}

#endif // HOSTGUIDELINE_HPP__

