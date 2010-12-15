#ifndef __RCON_HPP__
#define __RCON_HPP__

#include <QApplication>
#include <QSettings>
#include "MainWindow.hpp"
#include "ConnectWindow.hpp"
#include "Network.hpp"

namespace zhttpd
{
    namespace RCON
    {

        class Rcon
        {
            public:
                explicit Rcon(int ac, char** av);
                ~Rcon();
                int run();
                MainWindow& getMainWindow();
                ConnectWindow& getConnectWindow();
                Network& getNetwork();
                QSettings& getSettings();
            private:
                QApplication _app;
                QSettings _settings;
                MainWindow* _main_window;
                ConnectWindow* _connect_window;
                Network* _network;
        };

    }
}

#endif

