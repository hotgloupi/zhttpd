#include "Rcon.hpp"

namespace ZHTTPD
{
    namespace RCON
    {

        Rcon::Rcon(int ac, char** av) :
            _app(ac, av), _settings("rcon.ini", QSettings::IniFormat)
        {
            this->_network = new Network(*this);
            this->_main_window = new MainWindow(*this);
            this->_connect_window = new ConnectWindow(*this);
        }

        Rcon::~Rcon()
        {
            delete this->_network;
            delete this->_connect_window;
            delete this->_main_window;
        }

        int Rcon::run()
        {
            this->_main_window->show();
            return this->_app.exec();
        }

        QSettings& Rcon::getSettings()
        {
            return this->_settings;
        }

        Network& Rcon::getNetwork()
        {
            return *this->_network;
        }

        MainWindow& Rcon::getMainWindow()
        {
            return *this->_main_window;
        }

        ConnectWindow& Rcon::getConnectWindow()
        {
            return *this->_connect_window;
        }

    }
}

