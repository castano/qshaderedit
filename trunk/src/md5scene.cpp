#include "md5scene.h"
#include "effect.h"

void quatFromVec (vec3_t vec, quat_t quat)
{
	quat[0] = 0;
	quat[1] = vec[0];
	quat[2] = vec[1];
	quat[3] = vec[2];
}

void quatNormalize (quat_t quat)
{
	vec_t magnitude = sqrt(quat[0]*quat[0] + quat[1]*quat[1] + quat[2]*quat[2] + quat[3]*quat[3]);

	if (magnitude > 0.0f)
	{
		vec_t oneOverMagnitude = 1.0f/magnitude;

		quat[0] *= oneOverMagnitude;
		quat[1] *= oneOverMagnitude;
		quat[2] *= oneOverMagnitude;
		quat[3] *= oneOverMagnitude;
	}
}

void quatInverse (quat_t a, quat_t result)
{
	result[0] = a[0];
	result[1] = -a[1];
	result[2] = -a[2];
	result[3] = -a[3];
}

void quatFromAngle (vec_t angle, vec_t x, vec_t y, vec_t z, quat_t quat)
{
	vec_t result = sin(angle/2);

	quat[0] = cos(angle/2);
	quat[1] = x * result;
	quat[2] = y * result;
	quat[3] = z * result;
}

void quatProduct (quat_t a, quat_t b, quat_t c)
{
	c[0] = a[0]*b[0] - a[1]*b[1] - a[2]*b[2] - a[3]*b[3];
	c[1] = a[1]*b[0] + a[0]*b[1] + a[2]*b[3] - a[3]*b[2];
	c[2] = a[2]*b[0] + a[0]*b[2] + a[3]*b[1] - a[1]*b[3];
	c[3] = a[3]*b[0] + a[0]*b[3] + a[1]*b[2] - a[2]*b[1];
}

void quatProductV (quat_t q, vec3_t v, quat_t c)
{
  c[0] = -(q[1]*v[1]) - (q[2]*v[2]) - (q[3]*v[3]);
  c[1] =  (q[0]*v[1]) + (q[2]*v[3]) - (q[3]*v[2]);
  c[2] =  (q[0]*v[2]) + (q[3]*v[1]) - (q[1]*v[3]);
  c[3] =  (q[0]*v[3]) + (q[1]*v[2]) - (q[2]*v[1]);
}

void quatFromMD5 (vec_t x, vec_t y, vec_t z, quat_t result)
{
	vec_t temp = 1.0f - (x*x) - (y*y) - (z*z);
	if (temp < 0.0f)
		result[0] = 0.0f;
	else
		result[0] = - (vec_t)sqrt(temp);

	result[1] = x;
	result[2] = y;
	result[3] = z;
}

void quatComputeW (quat_t quat)
{
	vec_t temp = 1.0f - (quat[1]*quat[1]) - (quat[2]*quat[2]) - (quat[3]*quat[3]);
	if (temp < 0.0f)
		quat[0] = 0.0f;
	else
		quat[0] = - (vec_t)sqrt(temp);
}

void pointByQuat (vec3_t point, quat_t quat, vec3_t out)
{
	quat_t qVec, qTemp, qInv, qOut;

	quatInverse(quat, qInv);
	quatFromVec(point, qVec);

	quatProduct(quat, qVec, qTemp);
	quatProduct(qTemp, qInv, qOut);

	out[0] = qOut[1];
	out[1] = qOut[2];
	out[2] = qOut[3];
}

void invPointByQuat (vec3_t point, quat_t quat, vec3_t out)
{
	quat_t qVec, qTemp, qInv, qOut;

	quatInverse(quat, qInv);
	quatFromVec(point, qVec);

	quatProduct(qInv, qVec, qTemp);
	quatProduct(qTemp, quat, qOut);

	out[0] = qOut[1];
	out[1] = qOut[2];
	out[2] = qOut[3];
}

