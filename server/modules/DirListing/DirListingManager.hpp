
#ifndef __DIRLISTINGMANAGER_HPP__
# define __DIRLISTINGMANAGER_HPP__

# include "modules/common/AbstractManager.hpp"
# include "utils/Path.hpp"

# include "DirListing.hpp"

namespace zhttpd
{
    namespace mod
    {
        class DirListingManager : public StatefullManager<DirListing>
        {
            private:
            public:
                DirListingManager() :
                    StatefullManager<DirListing>("mod_dirlisting", zhttpd::api::category::PROCESSING)
                {
                }

                bool isRequired(zhttpd::api::IRequest const& req) const
                {
                    if (zhttpd::Path(req.getFilePath()).isDirectory())
                    {
                        LOG_DEBUG("It is a directory !");
                    }
                    else
                    {
                        LOG_DEBUG("It is NOT a directory !");
                    }
                    return zhttpd::Path(req.getFilePath()).isDirectory();
                }
        };
    }
}

#endif

