/**
  ********************************************************************************
  * @file           : data_global.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/8
  ********************************************************************************
  */

#ifndef DATA_GLOBAL_H
#define DATA_GLOBAL_H

#include <QRgb>
#include <QColor>

namespace DataSpace {
    enum Type {
        Real,
        Display,
        Align,
        Foreground,
        Background,
        Font,
    };

    class Font {
    public:
        enum FontType {
            None = 0,
            Bold = 1,
            Italic = 2,
            Underline = 4,
        };
        enum AlignType {
            Left = 0x01,        // 0000 0001
            Right = 0x02,       // 0000 0010
            HCenter = 0x04,     // 0000 0100
            Top = 0x20,         // 0010 0000
            Bottom = 0x30,      // 0011 0000
            VCenter = 0x40,     // 0100 0000
            Center = HCenter | VCenter,
        };
    };

    static QRgb gDefaultForeground = QColor(Qt::black).rgb();
    static QRgb gDefaultBackground = QColor(Qt::transparent).rgba();
}

#endif //DATA_GLOBAL_H
