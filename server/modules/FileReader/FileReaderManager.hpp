#ifndef __FILEREADERMANAGER_HPP__
# define __FILEREADERMANAGER_HPP__

# include <map>

# include "modules/common/AbstractManager.hpp"
# include "api/constants.hpp"

# include "FileReader.hpp"

namespace zhttpd
{
    namespace mod
    {

        class FileReaderManager : public StatefullManager<FileReader>
        {
            private:
                std::string _defaultMimeType;
                std::map<std::string, std::string> _types;
                unsigned int _delay;

            public:
                FileReaderManager();
                ~FileReaderManager();
                void addConfigurationEntry(std::string const& key,
                                           std::string const& value);
                api::category::Type getCategory() const;
                bool isRequired(api::IRequest const& request) const;
                std::string const& getMimeType(std::string const& ext) const;
                std::string const& getDefaultMimeType() const;
                unsigned int getDelay() const;
        };

    }
}

#endif
