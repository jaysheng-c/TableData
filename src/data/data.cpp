/**
  ********************************************************************************
  * @file           : data.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/10
  ********************************************************************************
  */

#include "data.h"
#include <QColor>
#include <QFontComboBox>
#include <QBuffer>
namespace {
    constexpr int BOLD = 0;
    constexpr int ITALIC = 1;
    constexpr int UNDERLINE = 2;
    quint16 fontFamilyIndex(const QString &text, const QFontComboBox &combo = QFontComboBox()) {
        auto index = combo.findText(text);
        if (index != -1 && index < combo.count()) {
            return index;
        }
        return 0;
    }
    QString fontFamilyString(int index, const QFontComboBox &combo = QFontComboBox()) {
        if (index < 0 || index >= combo.count()) {
            return combo.itemText(0);
        }
        return combo.itemText(index);
    }

    QList<bool> fontType(int type) {
        switch (type) {
            case DataSpace::Font::Bold:
                return {true, false, false};
            case DataSpace::Font::Italic:
                return {false, true, false};
            case DataSpace::Font::Underline:
                return {false, false, true};
            case DataSpace::Font::Bold | DataSpace::Font::Italic:
                return {true, true, false};
            case DataSpace::Font::Bold | DataSpace::Font::Underline:
                return {true, false, true};
            case DataSpace::Font::Italic | DataSpace::Font::Underline:
                return {false, true, true};
            case DataSpace::Font::Bold |DataSpace::Font::Italic | DataSpace::Font::Underline:
                return {true, true, true};
            case DataSpace::Font::None:
            default:
                return {false, false, false};
        }
    }
    DataSpace::Font::FontType fontType(const QList<bool> &types) {
        Q_ASSERT(types.size() == 3);
        int type = DataSpace::Font::FontType::None;
        type |= types.at(BOLD) ? DataSpace::Font::Bold : DataSpace::Font::None;
        type |= types.at(ITALIC) ? DataSpace::Font::Italic : DataSpace::Font::None;
        type |= types.at(UNDERLINE) ? DataSpace::Font::Underline : DataSpace::Font::None;
        return DataSpace::Font::FontType(type);
    }

    int toQtAlign(int align) {
        switch (align) {
            case DataSpace::Font::Left:
                return Qt::AlignLeft;
            case DataSpace::Font::Right:
                return Qt::AlignRight;
            case DataSpace::Font::HCenter:
                return Qt::AlignHCenter;
            case DataSpace::Font::Top:
                return Qt::AlignTop;
            case DataSpace::Font::Top | DataSpace::Font::Left:
                return Qt::AlignTop | Qt::AlignLeft;
            case DataSpace::Font::Top | DataSpace::Font::Right:
                return Qt::AlignTop | Qt::AlignRight;
            case DataSpace::Font::Top | DataSpace::Font::HCenter:
                return Qt::AlignTop | Qt::AlignHCenter;
            case DataSpace::Font::Bottom:
                return Qt::AlignBottom;
            case DataSpace::Font::Bottom | DataSpace::Font::Left:
                return Qt::AlignBottom | Qt::AlignLeft;
            case DataSpace::Font::Bottom | DataSpace::Font::Right:
                return Qt::AlignBottom | Qt::AlignRight;
            case DataSpace::Font::Bottom | DataSpace::Font::HCenter:
                return Qt::AlignBottom | Qt::AlignHCenter;
            case DataSpace::Font::VCenter:
                return Qt::AlignVCenter;
            case DataSpace::Font::VCenter | DataSpace::Font::Left:
                return Qt::AlignVCenter | Qt::AlignLeft;
            case DataSpace::Font::VCenter | DataSpace::Font::Right:
                return Qt::AlignVCenter | Qt::AlignRight;
            case DataSpace::Font::Center:
            default:
                break;
        }
        return Qt::AlignCenter;
    }
    int fromQtAlign(int align) {
        switch (align) {
            case Qt::AlignLeft:
                return DataSpace::Font::Left;
            case Qt::AlignRight:
                return DataSpace::Font::Right;
            case Qt::AlignHCenter:
                return DataSpace::Font::HCenter;
            case Qt::AlignTop:
                return DataSpace::Font::Top;
            case Qt::AlignTop | Qt::AlignLeft:
                return DataSpace::Font::Top | DataSpace::Font::Left;
            case Qt::AlignTop | Qt::AlignRight:
                return DataSpace::Font::Top | DataSpace::Font::Right;
            case Qt::AlignTop | Qt::AlignHCenter:
                return DataSpace::Font::Top | DataSpace::Font::HCenter;
            case Qt::AlignBottom:
                return DataSpace::Font::Bottom;
            case Qt::AlignBottom | Qt::AlignLeft:
                return DataSpace::Font::Bottom | DataSpace::Font::Left;
            case Qt::AlignBottom | Qt::AlignRight:
                return DataSpace::Font::Bottom | DataSpace::Font::Right;
            case Qt::AlignBottom | Qt::AlignHCenter:
                return DataSpace::Font::Bottom | DataSpace::Font::HCenter;
            case Qt::AlignVCenter:
                return DataSpace::Font::VCenter;
            case Qt::AlignVCenter | Qt::AlignLeft:
                return DataSpace::Font::VCenter | DataSpace::Font::Left;
            case Qt::AlignVCenter | Qt::AlignRight:
                return DataSpace::Font::VCenter | DataSpace::Font::Right;
            case Qt::AlignCenter:
            default:
                break;
        }
        return DataSpace::Font::Center;
    }
}

