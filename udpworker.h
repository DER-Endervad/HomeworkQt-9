#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define BIND_PORT2 12342

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );

    void ReadDatagramSize(QNetworkDatagram datagram, QString ip);
    void SendDatagramSize(QByteArray data );

private slots:
    void readPendingDatagrams(void);
    void readSizeDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket* sizeSocket;

signals:
    void sig_sendTimeToGUI(QDateTime size);
    void sig_sendSize(int data, QString ip);
};

#endif // UDPWORKER_H
