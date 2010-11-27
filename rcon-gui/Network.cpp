#include "Network.hpp"
#include "Rcon.hpp"

namespace zhttpd
{
    namespace RCON
    {

        Network::Network(Rcon& rcon) :
            _rcon(rcon)
        {
            this->_socket = new QTcpSocket(this);
            this->_socket->setTextModeEnabled(true);
            this->connect(this->_socket, SIGNAL(readyRead()), this, SLOT(_socketReady()));
            this->connect(this->_socket, SIGNAL(connected()), this, SLOT(_socketConnected()));
            this->connect(this->_socket, SIGNAL(disconnected()), this, SLOT(_socketDisconnected()));
            this->connect(this->_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(_socketError(QAbstractSocket::SocketError)));
        }

        Network::~Network()
        {
            delete this->_socket;
        }

        bool Network::isConnected() const
        {
            return this->_socket->state() == QAbstractSocket::ConnectedState;
        }

        void Network::connectToHost(QString const& host, quint16 port)
        {
            this->_socket->connectToHost(host, port);
        }

        void Network::disconnectFromHost()
        {
            this->_socket->disconnectFromHost();
        }

        void Network::sendCommand(QString const& command)
        {
            this->_rcon.getMainWindow().message("Sending command: \"" + command + "\".");
            this->_socket->write(command.toAscii());
            this->_socket->write("\n");
            this->_socket->flush();
        }

        void Network::_socketReady()
        {
            if (this->_socket->canReadLine())
            {
                QString line = this->_socket->readLine();
                if (line.size() > 0 && line.at(line.size() - 1) == '\n')
                    line.resize(line.size() - 1);
                this->_rcon.getMainWindow().message("Received line: \"" + line + "\".");
                this->_rcon.getMainWindow().parseMessage(line);
            }
        }

        void Network::_socketConnected()
        {
            this->_rcon.getConnectWindow().close();
            this->_rcon.getMainWindow().message("Connected!");
            this->_rcon.getMainWindow().setConnectedMode(true);
        }

        void Network::_socketDisconnected()
        {
            this->_rcon.getMainWindow().message("Disconnected!");
            this->_rcon.getMainWindow().setConnectedMode(false);
        }

        void Network::_socketError(QAbstractSocket::SocketError)
        {
            this->_rcon.getMainWindow().message("Socket error: " + this->_socket->errorString() + ".");
            if (this->_rcon.getConnectWindow().isVisible())
              this->_rcon.getConnectWindow().showError(this->_socket->errorString());
        }

    }
}

