
#ifndef __LISTENGUIDELINE_HPP__
# define __LISTENGUIDELINE_HPP__

# include "IVHostGuideline.hpp"

namespace zhttpd
{
    class ListenGuideline : public IVHostGuideline
    {
    private:
        api::uint16_t _port;
        bool _deny;

    public:
        ListenGuideline(api::uint16_t port, bool deny = false);
        virtual ~ListenGuideline();

        bool match(api::IRequest& request) const;
    };
}

#endif // __LISTENGUIDELINE_HPP__