Data::Data(const QString &text)
//    : m_value(new QString(text)), m_display(new QString(text)),
    : m_value(text), m_display(text),
    m_align(DataSpace::Font::VCenter | DataSpace::Font::Left),
//    m_foreground(gDefaultForeground), m_background(gDefaultBackground),
    m_foreground(4278190080), m_background(0),
    m_fontType(DataSpace::Font::None), m_fontSize(11), m_family(0)
{}

Data::Data(const Data &data) {
    m_value = data.m_value;
    m_display = data.m_display;
    m_align = data.m_align;
    m_foreground = data.m_foreground;
    m_background = data.m_background;
    m_family = data.m_family;
    m_fontType = data.m_fontType;
    m_fontSize = data.m_fontSize;
}

Data::Data(Data &&data) noexcept {
    m_value = std::move(data.m_value);
    m_display = std::move(data.m_display);
    m_align = data.m_align;
    m_foreground = data.m_foreground;
    m_background = data.m_background;
    m_family = data.m_family;
    m_fontType = data.m_fontType;
    m_fontSize = data.m_fontSize;
}

Data::~Data()
{
//    qDebug() << "Data::~Data()";
}

QVariant Data::data(DataSpace::Type type) const
{
    switch (type) {
        case DataSpace::Real: return m_value;
        case DataSpace::Display: return m_display;
        case DataSpace::Align: return toQtAlign(m_align);
        case DataSpace::Foreground: return QColor(m_foreground);
        case DataSpace::Background: {
            QColor color;
            color.setRgba(m_background);
            return QBrush(color);
        }
        case DataSpace::Font: {
            QFont font;
            font.setFamily(fontFamilyString(m_family));
            auto list = fontType(m_fontType);
            font.setBold(list.at(0));
            font.setItalic(list.at(1));
            font.setUnderline(list.at(2));
            font.setPointSizeF(m_fontSize);
            return font;
        }
    }
    return {};
}

