
#ifndef __POSTDATA_HPP__
# define __POSTDATA_HPP__

# include <string>
# include <list>
# include <map>
# include <cassert>

# include "api/IRequest.hpp"

namespace zhttpd
{
    namespace mod
    {
        template<bool> class AbstractProcessingModule;

        class PostData
        {
            friend class AbstractProcessingModule<true>;
        private:
            typedef std::list<zhttpd::api::IBuffer*> buflist_t;

        public:
            typedef std::map<std::string, std::list<std::string> > fields_t;

        public:
            fields_t fields;

        private:
            buflist_t _post_data;
            zhttpd::api::IRequest& _request;

        public:
            PostData(zhttpd::api::IRequest& request) : _request(request) {}
            virtual ~PostData() { this->clear(); }

        protected:
            void append(zhttpd::api::IRequest& request, zhttpd::api::IBuffer& buffer)
            {
                assert(&this->_request == &request && "Module used with different requests ! Use Statefull Manager !");
                this->_post_data.push_back(&buffer);
                //LOG_DEBUG("Received data: '" + std::string(buffer.getRawData(), buffer.getSize()) + "'");
            }

            void prepare(zhttpd::api::IRequest& request)
            {
                assert(&this->_request == &request && "Module used with different requests ! Use Statefull Manager !");
                buflist_t::iterator it = this->_post_data.begin(),
                                    end = this->_post_data.end();

                std::string key;
                std::string value;
                bool read_key = true;
                for (; it != end; ++it)
                {
                    unsigned int size = (*it)->getSize();
                    const char* data = (*it)->getRawData();
                    unsigned int i = 0;
                    while (i < size)
                    {
                        if (read_key)
                        {
                            while (i < size && data[i] != '=' && data[i] != '&')
                            {
                                i = this->_readChar(key, data, i, size);
                            }
                            if (i < size && data[i] == '&')
                                read_key = false;
                        }
                        if (i < size && data[i] == '=')
                        {
                            ++i;
                            while (i < size && data[i] != '&')
                            {
                                i = this->_readChar(value, data, i, size);
                            }
                            if (i < size)
                                read_key = true;
                            ++i;
                        }
                        if (read_key && i < size)
                        {
                            this->fields[key].push_back(value);
                            key.clear();
                            value.clear();
                        }
                    }
                    if (read_key && key.size() > 0)
                    {
                        this->fields[key].push_back(value);
                        key.clear();
                        value.clear();
                    }
                }
                this->clear();
                /*
                { // debug part
                    fields_t::iterator it = this->fields.begin(),
                        end = this->fields.end();
                    for (; it != end; ++it)
                    {
                        std::cout << "\t* Key: '" << (*it).first << "' Value: '"
                            << (*it).second.front() << "'" << std::endl;
                    }
                }
                */
            }

        private:
            void clear()
            {
                buflist_t::iterator it = this->_post_data.begin(),
                    end = this->_post_data.end();
                for (; it != end; ++it)
                {
                    this->_request.getBufferManager().release(*it);
                }
                this->_post_data.clear();
            }

            unsigned int _readChar(std::string& s, char const* data, unsigned int i, unsigned int size)
            {
                switch (data[i])
                {
                case '\0':
                    s += "\\0";
                    break;
                case '+':
                    s.push_back(' ');
                    break;
                case '%':
                    if (i + 2 < size)
                    {
# define IS_HEX(c) ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
# define HEX_TO_DEC(c) (\
    c >= '0' && c <= '9' ? c - '0' : (\
        c >= 'a' && c <= 'f' ? c - 'a' + 10 : (\
            c >= 'A' && c <= 'F' ? c - 'A' + 10 : '%')\
        )\
    )
                        if (IS_HEX(data[i + 1]) && IS_HEX(data[i + 2]))
                        {
                            s.push_back(HEX_TO_DEC(data[i + 1]) * 16 + HEX_TO_DEC(data[i + 2]));
                            return i + 3;
                        }
# undef HEX_TO_DEC
# undef IS_HEX
                    } // else push '%' into the string
                default:
                    s.push_back(data[i]);
                }
                return i + 1;
            }
        };
    }
}

#endif /* !__POSTDATA_HPP__ */

