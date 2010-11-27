#ifndef __PREOUTPUTBUILDER_HPP__
#define __PREOUTPUTBUILDER_HPP__

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"
#include "api/IModuleManager.hpp"

namespace zhttpd
{
	namespace mod
	{

		class PreOutputBuilderManager;

		class PreOutputBuilder : public zhttpd::api::IModule
		{
			private:
				PreOutputBuilderManager* _manager;
				bool _chunked;
				bool _headers;
				void _sendHeaders(api::IRequest* request);
				void _sendChunkInfo(api::IRequest* request, api::IBuffer* buffer);
			public:
				PreOutputBuilder(api::IModuleManager* manager);
				~PreOutputBuilder();
				bool processRequest(api::event::Type event, api::IRequest* request, api::IBuffer* buffer);
		};

	}
}

#endif

