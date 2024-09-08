#include <QApplication>
#include <QPushButton>
#include <QElapsedTimer>

#include "src/table_view.h"
#include "src/data/data.h"
#include "src/data/table_data.h"

#include <windows.h>
#include <QFile>
#include <QDataStream>

TableView *view() {
    static TableView view;
    return &view;
}

LONG CALLBACK MyExceptionHandler(struct _EXCEPTION_POINTERS* ExceptionInfo) {
    // 记录异常信息
    qDebug() << "Application crashed with exception code:"
             << QString::number(ExceptionInfo->ExceptionRecord->ExceptionCode);

    // 可以在这里保存用户数据
    QFile file("./cache.txt");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(view()->serializeData());
        file.close();
    } else {
        qDebug() << "Failed to save cache data.";
    }

    // 返回1表示异常已被处理
    return 1;
}

QByteArray RestoreUserData(bool &flag) {
    flag = false;
    QFile file("./cache.txt");
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            file.close();
            flag = true;
            return data;
        }
    }
    return {};
}

int main(int argc, char *argv[])
{
    SetUnhandledExceptionFilter(MyExceptionHandler);
    QApplication a(argc, argv);


    TableView *view = ::view();
//
//    bool flag;
//    QByteArray data = RestoreUserData(flag);
//    if (flag) {
//        view->deserializeData(data);
//    }
//
    view->setMinimumSize(800, 600);
    view->showNormal();


    return QApplication::exec();
}
