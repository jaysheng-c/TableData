/**
  ********************************************************************************
  * @file           : table_data.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/11
  ********************************************************************************
  */

#include "table_data.h"
#include <QVariant>
#include <QBuffer>
#include <QSharedPointer>

std::shared_ptr<Data> TableData::s_nullData{new Data()};
QMap<size_t, std::shared_ptr<Data>> TableData::s_backUpDataMap {};


const std::shared_ptr<Data> &TableData::backupData(const std::shared_ptr<Data> &data)
{
    if (*data == *s_nullData) {
        return s_nullData;
    }
    auto key = qHash(data.get());
    if (!s_backUpDataMap.contains(key)) {
        s_backUpDataMap.insert(key, data);
    }
    return s_backUpDataMap[key];
}

QMap<size_t, std::shared_ptr<Data>> &TableData::backupData()
{
    return s_backUpDataMap;
}

qsizetype TableData::backupDataSize()
{
    return s_backUpDataMap.size();
}

void TableData::print()
{
    qDebug() << Q_FUNC_INFO;
    for (const auto &d: s_backUpDataMap) {
        qDebug() << d->data(DataSpace::Type::Display) << d.use_count();
    }
    qDebug() << s_nullData.use_count();
}

TableData::TableData(qsizetype row, qsizetype column, QObject *parent)
        : QObject(parent), m_rowCount(row), m_columnCount(column)
{
    m_data.resize(m_rowCount * m_columnCount);
}

void TableData::clear()
{
    m_rowCount = m_columnCount = 0;
    m_data.clear();
}

void TableData::setData(const matrix2D &data)
{
    clear();
    qsizetype rowCount = data.size();
    qsizetype columnCount {0};
    for (auto &rowData : data) {
        columnCount = qMax(columnCount, rowData.size());
    }

    m_data.resize(rowCount * columnCount);
    for (auto col{0}; col < columnCount; ++col) {
        for (auto row{0}; row < rowCount; ++row) {
            const auto &rowData = data.at(row);
            if (col >= rowData.size()) {
                break;
            }
            m_data[col * row] = rowData.at(col);
        }
    }

    m_rowCount = rowCount;
    m_columnCount = columnCount;
}

void TableData::setData(matrix2D &&data) noexcept
{
    clear();
    qsizetype rowCount = data.size();
    qsizetype columnCount {0};
    for (auto &rowData : data) {
        columnCount = qMax(columnCount, rowData.size());
    }

    m_data.resize(rowCount * columnCount);
    for (auto col{0}; col < columnCount; ++col) {
        for (auto row{0}; row < rowCount; ++row) {
            auto &rowData = data[row];
            if (col >= rowData.size()) {
                break;
            }
            m_data[col * rowCount + row] = rowData[col];
        }
    }

    m_rowCount = rowCount;
    m_columnCount = columnCount;
}

void TableData::setData(qsizetype row, qsizetype col, const Data &data)
{
    if (row >= m_rowCount) {
        adjustRow(row + 1);
    }
    if (col >= m_columnCount) {
        adjustColumn(col + 1);
    }
    if (data == *s_nullData) {
        m_data[row + col * m_rowCount] = s_nullData;
    } else {
        m_data[row + col * m_rowCount].reset(new Data(data));
    }
}

void TableData::setData(qsizetype row, qsizetype col, Data &&data) noexcept
{
    if (row >= m_rowCount) {
        adjustRow(row + 1);
    }
    if (col >= m_columnCount) {
        adjustColumn(col + 1);
    }
    if (data == *s_nullData) {
        m_data[row + col * m_rowCount] = s_nullData;
    } else {
        m_data[row + col * m_rowCount].reset(new Data(std::move(data)));
    }
}

bool TableData::setMatrixData(const TableRange &range, const matrix2D &data)
{
    qDebug() << Q_FUNC_INFO;
    if (!range.isValid() || range.right() > m_columnCount || range.bottom() > m_rowCount) {
        return false;
    }
    for (auto rIdx{0}; rIdx < range.height(); ++rIdx) {
        auto row = range.top() + rIdx;
        if (rIdx >= data.size()) {
            for (auto cIdx{0}; cIdx < range.width(); ++cIdx) {
                auto column = range.left() + cIdx;
                m_data[row + column * m_rowCount] = s_nullData;
            }
            continue;
        }
        const auto &rowDataList = data.at(rIdx);
        for (auto cIdx{0}; cIdx < range.width(); ++cIdx) {
            auto column = range.left() + cIdx;
            if (cIdx >= rowDataList.size()) {
                m_data[row + column * m_rowCount] = s_nullData;
                continue;
            }
            m_data[row + column * m_rowCount] = TableData::backupData(rowDataList.at(cIdx));
        }
    }
    return true;
}

