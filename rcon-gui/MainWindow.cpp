#include "MainWindow.hpp"
#include "Rcon.hpp"
#include "GraphWidget.hpp"
#include <QTimer>
#include <QDate>
#include <QDateTime>

namespace ZHTTPD
{
    namespace RCON
    {

        MainWindow::MainWindow(Rcon& rcon) :
            _rcon(rcon)
        {
            this->setupUi(this);
            this->_rcon.getSettings().beginGroup("MainWindow");
            if (this->_rcon.getSettings().contains("size"))
                this->resize(this->_rcon.getSettings().value("size", QSize(650, 400)).toSize());
            if (this->_rcon.getSettings().contains("pos"))
                this->move(this->_rcon.getSettings().value("pos", QPoint(50, 50)).toPoint());
            this->splitter->restoreState(this->_rcon.getSettings().value("splitter").toByteArray());
            this->_rcon.getSettings().endGroup();
            this->_uploadGraph = new GraphWidget(this->_rcon, this->scrollArea);
			this->verticalLayout->addWidget(this->_uploadGraph);
            this->_uploadGraph->setName("Upload (kb/s)");
            this->_downloadGraph = new GraphWidget(this->_rcon, this->scrollArea);
			this->verticalLayout->addWidget(this->_downloadGraph);
            this->_downloadGraph->setName("Download (kb/s)");
            this->_requestsGraph = new GraphWidget(this->_rcon, this->scrollArea);
			this->verticalLayout->addWidget(this->_requestsGraph);
            this->_requestsGraph->setName("Requests per sec");
            this->_buffersGraph = new GraphWidget(this->_rcon, this->scrollArea);
			this->verticalLayout->addWidget(this->_buffersGraph);
            this->_buffersGraph->setName("Buffers");
            this->_memoryGraph = new GraphWidget(this->_rcon, this->scrollArea);
			this->verticalLayout->addWidget(this->_memoryGraph);
            this->_memoryGraph->setName("Memory (kb)");
            QTimer::singleShot(1000, this, SLOT(_getStats()));
        }

        MainWindow::~MainWindow()
        {
            delete this->_uploadGraph;
            delete this->_downloadGraph;
            delete this->_requestsGraph;
            delete this->_buffersGraph;
            delete this->_memoryGraph;
        }

        void MainWindow::closeEvent(QCloseEvent*)
        {
            if (this->_rcon.getConnectWindow().isVisible())
                this->_rcon.getConnectWindow().close();
            this->_rcon.getSettings().beginGroup("MainWindow");
            this->_rcon.getSettings().setValue("size", this->size());
            this->_rcon.getSettings().setValue("pos", this->pos());
            this->_rcon.getSettings().setValue("splitter", this->splitter->saveState());
            this->_rcon.getSettings().endGroup();
        }

        void MainWindow::_getStats()
        {
            if (this->_rcon.getNetwork().isConnected())
                this->_rcon.getNetwork().sendCommand("get");
            else
            {
                this->_uploadGraph->addValue(0);
                this->_downloadGraph->addValue(0);
                this->_requestsGraph->addValue(0);
                this->_buffersGraph->addValue(0);
                this->_memoryGraph->addValue(0);
            }
            QTimer::singleShot(1000, this, SLOT(_getStats()));
        }

        void MainWindow::parseMessage(QString const& message)
        {
            std::vector<QString> stats;
            stats.push_back(QString());
            QString::const_iterator it = message.begin();
            QString::const_iterator itEnd = message.end();
            for (; it != itEnd; ++it)
                if (*it == ' ')
                    stats.push_back(QString());
                else
                    stats.back() += *it;
            if (stats.size() >= 6)
            {
                // uptime
                QDateTime t(QDate::currentDate());
                t = t.addMSecs(stats[0].toInt());
                this->uptimeLabel->setText("Uptime: " + t.toString("hh:mm:ss"));
                // upload
                this->_uploadGraph->addValue(static_cast<float>(stats[1].toInt()) / 1000.0f);
                // download
                this->_downloadGraph->addValue(static_cast<float>(stats[2].toInt()) / 1000.0f);
                // requests
                this->_requestsGraph->addValue(stats[3].toInt());
                // buffers
                this->_buffersGraph->addValue(stats[4].toInt());
                // memory
                this->_memoryGraph->addValue(static_cast<float>(stats[5].toInt()) / 1000.0f);
            }
        }

        void MainWindow::setConnectedMode(bool enabled)
        {
            if (enabled)
            {
                this->actionConnect->setEnabled(false);
                this->actionDisconnect->setEnabled(true);
                this->actionStop->setEnabled(true);
                this->actionReload->setEnabled(true);
            }
            else
            {
                this->actionConnect->setEnabled(true);
                this->actionDisconnect->setEnabled(false);
                this->actionStop->setEnabled(false);
                this->actionReload->setEnabled(false);
                this->uptimeLabel->setText("");
            }
        }

        void MainWindow::message(QString const& message)
        {
            this->plainTextEdit->appendHtml(message);
        }

        void MainWindow::on_actionStop_triggered()
        {
            this->_rcon.getNetwork().sendCommand("stop");
        }

        void MainWindow::on_actionReload_triggered()
        {
            this->_rcon.getNetwork().sendCommand("reload");
        }

        void MainWindow::on_actionDisconnect_triggered()
        {
            this->_rcon.getNetwork().disconnectFromHost();
        }

        void MainWindow::on_actionConnect_triggered()
        {
            this->_rcon.getConnectWindow().show();
        }

        void MainWindow::on_actionQuit_triggered()
        {
            this->_rcon.getConnectWindow().close();
            this->close();
        }

    }
}

