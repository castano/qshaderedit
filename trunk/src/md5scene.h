/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2008 Rômulo Fernandes <abra185@gmail.com>

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

#ifndef MD5_H
#define MD5_H

#include "scene.h"

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtGui/QFileDialog>
#include <QtGui/QAction>
#include <QtGui/QMenu>

#include <GL/glew.h>
#include <fstream>
#include <cmath>

typedef GLfloat vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t quat_t[4];

typedef struct
{
	int index;
	int parentIndex;

	// To Bind
	vec3_t pos;
	quat_t orientation;

	// Base
	vec3_t basePos;
	quat_t baseOrientation;
} bone_t;

typedef struct
{
	int jointIndex;

	vec_t value;
	vec3_t pos;
} weight_t;

typedef struct
{
	vec2_t uv;
	int weight[2];

	vec3_t renderPos;
	vec3_t renderNormal;

	// normal unasigned to a bone
	vec3_t normal;

	// For the bind position, prevent recalculation
	vec3_t basePos;
	vec3_t baseNormal;
} vert_t;

typedef struct
{
	int index[3];
} triangle_t;

typedef struct
{
	// Vertex Data
	int numVerts;
	vert_t* vertex;

	// Triangle Data
	int numTris;
	triangle_t* tris;

	// Weight Data
	int numWeight;
	weight_t* weights;

	// for glDrawElements =)
	vec_t* normalList;
	GLuint* indexList;
} mesh_t;

#define VectorClear(a)			((a)[0]=(a)[1]=(a)[2]=0)
#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define quatClear(a) 			((a)[0]=1, (a)[1]=(a)[2]=(a)[3]=0)
#define quatCopy(a,b) 			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])

class parsingFile
{
	private:
		unsigned long index;
		char* string;
	
	public:
		parsingFile(const char* filename);
		~parsingFile();

		const char* getNextToken();
};
		

class md5Scene : public Scene
{
	private:
		vec_t scale;
		vec3_t position;

		QList<mesh_t*> meshList;

		unsigned int numBones;
		bone_t* bones;

	public:
		md5Scene();
		~md5Scene();

		void load(QString filename);
		void compileBase();

		virtual void transform() const;
		virtual void draw(Effect* effect) const;

		virtual void setupMenu(QMenu * menu) const
		{
			Q_UNUSED(menu);
		}

};

#endif
