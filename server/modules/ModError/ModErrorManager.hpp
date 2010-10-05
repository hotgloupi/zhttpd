
#ifndef __MODERRORMANAGER_HPP__
# define __MODERRORMANAGER_HPP__

# include "modules/common/AbstractManager.hpp"

# include "ModError.hpp"

namespace ZHTTPD
{
    namespace MOD
    {
        class ModErrorManager :
            public StatelessManager<ModError>
        {
        private:

        public:
            ModErrorManager() :
                StatelessManager<ModError>("mod_error", ZHTTPD::API::CATEGORY::PROCESSING)
            {
            }

            virtual bool isRequired(ZHTTPD::API::IRequest const& ) const
            {
                LOG_FATAL("");
                return false;
            }
        };
    }
}

#endif /* !__MODERRORMANAGER_HPP__ */

