
#include "BarbazModuleManager.hpp"

BarbazModuleManager::BarbazModuleManager() :
    zhttpd::mod::StatefullManager<BarbazModule>("mod_barbaz", zhttpd::api::category::PROCESSING)
{
}

BarbazModuleManager::~BarbazModuleManager()
{
}

bool BarbazModuleManager::isRequired(zhttpd::api::IRequest const& request) const
{
    return true;
}
