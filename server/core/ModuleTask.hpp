
#ifndef __MODULETASK_HPP__
# define __MODULETASK_HPP__

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "thread/ITask.hpp"
# include "utils/macros.hpp"

# include "ModuleWrapper.hpp"
# include "Request.hpp"
# include "TaskManager.hpp"

namespace ZHTTPD
{
    class ModuleTask : public ITask
    {
    private:
        API::EVENT::Type    _event;
        Request&            _request;
        ModuleWrapper&      _module;
        API::IBuffer*       _buffer;
        bool                _triggered;

    public:
        ModuleTask(API::EVENT::Type event,
                   Request& request,
                   ModuleWrapper& module,
                   API::IBuffer* buffer) :
            _event(event),
            _request(request),
            _module(module),
            _buffer(buffer),
            _triggered(false)
        {
        }

        virtual ~ModuleTask()
        {
            if (this->_triggered == false && this->_buffer != 0)
            {
                this->_request.getBufferManager().release(this->_buffer);
            }
        }

        void run()
        {
            try
            {
#ifdef DEBUG
                std::string name = this->_module.getModuleManager().getName();
#endif
                ZHTTPD_MOD_START
                this->_request.processTask(this->_event, &this->_module, this->_buffer);
                ZHTTPD_MOD_END(name);
                TaskManager::getInstance()->notifyEndTask(this->_request);
                this->_triggered = true;
            } catch (std::exception& e)
            {
                LOG_ERROR("ModuleTask error handled: " + std::string(e.what()));
                this->_request.getRequestTasks().notifyHasError();
                TaskManager::getInstance()->notifyEndTask(this->_request);
            }
        }
    };
}

#endif /* !__MODULETASK_HPP__ */

