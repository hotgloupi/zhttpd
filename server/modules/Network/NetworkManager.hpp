
#ifndef __ITWORKSMANAGER_HPP__
# define __ITWORKSMANAGER_HPP__

# include <string>

# include "api/IModuleManager.hpp"
# include "api/constants.hpp"
# include "modules/common/AbstractManager.hpp"

# include "Network.hpp"

namespace zhttpd
{
    namespace mod
    {
        class NetworkManager : public StatelessManager<Network>
        {

        public:
            NetworkManager() : StatelessManager<Network>("mod_network") {}


            void addConfigurationEntry(std::string const&, std::string const&)
            {
            }

            api::category::Type getCategory() const
            {
                return api::category::INPUTOUTPUT;
            }


            bool isRequired(api::IRequest const&) const
            {
                return true;
            }
        };
    }
}
#endif /* !__ITWORKSMANAGER_HPP__ */