bool Data::setData(DataSpace::Type type, const QVariant &data)
{
    switch (type) {
        case DataSpace::Real: m_value = data.toString(); break;
        case DataSpace::Display: m_display = data.toString(); break;
        case DataSpace::Align: m_align = fromQtAlign(data.toInt()); break;
        case DataSpace::Foreground: m_foreground = data.value<QColor>().rgb(); break;
        case DataSpace::Background: m_background = data.value<QBrush>().color().rgba(); break;
        case DataSpace::Font: {
            auto font = data.value<QFont>();
            m_family = fontFamilyIndex(font.family());
            m_fontType = fontType(QList<bool>() << font.bold() << font.italic() << font.underline());
            m_fontSize = font.pointSize();
            break;
        }
        default:
            return false;
    }
    return true;
}

bool Data::setData(DataSpace::Type type, QVariant &&data)
{
    switch (type) {
        case DataSpace::Real: m_value = data.toString(); break;
        case DataSpace::Display: m_display = data.toString(); break;
        case DataSpace::Align: m_align = fromQtAlign(data.toInt()); break;
        case DataSpace::Foreground: m_foreground = data.value<QColor>().rgb(); break;
        case DataSpace::Background: m_background = data.value<QBrush>().color().rgba(); break;
        case DataSpace::Font: {
            auto font = data.value<QFont>();
            m_family = fontFamilyIndex(font.family());
            m_fontType = fontType(QList<bool>() << font.bold() << font.italic() << font.underline());
            m_fontSize = font.pointSize();
            break;
        }
        default:
            return false;
    }
    return true;
}

void Data::reset()
{
    m_value = "";
    m_display = "";
    m_align = DataSpace::Font::VCenter | DataSpace::Font::Left;
    m_foreground = QColor(Qt::black).rgb();
    m_background = QColor(Qt::transparent).rgba();
    m_family = fontFamilyIndex("Arial");
    m_fontType = DataSpace::Font::None;
    m_fontSize = 11;
}