bool TableData::setMatrixData(const TableRange &range, matrix2D &&data) noexcept
{
    qDebug() << Q_FUNC_INFO;
    if (!range.isValid() || range.right() > m_columnCount || range.bottom() > m_rowCount) {
        return false;
    }
    for (auto rIdx{0}; rIdx < range.height(); ++rIdx) {
        auto row = range.top() + rIdx;
        if (rIdx >= data.size()) {
            for (auto cIdx{0}; cIdx < range.width(); ++cIdx) {
                auto column = range.left() + cIdx;
                m_data[row + column * m_rowCount] = s_nullData;
            }
            continue;
        }
        auto rowDataList = std::move(data[rIdx]);
        for (auto cIdx{0}; cIdx < range.width(); ++cIdx) {
            auto column = range.left() + cIdx;
            if (cIdx >= rowDataList.size()) {
                m_data[row + column * m_rowCount] = s_nullData;
                continue;
            }
            m_data[row + column * m_rowCount] = TableData::backupData(rowDataList.at(cIdx));
        }
    }
    return true;
}

bool TableData::setMatrixData(const TableRange &range, const QList<QList<QString>> &data)
{
    if (!range.isValid() || range.right() > m_columnCount || range.bottom() > m_rowCount) {
        return false;
    }
    for (auto rIdx{0}; rIdx < range.height(); ++rIdx) {
        auto row = range.top() + rIdx;
        if (rIdx >= data.size()) {
            for (auto cIdx{0}; cIdx < range.width(); ++cIdx) {
                auto column = range.left() + cIdx;
                m_data[row + column * m_rowCount] = s_nullData;
            }
            continue;
        }
        const auto &rowDataList = data.at(rIdx);
        for (auto cIdx{0}; cIdx < range.width(); ++cIdx) {
            auto column = range.left() + cIdx;
            if (cIdx >= rowDataList.size()) {
                m_data[row * column] = s_nullData;
                continue;
            }
            std::shared_ptr<Data> dPtr(new Data(*m_data[row + column * m_rowCount]));
            dPtr->setData(DataSpace::Real, rowDataList.at(cIdx));
            dPtr->setData(DataSpace::Display, rowDataList.at(cIdx));
            m_data[row + column * m_rowCount] = TableData::backupData(dPtr);
        }
    }
    return true;
}

bool TableData::setMatrixData(const TableRange &range, QList<QList<QString>> &&data) noexcept
{
    if (!range.isValid() || range.right() > m_columnCount || range.bottom() > m_rowCount) {
        return false;
    }
    for (auto rIdx{0}; rIdx < range.height(); ++rIdx) {
        auto row = range.top() + rIdx;
        if (rIdx >= data.size()) {
            for (auto cIdx{0}; cIdx < range.width(); ++cIdx) {
                auto column = range.left() + cIdx;
                m_data[row * column] = s_nullData;
            }
            continue;
        }
        auto rowDataList = std::move(data[rIdx]);
        for (auto cIdx{0}; cIdx < range.width(); ++cIdx) {
            auto column = range.left() + cIdx;
            if (cIdx >= rowDataList.size()) {
                m_data[row + column * m_rowCount] = s_nullData;
                continue;
            }
            std::shared_ptr<Data> dPtr(new Data(*m_data[row + column * m_rowCount]));
            dPtr->setData(DataSpace::Real, rowDataList.at(cIdx));
            dPtr->setData(DataSpace::Display, rowDataList.at(cIdx));
            m_data[row + column * m_rowCount] = TableData::backupData(dPtr);
        }
    }
    return true;
}

bool TableData::insertRows(qsizetype row, qsizetype n, List &&data)
{
    if (row > m_rowCount || n <= 0) {
        return false;
    }
    m_rowCount += n;
    for (auto col{0}; col < m_columnCount; ++col) {
        m_data.insert(col * m_rowCount + row, n, s_nullData);
    }
    if (data.isEmpty()) {
        return true;
    }
    data.resize(m_columnCount);
    for (auto i{0}; i < n; ++i) {
        for (auto c{0}; c < m_columnCount; ++c) {
            m_data[c * m_rowCount + row + i] = TableData::backupData(data.at(c));
        }
    }
    return true;
}

