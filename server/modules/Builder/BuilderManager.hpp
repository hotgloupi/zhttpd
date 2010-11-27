
#ifndef __BUILDERMANAGER_HPP__
# define __BUILDERMANAGER_HPP__

# include "modules/common/AbstractManager.hpp"

# include "Builder.hpp"

namespace zhttpd
{
    namespace mod
    {
        class BuilderManager : public StatefullManager<Builder>
        {
            public:
                BuilderManager() : StatefullManager<Builder>("mod_builder") {}
        };
    }
}

#endif /* !__BUILDERMANAGER_HPP__ */

