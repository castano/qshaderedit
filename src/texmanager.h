/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>
    Copyright (C) 2007 Lars Uebernickel <larsuebernickel@gmx.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef TEXMANAGER_H
#define TEXMANAGER_H

#include <GL/glew.h>

#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QMetaType>
#include <QtGui/QPixmap>


// Implicitly shared texture class.
class GLTexture
{
public:
	GLTexture();
	GLTexture(const GLTexture & t);
	void operator= (const GLTexture & t);
	~GLTexture();
	
	static GLTexture open(const QString & name);

	const QString& name() const;
	GLuint object() const;
	GLuint target() const;
	QImage icon() const;
	QImage image() const;

	
	GLint wrapS() const;
	GLint wrapT() const;
	// HACK: the set texture mode functions must be const to prevent QSharedData to copy itself 
	void setWrapMode(GLint s, GLint t) const;

	GLint minifyingFilter() const;
	GLint magnificationFilter() const;
	void setFilteringMode(GLint min, GLint mag) const;
	
	
private:
	class Private;
	GLTexture(Private * p);
	QSharedDataPointer<Private> m_data;
};

Q_DECLARE_METATYPE(GLTexture)


#endif // TEXMANAGER_H