void CrossProduct(const vec3_t v1, const vec3_t v2, vec3_t cross) 
{
	cross[0] = v1[1]*v2[2] - v1[2]*v2[1];
	cross[1] = v1[2]*v2[0] - v1[0]*v2[2];
	cross[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

vec_t VectorNormalize(vec3_t v) 
{
	float	length, ilength;

	length = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	length = sqrt (length);

	if (length) 
	{
		ilength = 1/length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}
		
	return length;
}

parsingFile::parsingFile(const char* filename)
{
	if (!filename)
		return;

	FILE* file = fopen(filename, "r");
	if (!file)
		return;

	fseek(file, SEEK_END, SEEK_END);
	unsigned long fileSize = ftell(file);
	fseek(file, SEEK_SET, SEEK_SET);

	string = (char*)malloc(fileSize);
	if (!string)
		return;

	memset(string, 0, fileSize);
	fread(string, fileSize, 1, file);
	fclose(file);

	index = 0;
}

parsingFile::~parsingFile()
{
	free(string);
}

const char* parsingFile::getNextToken()
{
	if (!string)
		return NULL;

	char result[1024];
	char token[1024];
	char actualChar;
	int len;

	memset(token, 0, 1024);
	memset(result, 0, 1024);

	actualChar = string[index];
	while (actualChar)
	{
		// We have a comment here, take everything until the end of the line
		if (actualChar == '/' && string[index + 1] == '/')
		{
			// Next Character is a slash too
			index++;
			while (actualChar != '\n')
				actualChar = string[++index];

			actualChar = string[++index];
			continue;
		}
		else
		// Double Quotes, means a full string until the next double quotes
		if (actualChar == '\"')
		{
			actualChar = string[++index];

			if (actualChar == '\"')
				actualChar = string[++index];
			else
			while (actualChar && actualChar != '\"')
			{
				len = strlen(token);
				token[len] = actualChar;

				actualChar = string[++index];
			}

			if (actualChar == '\"')
				actualChar = string[++index];
		}
		
		// Tab, WhiteSpace, newline - breaks the string
		if (actualChar == ' ' || actualChar == '\t' || actualChar == '\n')
		{
			actualChar = string[++index];
			
			// If our token is empty, continue looking.
			// We dont want to spend time here
			if (!token[0])
				continue;
			else
				break;
		}

		// Insert Character into "Token"
		len = strlen(token);
		token[len] = actualChar;

		actualChar = string[++index];
	}

	len = strlen(token);
	if (!len)
		result[0] = '\0';

	if (len > 1024)
	{
		strncpy(result, token, 1024);
		result[1024] = 0;
	}
	else
	{
		strncpy(result, token, len);
		result[len] = 0;
	}

	return result;
}

md5Scene::md5Scene()
{
	QString fileName = QFileDialog::getOpenFileName(NULL, QObject::tr("Open File"), 
			SceneFactory::lastFile(), QString(QObject::tr("md5mesh (%1)")).arg("*.md5mesh"));

	if(!fileName.isEmpty()) 
	{
		meshList.clear();
		load(fileName);
		SceneFactory::setLastFile(fileName);
	}		
}

md5Scene::~md5Scene()
{
	foreach (mesh_t * mesh, meshList)
	{
		free(mesh->normalList);
		free(mesh->indexList);
		free(mesh->tris);
		free(mesh->weights);
		free(mesh->vertex);
		delete mesh;
	}
}

void md5Scene::transform() const
{}

void md5Scene::draw(Effect* effect) const
{
	// Nothing to draw
	if (!meshList.size())
		return;

	foreach (const mesh_t * mesh, meshList)
	{
		float scale = 0.05;
		glScalef(scale, scale, scale);

		if(effect) 
			effect->beginMaterialGroup();

		// Draw Mesh
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(vert_t), mesh->vertex[0].basePos);
		
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, mesh->normalList);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(vert_t), mesh->vertex[0].uv);

		glDrawElements(GL_TRIANGLES, mesh->numTris*3, GL_UNSIGNED_INT, mesh->indexList);
	}
}
		
