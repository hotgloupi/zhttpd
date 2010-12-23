
#include "sqlite/Connection.hpp"
#include "BarbazModuleManager.hpp"

BarbazModuleManager::BarbazModuleManager() :
    manager_base_t("mod_barbaz", zhttpd::api::category::PROCESSING)
{
}

BarbazModuleManager::~BarbazModuleManager()
{
}

bool BarbazModuleManager::isRequired(zhttpd::api::IRequest const&) const
{
    return true;
}

db::IConnection* BarbazModuleManager::getNewDBConnection()
{
    LOG_INFO("Database connection to '" + this->_configuration["database"] + '"');
    return sqlite::open(this->_configuration["database"].c_str());
}

void BarbazModuleManager::releaseDBConnection(db::IConnection* conn)
{
    sqlite::close(conn);
}

