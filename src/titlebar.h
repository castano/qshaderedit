//
// C++ Interface: titlebar
//
// Description: 
//
//
// Author: Peter <marcusk@i.ua>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QtGui/QWidget>

/**
	@author Peter <marcusk@i.ua>
*/
class TitleBar : public QWidget
{
Q_OBJECT
public:
    TitleBar(QWidget *parent = 0);

    ~TitleBar();
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void setTitleText(const QString& title);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent ( QMouseEvent * event );
private:
    QPixmap centerPm, rightPm;
    QString titleTxt;
    bool bpress; //daraw press button or no
    bool but_press; //press button or no

    void create_pixmap();
    void draw_close_btn();
};

#endif
