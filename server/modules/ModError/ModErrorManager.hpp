
#ifndef __MODERRORMANAGER_HPP__
# define __MODERRORMANAGER_HPP__

# include "modules/common/AbstractManager.hpp"

# include "ModError.hpp"

namespace zhttpd
{
    namespace mod
    {
        class ModErrorManager : public StatelessManager<ModError>
        {
        public:
            ModErrorManager() :
                StatelessManager<ModError>("mod_error", zhttpd::api::category::PROCESSING)
            {
            }

            virtual bool isRequired(zhttpd::api::IRequest const& ) const
            {
                return false;
            }
        };
    }
}

#endif /* !__MODERRORMANAGER_HPP__ */

