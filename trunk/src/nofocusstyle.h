//
// C++ Interface: NoFocusStyle
//
// Description:
//
// august 2009
// Author: Peter Komar <marcusk@i.ua>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
#ifndef NOFOCUSSTYLE_H
#define NOFOCUSSTYLE_H

#include <QtGui/QPlastiqueStyle>

class NoFocusStyle : public QPlastiqueStyle
{
public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = 0) const;
};

#endif // NOFOCUSSTYLE_H
