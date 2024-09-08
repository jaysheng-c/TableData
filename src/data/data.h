/**
  ********************************************************************************
  * @file           : data.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/10
  ********************************************************************************
  */

#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QDataStream>
#include "data_global.h"

#if defined(_MSC_VER)
#pragma pack(push, 1)
class Data {
#else
class /*__attribute__((packed))*/ Data {
#endif
public:
    explicit Data(const QString &text = "");
    Data(const Data &data);
    Data(Data &&data) noexcept;
    virtual ~Data();

    QVariant data(DataSpace::Type type) const;
    bool setData(DataSpace::Type type, const QVariant &data);
    bool setData(DataSpace::Type type, QVariant &&data);
    void reset();

    QByteArray serializeData(bool compress = false, int compressionLevel = -1) const;
    bool deserializeData(const QByteArray &data, bool decompress = false);

    bool equal(const Data &other, const QList<DataSpace::Type> &types) const;
    bool equal(const Data &other, DataSpace::Type type) const;

    bool operator==(const Data &other) const;
    bool operator!=(const Data &other) const;
    Data &operator=(const Data &data);
    Data &operator=(Data &&data) noexcept;
    bool operator<(const Data& other) const;

    friend QDebug operator<<(QDebug debug, const Data &obj);
    friend QDebug operator<<(QDebug debug, const Data *obj);
    friend QDataStream &operator<<(QDataStream &out, const Data &obj);
    friend QDataStream &operator>>(QDataStream &in, Data &obj);
    friend size_t qHash(const Data& obj);
    friend size_t qHash(Data *obj);
protected:
    // value
    QString m_value;
    QString m_display;
    quint8 m_align;
    // color
    QRgb m_foreground;
    QRgb m_background;
    // font
    quint16 m_family;
    quint8 m_fontType;  // bold italic underline
    quint8 m_fontSize;
};

#if defined(_MSC_VER)
#pragma pack(pop)
#endif

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


#endif //DATA_H
