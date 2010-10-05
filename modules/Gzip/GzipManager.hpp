#ifndef __GZIPMANAGER_HPP__
#define __GZIPMANAGER_HPP__

#include <string>

#include "server/modules/common/AbstractManager.hpp"

#include "Gzip.hpp"

class GzipManager : public ZHTTPD::MOD::StatefullManager<Gzip>
{
    private:
        std::string _name;
    public:
        GzipManager() :
            ZHTTPD::MOD::StatefullManager<Gzip>("mod_gzip", ZHTTPD::API::CATEGORY::COMPRESS)
        {
        }

        virtual bool isRequired(ZHTTPD::API::IRequest const& request) const
        {
            if (request.getRequestHeader("Accept-Encoding").find("gzip") != std::string::npos ||
                request.getRequestHeader("Accept-Encoding").find("deflate") != std::string::npos)
                return true;
            return false;
        }
};

#endif

