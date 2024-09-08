/**
  ********************************************************************************
  * @file           : table_model.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/10
  ********************************************************************************
  */

#ifndef HIGHPERFORMANCETABLEVIEW_TABLE_MODEL_H
#define HIGHPERFORMANCETABLEVIEW_TABLE_MODEL_H

#include <QAbstractItemModel>
#include <QColor>
#include "data/table_data.h"

class TableModel : public QAbstractTableModel {
Q_OBJECT
public:
    explicit TableModel(QObject *parent = nullptr);
    ~TableModel() override;

    QByteArray serializeData() const;
    void deserializeData(const QByteArray &data);

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;

    void resize(qsizetype row, qsizetype column) {
        m_dataMap.adjustSize(row, column);
    }

protected:
    TableData m_dataMap;
};


#endif //HIGHPERFORMANCETABLEVIEW_TABLE_MODEL_H
