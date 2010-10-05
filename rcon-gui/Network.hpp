#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

#include <QTcpSocket>

namespace ZHTTPD
{
    namespace RCON
    {

        class Rcon;

        class Network :
            public QObject
        {
            Q_OBJECT
            public:
                explicit Network(Rcon& rcon);
                ~Network();
                void connectToHost(QString const& host, quint16 port);
                void disconnectFromHost();
                void sendCommand(QString const& command);
                bool isConnected() const;
            private slots:
                void _socketReady();
                void _socketConnected();
                void _socketDisconnected();
                void _socketError(QAbstractSocket::SocketError error);
            private:
                Rcon& _rcon;
                QTcpSocket* _socket;
        };

    }
}

#endif