bool TableData::insertColumns(qsizetype col, qsizetype n, List &&data)
{
    if (col > m_columnCount || n < 0) {
        return false;
    }
    m_data.insert(col * m_rowCount, n * m_rowCount, s_nullData);

    m_columnCount += n;
    if (data.isEmpty()) {
        return true;
    }
    data.resize(m_rowCount);
    for (auto i{0}; i < n; ++i) {
        for (auto r{0}; r < m_rowCount; ++r) {
            m_data[(col + i) * m_rowCount + r] = TableData::backupData(data.at(r));
        }
    }
    return true;
}

void TableData::removeRows(qsizetype row, qsizetype n)
{
    if (row < 0) {
        return;
    } else if (row == 0) {
        if (n >= m_rowCount) {
            clear();
            return;
        }
    } else if (row + n > m_rowCount) {
        n = m_rowCount - row;
    }
    m_rowCount -= n;
    for (auto col{0}; col < m_columnCount; ++col) {
        m_data.remove(col * m_rowCount + row, n);
    }
}

void TableData::removeColumns(qsizetype col, qsizetype n)
{
    if (col < 0) {
        return;
    } else if (col == 0) {
        if (n >= m_columnCount) {
            clear();
            return;
        }
    } else if (col + n > m_columnCount) {
        n = m_columnCount - col;
    }
    m_columnCount -= n;
    m_data.remove(col * m_rowCount, n * m_rowCount);
}

void TableData::adjustSize(qsizetype row, qsizetype col)
{
    adjustColumn(col);
    adjustRow(row);
}

void TableData::adjustRow(qsizetype row)
{
    if (row == m_rowCount) {
        return;
    } else if (row < m_rowCount) {
        auto div = m_rowCount - row;
        for (auto i{0}; i < m_columnCount; ++i) {
            m_data.remove(i * m_columnCount + row, div);
        }
    } else {
        auto div = row - m_rowCount;
        for (auto i{0}; i < m_columnCount; ++i) {
            m_data.insert(i * m_columnCount + m_rowCount, div, s_nullData);
        }
    }
    m_rowCount = row;
}

void TableData::adjustColumn(qsizetype col)
{
    if (col == m_columnCount) {
        return;
    }
    m_columnCount = col;
    m_data.resize(m_columnCount * m_rowCount, s_nullData);
}

QByteArray TableData::serializeData(bool compress, int compressionLevel) const {
    QByteArray serializedData;
    QBuffer buffer(&serializedData);
    buffer.open(QBuffer::WriteOnly);
    QDataStream out(&buffer);
    out << *this;
    buffer.close();
    if (compress) {
        return qCompress(serializedData, compressionLevel);
    }
    return serializedData;
}

bool TableData::deserializeData(const QByteArray &data, bool decompress) {
    auto decompressedData = decompress ? qUncompress(data) : data;
    if (decompressedData.isEmpty()) {
        return false;
    }
    QBuffer buffer(const_cast<QByteArray*>(&decompressedData)); // QBuffer需要可修改的指针
    if (buffer.open(QBuffer::ReadOnly)) {
        QDataStream in(&buffer);
        in >> *this;
        buffer.close();
        return true;
    }
    return false;
}

const Data &TableData::at(qsizetype row, qsizetype col) const noexcept
{
    Q_ASSERT_X(row < m_rowCount && col < m_columnCount, "TableData::at", "index out of range");
    return *m_data.at(row + col * m_rowCount);
}

std::shared_ptr<Data> &TableData::reference(qsizetype row, qsizetype col)
{
    Q_ASSERT_X(row < m_rowCount && col < m_columnCount, "TableData::reference", "index out of range");
    return m_data[row + col * m_rowCount];
}

std::shared_ptr<Data> &TableData::operator[](qsizetype index)
{
    Q_ASSERT_X(index < (m_rowCount * m_columnCount), "TableData::reference", "index out of range");
    return m_data[index];
}

QDataStream &operator<<(QDataStream &out, const TableData &obj) {
    out << obj.m_rowCount << obj.m_columnCount /*<< obj.m_data*/;
    return out;
}

QDataStream &operator>>(QDataStream &in, TableData &obj) {
    in >> obj.m_rowCount >> obj.m_columnCount /*>> obj.m_data.data()*/;
    qDebug() << obj.m_data.size();
    return in;
}