void md5Scene::load(QString filename)
{
	parsingFile file(filename.toAscii().data());
	
	const char* token;
	const char* param;

	token = file.getNextToken();
	while(token[0])
	{
		if (!strcmp(token, "mesh"))
		{
			mesh_t* thisMesh = new mesh_t;
			if (!thisMesh)
				return;

			// Get The number of vertices and allocate
			param = file.getNextToken();
			while (strcmp(param, "numverts"))
				param = file.getNextToken();

			param = file.getNextToken();
			thisMesh->numVerts = atoi(param);
			if (!thisMesh->numVerts)
			{
				delete thisMesh;
				return;
			}
			else
			{
				int i;

				thisMesh->vertex = (vert_t*)malloc(sizeof(vert_t)*thisMesh->numVerts);
				if (!thisMesh->vertex)
				{
					delete thisMesh;
					continue;
				}
				else
				for (i = 0; i < thisMesh->numVerts; i++)
				{
					param = file.getNextToken();
					if (strcmp(param, "vert"))
						break;

					// Should Get Index
					param = file.getNextToken();

					// UV DATA
					param = file.getNextToken(); // (
					param = file.getNextToken();
					thisMesh->vertex[i].uv[0] = atof(param);
					param = file.getNextToken();
					thisMesh->vertex[i].uv[1] = atof(param);
					param = file.getNextToken(); // )

					// Weight Data
					param = file.getNextToken();
					thisMesh->vertex[i].weight[0] = atoi(param);
					param = file.getNextToken();
					thisMesh->vertex[i].weight[1] = atoi(param);

					// Clean normals
					VectorClear(thisMesh->vertex[i].baseNormal);
				}
			}

			param = file.getNextToken();
			while (strcmp(param, "numtris"))
				param = file.getNextToken();

			param = file.getNextToken();
			thisMesh->numTris = atoi(param);
			if (!thisMesh->numTris)
			{
				free(thisMesh->vertex);
				delete thisMesh;

				return;
			}
			else
			{
				int i;

				thisMesh->tris = (triangle_t*)malloc(sizeof(triangle_t)*thisMesh->numTris);
				memset(thisMesh->tris, 0, sizeof(triangle_t)*thisMesh->numTris);
				if (!thisMesh->tris)
				{
					free(thisMesh->vertex);
					delete thisMesh;

					return;
				}
				else
				for (i = 0; i < thisMesh->numTris; i++)
				{
					param = file.getNextToken();
					if (strcmp(param, "tri"))
						break;

					param = file.getNextToken(); // index
					param = file.getNextToken();
					thisMesh->tris[i].index[0] = atoi(param);
					param = file.getNextToken();
					thisMesh->tris[i].index[1] = atoi(param);
					param = file.getNextToken();
					thisMesh->tris[i].index[2] = atoi(param);
				}
			}

			// Read Weight Data
			param = file.getNextToken();
			while (strcmp(param, "numweights"))
				param = file.getNextToken();

			param = file.getNextToken();
			thisMesh->numWeight = atoi(param);
			if (!thisMesh->numWeight)
			{
				free(thisMesh->vertex);
				free(thisMesh->tris);
				delete thisMesh;

				return;
			}
			else
			{
				thisMesh->weights = (weight_t*)malloc(sizeof(weight_t)*thisMesh->numWeight);
				memset(thisMesh->weights, 0, sizeof(weight_t)*thisMesh->numWeight);
				if (!thisMesh->weights)
				{
					free(thisMesh->vertex);
					free(thisMesh->tris);
					delete thisMesh;

					return;
				}
				else
				{
					int i;
					for (i = 0; i < thisMesh->numWeight; i++)
					{
						param = file.getNextToken();
						if (strcmp(param, "weight"))
							break;

						param = file.getNextToken(); // index
						param = file.getNextToken();
						thisMesh->weights[i].jointIndex = atoi(param);

						param = file.getNextToken();
						thisMesh->weights[i].value = atof(param);

						param = file.getNextToken(); // (
						param = file.getNextToken();
						thisMesh->weights[i].pos[0] = atof(param);
						param = file.getNextToken();
						thisMesh->weights[i].pos[1] = atof(param);
						param = file.getNextToken();
						thisMesh->weights[i].pos[2] = atof(param);
						param = file.getNextToken(); // )
					}
				}
			}

			thisMesh->normalList = 0;
			thisMesh->indexList = 0;
			meshList.push_back(thisMesh);
		}
		else
		if (!strcmp(token, "numJoints"))
		{
			param = file.getNextToken();
			numBones = atoi(param);

			if (!numBones)
			{
				numBones = 0;
				bones = 0;
				return;
			}

			printf("Number of Bones %d\n", numBones);

			bones = (bone_t*)malloc(sizeof(bone_t)*numBones);
			memset(bones, 0, sizeof(bone_t)*numBones);
		}
		else
		if (!strcmp(token, "joints"))
		{
			if (!bones)
				return;

			param = file.getNextToken(); // {
			for (int i = 0; i < numBones; i++)
			{
				int len;
				float x, y, z;

				// Name
				param = file.getNextToken();

				// Index
				bones[i].index = i;

				// Parent
				param = file.getNextToken();
				bones[i].parentIndex = atoi(param);

				param = file.getNextToken(); // (
				param = file.getNextToken();
				bones[i].pos[0] = atof(param);
				param = file.getNextToken();
				bones[i].pos[1] = atof(param);
				param = file.getNextToken();
				bones[i].pos[2] = atof(param);
				param = file.getNextToken(); // )

				param = file.getNextToken(); // (
				param = file.getNextToken();
				x = atof(param);
				param = file.getNextToken();
				y = atof(param);
				param = file.getNextToken();
				z = atof(param);
				param = file.getNextToken(); // )
				
				printf("Bone %d  ( %f %f %f ) ( %f %f %f )\n", i, bones[i].pos[0], bones[i].pos[1], bones[i].pos[2],
						x, y, z);

				quatFromMD5(x, y, z, bones[i].orientation);

				VectorCopy(bones[i].pos, bones[i].basePos);
				quatCopy(bones[i].orientation, bones[i].baseOrientation);
			}
			param = file.getNextToken(); // }
		}
		token = file.getNextToken();
	}

	compileBase();
}
		
