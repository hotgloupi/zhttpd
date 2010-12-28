
#ifndef __USERSBROKER_HPP__
# define __USERSBROKER_HPP__

# include "db/IConnection.hpp"
# include "types/User.hpp"

namespace broker
{
    struct UsersBroker
    {
    public:
        static bool isUserExists(db::IConnection& conn, std::string const& email);
        static bool registerUser(db::IConnection& conn,
                                 types::User& user,
                                 std::string const& password,
                                 std::string const& auth_type);
        static zhttpd::api::uint64_t authenticate(db::IConnection& conn,
                                                  std::string const& email,
                                                  std::string const& password);
        static std::string renewSessionHash(db::IConnection& conn, zhttpd::api::uint64_t user_id);
        static zhttpd::api::uint64_t getUserIdFromHash(db::IConnection& conn, std::string const& hash);
    };
}

#endif /* !__USERSBROKER_HPP__ */

