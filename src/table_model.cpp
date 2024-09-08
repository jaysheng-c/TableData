/**
  ********************************************************************************
  * @file           : table_model.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/10
  ********************************************************************************
  */

#include "table_model.h"
#include <QFontComboBox>

namespace {
    constexpr int DEFAULT_COLUMN_NUM = 50;
    constexpr int DEFAULT_ROW_NUM = 100;

    QList<Data> gBackupList { Data() };
}

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_dataMap.adjustSize(DEFAULT_ROW_NUM, DEFAULT_COLUMN_NUM);
}

TableModel::~TableModel() {
    m_dataMap.clear();
}

QByteArray TableModel::serializeData() const
{
    return m_dataMap.serializeData(true, 9);
}

void TableModel::deserializeData(const QByteArray &data)
{
    beginResetModel();
    m_dataMap.deserializeData(data, true);
    endResetModel();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return parent.row();
    }
    return static_cast<int>(m_dataMap.rowCount());
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return parent.column();
    }
    return static_cast<int>(m_dataMap.columnCount());
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_dataMap.rowCount() || index.column() >= m_dataMap.columnCount()) {
        return false;
    }
    int row = index.row();
    int column = index.column();
    if (row >= m_dataMap.rowCount()) {
        m_dataMap.adjustRow(row + 1);
    }
    if (column >= m_dataMap.columnCount()) {
        m_dataMap.adjustColumn(column + 1);
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (m_dataMap.reference(row, column).use_count() > 1) {
            std::shared_ptr<Data> temp(new Data(value.toString()));
            TableData::backupData(temp);
            m_dataMap.reference(row, column) = temp;
        } else {
            m_dataMap.reference(row, column)->setData(DataSpace::Display, value);
        }
        return true;
    }
    return QAbstractItemModel::setData(index, value, role);
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_dataMap.rowCount() || index.column() >= m_dataMap.columnCount()) {
        return {};
    }
    int row = index.row();
    int column = index.column();
    if (row >= m_dataMap.rowCount() || column >= m_dataMap.columnCount()) {
        return {};
    }
    if (role == Qt::DisplayRole) {
        return m_dataMap.at(row, column).data(DataSpace::Display);
    }
    return {};
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QAbstractItemModel::headerData(section, orientation, role);
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row > rowCount() || count <= 0)
        return false;
//    QSignalBlocker blocker(this);
    beginInsertRows(QModelIndex(), row, row + count - 1);
    m_dataMap.insertRows(row, count);
    endInsertRows();

    return true;
}

bool TableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    return QAbstractItemModel::insertColumns(column, count, parent);
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row + count > rowCount() || count <= 0)
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    m_dataMap.removeRows(row, count);
    endRemoveRows();


    return true;
}

bool TableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    return QAbstractItemModel::removeColumns(column, count, parent);
}

