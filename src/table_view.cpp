/**
  ********************************************************************************
  * @file           : table_view.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/10
  ********************************************************************************
  */

#include "table_view.h"

#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QElapsedTimer>

TableView::TableView(QWidget *parent) : QTableView(parent)
{
    model = new TableModel(this);
    this->setModel(model);
}

TableView::~TableView()
{
    model->deleteLater();
}

void TableView::copy() {

}

void TableView::paste() {
    auto selection = this->selectionModel()->selection();
    if (selection.isEmpty()) {
        return;
    }

    // 解析剪贴板内容
    QClipboard *clip = QApplication::clipboard();
    auto content = clip->text();
    if(content.isEmpty()) {
        return;
    }
    QList<QStringList> dataMap;
    QStringList rowDataList = content.split('\n');
    if (rowDataList.size() > 1) {
        // excel中复制的时候, 会在最后一行增加一行空行
        if (rowDataList[rowDataList.size() - 1].isEmpty()) {
            rowDataList.remove(rowDataList.size() - 1);
        }
    }
    qDebug() << rowDataList[0];
    for (const auto &data : rowDataList) {
        dataMap.append(data.split('\t'));
    }

    qDebug() << dataMap[0].size() << dataMap[0];
    qsizetype div = dataMap.size() - model->rowCount();
    if (div > 0) {
        model->insertRows(model->rowCount(), static_cast<int>(div));
    }
    auto rowCount = qMax(model->rowCount(), dataMap.size());
    auto colCount = qMax(model->columnCount(), dataMap[0].size());
    model->resize(rowCount, colCount);
    QElapsedTimer timer;
    timer.start();
    for (int row = 0; row < dataMap.size(); ++row) {
        for (int column = 0; column < dataMap[row].size(); ++column) {
            model->setData(model->index(row, column), dataMap[row][column], Qt::EditRole);
        }
    }
    qDebug() << model->rowCount() << timer.elapsed() << "ms" << TableData::backupDataSize();
//    TableData::print();
}

#include <windows.h>

void TableView::cut() {
    try {
        int *p = nullptr;
        *p = 5; // 触发非法内存访问
    } catch( ... ) {
        qDebug() << "out of range";
    }
}

QByteArray TableView::serializeData() const
{
    return model->serializeData();
}

void TableView::deserializeData(const QByteArray &data)
{
    model->deserializeData(data);
}


void TableView::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        switch(event->key()) {
            case Qt::Key::Key_C: copy(); break;
            case Qt::Key::Key_V: paste(); break;
            case Qt::Key::Key_X: cut(); break;
            default:
                break;
        }
        this->update();
    }
    QAbstractItemView::keyPressEvent(event);
}