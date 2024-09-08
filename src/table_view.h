/**
  ********************************************************************************
  * @file           : TableView.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/10
  ********************************************************************************
  */

#ifndef PLUGINSYSTEM_TABLEVIEW_H
#define PLUGINSYSTEM_TABLEVIEW_H

#include <QTableView>
#include "table_model.h"

class TableView : public QTableView {
Q_OBJECT
public:
    explicit TableView(QWidget *parent = nullptr);
    ~TableView() override;

    void copy();
    void paste();
    void cut();

    QByteArray serializeData() const;
    void deserializeData(const QByteArray &data);


    void keyPressEvent(QKeyEvent *event) override;

    TableModel *model;
};


#endif //PLUGINSYSTEM_TABLEVIEW_H
