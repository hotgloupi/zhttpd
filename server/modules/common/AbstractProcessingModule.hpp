
#ifndef __ABSTRACTPROCESSINGMODULE_HPP__
# define __ABSTRACTPROCESSINGMODULE_HPP__

# include <cassert>

# include "AbstractModule.hpp"
# include "PostData.hpp"

namespace zhttpd
{
    namespace mod
    {
        class IAbstractProcessingModule
        {
        public:
            virtual bool processOnRequestReady(zhttpd::api::IRequest& request,
                                               zhttpd::mod::PostData* post_data) = 0;
        };

        template<bool handle_post_data>
        class AbstractProcessingModule : public IAbstractProcessingModule, public AbstractModule
        {
        private:
            PostData* _post_data;

        public:
            AbstractProcessingModule() : _post_data(0) {}
            virtual ~AbstractProcessingModule()
            {
                delete this->_post_data;
                this->_post_data = 0;
            }

        protected:
            virtual bool _processOnRequestData(zhttpd::api::IRequest& request,
                                               zhttpd::api::IBuffer& buffer)
            {
                if (this->_post_data == 0)
                    this->_post_data = new PostData(request);
                this->_post_data->append(request, buffer);
                return true;
            }

            virtual bool _processOnEnd(zhttpd::api::IRequest& request)
            {
                if (this->_post_data != 0)
                    this->_post_data->prepare(request);
                return this->processOnRequestReady(request, this->_post_data);
            }
        };

        template<> class AbstractProcessingModule<false> : public IAbstractProcessingModule, public AbstractModule
        {
        protected:
            virtual bool _processOnRequestData(zhttpd::api::IRequest&,
                                               zhttpd::api::IBuffer&)
            {
                throw std::runtime_error("This module should not receive any post data");
                return false;
            }

            virtual bool _processOnEnd(zhttpd::api::IRequest& request)
            {
                return this->processOnRequestReady(request, 0);
            }
        };
    }
}

#endif /* !__ABSTRACTPROCESSINGMODULE_HPP__ */

