// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QDir>
#include <QScreen>
#include <QHttpServer>

#include "httpwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    HttpWindow httpWin;
    const QRect availableSize = httpWin.screen()->availableGeometry();
    httpWin.resize(availableSize.width() / 5, availableSize.height() / 5);
    httpWin.move((availableSize.width() - httpWin.width()) / 2, (availableSize.height() - httpWin.height()) / 2);
#ifdef Q_OS_ANDROID
    httpWin.showMaximized();
#else
    httpWin.show();
#endif

    QHttpServer httpServer; //http 서버 변수 선언

    httpServer.route("/", []() {
        //return "Hello world";           //웹 페이지에 출력할 구문
        QString str = "hello";
        return str;
    });

    httpServer.afterRequest([](QHttpServerResponse &&resp) {
        resp.setHeader("Server", "Super server!");
        return std::move(resp);
    });

    const auto port = httpServer.listen(QHostAddress::Any);
    if (!port) {
        qDebug() << QCoreApplication::translate(
                "QHttpServerExample", "Server failed to listen on a port.");
        return 0;
    }

    qDebug() << QCoreApplication::translate(
            "QHttpServerExample", "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port);

    return app.exec();
}
