
#include "BarbazModuleManager.hpp"
#include "Subscribe.hpp"
#include "types/Response.hpp"
#include "types/User.hpp"
#include "broker/UsersBroker.hpp"
#include "md5/md5.hpp"

using namespace traversal;

#include <cstring>
namespace
{
    // This example is taken from http://www.secureprogramming.com/?action=view&feature=recipes&recipeid=3
    // Its author are John Viega and Matt Messier
    // Licensing seems to be new BSD
    int isvalid_email(const char *address) {
        int        count = 0;
        const char *c, *domain;
        static char const* rfc822_specials = "()<>@,;:\\\"[]";

        /* first we validate the name portion (name@domain) */
        for (c = address;  *c;  c++) {
            if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == '\"')) {
                while (*++c) {
                    if (*c == '\"') break;
                    if (*c == '\\' && (*++c == ' ')) continue;
                    if (*c < ' ' || *c >= 127) return 0;
                }
                if (!*c++) return 0;
                if (*c == '@') break;
                if (*c != '.') return 0;
                continue;
            }
            if (*c == '@') break;
            if (*c <= ' ' || *c >= 127) return 0;
            if (strchr(rfc822_specials, *c)) return 0;
        }
        if (c == address || *(c - 1) == '.') return 0;

        /* next we validate the domain portion (name@domain) */
        if (!*(domain = ++c)) return 0;
        do {
            if (*c == '.') {
                if (c == domain || *(c - 1) == '.') return 0;
                count++;
            }
            if (*c <= ' ' || *c >= 127) return 0;
            if (strchr(rfc822_specials, *c)) return 0;
        } while (*++c);

        return (count >= 1);
    }
}

Subscribe::Subscribe(BarbazModuleManager& manager) : ATraversal<Subscribe>(this), _manager(manager)
{
    this->registerMethod("new_account", &Subscribe::new_account);
}

view::IViewable* Subscribe::index(std::list<std::string>& path, zhttpd::api::IRequest&, zhttpd::mod::PostData&)
{
    return 0;
}

view::IViewable* Subscribe::new_account(zhttpd::api::IRequest&, zhttpd::mod::PostData& data)
{
    types::User user;
    std::auto_ptr<types::Response> response(new types::Response());
    std::auto_ptr<db::IConnection> conn = this->_manager.getNewDBConnection();
    if (data.fields["fullname"].size() > 0)
        user.set_fullname(data.fields["fullname"].front());
    if (user.get_fullname().size() == 0)
        response->addDetail("fullname", "You must specify a full name");
    else if (user.get_fullname().size() < 5)
        response->addDetail("fullname", "Your full name is too short");

    if (data.fields["email"].size() > 0)
    {
        user.set_email(data.fields["email"].front());
        if (!isvalid_email(user.get_email().c_str()))
            response->addDetail("email", "Given email is not valid");
        else if (broker::UsersBroker::isUserExists(*conn, user.get_email()))
            response->addDetail("email", "This email is already registered");
    }

    std::string password, password2;
    if (data.fields["password"].size() > 0)
        password = data.fields["password"].front();
    if (data.fields["password2"].size() > 0)
        password2 = data.fields["password2"].front();
    if (password.size() < 6)
        response->addDetail("password", "Your password is too short");
    else if (password != password2)
        response->addDetail("password2", "Passwords don't match");

    if (response->getDetails().size() > 0)
        response->addField("error", "true");
    else
    {
        password = md5(password).hexdigest();
        if (broker::UsersBroker::registerUser(*conn, user, password, "local"))
            response->addField("success", "true");
        else
            response->addField("error", "true");
    }
    return response.release();
}
