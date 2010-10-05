
#ifndef __ITWORKS_HPP
# define __ITWORKS_HPP

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModule.hpp"


class ItWorks : public ZHTTPD::API::IModule
{
private:
    int _count;

public:
    ItWorks();
    bool processRequest(ZHTTPD::API::EVENT::Type event, ZHTTPD::API::IRequest* request, ZHTTPD::API::IBuffer* buffer);
};

#endif      /* !__ITWORKS_H_*/
