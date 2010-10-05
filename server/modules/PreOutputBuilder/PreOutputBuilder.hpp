#ifndef __PREOUTPUTBUILDER_HPP__
#define __PREOUTPUTBUILDER_HPP__

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"
#include "api/IModuleManager.hpp"

namespace ZHTTPD
{
	namespace MOD
	{

		class PreOutputBuilderManager;

		class PreOutputBuilder : public ZHTTPD::API::IModule
		{
			private:
				PreOutputBuilderManager* _manager;
				bool _chunked;
				bool _headers;
				void _sendHeaders(API::IRequest* request);
				void _sendChunkInfo(API::IRequest* request, API::IBuffer* buffer);
			public:
				PreOutputBuilder(API::IModuleManager* manager);
				~PreOutputBuilder();
				bool processRequest(API::EVENT::Type event, API::IRequest* request, API::IBuffer* buffer);
		};

	}
}

#endif

