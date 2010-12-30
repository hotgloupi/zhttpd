
#include "md5.hpp"
#include "types/Response.hpp"
#include "broker/Users.hpp"
#include "BarbazModuleManager.hpp"
#include "Login.hpp"

using namespace traversal;

Login::Login(BarbazModuleManager& manager) : ATraversal<Login>(this), _manager(manager)
{
    this->registerMethod("login", &Login::login);
    this->registerMethod("logout", &Login::logout);
    this->registerMethod("loginWithHash", &Login::loginWithHash);
}

view::IViewable* Login::login(zhttpd::api::IRequest&, zhttpd::mod::PostData& data)
{
    std::string email, password;
    std::auto_ptr<types::Response> response(new types::Response());
    if (data.fields["email"].size() > 0)
        email = data.fields["email"].front();
    if (data.fields["password"].size() > 0)
        password = data.fields["password"].front();
    if (email.size() == 0)
        response->addDetail("email", "This field is required");
    if (password.size() == 0)
        response->addDetail("password", "This field is required");
    if (response->getDetails().size() > 0)
        response->addField("error", "true");
    else
    {
        std::auto_ptr<db::IConnection> conn = this->_manager.getNewDBConnection();
        password = md5(password).hexdigest();
        zhttpd::api::uint64_t user_id = broker::Users::authenticate(*conn, email, password);
        if (user_id == 0)
        {
            response->addField("error", "true");
            response->addDetail("email", "Wrong email or password");
        }
        else
        {
            std::string hash = broker::Users::renewSessionHash(*conn, user_id);
            response->addField("success", "true");
            response->addField("hash", hash);
        }
    }
    return response.release();
}

view::IViewable* Login::loginWithHash(zhttpd::api::IRequest&, zhttpd::mod::PostData& data)
{
    if (data.fields["hash"].size() == 0)
        return 0;
    std::string hash = data.fields["hash"].front();
    if (hash.size() == 0)
        return 0;
    std::auto_ptr<types::Response> response(new types::Response());
    std::auto_ptr<db::IConnection> conn = this->_manager.getNewDBConnection();
    zhttpd::api::uint64_t user_id = broker::Users::getUserIdFromHash(*conn, hash);
    if (user_id == 0)
        response->addField("error", "true");
    else
    {
        std::string hash = broker::Users::renewSessionHash(*conn, user_id);
        response->addField("success", "true");
        response->addField("hash", hash);
    }
    return response.release();
}

view::IViewable* Login::logout(zhttpd::api::IRequest& r, zhttpd::mod::PostData&)
{
    std::auto_ptr<types::User> user(this->_manager.getUser(r));
    if (user.get() != 0)
    {
        broker::Users::deleteSession(*this->_manager.getNewDBConnection(), user->get_id());
        r.setResponseHeader("Location", "/");
        r.setResponseCode(zhttpd::api::http_code::FOUND);
    }
    return 0;
}
