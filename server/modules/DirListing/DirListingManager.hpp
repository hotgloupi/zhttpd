
#ifndef __DIRLISTINGMANAGER_HPP__
# define __DIRLISTINGMANAGER_HPP__

# include "modules/common/AbstractManager.hpp"
# include "utils/Path.hpp"

# include "DirListing.hpp"

namespace ZHTTPD
{
    namespace MOD
    {
        class DirListingManager : public StatefullManager<DirListing>
        {
            private:
            public:
                DirListingManager() :
                    StatefullManager<DirListing>("mod_dirlisting", ZHTTPD::API::CATEGORY::PROCESSING)
                {
                }

                bool isRequired(ZHTTPD::API::IRequest const& req) const
                {
                    if (ZHTTPD::Path(req.getFilePath()).isDirectory())
                    {
                        LOG_DEBUG("It is a directory !");
                    }
                    else
                    {
                        LOG_DEBUG("It is NOT a directory !");
                    }
                    return ZHTTPD::Path(req.getFilePath()).isDirectory();
                }
        };
    }
}

#endif

