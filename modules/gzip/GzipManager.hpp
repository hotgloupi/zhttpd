
#ifndef __GZIPMANAGER_HPP__
# define __GZIPMANAGER_HPP__

#include <string>

#include "server/modules/common/AbstractManager.hpp"

#include "Gzip.hpp"

class GzipManager : public zhttpd::mod::StatefullManager<Gzip>
{
    public:
        GzipManager() :
            zhttpd::mod::StatefullManager<Gzip>("mod_gzip", zhttpd::api::category::COMPRESS)
        {
        }

        virtual bool isRequired(zhttpd::api::IRequest const& request) const
        {
            if (request.getRequestHeader("Accept-Encoding").find("gzip") != std::string::npos ||
                request.getRequestHeader("Accept-Encoding").find("deflate") != std::string::npos)
                return true;
            return false;
        }
};

#endif

