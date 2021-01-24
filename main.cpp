#include <QCoreApplication>

#include <QDebug>

#include "qringbuffer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QRingBuffer buf(0);
    buf.setChunkSize(0);
    buf.append("Hello");
    buf.append("Qt");
    buf.reserve(10);
    qDebug() << buf.chunkSize() << buf.size() << buf.read() << buf.read();

    return a.exec();
}
