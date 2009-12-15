//
// C++ Interface: Completer
//
// Description:
//
// august 2009
// Author: Peter Komar <marcusk@i.ua>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
#ifndef COMPLETER_H
#define COMPLETER_H
//
#include <QtGui/QFrame>
#include <QtGui/QListWidget>
#include <QtCore/QMultiHash>
//
class QPlainTextEdit;
//
class Completer : public QFrame
{
	Q_OBJECT
	//
public:
        Completer( const QString& extation, QWidget* = 0, Qt::WFlags = Qt::Popup );
        bool prepareCompletion();
        //
        QListWidget* lwItems;

        //
public slots:
        void itemClicked( QListWidgetItem* );
        void invokeCompletion( QPlainTextEdit* );
        void showToolTip(QListWidgetItem * current, QListWidgetItem * previous );
	//
private:
        void loadAtocompliteData();
        QString leftTextCursor() const; //return left text under cursor
        void reset();
        const int CLASS_METOD;
	//
        QString m_extation;
        QMultiHash<QString, QString> m_propertiesStruct;
	//
protected:
        QPlainTextEdit* mEditor;
};
//
#endif // COMPLETER_H
