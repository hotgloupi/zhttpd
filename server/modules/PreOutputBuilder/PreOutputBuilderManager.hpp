
#ifndef __PREOUTPUTBUILDERMANAGER_HPP__
# define __PREOUTPUTBUILDERMANAGER_HPP__

# include "modules/common/AbstractManager.hpp"
# include "PreOutputBuilder.hpp"

namespace zhttpd
{
    namespace mod
    {
        class PreOutputBuilderManager : public StatefullManager<PreOutputBuilder>
        {
            private:

            public:
                PreOutputBuilderManager() :
                    StatefullManager<PreOutputBuilder>("mod_preoutputbuilder", zhttpd::api::category::PREOUTPUT)
            {
            }
        };
    }
}

#endif
