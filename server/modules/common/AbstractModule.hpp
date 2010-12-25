/**
 * @file AbstractModule.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ABSTRACTMODULE_HPP__
# define __ABSTRACTMODULE_HPP__

# include "utils/Logger.hpp"
# include "api/IModule.hpp"

namespace zhttpd
{
    namespace mod
    {
        class AbstractModule : public zhttpd::api::IModule
        {
        protected:
            virtual bool _processOnRequestData(zhttpd::api::IRequest&, zhttpd::api::IBuffer&)
            {
                return false;
            }

            virtual bool _processOnResponseData(zhttpd::api::IRequest&, zhttpd::api::IBuffer&)
            {
                return false;
            }

            virtual bool _processOnEnd(zhttpd::api::IRequest&)
            {
                return false;
            }

            virtual bool _processOnIdle(zhttpd::api::IRequest&)
            {
                return false;
            }

            virtual bool _processOnCanRead(zhttpd::api::IRequest&)
            {
                return false;
            }

            virtual bool _processOnCanWrite(zhttpd::api::IRequest&, zhttpd::api::IBuffer&)
            {
                return false;
            }

            virtual bool _processOnError(zhttpd::api::IRequest&)
            {
                return false;
            }
        public:
            virtual bool processRequest(zhttpd::api::event::Type event,
                                        zhttpd::api::IRequest* request,
                                        zhttpd::api::IBuffer* buffer)
            {
                assert(request != 0 && "request pointer is NULL !");
                switch (event)
                {
                case zhttpd::api::event::ON_END:
                    return this->_processOnEnd(*request);

                case zhttpd::api::event::ON_REQUEST_DATA:
                    assert(buffer != 0 && "buffer pointer is NULL for 'ON_REQUEST_DATA' event");
                    return this->_processOnRequestData(*request, *buffer);

                case zhttpd::api::event::ON_CAN_READ:
                    return this->_processOnCanRead(*request);

                case zhttpd::api::event::ON_CAN_WRITE:
                    assert(buffer != 0 && "buffer pointer is NULL for 'ON_CAN_WRITE' event");
                    return this->_processOnCanWrite(*request, *buffer);

                case zhttpd::api::event::ON_RESPONSE_DATA:
                    assert(buffer != 0 && "buffer pointer is NULL for 'ON_RESPONSE_DATA' event");
                    return this->_processOnResponseData(*request, *buffer);

                case zhttpd::api::event::ON_IDLE:
                    return this->_processOnIdle(*request);

                case zhttpd::api::event::ON_ERROR:
                    return this->_processOnError(*request);

                default:
                    throw std::runtime_error("Unknown event type '" + zhttpd::Logger::toString<int>(event) + "'");
                }
            }
        };
    }
}

#endif /* !__ABSTRACTMODULE_HPP__ */

