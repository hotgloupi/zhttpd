
#ifndef __URLGUIDELINE_HPP__
# define __URLGUIDELINE_HPP__

# include "IVHostGuideline.hpp"

namespace zhttpd
{
    class UrlGuideline : public IVHostGuideline
    {
    private:
        std::string _match;
        bool _deny;

    public:
        UrlGuideline(std::string const& match_string, bool deny = false);
        virtual ~UrlGuideline();

        bool match(api::IRequest& request) const;
    };
}

#endif // __URLGUIDELINE_HPP__