QByteArray Data::serializeData(bool compress, int compressionLevel) const {
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

bool Data::deserializeData(const QByteArray &data, bool decompress) {
    auto decompressedData = decompress ? qUncompress(data) : data;
    QBuffer buffer(const_cast<QByteArray*>(&decompressedData)); // QBuffer需要可修改的指针
    if (buffer.open(QBuffer::ReadOnly)) {
        QDataStream in(&buffer);
        in >> *this;
        buffer.close();
        return true;
    }
    return false;
}

bool Data::equal(const Data &other, const QList<DataSpace::Type> &types) const {
    if (types.isEmpty()) {
        return *this == other;
    }
    bool res = true;
    for (auto type : types) {
        res &= equal(other, type);
    }
    return res;
}

bool Data::equal(const Data &other, DataSpace::Type type) const {
    switch (type) {
        case DataSpace::Real: return m_value == other.m_value;
        case DataSpace::Display: return m_display == other.m_display;
        case DataSpace::Align: return m_align == other.m_align;
        case DataSpace::Foreground: return m_foreground == other.m_foreground;
        case DataSpace::Background: return m_background == other.m_background;
        case DataSpace::Font:
            return m_family == other.m_family && m_fontType == other.m_fontType && m_fontSize == other.m_fontSize;
    }
    return false;
}

bool Data::operator==(const Data &other) const {
    return m_value == other.m_value && m_display == other.m_display && m_align == other.m_align &&
           m_foreground == other.m_foreground && m_background == other.m_background && m_family == other.m_family &&
           m_fontType == other.m_fontType && m_fontSize == other.m_fontSize;
}

Data &Data::operator=(const Data &data) {
    if (this != &data) {
        m_value = data.m_value;
        m_display = data.m_display;
        m_align = data.m_align;
        m_foreground = data.m_foreground;
        m_background = data.m_background;
        m_family = data.m_family;
        m_fontType = data.m_fontType;
        m_fontSize = data.m_fontSize;
    }
    return *this;
}

Data &Data::operator=(Data &&data) noexcept {
    if (this != &data) {
        m_value = std::move(data.m_value);
        m_display = std::move(data.m_display);
        m_align = data.m_align;
        m_foreground = data.m_foreground;
        m_background = data.m_background;
        m_family = data.m_family;
        m_fontType = data.m_fontType;
        m_fontSize = data.m_fontSize;
    }
    return *this;
}

bool Data::operator!=(const Data &other) const {
    return !(*this == other);
}

bool Data::operator<(const Data& other) const
{
    return qHash(*this) < qHash(other);
}

QDebug operator<<(QDebug debug, const Data &obj) {
    QColor background;
    background.setRgba(obj.m_background);
    debug.nospace() << "{\n [Real: " << obj.m_value << "]\n" << " [Display: " << obj.m_display << "]\n"
        << " [Align: " << Qt::AlignmentFlag(toQtAlign(obj.m_align)) << "]\n"
        << " [Foreground: " << QColor(obj.m_foreground).name() << "]\n"
        << " [Background: " << QBrush(background) << "]\n"
        << " [Font(family:" << fontFamilyString(obj.m_family)
        << ", \"bold, italic, underline\": " << fontType(obj.m_fontType) << ", size:" << obj.m_fontSize << ")]\n}";
    return debug.space();
}

QDebug operator<<(QDebug debug, const Data *obj) {
    if (obj == nullptr) {
        return debug.nospace() << "0x0";
    } else {
        QColor background;
        background.setRgba(obj->m_background);
        debug.nospace() << "{\n [Real: " << obj->m_value << "]\n" << " [Display: " << obj->m_display << "]\n"
        << " [Align: " << Qt::AlignmentFlag(toQtAlign(obj->m_align)) << "]\n"
        << " [Foreground: " << QColor(obj->m_foreground).name() << "]\n"
        << " [Background: " << QBrush(background) << "]\n"
        << " [Font(family:" << fontFamilyString(obj->m_family)
        << ", \"bold, italic, underline\": " << fontType(obj->m_fontType) << ", size:" << obj->m_fontSize << ")]\n}";
    }
    return debug.space();
}

QDataStream &operator<<(QDataStream &out, const Data &obj) {
    QColor background;
    background.setRgba(obj.m_background);
    out << obj.m_value
        << obj.m_display
        << obj.m_align
        << obj.m_foreground
        << obj.m_background
        << obj.m_family
        << obj.m_fontType
        << obj.m_fontSize;
    return out;
}

QDataStream &operator>>(QDataStream &in, Data &obj) {
    quint8 align, fontSize, type;
    QRgb foreground, background;
    quint16 family;
    in >> obj.m_value >> obj.m_display
       >> align >> foreground >> background
       >> family >> type >> fontSize;

    obj.m_align = align;
    obj.m_foreground = foreground;
    obj.m_background = background;
    obj.m_family = family;
    obj.m_fontType = type;
    obj.m_fontSize = fontSize;
    return in;
}

size_t qHash(const Data &obj)
{
    auto hash = qHash(obj.m_display + "dmsas" + obj.m_value);
    hash ^= static_cast<quint64>(obj.m_align) * 31;
    hash ^= static_cast<quint64>(obj.m_foreground) * 37;
    hash ^= static_cast<quint64>(obj.m_background) * 41;
    hash ^= static_cast<quint64>(obj.m_family) * 43;
    hash ^= static_cast<quint64>(obj.m_fontType) * 47;
    hash ^= static_cast<quint64>(obj.m_fontSize) * 53;
    return hash;
}

size_t qHash(Data *obj)
{
    auto hash = qHash(obj->m_display + "dmsas" + obj->m_value);
    hash ^= static_cast<quint64>(obj->m_align) * 31;
    hash ^= static_cast<quint64>(obj->m_foreground) * 37;
    hash ^= static_cast<quint64>(obj->m_background) * 41;
    hash ^= static_cast<quint64>(obj->m_family) * 43;
    hash ^= static_cast<quint64>(obj->m_fontType) * 47;
    hash ^= static_cast<quint64>(obj->m_fontSize) * 53;
    return hash;
}
