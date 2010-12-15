/**
 * @file Logger.hpp
 * @author <londei_a@epitech.net> Raphaël Londeix
 */

#ifndef __LOGGER_HPP__
# define __LOGGER_HPP__

# include <iostream>
# include <string>
# include <sstream>

# include "utils/Timer.hpp"
# include "utils/Singleton.hpp"
# include "api/constants.hpp"

namespace zhttpd
{
    namespace LOGGER
    {
        inline std::string cleanFunctionName(char const* str)
        {
            std::string s(str);
            size_t start = s.find(" zhttpd::");
            if (start != std::string::npos && start < s.find_first_of('('))
                start += 9;
            else
                start = 0;
            size_t size = s.find('(') - start;
            return s.substr(start, size) + std::string("(): ");
        }

        inline std::string cleanFileName(char const* str)
        {
            std::string s(str);
            size_t start = s.find_last_of('/');
            if (start != std::string::npos)
                return s.substr(start+1);
#ifdef _WIN32
            start = s.find_last_of('\\');
            if (start != std::string::npos)
                return s.substr(start+1);
#endif // _WIN32
            return s;
        }

        template<typename T>
        struct Stringify
        {
            static std::string toString(T const& t)
            {
                std::stringstream ss;
                ss << t;
                return ss.str();
            }
        };

        template<typename T>
        struct Stringify<T*>
        {
            static std::string toString(T* t)
            {
                std::stringstream ss;
                ss << std::hex << t;
                return ss.str();
            }
        };


        template<>
        struct Stringify<bool>
        {
            static std::string toString(bool const& t)
            {
                if (t)
                    return std::string("true");
                else
                    return std::string("false");
            }
        };

        template<>
        struct Stringify<api::http_method::Type>
        {
            static std::string toString(api::http_method::Type const& method)
            {
                switch (method)
                {
                case api::http_method::UNDEFINED:
                    return "UNDEFINED";
                case api::http_method::GET:
                    return "GET";
                case api::http_method::POST:
                    return "POST";
                case api::http_method::PUT:
                    return "PUT";
                case api::http_method::DELETE:
                    return "DELETE";
                case api::http_method::TRACE:
                    return "TRACE";
                case api::http_method::OPTIONS:
                    return "OPTIONS";
                case api::http_method::CONNECT:
                    return "CONNECT";
                case api::http_method::HEAD:
                    return "HEAD";
                default:
                    return "Unknown method!";
                }
            }
        };
    }
}

# ifndef _WIN32
#  define ZHTTPD_LOGGER_FUNC_NAME (zhttpd::LOGGER::cleanFunctionName(__PRETTY_FUNCTION__))
# else
#  define ZHTTPD_LOGGER_FUNC_NAME (std::string(__FUNCTION__) + "(): ")
# endif

# define ZHTTPD_LOGGER_FILE (::zhttpd::LOGGER::cleanFileName(__FILE__))

# define LOG_INFO(m) ::zhttpd::Logger::getInstance()->info(ZHTTPD_LOGGER_FUNC_NAME + m)
# define LOG_WARN(m) ::zhttpd::Logger::getInstance()->warn(ZHTTPD_LOGGER_FUNC_NAME + m)
# define LOG_ERROR(m) ::zhttpd::Logger::getInstance()->error(ZHTTPD_LOGGER_FUNC_NAME + m)
# define LOG_FATAL(m) ::zhttpd::Logger::getInstance()->fatal(ZHTTPD_LOGGER_FUNC_NAME + m)

# ifdef ZHTTPD_DEBUG
#  define LOG_DEBUG(m) ::zhttpd::Logger::getInstance()->debug(ZHTTPD_LOGGER_FILE + ": " + zhttpd::Logger::toString<int>(__LINE__) + ": " + ZHTTPD_LOGGER_FUNC_NAME + m)
# else
#  define LOG_DEBUG(m) (static_cast<void>(0))
# endif

// TODO : logger thread safe ?
namespace zhttpd
{
    /**
     * \class Logger
     * \brief Classe permettant de Logger des informations de manière uniforme
     */
    class Logger : public Singleton<Logger>
    {
        friend class Singleton<Logger>;
    public:
        enum Level
        {
            LVL_DEBUG = 0,
            LVL_INFO = 1,
            LVL_WARNING = 2,
            LVL_PROBLEM = 3,
            LVL_FATAL = 4,
        };

    private:
        Logger::Level _level;
        std::ostream* _output;
        static const Logger::Level _default_level;
        static std::ostream* const _default_output;
        static char const* const _level_strings[];
        Timer _timer;

    protected:
        Logger();
        ~Logger();
        void _log(Logger::Level level, std::string const& message);

    public:
        /// Défini la sortie du logger (commune pour tous les niveaux)
        void setOutput(std::ostream& output);

        /// Défini le niveau a partir duquel les messages sont loggés
        void setLevel(Logger::Level level);

        /// Logge un message de niveau DEBUG
        void debug(std::string const& s);

        /// Logge un message de niveau INFO
        void info(std::string const& s);

        /// Logge un message de niveau WARNING
        void warn(std::string const& s);

        /// Logge un message de niveau ERROR
        void error(std::string const& s);

        /// Logge un message de niveau FATAL
        void fatal(std::string const& s);


        template<typename T> static std::string toString(const T& t)
        {
            return LOGGER::Stringify<T>::toString(t);
        }

    };

}

#endif /* !__LOGGER_HPP__ */

