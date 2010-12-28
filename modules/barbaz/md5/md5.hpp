
#ifndef __MD5_HPP__
# define __MD5_HPP__

# include <string>
# include <sstream>
# include <iomanip>

# include "md5.h"

class md5
{
private:
    md5_state_t _state;
    md5_byte_t _digest[16];
    std::string _hexdigest;

public:
    md5(std::string const& str)
    {
        md5_init(&this->_state);
        md5_append(&this->_state, (const md5_byte_t *) str.c_str(), str.size());
        md5_finish(&this->_state, this->_digest);
    }

    std::string const& hexdigest()
    {
        if (this->_hexdigest.size() == 0)
        {
            std::stringstream ss;
            ss.fill('0');
            ss.flags(std::ios::hex);
            for (unsigned int i = 0; i < 16; ++i)
                ss << std::setw(2) << (int) this->_digest[i];
            this->_hexdigest = ss.str();
        }
        return this->_hexdigest;
    }
};

#endif /* !__MD5_HPP__ */

