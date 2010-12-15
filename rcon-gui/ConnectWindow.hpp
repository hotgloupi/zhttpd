#ifndef __CONNECTWINDOW_HPP__
#define __CONNECTWINDOW_HPP__

#include "ui_connect.h"

namespace zhttpd
{
    namespace RCON
    {

        class Rcon;

        class ConnectWindow :
            public QMainWindow, private Ui::ConnectWindow
        {
            Q_OBJECT
            public:
                explicit ConnectWindow(Rcon& rcon);
                ~ConnectWindow();
                void showError(QString const& error);
            private slots:
                void on_goPushButton_clicked();
            private:
                virtual void closeEvent(QCloseEvent*);
                virtual void keyPressEvent(QKeyEvent* e);
                Rcon& _rcon;
        };

    }
}

#endif

