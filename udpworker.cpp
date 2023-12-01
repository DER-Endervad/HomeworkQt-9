#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    sizeSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);
    sizeSocket->bind(QHostAddress::LocalHost, BIND_PORT2);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
    connect(sizeSocket, &QUdpSocket::readyRead, this, &UDPworker::readSizeDatagrams);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

void UDPworker::ReadDatagramSize(QNetworkDatagram datagram, QString ip)
{
    QByteArray data;
    data = datagram.data();
    QDataStream inStr(&data, QIODevice::ReadOnly);
    int size;
    inStr >> size;
    emit sig_sendSize(size, ip);
}

void UDPworker::SendDatagramSize(QByteArray data)
{
    sizeSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT2);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}

void UDPworker::readSizeDatagrams( void )
{
    QNetworkDatagram datagram = sizeSocket->receiveDatagram();
    QString ip = datagram.senderAddress().toString();
    ReadDatagramSize(datagram, ip);
}
