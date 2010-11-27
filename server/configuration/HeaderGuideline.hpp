
#ifndef HEADERGUIDELINE_HPP__
# define HEADERGUIDELINE_HPP__

# include <string>
# include "IVHostGuideline.hpp"

namespace zhttpd
{
    class HeaderGuideline : public IVHostGuideline
    {
    private:
        std::string _key;
        std::string _match;
        bool _deny;

    public:
        HeaderGuideline(std::string const& key, std::string const& match, bool deny = false);
        virtual ~HeaderGuideline();

        bool match(api::IRequest& request) const;
    };
}

#endif // HEADERGUIDELINE_HPP__

