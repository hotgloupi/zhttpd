
#include <iomanip>

#include "Logger.hpp"

using namespace zhttpd;

Logger::Level const Logger::_default_level = Logger::LVL_DEBUG;
std::ostream* const Logger::_default_output = &std::cerr;
char const* const Logger::_level_strings[] = {
    "DEBUG", "INFO", "WARNING", "ERROR", "FATAL",
};

#ifdef _WIN32
const char* colors[] = {
    "",
    "",
    "",
    "",
    "",
};
const char* reset_color = "";
#else
const char* colors[] = {
    "\033[1;30m",
    "\033[34m",
    "\033[1;33m",
    "\033[1;31m",
    "\033[47m\033[1;31m",
};
const char* reset_color = "\033[0m";
#endif

Logger::Logger() : _level(Logger::_default_level), _output(Logger::_default_output)
{
}

Logger::~Logger() {}

void Logger::setOutput(std::ostream& output)
{
    this->info("Changing logger output");
    this->_output = &output;
}

void Logger::setLevel(Logger::Level level)
{
    if (this->_level == level)
        return ;
    *(this->_output) << "[" << this->_level_strings[Logger::LVL_INFO] << "] "
        << "Changing logger level from '"
        << this->_level_strings[this->_level]
        << "' to '"
        << this->_level_strings[level]
        << "'" << std::endl;
    this->_level = level;
}

void Logger::_log(Logger::Level level, std::string const& message)
{
    if (level < this->_level)
    {
        return ;
    }
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3) << std::setw(11) << std::setfill('0')
       << ((long double)this->_timer.getPreciseElapsedTime()) / 1000.0f
       << colors[level]
       << " [" << this->_level_strings[level] << "] "
       << reset_color
       << message
       << std::endl;
    *(this->_output) << ss.str();
}

void Logger::debug(std::string const& s) { this->_log(Logger::LVL_DEBUG, s); }
void Logger::info(std::string const& s) { this->_log(Logger::LVL_INFO, s); }
void Logger::warn(std::string const& s) { this->_log(Logger::LVL_WARNING, s); }
void Logger::error(std::string const& s) { this->_log(Logger::LVL_PROBLEM, s); }
void Logger::fatal(std::string const& s) { this->_log(Logger::LVL_FATAL, s); }

