
#ifndef __ITWORKS_HPP
# define __ITWORKS_HPP

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModule.hpp"


class ItWorks : public zhttpd::api::IModule
{
private:
    int _count;

public:
    ItWorks();
    bool processRequest(zhttpd::api::event::Type event, zhttpd::api::IRequest* request, zhttpd::api::IBuffer* buffer);
};

#endif      /* !__ITWORKS_H_*/
