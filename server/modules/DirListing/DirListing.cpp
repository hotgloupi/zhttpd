
#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModuleManager.hpp"
#include "api/IModule.hpp"
#include "utils/Path.hpp"
#include "utils/Logger.hpp"
#include "utils/macros.hpp"

#include "DirListing.hpp"

using namespace zhttpd;
using namespace zhttpd::mod;

DirListing::DirListing(api::IModuleManager*)
{
}

DirListing::~DirListing()
{
}

bool DirListing::processRequest(api::event::Type event, api::IRequest* request, api::IBuffer*)
{
    if (event == api::event::ON_END)
    {
        Path dir(request->getFilePath());
        std::list<std::string> files = dir.getDirectoryContent();
        std::string slash("/");
        std::string const& path = request->getRequestQuery(); // XXX stop at '?' or '#'
        if (path == "/")
            slash.clear();
        std::string out =
            "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
            "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"fr\">\n"
                "<head>\n"
                    "<title>Index of " + path + "</title>\n"
                    "<style type=\"text/css\">\n"
                        "body { font-family: Verdana; font-size: 13px; }\n"
                        "a:link { color: #c00; text-decoration: none; }\n"
                        "a:hover, a:active, a:focus { color: #c00; background-color: #ccc; text-decoration: none; }\n"
                        "a:visited { color: #c00; text-decoration: underline; }\n"
                    "</style>\n"
                "</head>\n"
                "<body>\n"
                    "<h3> Index of " + path + "</h3>\n"
                    "<hr />\n"
                    "<ul>\n"
                        "<li><a href=\"" + path + slash + "../\">..</a></li>";

        std::list<std::string>::const_iterator it = files.begin();
        std::list<std::string>::const_iterator itEnd = files.end();
        for (; it != itEnd; ++it)
            out += "<li><a href=\"" + path + slash + *it + "\">" + *it + "</a></li>\n"; // XXX replace '"' in path !

        out +=      "</ul>\n"
                    "<hr />\n"
                    "<p>"
                        ZHTTPD_FULLNAME
                    "</p>\n"
                "</body>\n"
            "</html>";
        request->giveData(request->getBufferManager().allocate(out.data(), out.size()));
        request->raiseEnd();
        return true;
    }
    return false;
}

