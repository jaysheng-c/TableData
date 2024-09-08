/**
  ********************************************************************************
  * @file           : table_data.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/11
  ********************************************************************************
  */

#ifndef HIGHPERFORMANCETABLEVIEW_TABLE_DATA_H
#define HIGHPERFORMANCETABLEVIEW_TABLE_DATA_H

#include <QObject>
#include "data.h"

using List = QList<std::shared_ptr<Data>>;
using matrix2D = QList<List>;

class TableRange {
public:
    TableRange() : m_left(0), m_right(-1), m_top(0), m_bottom(-1) {}
    TableRange(qsizetype l, qsizetype t, qsizetype r, qsizetype b) : m_left(l), m_right(r), m_top(t), m_bottom(b) {}
    virtual ~TableRange() = default;

    bool isValid() const {
        return m_left >= 0 && m_right >= 0 && m_top >= 0 && m_bottom >= 0 && m_left <= m_right && m_top <= m_bottom;
    }
    qsizetype width() const { return m_right - m_left + 1; }
    qsizetype height() const { return m_bottom - m_top + 1; }
    qsizetype size() const { return width() * height(); }

    qsizetype left() const { return m_left; }
    qsizetype right() const { return m_right; }
    qsizetype top() const { return m_top; }
    qsizetype bottom() const { return m_bottom; }
protected:
    qsizetype m_left;
    qsizetype m_right;
    qsizetype m_top;
    qsizetype m_bottom;
};

class TableData : public QObject {
Q_OBJECT
public:
    static const std::shared_ptr<Data> &backupData(const std::shared_ptr<Data> &data);
    static QMap<size_t, std::shared_ptr<Data>> &backupData();
    static qsizetype backupDataSize();
    static void print();

    explicit TableData(QObject *parent = nullptr) : QObject(parent) {}
    TableData(qsizetype row, qsizetype column, QObject *parent = nullptr);

    void clear();
    void setData(const matrix2D &data);
    void setData(matrix2D &&data) noexcept;
    void setData(qsizetype row, qsizetype col, const Data &data);
    void setData(qsizetype row, qsizetype col, Data &&data) noexcept;

    bool setMatrixData(const TableRange &range, const matrix2D &data);
    bool setMatrixData(const TableRange &range, matrix2D &&data) noexcept;

    bool setMatrixData(const TableRange &range, const QList<QList<QString>> &data);
    bool setMatrixData(const TableRange &range, QList<QList<QString>> &&data) noexcept;

    bool insertRows(qsizetype row, qsizetype n = 1, List &&data = {});
    bool insertColumns(qsizetype col, qsizetype n = 1, List &&data = {});
    void removeRows(qsizetype row, qsizetype n = 1);
    void removeColumns(qsizetype col, qsizetype n = 1);

    void adjustSize(qsizetype row, qsizetype col);
    void adjustRow(qsizetype row);
    void adjustColumn(qsizetype col);


    QByteArray serializeData(bool compress = false, int compressionLevel = -1) const;
    bool deserializeData(const QByteArray &data, bool decompress = false);

    qsizetype rowCount() const noexcept { return m_rowCount; }
    qsizetype columnCount() const noexcept { return m_columnCount; }
    TableRange editRange() const noexcept { return {0, 0, columnCount() - 1, rowCount() - 1}; }
    const Data &at(qsizetype row, qsizetype col) const noexcept;
    std::shared_ptr<Data> &reference(qsizetype row, qsizetype col);
    std::shared_ptr<Data> &operator[](qsizetype index);


    friend QDataStream &operator<<(QDataStream &out, const TableData &obj);
    friend QDataStream &operator>>(QDataStream &in, TableData &obj);

    const List &data() const { return m_data; }

protected:
    static std::shared_ptr<Data> s_nullData;
    static QMap<size_t, std::shared_ptr<Data>> s_backUpDataMap;

    qsizetype m_rowCount = 0;
    qsizetype m_columnCount = 0;
    List m_data;    // 按照列进行存储
};


#endif //HIGHPERFORMANCETABLEVIEW_TABLE_DATA_H
