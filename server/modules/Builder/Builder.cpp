
#include "Builder.hpp"

#include "core/Request.hpp"
#include "core/ConfigurationManager.hpp"
#include "api/IModuleManager.hpp"
#include "configuration/VHost.hpp"
#include "utils/Path.hpp"


using namespace ZHTTPD::MOD;

Builder::Builder(ZHTTPD::API::IModuleManager*) : _builded(false) {}

bool Builder::processRequest(ZHTTPD::API::EVENT::Type event,
                             ZHTTPD::API::IRequest* request,
                             ZHTTPD::API::IBuffer* buffer)
{
    bool error = false;
    if (!this->_builded)
    {
#ifdef ZHTTPD_DEBUG
        Request* req = dynamic_cast<Request*>(request);
        assert(req != 0);
#else
        Request* req = reinterpret_cast<Request*>(request);
#endif
        SmartPtr<Configuration> config = req->getConfiguration();
        VHost* vhost = config->getVHost().match(*request);
        if (vhost != 0)
        {
            LOG_DEBUG("VHost found: " + vhost->getName());
            this->_setPath(request, config.get(), vhost);
            this->_findModules(request, config.get(), vhost);
        }
        else
        {
            request->raiseError(API::HTTP_CODE::NOT_FOUND);
            LOG_ERROR("No vhost found");
            this->_builded = true;
            error = true;
        }
        this->_builded = true;
    }
    if (event == ZHTTPD::API::EVENT::ON_REQUEST_DATA)
    {
        if (error)
            request->getBufferManager().release(buffer);
        else
            request->giveData(buffer);
        return true;
    }
    return false;
}

void Builder::_findModules(ZHTTPD::API::IRequest* request, ZHTTPD::Configuration* config, ZHTTPD::VHost* vhost)
{
    LOG_DEBUG("creating modules chain");
    this->_addModule(ZHTTPD::API::CATEGORY::PREOUTPUT, config, vhost, request);
    this->_addModule(ZHTTPD::API::CATEGORY::POSTRESPONSEFILTER, config, vhost, request);
    this->_addModule(ZHTTPD::API::CATEGORY::COMPRESS, config, vhost, request);
    this->_addModule(ZHTTPD::API::CATEGORY::PRERESPONSEFILTER, config, vhost, request);
    this->_addModule(ZHTTPD::API::CATEGORY::POSTPROCESSING, config, vhost, request);
    this->_addModule(ZHTTPD::API::CATEGORY::PROCESSING, config, vhost, request);
    this->_addModule(ZHTTPD::API::CATEGORY::PREPROCESSING, config, vhost, request);
    this->_addModule(ZHTTPD::API::CATEGORY::POSTREQUESTFILTER, config, vhost, request);
    this->_addModule(ZHTTPD::API::CATEGORY::UNCOMPRESS, config, vhost, request);
    this->_addModule(ZHTTPD::API::CATEGORY::PREREQUESTFILTER, config, vhost, request);
}

void Builder::_addModule(ZHTTPD::API::CATEGORY::Type category, ZHTTPD::Configuration* config, ZHTTPD::VHost* vhost, ZHTTPD::API::IRequest* request)
{
    ZHTTPD::Configuration::available_modules_t const& mods = config->getAvailableModules();
    ZHTTPD::Configuration::available_modules_t::const_iterator cat_mods = mods.find(category);
    if (cat_mods != mods.end())
    {
        std::list<std::string>::const_iterator it = (cat_mods->second).begin();
        std::list<std::string>::const_iterator ite = (cat_mods->second).end();
        while (it != ite)
        {
            if (*it == "mod_filereader")
            {
                ++it;
                continue;
            }
            ModuleConfiguration& mod_conf = vhost->getModuleConfiguration(*it);
            if (mod_conf.isEnabled())
            {
                ZHTTPD::API::IModuleManager* mod_manager = mod_conf.getModuleManager();
                assert(mod_manager != 0);
                if (mod_manager->isRequired(*request))
                {
                    Request* r = reinterpret_cast<Request*>(request);
                    r->insertAfter(*mod_manager, *mod_manager->getInstance());
                    LOG_DEBUG("Adding module " + mod_manager->getName() + " in chain");
                    return ;
                }
            }
            ++it;
        }
    }
    if (category == ZHTTPD::API::CATEGORY::PROCESSING)
    {
        ModuleConfiguration& mod_conf = vhost->getModuleConfiguration("mod_filereader");
        if (mod_conf.isEnabled())
        {
            ZHTTPD::API::IModuleManager* mod_manager = mod_conf.getModuleManager();
            assert(mod_manager != 0);
            if (mod_manager->isRequired(*request))
            {
                Request* r = reinterpret_cast<Request*>(request);
                r->insertAfter(*mod_manager, *mod_manager->getInstance());
                LOG_DEBUG("Adding module " + mod_manager->getName() + " in chain");
                return ;
            }
        }
        // only reached when filereader is not available
        request->raiseError(ZHTTPD::API::HTTP_CODE::FORBIDDEN);
    }
}

void Builder::_setPath(ZHTTPD::API::IRequest* request, ZHTTPD::Configuration* config, ZHTTPD::VHost* vhost)
{
    Path path(vhost->getDocumentRoot());
    std::string filepath = request->getRequestQuery();
    std::size_t i = std::min(filepath.find_first_of('?'), filepath.find_first_of('#'));
    if (i != std::string::npos)
        filepath = filepath.substr(0, i);
    path += filepath;
    if (path.isDirectory())
    {
        std::list<std::string> const& index_files = config->getIndexFiles();
        std::list<std::string>::const_iterator it = index_files.begin();
        std::list<std::string>::const_iterator end = index_files.end();
        std::set<std::string> files = path.getDirectoryContentSet();
        for (; it != end; ++it)
        {
            if (files.find(*it) != files.end())
            {
                path += *it;
                break;
            }
        }
    }
    Request* r = reinterpret_cast<Request*>(request);
    r->setFilePath(path.getAbsolutePath());
    LOG_DEBUG("Request file path = " + request->getFilePath());
}

