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
#include "nofocusstyle.h"
#include <QtGui/QPushButton>

void NoFocusStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const
{
    if(element == QStyle::PE_FrameFocusRect)
        return;

    QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
}