void md5Scene::compileBase()
{
	foreach (mesh_t * mesh, meshList)
	{
		int v, v1;
	
		// Vertex Positions
		for (v = 0; v < mesh->numVerts; v++)
		{
			vec3_t pos;
			int w;

			vert_t* vertex = &mesh->vertex[v];
			if (!vertex)
				return;

			memset(pos, 0, sizeof(vec3_t));
			for (w = vertex->weight[0]; w < vertex->weight[0]+vertex->weight[1]; w++)
			{
				vec3_t tempPos;
				bone_t* bone = NULL;
				weight_t* weight = &mesh->weights[w];
				if (!weight)
					return;
				
				bone = &bones[weight->jointIndex];
				if (!bone)
					return;

				pointByQuat(weight->pos, bone->orientation, tempPos);
				pos[0] += (tempPos[0]+bone->pos[0])*weight->value;
				pos[1] += (tempPos[1]+bone->pos[1])*weight->value;
				pos[2] += (tempPos[2]+bone->pos[2])*weight->value;
			}

			VectorCopy(pos, vertex->basePos);
			VectorCopy(pos, vertex->renderPos);
			VectorClear(vertex->baseNormal);
		}

		// Normals
		for (unsigned int t = 0; t < mesh->numTris; t++)
		{
			vec3_t v1, v2, v3;
			vec3_t edge1, edge2;
			vec3_t normal;

			triangle_t* tri = &mesh->tris[t];
			if (!tri)
				return;

			VectorCopy(mesh->vertex[tri->index[0]].basePos, v1);
			VectorCopy(mesh->vertex[tri->index[1]].basePos, v2);
			VectorCopy(mesh->vertex[tri->index[2]].basePos, v3);

			VectorSubtract(v2, v1, edge1);
			VectorSubtract(v3, v1, edge2);

			CrossProduct(edge1, edge2, normal);
			VectorNormalize(normal);

			VectorAdd(normal, mesh->vertex[tri->index[0]].baseNormal, mesh->vertex[tri->index[0]].baseNormal);
			VectorAdd(normal, mesh->vertex[tri->index[1]].baseNormal, mesh->vertex[tri->index[1]].baseNormal);
			VectorAdd(normal, mesh->vertex[tri->index[2]].baseNormal, mesh->vertex[tri->index[2]].baseNormal);
		}

		// Normalize normals
		for (v1 = 0; v1 < mesh->numVerts; v1++)
		{
			vert_t* vertex = &mesh->vertex[v1];
			if (!vertex)
				return;

			VectorNormalize(vertex->baseNormal);
			VectorCopy(vertex->baseNormal, vertex->renderNormal);
		}

		// Rotate normals by the inverse weight
		// joint orientation in order to get
		// normals for each frame
		for (v = 0; v < mesh->numVerts; v++)
		{
			vec3_t normal;
			int w;

			vert_t* vertex = &mesh->vertex[v];
			if (!vertex)
				return;

			VectorClear(normal);
			for (w = vertex->weight[0]; w < vertex->weight[0]+vertex->weight[1]; w++)
			{
				vec3_t tempPos;
				bone_t* bone = NULL;
				weight_t* weight = &mesh->weights[w];
				if (!weight)
					return;
				
				bone = &bones[weight->jointIndex];
				if (!bone)
					return;

				invPointByQuat(vertex->baseNormal, bone->orientation, tempPos);
				VectorAdd(normal, tempPos, normal);
			}
			VectorNormalize(normal);
			VectorCopy(normal, vertex->normal);
		}
	}

	// Time to make the vertexLists ;)
	foreach (mesh_t * mesh, meshList)
	{
		if (!mesh->normalList)
			mesh->normalList = (vec_t*)malloc(sizeof(vec_t)*mesh->numVerts*3);

		if (!mesh->normalList)
			return;

		memset(mesh->normalList, 0, sizeof(vec_t)*3*mesh->numVerts);
		for (unsigned int i = 0; i < mesh->numVerts; i++)
		{
			mesh->normalList[i+2*i] = mesh->vertex[i].renderNormal[0];
			mesh->normalList[i+2*i+1] = mesh->vertex[i].renderNormal[1];
			mesh->normalList[i+2*i+2] = mesh->vertex[i].renderNormal[2];
		}

		if (!mesh->indexList)
			mesh->indexList = (GLuint*)malloc(sizeof(GLuint)*mesh->numTris*3);

		if (!mesh->indexList)
			return;

		memset(mesh->indexList, 0, sizeof(GLuint)*3*mesh->numTris);
		for (unsigned int i = 0; i < mesh->numTris; i++)
		{
			mesh->indexList[i+2*i] = mesh->tris[i].index[0];
			mesh->indexList[i+2*i+1] = mesh->tris[i].index[1];
			mesh->indexList[i+2*i+2] = mesh->tris[i].index[2];
		}
	}
}

