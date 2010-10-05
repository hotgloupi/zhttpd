#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include "ui_main.h"

namespace ZHTTPD
{
    namespace RCON
    {

        class Rcon;
        class GraphWidget;

        class MainWindow :
            public QMainWindow, private Ui::MainWindow
        {
            Q_OBJECT
            public:
                explicit MainWindow(Rcon& rcon);
                ~MainWindow();
                void message(QString const& message);
                void setConnectedMode(bool enabled);
                void parseMessage(QString const& message);
            private slots:
                void on_actionConnect_triggered();
                void on_actionDisconnect_triggered();
                void on_actionQuit_triggered();
                void on_actionStop_triggered();
                void on_actionReload_triggered();
                void _getStats();
            private:
                virtual void closeEvent(QCloseEvent*);
                Rcon& _rcon;
                GraphWidget* _uploadGraph;
                GraphWidget* _downloadGraph;
                GraphWidget* _requestsGraph;
                GraphWidget* _buffersGraph;
                GraphWidget* _memoryGraph;
        };

    }
}

#endif

