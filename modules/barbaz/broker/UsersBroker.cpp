
#include "md5/md5.hpp"
#include "db/DatabaseError.hpp"
#include "UsersBroker.hpp"

using namespace broker;

bool UsersBroker::isUserExists(db::IConnection& conn, std::string const& email)
{
    db::ICursor& curs = conn.cursor();
    curs.execute("SELECT 1 FROM users WHERE email = ?").bind(email);
    return curs.hasData();
}

bool UsersBroker::registerUser(db::IConnection& conn,
                               types::User& user,
                               std::string const& password,
                               std::string const& auth_type)
{
    db::ICursor& curs = conn.cursor();
    curs.execute(
        "INSERT INTO users (\
            id, fullname, email, auth_type, role, inscription_date, last_login_date\
        ) VALUES (NULL, ?, ?, ?, ?, ?, 0)"
    ).bind(user.get_fullname())
     .bind(user.get_email())
     .bind(auth_type)
     .bind(user.get_role())
     .bind(zhttpd::api::uint64_t(::time(0)));
    zhttpd::api::uint64_t id = curs.lastrowid();
    assert(id != 0 && "user id is NULL");
    if (auth_type == "local")
    {
        curs.execute(
            "INSERT INTO auth_local (user_id, password, confirmed) VALUES (?, ?, ?)"
        ).bind(id).bind(password).bind(false);
    }
    else
        throw db::DatabaseError("Unknown authentication type '" + auth_type + "'");
    return true;
}
zhttpd::api::uint64_t UsersBroker::authenticate(db::IConnection& conn,
                                                std::string const& email,
                                                std::string const& password)
{
    db::ICursor& curs = conn.cursor();
    curs.execute("SELECT id FROM users WHERE email = ?").bind(email);
    if (!curs.hasData())
        return 0;
    zhttpd::api::uint64_t id = curs.fetchone()[0].getUint64();
    curs.execute("SELECT 1 FROM auth_local WHERE user_id = ? AND password = ?").bind(id).bind(password);
    if (!curs.hasData())
        return 0;
    return id;
}
std::string UsersBroker::renewSessionHash(db::IConnection& conn, zhttpd::api::uint64_t user_id)
{
    db::ICursor& curs = conn.cursor();
    curs.execute("DELETE FROM sessions WHERE user_id = ?").bind(user_id);
    std::string hash = md5(zhttpd::Logger::toString(user_id) +
                           zhttpd::Logger::toString(::time(0)) +
                           "bite").hexdigest();
    curs.execute("INSERT INTO sessions (user_id, hash) VALUES (?, ?)").bind(user_id).bind(hash);
    return hash;
}

zhttpd::api::uint64_t UsersBroker::getUserIdFromHash(db::IConnection& conn, std::string const& hash)
{
    db::ICursor& curs = conn.cursor();
    curs.execute("SELECT user_id FROM sessions WHERE hash = ?").bind(hash);
    if (!curs.hasData())
        return 0;
    return curs.fetchone()[0].getUint64();
}
