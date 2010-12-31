
#include "Builder.hpp"

#include "core/Request.hpp"
#include "core/ConfigurationManager.hpp"
#include "api/IModuleManager.hpp"
#include "configuration/VHost.hpp"
#include "utils/Path.hpp"

#ifdef _WIN32
#undef min
#endif // _WIN32

using namespace zhttpd::mod;

Builder::Builder(zhttpd::api::IModuleManager*) : _builded(false) {}

bool Builder::processRequest(zhttpd::api::event::Type event,
                             zhttpd::api::IRequest* request,
                             zhttpd::api::IBuffer* buffer)
{
    bool error = false;
    if (!this->_builded)
    {
        Request* req = reinterpret_cast<Request*>(request);
        assert(req != 0);
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
            request->raiseError(api::http_code::NOT_FOUND);
            LOG_ERROR("No vhost found");
            this->_builded = true;
            error = true;
        }
        this->_builded = true;
    }
    if (event == zhttpd::api::event::ON_REQUEST_DATA)
    {
        if (error)
            request->getBufferManager().release(buffer);
        else
            request->giveData(buffer);
        return true;
    }
    return false;
}

void Builder::_findModules(zhttpd::api::IRequest* request, zhttpd::Configuration* config, zhttpd::VHost* vhost)
{
    this->_addModule(zhttpd::api::category::PREOUTPUT, config, vhost, request);
    this->_addModule(zhttpd::api::category::POSTRESPONSEFILTER, config, vhost, request);
    this->_addModule(zhttpd::api::category::COMPRESS, config, vhost, request);
    this->_addModule(zhttpd::api::category::PRERESPONSEFILTER, config, vhost, request);
    this->_addModule(zhttpd::api::category::POSTPROCESSING, config, vhost, request);
    this->_addModule(zhttpd::api::category::PROCESSING, config, vhost, request);
    this->_addModule(zhttpd::api::category::PREPROCESSING, config, vhost, request);
    this->_addModule(zhttpd::api::category::POSTREQUESTFILTER, config, vhost, request);
    this->_addModule(zhttpd::api::category::UNCOMPRESS, config, vhost, request);
    this->_addModule(zhttpd::api::category::PREREQUESTFILTER, config, vhost, request);
}

void Builder::_addModule(zhttpd::api::category::Type category, zhttpd::Configuration* config, zhttpd::VHost* vhost, zhttpd::api::IRequest* request)
{
    Request* r = dynamic_cast<Request*>(request);
    zhttpd::Configuration::available_modules_t const& mods = config->getAvailableModules();
    zhttpd::Configuration::available_modules_t::const_iterator cat_mods = mods.find(category);
    if (cat_mods != mods.end())
    {
        std::list<std::string>::const_iterator it = (cat_mods->second).begin();
        std::list<std::string>::const_iterator ite = (cat_mods->second).end();
        while (it != ite)
        {
            if (*it != "mod_filereader")
            {
                ModuleConfiguration& mod_conf = vhost->getModuleConfiguration(*it);
                if (mod_conf.isEnabled())
                {
                    zhttpd::api::IModuleManager* mod_manager = mod_conf.getModuleManager();
                    assert(mod_manager != 0);
                    if (mod_manager->isRequired(*request))
                    {
                        r->insertAfter(*mod_manager, *mod_manager->getInstance());
                        LOG_DEBUG("Adding module " + mod_manager->getName() + " in chain");
                        return ;
                    }
                }
            }
            ++it;
        }
    }
    if (category == zhttpd::api::category::PROCESSING)
    {
        ModuleConfiguration& mod_conf = vhost->getModuleConfiguration("mod_filereader");
        if (mod_conf.isEnabled())
        {
            zhttpd::api::IModuleManager* mod_manager = mod_conf.getModuleManager();
            assert(mod_manager != 0);
            if (mod_manager->isRequired(*request))
            {
                r->insertAfter(*mod_manager, *mod_manager->getInstance());
                LOG_DEBUG("Adding module " + mod_manager->getName() + " in chain");
                return ;
            }
        }
        // only reached when filereader is not available
        request->raiseError(zhttpd::api::http_code::FORBIDDEN);
    }
}

void Builder::_setPath(zhttpd::api::IRequest* request, zhttpd::Configuration* config, zhttpd::VHost* vhost)
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
    Request* r = dynamic_cast<Request*>(request);
    r->setFilePath(path.getAbsolutePath());
    LOG_DEBUG("Request file path = " + request->getFilePath());
}

