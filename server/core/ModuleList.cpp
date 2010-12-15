
#include <cassert>

#include "ModuleList.hpp"

using namespace zhttpd;

ModuleList::ModuleList() : _head(0), _tail(0)
{
}

ModuleList::~ModuleList()
{
    ModuleWrapper* node = this->_head;
    ModuleWrapper* next;

    while (node != 0)
    {
        next = node->getNext();
        delete node;
        node = next;
    }
}

ModuleWrapper* ModuleList::append(api::IModuleManager& manager, api::IModule& module)
{
    if (this->_tail == 0)
    {
        this->_head = this->_tail = new ModuleWrapper(manager, module);
    }
    else
    {
        ModuleWrapper* wrapper = new ModuleWrapper(manager, module);
        this->_tail->setNext(wrapper);
        this->_tail = wrapper;
    }
    return this->_tail;
}

ModuleWrapper* ModuleList::setProcessingModule(api::IModuleManager& manager,
                                   api::IModule& module)
{
    ModuleWrapper* node = this->_head;
    ModuleWrapper* next;

    while (node != 0)
    {
        next = node->getNext();
        if (next->getModuleManager().getCategory() == api::category::PROCESSING)
        {
            ModuleWrapper* backup = next->getNext();
            delete next;
            next = new ModuleWrapper(manager, module);
            next->setNext(backup);
            node->setNext(0);
            node->setNext(next);
            return next;
        }
        else if (next->getModuleManager().getCategory() == api::category::INPUTOUTPUT)
        {
            ModuleWrapper* m = new ModuleWrapper(manager, module);
            node->setNext(0);
            node->setNext(m);
            m->setNext(next);
            return m;
        }
        node = next;
    }
    return 0;
}

ModuleWrapper* ModuleList::head()
{
    return this->_head;
}

ModuleWrapper* ModuleList::tail()
{
    return this->_tail;
}

bool ModuleList::isInResponsePart(ModuleWrapper* wrapper)
{
    ModuleWrapper* node = this->_head;

    while (node != 0)
    {
        if (node->getModuleManager().getCategory() == api::category::PROCESSING)
            return true;
        else if (node == wrapper)
            return false;
        node = node->getNext();
    }
    assert(false); // Le module devrait être dans la chaîne !
    return false;
}
