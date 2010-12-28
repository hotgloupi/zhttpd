
#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

# include <vector>
# include <string>

# include "server/utils/NonCopyable.hpp"
# include "view/IViewable.hpp"

namespace types
{
    class Response : public view::IViewable, public zhttpd::NonCopyable
    {
    public:
        typedef std::pair<std::string, std::string> pair_t;
        typedef std::vector<pair_t> fields_t;

    protected:
        fields_t _fields;
        fields_t _details;

    public:
        virtual void addField(std::string const& key, std::string const& val);
        virtual fields_t const& getFields() const;
        virtual void addDetail(std::string const& key, std::string const& val);
        virtual fields_t const& getDetails() const;
        virtual viewable_types::Type getViewableTypeId() const;
        virtual ~Response();
    };
}
#endif /* !__RESPONSE_HPP__ */

