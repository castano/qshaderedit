
#include "effect.h"
#include "messagepanel.h"
#include "outputparser.h"
#include "texmanager.h"
#include "parameter.h"
#include "glutils.h"

#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QTime>
#include <QtCore/QVariant>
#include <QtCore/QDir>

#include <QtGui/QImage>

#include <GL/glew.h>

#include <QtOpenGL/QGLContext>

namespace {

	// Default vertex shader.
	static const char * s_vertexShaderText =
		"varying vec3 v_V;\n"
		"varying vec3 v_N;\n\n"
		"void main() {\n"
		"	gl_Position = ftransform();\n"
		"	v_V = (gl_ModelViewMatrix * gl_Vertex).xyz;\n"
		"	v_N = gl_NormalMatrix * gl_Normal;\n"
		"}\n";

	// Default fragment shader.
	static const char * s_fragmentShaderText =
		"varying vec3 v_V;\n"
		"varying vec3 v_N;\n\n"
		"void main() {\n"
		"	vec3 N = normalize(v_N);\n"
		"	vec3 V = normalize(v_V);\n"
		"	vec3 R = reflect(V, N);\n"
		"	vec3 L = normalize(vec3(gl_LightSource[0].position));\n\n"
		"	vec4 ambient = gl_FrontMaterial.ambient;\n"
		"	vec4 diffuse = gl_FrontMaterial.diffuse * max(dot(L, N), 0.0);\n"
		"	vec4 specular = gl_FrontMaterial.specular * pow(max(dot(R, L), 0.0), gl_FrontMaterial.shininess);\n\n"
		"	gl_FragColor = ambient + diffuse + specular;\n"
		"}\n";

	// GLSL shader file tags.
	static const char * s_vertexShaderTag = "[VertexShader]\n";
	static const char * s_fragmentShaderTag = "[FragmentShader]\n";
	static const char * s_parametersTag = "[Parameters]\n";

	static QString getTypeName(GLint type)
	{
		switch( type ) {
			case GL_FLOAT:
				return "float";
			case GL_FLOAT_VEC2_ARB:
				return "vec2";
			case GL_FLOAT_VEC3_ARB:
				return "vec3";
			case GL_FLOAT_VEC4_ARB:
				return "vec4";
			case GL_INT:
				return "int";
			case GL_INT_VEC2_ARB:
				return "ivec2";
			case GL_INT_VEC3_ARB:
				return "ivec3";
			case GL_INT_VEC4_ARB:
				return "ivec4";
			case GL_BOOL_ARB:
				return "bool";
			case GL_BOOL_VEC2_ARB:
				return "bvec2";
			case GL_BOOL_VEC3_ARB:
				return "bvec3";
			case GL_BOOL_VEC4_ARB:
				return "bvec4";
			case GL_FLOAT_MAT2_ARB:
				return "mat2";
			case GL_FLOAT_MAT3_ARB:
				return "mat3";
			case GL_FLOAT_MAT4_ARB:
				return "mat4";
			case GL_SAMPLER_1D_ARB:
				return "sampler1D";
			case GL_SAMPLER_2D_ARB:
				return "sampler2D";
			case GL_SAMPLER_3D_ARB:
				return "sampler3D";
			case GL_SAMPLER_CUBE_ARB:
				return "samplerCUBE";
			case GL_SAMPLER_2D_RECT_ARB:
				return "samplerRECT";
			case GL_SAMPLER_1D_SHADOW_ARB:
				return "sampler1DShadow";
			case GL_SAMPLER_2D_SHADOW_ARB:
				return "sampler2DShadow";
			case GL_SAMPLER_2D_RECT_SHADOW_ARB:
				return "samplerRECTShadow";
		}

		return "";
	}

	static GLint getType(QString str)
	{
		if( str == "float" ) return GL_FLOAT;
		if( str == "vec2" ) return GL_FLOAT_VEC2_ARB;
		if( str == "vec3" ) return GL_FLOAT_VEC3_ARB;
		if( str == "vec4" ) return GL_FLOAT_VEC4_ARB;

		if( str == "int" ) return GL_INT;
		if( str == "ivec2" ) return GL_INT_VEC2_ARB;
		if( str == "ivec3" ) return GL_INT_VEC3_ARB;
		if( str == "ivec4" ) return GL_INT_VEC4_ARB;

		if( str == "bool" ) return GL_BOOL;
		if( str == "bvec2" ) return GL_BOOL_VEC2_ARB;
		if( str == "bvec3" ) return GL_BOOL_VEC3_ARB;
		if( str == "bvec4" ) return GL_BOOL_VEC4_ARB;

		if( str == "mat2" ) return GL_FLOAT_MAT2_ARB;
		if( str == "mat3" ) return GL_FLOAT_MAT3_ARB;
		if( str == "mat4" ) return GL_FLOAT_MAT4_ARB;

		if( str == "sampler1D" ) return GL_SAMPLER_1D_ARB;
		if( str == "sampler2D" ) return GL_SAMPLER_2D_ARB;
		if( str == "sampler3D" ) return GL_SAMPLER_3D_ARB;
		if( str == "samplerCUBE" ) return GL_SAMPLER_CUBE_ARB;
		if( str == "samplerRECT" ) return GL_SAMPLER_2D_RECT_ARB;
		if( str == "sampler1DShadow" ) return GL_SAMPLER_1D_SHADOW_ARB;
		if( str == "sampler2DShadow" ) return GL_SAMPLER_2D_SHADOW_ARB;
		if( str == "samplerRECTShadow" ) return GL_SAMPLER_2D_RECT_SHADOW_ARB;

		return GL_ZERO;
	}

	static GLenum getBaseType(GLenum type)
	{
		switch( type ) {
			case GL_FLOAT:
			case GL_FLOAT_VEC2_ARB:
			case GL_FLOAT_VEC3_ARB:
			case GL_FLOAT_VEC4_ARB:
			case GL_FLOAT_MAT2_ARB:
			case GL_FLOAT_MAT3_ARB:
			case GL_FLOAT_MAT4_ARB:
				return GL_FLOAT;
			case GL_INT:
			case GL_INT_VEC2_ARB:
			case GL_INT_VEC3_ARB:
			case GL_INT_VEC4_ARB:
				return GL_INT;
			case GL_BOOL_ARB:
			case GL_BOOL_VEC2_ARB:
			case GL_BOOL_VEC3_ARB:
			case GL_BOOL_VEC4_ARB:
				return GL_BOOL_ARB;
			case GL_SAMPLER_1D_ARB:
			case GL_SAMPLER_2D_ARB:
			case GL_SAMPLER_3D_ARB:
			case GL_SAMPLER_CUBE_ARB:
			case GL_SAMPLER_2D_RECT_ARB:
			case GL_SAMPLER_1D_SHADOW_ARB:
			case GL_SAMPLER_2D_SHADOW_ARB:
			case GL_SAMPLER_2D_RECT_SHADOW_ARB:
				//return GL_SAMPLER;
				break;
		}
		return type;
	}
	
	static int getRowNum(GLenum type)
	{
		switch( type ) {
			case GL_FLOAT_VEC2_ARB:
			case GL_INT_VEC2_ARB:
			case GL_BOOL_VEC2_ARB:
			case GL_FLOAT_MAT2_ARB:
				return 2;
			case GL_FLOAT_VEC3_ARB:
			case GL_INT_VEC3_ARB:
			case GL_BOOL_VEC3_ARB:
			case GL_FLOAT_MAT3_ARB:
				return 3;
			case GL_FLOAT_VEC4_ARB:
			case GL_INT_VEC4_ARB:
			case GL_BOOL_VEC4_ARB:
			case GL_FLOAT_MAT4_ARB:				
				return 4;
		}
		return 0;
	}
	
	static int getColumnNum(GLenum type)
	{
		switch( type ) {
			case GL_FLOAT_VEC2_ARB:
			case GL_INT_VEC2_ARB:
			case GL_BOOL_VEC2_ARB:
			case GL_FLOAT_VEC3_ARB:
			case GL_INT_VEC3_ARB:
			case GL_BOOL_VEC3_ARB:
			case GL_FLOAT_VEC4_ARB:
			case GL_INT_VEC4_ARB:
			case GL_BOOL_VEC4_ARB:
				return 1;
			case GL_FLOAT_MAT2_ARB:
				return 2; 
			case GL_FLOAT_MAT3_ARB:
				return 3;
			case GL_FLOAT_MAT4_ARB:				
				return 4;
		}
		return 0;
	}
	
	/// GLSL Parameter
	class GLSLParameter : public Parameter
	{
	private:
		GLenum m_type;
		GLint m_location;
		int m_texUnit; // only valid if isTexture() returns true
		
	public:
		GLSLParameter(const QString& name, GLenum type, GLint location):
			m_type(type), m_location(location), m_texUnit(0)
		{
			setName(name);
			
			if( name.contains("color", Qt::CaseInsensitive) ) {
				if (m_type == GL_FLOAT_VEC3_ARB || m_type == GL_FLOAT_VEC4_ARB) {
					setWidget(Widget_Color);
				}
			}
		}
		
		virtual int rows() const { return getRowNum(m_type); }
		virtual int columns() const { return getColumnNum(m_type); }
		
		GLenum glType() const { return m_type; }
		GLint location() const { return m_location; }
		
		void setLocation(GLint location)
		{
			m_location = location;
		}
		
		bool isTexture() const
		{
			return m_type == GL_SAMPLER_1D_ARB || m_type == GL_SAMPLER_2D_ARB || m_type == GL_SAMPLER_3D_ARB ||
				m_type == GL_SAMPLER_CUBE_ARB || m_type == GL_SAMPLER_2D_RECT_ARB;
		}
		
		int textureUnit() const
		{
			return m_texUnit;
		}
			
		void setTextureUnit(int unit)
		{
			m_texUnit = unit;
		}
		
		GLenum baseType() const {
			return getBaseType(m_type);		
		}
	};

}


/// GLSL Effect.
class GLSLEffect : public Effect
{
private:

	GLhandleARB m_vertexShader;
	GLhandleARB m_fragmentShader;
	GLhandleARB m_program;

	QByteArray m_vertexShaderText;
	QByteArray m_fragmentShaderText;

	QTime m_time;
	GLint m_timeUniform;

	QVector<GLSLParameter*> m_parameterArray;

	OutputParser* m_outputParser;

	// Builder thread.
	class BuilderThread : public GLThread
	{
		GLSLEffect * m_effect;
		
	public:
		BuilderThread(QGLWidget * widget, GLSLEffect * effect) : GLThread(widget), m_effect(effect)
		{
		}
		void run() 
		{
			this->makeCurrent();
			bool succeed = m_effect->threadedBuild();
			this->doneCurrent();
			emit m_effect->built(succeed);
		}
	};
	friend class BuilderThread;
	BuilderThread m_thread;
	
public:

	// Ctor.
	GLSLEffect(const EffectFactory * factory, QGLWidget * widget) : Effect(factory, widget),
		m_vertexShader(0),
		m_fragmentShader(0),
		m_program(0),
		m_vertexShaderText(s_vertexShaderText),
		m_fragmentShaderText(s_fragmentShaderText),
		m_timeUniform(-1),
		m_outputParser(0),
		m_thread(widget, this)
	{
		this->makeCurrent();
		
		const char* vendor = (const char*)glGetString(GL_VENDOR);
		if (strcmp(vendor, "ATI Technologies Inc.") == 0)
			m_outputParser = new AtiGlslOutputParser;
		else if (strcmp(vendor, "NVIDIA Corporation") == 0)
			m_outputParser = new NvidiaOutputParser;
		
		m_time.start();
	}

	// Dtor.
	virtual ~GLSLEffect()
	{
		this->makeCurrent();
		
		deleteProgram();
		ReportGLErrors();
		delete m_outputParser;
		qDeleteAll(m_parameterArray);
	}

        // Load the effect from the given file.
	virtual void load(QFile * file)
	{
		Q_ASSERT(file != NULL);

		m_vertexShaderText.clear();
		m_fragmentShaderText.clear();

		this->makeCurrent();
		
		QDir dir = QFileInfo(*file).dir();
		
		QByteArray line;
		while (!file->atEnd()) {

			if( line.startsWith(s_vertexShaderTag) ) {
				// Read vertex shader.
				while (!file->atEnd()) {
					line = file->readLine();
					if( line.startsWith('[') ) {
						break;
					}
					m_vertexShaderText.push_back(line);
				}

				continue;
			}

			if( line.startsWith(s_fragmentShaderTag) ) {
				// Read fragment shader.
				while (!file->atEnd()) {
					line = file->readLine();
					if( line.startsWith('[') ) {
						break;
					}
					m_fragmentShaderText.push_back(line);
				}

				continue;
			}

			if( line.startsWith(s_parametersTag) ) {
				// Parse parameters.
				while (!file->atEnd()) {
					line = file->readLine();
					if( line.startsWith('[') ) {
						break;
					}
					// Parse parameter->
					parseParameter(line, dir);
				}
				continue;
			}

			line = file->readLine();
		}

		m_time.start();
	}

	// Save the effect in the given file.
	virtual void save(QFile * file)
	{
		Q_ASSERT(file != NULL);

		// [VertexShader]
		file->write(s_vertexShaderTag, strlen(s_vertexShaderTag));
		file->write(m_vertexShaderText);
		if(!m_vertexShaderText.endsWith('\n')) {
			file->write("\n");
		}

		// [FragmentShader]
		file->write(s_fragmentShaderTag, strlen(s_fragmentShaderTag));
		file->write(m_fragmentShaderText);
		if(!m_fragmentShaderText.endsWith('\n')) {
			file->write("\n");
		}

		QDir dir = QFileInfo(*file).dir();
		
		if( this->parameterCount() > 0 ) {
			// [Parameters]
			file->write(s_parametersTag, strlen(s_parametersTag));

			foreach(GLSLParameter* p, m_parameterArray) {
				QString str = getParameterAssignment(p, dir);
				file->write(str.toLatin1());
			}
		}
	}


	virtual int getInputNum()
	{
		return 2;
	}

	virtual QString getInputName(int i)
	{
		Q_ASSERT(i == 0 || i == 1);

		if( i == 0 ) {
			return tr("Vertex Shader");
		}
		else {
			return tr("Fragment Shader");
		}
	}

	virtual const QByteArray & getInput(int i) const
	{
		Q_ASSERT(i == 0 || i == 1);

		if( i == 0 ) {
			return m_vertexShaderText;
		}
		else {
			return m_fragmentShaderText;
		}
	}

	virtual void setInput(int i, const QByteArray & txt)
	{
		Q_ASSERT(i == 0 || i == 1);

		if( i == 0 ) {
			m_vertexShaderText = txt;
		}
		else {
			m_fragmentShaderText = txt;
		}
	}

	bool threadedBuild()
	{
		GLhandleARB vertexShader;
		GLhandleARB fragmentShader;
		GLhandleARB program;
		
		
		vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		
		emit infoMessage(tr("Compiling vertex shader..."));
		const char * vertexStrings[] = { m_vertexShaderText.data() };
		glShaderSourceARB(vertexShader, 1, vertexStrings, NULL);
		glCompileShaderARB(vertexShader);
		
		// Get error log.
		QByteArray infoLog;
		GLint charsWritten, infoLogLength;
		glGetObjectParameterivARB(vertexShader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infoLogLength);
		infoLog.resize(infoLogLength);
		glGetInfoLogARB(vertexShader, infoLogLength, &charsWritten, infoLog.data());
		emit buildMessage(infoLog, 0, m_outputParser);
		
		fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		
		emit infoMessage(tr("Compiling fragment shader..."));
		const char * fragmentStrings[] = { m_fragmentShaderText.data() };
		glShaderSourceARB(fragmentShader, 1, fragmentStrings, NULL);
		glCompileShaderARB(fragmentShader);
		
		// Get error log.
		infoLog.clear();
		glGetObjectParameterivARB(fragmentShader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infoLogLength);
		infoLog.resize(infoLogLength);
		glGetInfoLogARB(fragmentShader, infoLogLength, &charsWritten, infoLog.data());
		emit buildMessage(infoLog, 1, m_outputParser);
		
		// Check compilation.
		GLint vertexCompileSucceed = GL_FALSE;
		glGetObjectParameterivARB(vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &vertexCompileSucceed);
		if( vertexCompileSucceed == GL_FALSE )
		{
			glDeleteObjectARB(vertexShader);
			return false;
		}
		
		GLint fragmentCompileSucceed = GL_FALSE;
		glGetObjectParameterivARB(fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &fragmentCompileSucceed);
		if( fragmentCompileSucceed == GL_FALSE )
		{
			glDeleteObjectARB(vertexShader);
			glDeleteObjectARB(fragmentShader);
			return false;
		}
		
		program = glCreateProgramObjectARB();
		
		// Link the program.
		emit infoMessage(tr("Linking..."));
		glAttachObjectARB(program, vertexShader);
		glAttachObjectARB(program, fragmentShader);
		glLinkProgramARB(program);
		
		// Get error log.
		infoLog.clear();
		glGetObjectParameterivARB(program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infoLogLength);
		infoLog.resize(infoLogLength);
		glGetInfoLogARB(program, infoLogLength, &charsWritten, infoLog.data());
		emit buildMessage(infoLog, -1, m_outputParser);
		
		// Test linker result.
		GLint linkSucceed = GL_FALSE;
		glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &linkSucceed);
		if( linkSucceed == GL_FALSE )
		{
			glDetachObjectARB(program, vertexShader);
			glDetachObjectARB(program, fragmentShader);
			glDeleteObjectARB(vertexShader);
			glDeleteObjectARB(fragmentShader);
			glDeleteObjectARB(program);
			return false;
		}
		
		// Delete previous effect.
		deleteProgram();
		
		Q_ASSERT( m_vertexShader == 0 && vertexShader != 0 );
		Q_ASSERT( m_fragmentShader == 0 && fragmentShader != 0 );
		Q_ASSERT( m_program == 0 && program != 0 );
		
		m_vertexShader = vertexShader;
		m_fragmentShader = fragmentShader;
		m_program = program;
		
		initParameters();
		
		return true;
	}
	
	virtual void build(bool threaded)
	{
#if defined(Q_OS_LINUX)
		threaded = false;
#endif

		if (threaded) {
			m_thread.start();
		}
		else {
			this->makeCurrent();
			bool succeed = threadedBuild();
			emit built(succeed);
		}
	}
	
	virtual bool isBuilding() const 
	{
		return m_thread.isRunning();
	}

	// Parameter info.
	virtual int parameterCount() const
	{
		return m_parameterArray.count();		
	}

	virtual const Parameter * parameterAt(int idx) const
	{
		Q_ASSERT(idx >= 0 && idx < m_parameterArray.count());
		return m_parameterArray.at(idx);
	}

	virtual Parameter * parameterAt(int idx)
	{
		Q_ASSERT(idx >= 0 && idx < m_parameterArray.count());
		return m_parameterArray.at(idx);
	}

	virtual bool isValid() const
	{
		return m_program != 0;
	}

	virtual bool isAnimated() const
	{
		return m_timeUniform != -1;
	}


	// Technique info.
	virtual int getTechniqueNum() const
	{
		return 1;
	}

	virtual QString getTechniqueName(int) const
	{
		return tr("Default");
	}

	virtual void selectTechnique(int)
	{
		// nothing to do here.
	}

	// Pass info.
	virtual int getPassNum() const
	{
		return 1;
	}

	// Rendering.
	virtual void begin()
	{
		Q_ASSERT(m_program != 0);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glUseProgramObjectARB(m_program);

		// Set uniforms.
		setParameters();
	}

	virtual void beginPass(int )
	{
		// nothing to do.
	}
	
	virtual void beginMaterialGroup()
	{
		// needs to be called every time the material changes on ATI hardware
		glUseProgramObjectARB(m_program);
	}

	virtual void endPass()
	{
	}

	virtual void end()
	{
		glUseProgramObjectARB(0); // ???
	}

private:

	void deleteProgram()
	{
		if( m_program != 0 ) {
			if( m_vertexShader != 0 ) {
				glDetachObjectARB(m_program, m_vertexShader);
			}
			if( m_fragmentShader != 0 ) {
				glDetachObjectARB(m_program, m_fragmentShader);
			}
			glDeleteObjectARB(m_program);
			m_program = 0;
		}
		if( m_vertexShader != 0 ) {
			glDeleteObjectARB(m_vertexShader);
			m_vertexShader = 0;
		}
		if( m_fragmentShader != 0 ) {
			glDeleteObjectARB(m_fragmentShader);
			m_fragmentShader = 0;
		}
	}

	void initParameters()
	{
		m_timeUniform = -1;
		
		if( m_program == 0 ) {
			return;
		}

		GLint count = 0;
		glGetObjectParameterivARB(m_program, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &count);

		QVector<GLSLParameter*> newParameterArray;

		for(int i = 0; i < count; i++) {
			char str[1024];
			GLsizei length;
			GLint size;
			GLenum type;
			glGetActiveUniformARB(m_program, i, 1024, &length, &size, &type, str);

			QString name(str);

			// skip gl uniforms.
			if( name.startsWith("gl_") ) {
				continue;
			}

			// Get standard uniforms.
			if( name.toLower() == "time" ) {
				m_timeUniform = glGetUniformLocationARB(m_program, str);
				continue;
			}

			int location = glGetUniformLocationARB(m_program, str);
			
			// Add parameter
			if( size == 1 ) {
				GLSLParameter * param = new GLSLParameter(name, type, location);
				param->setValue(getParameterValue(param));
				newParameterArray.push_back(param);
			}
			else {
				// parameter array.
				for(int i = 0; i < size; i++) {
					GLSLParameter * param = new GLSLParameter(name + "[" + QString::number(i) + "]", type, location+i);
					param->setValue(getParameterValue(param));
					newParameterArray.push_back(param);
				}
			}
		}

		qDeleteAll(m_parameterArray);
		m_parameterArray = newParameterArray;

		// Get number of texture units.
		GLint texUnitNum = 8;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &texUnitNum);

		// Init texture parameters.
		int parameterNum = m_parameterArray.count();
		int unit = 0;

		for(int i = 0; i < parameterNum; i++) {
			GLSLParameter * parameter = m_parameterArray[i];

			if (parameter->isTexture())
			{
				if( unit < texUnitNum ) {
					parameter->setTextureUnit(unit++);
				}
				else {
					emit errorMessage(tr("Texture unit limit hit, ignoring parameter '%1'").arg(parameter->name()));
				}
			}
		}
	}

	void setParameters()
	{
		// Set user parameters
		foreach(GLSLParameter * p, m_parameterArray) {
			setParameter(p);
		}

		// Set standard parameters.
		if( m_timeUniform != -1 ) {
			glUniform1fARB(m_timeUniform, 0.001f * m_time.elapsed());
		}
	}

	void setParameter(const GLSLParameter * param)
	{
		switch( param->glType() ) {
			case GL_FLOAT:
				glUniform1fARB(param->location(), float(param->value().toDouble()));
				break;
			case GL_FLOAT_VEC2_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 2);
				glUniform2fARB(param->location(), list.at(0).toDouble(), list.at(1).toDouble());
				break;
			}
			case GL_FLOAT_VEC3_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 3);	
				glUniform3fARB(param->location(), list.at(0).toDouble(), list.at(1).toDouble(), list.at(2).toDouble());
				break;
			}
			case GL_FLOAT_VEC4_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 4);
				glUniform4fARB(param->location(), list.at(0).toDouble(), list.at(1).toDouble(), list.at(2).toDouble(), list.at(3).toDouble());
				break;
			}
			case GL_INT:
				glUniform1iARB(param->location(), param->value().toInt());
				break;
			case GL_INT_VEC2_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 2);
				glUniform2iARB(param->location(), list.at(0).toInt(), list.at(1).toInt());
				break;
			}
			case GL_INT_VEC3_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 3);
				glUniform3iARB(param->location(), list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
				break;
			}
			case GL_INT_VEC4_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 4);
				glUniform4iARB(param->location(), list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt(), list.at(3).toInt());
				break;
			}
			case GL_BOOL_ARB:
				glUniform1iARB(param->location(), param->value().toBool());
				break;
			case GL_BOOL_VEC2_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 2);
				glUniform2iARB(param->location(), list.at(0).toBool(), list.at(1).toBool());
				break;
			}
			case GL_BOOL_VEC3_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 3);
				glUniform3iARB(param->location(), list.at(0).toBool(), list.at(1).toBool(), list.at(2).toBool());
				break;
			}
			case GL_BOOL_VEC4_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 4);
				glUniform4iARB(param->location(), list.at(0).toBool(), list.at(1).toBool(), list.at(2).toBool(), list.at(3).toBool());
				break;
			}
			case GL_FLOAT_MAT2_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 4);
				
				float values[4];
				for(int i = 0; i < 2*2; i++) {
					values[i] = (float)list.at(0).toDouble();
				}
				
				glUniformMatrix2fv(param->location(), 1, false, values);
				break;
			}
			case GL_FLOAT_MAT3_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 9);
				
				float values[9];
				for(int i = 0; i < 3*3; i++) {
					values[i] = (float)list.at(0).toDouble();
				}
				
				glUniformMatrix3fv(param->location(), 1, false, values);
				break;
			}
			case GL_FLOAT_MAT4_ARB:
			{
				Q_ASSERT(param->value().canConvert(QVariant::List));
				QVariantList list = param->value().toList();
				Q_ASSERT(list.count() == 16);
				
				float values[16];
				for(int i = 0; i < 4*4; i++) {
					values[i] = (float)list.at(0).toDouble();
				}
				
				glUniformMatrix4fv(param->location(), 1, false, values);
				break;
			}
			case GL_SAMPLER_1D_ARB:
			case GL_SAMPLER_2D_ARB:
			case GL_SAMPLER_3D_ARB:
			case GL_SAMPLER_CUBE_ARB:
			case GL_SAMPLER_2D_RECT_ARB: {
				GLTexture tex = param->value().value<GLTexture>();
				glUniform1iARB(param->location(), param->textureUnit());
				glActiveTextureARB(GL_TEXTURE0_ARB + param->textureUnit());
				glBindTexture(tex.target(), tex.object());
				break;
			}
			case GL_SAMPLER_1D_SHADOW_ARB:
			case GL_SAMPLER_2D_SHADOW_ARB:
			case GL_SAMPLER_2D_RECT_SHADOW_ARB:
				break;
		}
	}

	QVariant getParameterValue(const GLSLParameter * param)
	{
		// Try to get old value.
		foreach(const GLSLParameter * p, m_parameterArray) {
			if( p->name() == param->name() ) {
				return p->value();
			}
		}					
		
		// Get default value of the corresponding type.
		switch( param->glType() ) {
			case GL_FLOAT:
			{
				GLfloat fvalue[1];
				glGetUniformfvARB(m_program, param->location(), fvalue);
				return fvalue[0];
			}
			case GL_FLOAT_VEC2_ARB:
			{
				GLfloat fvalue[2];
				QList<QVariant> list;
				glGetUniformfvARB(m_program, param->location(), fvalue);
				list << fvalue[0] << fvalue[1];
				return list;
			}
			case GL_FLOAT_VEC3_ARB:
			{
				GLfloat fvalue[3];
				QList<QVariant> list;
				glGetUniformfvARB(m_program, param->location(), fvalue);
				list << fvalue[0] << fvalue[1] << fvalue[2];
				return list;
			}
			case GL_FLOAT_VEC4_ARB:
			{
				GLfloat fvalue[4];
				QList<QVariant> list;
				glGetUniformfvARB(m_program, param->location(), fvalue);
				list << fvalue[0] << fvalue[1] << fvalue[2] << fvalue[3];
				return list;
			}
			case GL_INT:
			{
				GLint ivalue[1];
				glGetUniformivARB(m_program, param->location(), ivalue);
				return (int)ivalue[0];
			}
			case GL_INT_VEC2_ARB:
			{
				GLint ivalue[2];
				QList<QVariant> list;
				glGetUniformivARB(m_program, param->location(), ivalue);
				list << (int)ivalue[0] << (int)ivalue[1];
				return list;
			}
			case GL_INT_VEC3_ARB:
			{
				GLint ivalue[3];
				QList<QVariant> list;
				glGetUniformivARB(m_program, param->location(), ivalue);
				list << (int)ivalue[0] << (int)ivalue[1] << (int)ivalue[2];
				return list;
			}
			case GL_INT_VEC4_ARB:
			{
				GLint ivalue[4];
				QList<QVariant> list;
				glGetUniformivARB(m_program, param->location(), ivalue);
				list << (int)ivalue[0] << (int)ivalue[1] << (int)ivalue[2] << (int)ivalue[3];
				return list;
			}
			case GL_BOOL_ARB:
			{
				GLint ivalue[1];
				glGetUniformivARB(m_program, param->location(), ivalue);
				return ivalue[0] != 0;
			}
			case GL_BOOL_VEC2_ARB:
			{
				GLint ivalue[2];
				QList<QVariant> list;
				glGetUniformivARB(m_program, param->location(), ivalue);
				list << (ivalue[0] != 0) << (ivalue[1] != 0);
				return list;
			}
			case GL_BOOL_VEC3_ARB:
			{
				GLint ivalue[3];
				QList<QVariant> list;
				glGetUniformivARB(m_program, param->location(), ivalue);
				list << (ivalue[0] != 0) << (ivalue[1] != 0) << (ivalue[2] != 0);
				return list;
			}
			case GL_BOOL_VEC4_ARB:
			{
				GLint ivalue[4];
				QList<QVariant> list;
				glGetUniformivARB(m_program, param->location(), ivalue);
				list << (ivalue[0] != 0) << (ivalue[1] != 0) << (ivalue[2] != 0) << (ivalue[3] != 0);
				return list;
			}
			case GL_FLOAT_MAT2_ARB:
			{
				GLfloat fvalue[2*2];
				QList<QVariant> list;
				glGetUniformfvARB(m_program, param->location(), fvalue);
				for(int i = 0; i < 2*2; i++) {
					list.append(fvalue[i]);
				}
				return list;
			}
			case GL_FLOAT_MAT3_ARB:
			{
				GLfloat fvalue[3*3];
				QList<QVariant> list;
				glGetUniformfvARB(m_program, param->location(), fvalue);
				for(int i = 0; i < 3*3; i++) {
					list.append(fvalue[i]);
				}
				return list;
			}
			case GL_FLOAT_MAT4_ARB:
			{
				GLfloat fvalue[4*4];
				QList<QVariant> list;
				glGetUniformfvARB(m_program, param->location(), fvalue);
				for(int i = 0; i < 4*4; i++) {
					list.append(fvalue[i]);
				}
				return list;
			}
			case GL_SAMPLER_1D_ARB:
			case GL_SAMPLER_2D_ARB:
			case GL_SAMPLER_3D_ARB:
			case GL_SAMPLER_CUBE_ARB:
			case GL_SAMPLER_2D_RECT_ARB:
				return qVariantFromValue(GLTexture());
			case GL_SAMPLER_1D_SHADOW_ARB:
			case GL_SAMPLER_2D_SHADOW_ARB:
			case GL_SAMPLER_2D_RECT_SHADOW_ARB:
				break;
		}

		return QVariant();
	}

	static QString getParameterAssignment(const GLSLParameter * param, const QDir & dir)
	{
		QString typeName = getTypeName(param->glType());
		
		switch( param->glType() ) {
			case GL_FLOAT:
			case GL_INT:
			case GL_BOOL_ARB:
				return typeName + " " + param->name() + " = " + param->value().toString() + ";\n";
			case GL_FLOAT_VEC2_ARB:
			case GL_FLOAT_VEC3_ARB:
			case GL_FLOAT_VEC4_ARB:
			case GL_INT_VEC2_ARB:
			case GL_INT_VEC3_ARB:
			case GL_INT_VEC4_ARB:
			case GL_BOOL_VEC2_ARB:
			case GL_BOOL_VEC3_ARB:
			case GL_BOOL_VEC4_ARB:
			case GL_FLOAT_MAT2_ARB:
			case GL_FLOAT_MAT3_ARB:
			case GL_FLOAT_MAT4_ARB:
			{
				QStringList list = param->value().toStringList();
				QString value = list.join(", ");
				return typeName + " " + param->name() + " = " + typeName + "(" + value + ");\n";
			}
			case GL_SAMPLER_1D_ARB:
			case GL_SAMPLER_2D_ARB:
			case GL_SAMPLER_3D_ARB:
			case GL_SAMPLER_CUBE_ARB:
			case GL_SAMPLER_2D_RECT_ARB:
			case GL_SAMPLER_1D_SHADOW_ARB:
			case GL_SAMPLER_2D_SHADOW_ARB:
			case GL_SAMPLER_2D_RECT_SHADOW_ARB:
			{
				GLTexture tex = param->value().value<GLTexture>();
				QString path = dir.relativeFilePath(tex.name());
				return typeName + " " + param->name() + " = load(\"" + path + "\");\n";
			}
		}
		
		return "";
	}
	

	// Hacky parameter parser.
	void parseParameter(QString line, const QDir & dir)
	{
		if( line.startsWith("//") ) {
			// Skip C++ comments.
			return;
		}

		static QRegExp paramRegExp("^\\s*(\\w+)\\s+(\\w+)\\s*=(.*);\\s*$");

		if( !paramRegExp.exactMatch(line) ) {
			// @@ Display warning.
			return;
		}

		QStringList tokens =  paramRegExp.capturedTexts();
		const int count = tokens.count();
		Q_ASSERT(count == 4);

		GLSLParameter * param = new GLSLParameter(tokens[2], getType(tokens[1]), -1);

		QString value = tokens[3].trimmed();

		if( param->glType() == GL_FLOAT ) {
			param->setValue(value.toDouble());
		}
		else if( param->glType() == GL_INT ) {
			param->setValue(value.toInt());
		}
		else if( param->glType() == GL_BOOL ) {
			param->setValue(value == "true");
		}
		else if( value.startsWith(tokens[1]) ) {
			int begin = value.indexOf("(");
			int end = value.indexOf(")");
			QStringList args = value.mid(begin+1, end-begin-1).split(",");

			int baseType = getBaseType(param->glType());

			// Convert the components.
			QVariantList valueList;
			foreach(QString arg, args) {
				if( baseType == GL_FLOAT ) {
					valueList.append(arg.toDouble());
				}
				else if( baseType == GL_INT ) {
					valueList.append(arg.toInt());
				}
				else if( baseType == GL_BOOL ) {
					valueList.append(arg.trimmed() == "true");
				}
				else {	// @@ WTF ???
					valueList.append(arg.trimmed());
				}
			}
			param->setValue(valueList);
		}
		else if( param->isTexture())
		{
			static QRegExp loadRegExp("^\\s*load\\(\"(.*)\"\\)\\s*$");

			if( !loadRegExp.exactMatch(tokens[3]) ) {
				// @@ Display warning.
				return;
			}

			QString fileName = loadRegExp.cap(1);
			QString filePath = dir.absoluteFilePath(fileName);
			
			QFileInfo pathInfo(filePath);
			if (!pathInfo.isFile() || !pathInfo.exists())
			{
				filePath = fileName;
			}
			
			QVariant tex;
			tex.setValue(GLTexture::open(filePath));
			param->setValue(tex);
		}
		else {
			qDebug("unknown parameter type");
		}

		m_parameterArray.append(param);
	}
};


/// GLSL effect factory.
class GLSLEffectFactory : public EffectFactory
{
	virtual bool isSupported() const
	{
		return GLEW_ARB_fragment_shader && GLEW_ARB_vertex_shader && GLEW_ARB_shader_objects && GLEW_ARB_shading_language_100;
	}

	virtual QString name() const
	{
		return tr("GLSL Shader");
	}

	virtual QString namePlural() const
	{
		return tr("GLSL Shaders");
	}

	virtual QString extension() const
	{
		return "glsl";
	}

	virtual QIcon icon() const
	{
		return QIcon();
	}

	virtual bool savesParameters() const
	{
		return true;
	}
	
	virtual Effect * createEffect(QGLWidget * widget) const
	{
		Q_ASSERT(isSupported());
 		return new GLSLEffect(this, widget);
	}

	QList<Highlighter::Rule> highlightingRules() const
	{
		QList<Highlighter::Rule> rules;
		Highlighter::Rule rule;

		rule.type = Highlighter::Keyword;
                /*rule.pattern = QRegExp("\\b(if|else|for|while|do|struct|break|continue|discard|return|true|false)\\b");*/
                QString s = Highlighter::getSyntaxKeyWords(extension(),"kw");
                if(!s.isEmpty())
                    rule.pattern = QRegExp("\\b("+s+")\\b");
		rules.append(rule);

		rule.type = Highlighter::DataType;
                /*rule.pattern = QRegExp(
			"\\b(void|float|vec[2-4]|int|ivec[2-4]|bool|bvec[2-4]|mat[2-4]|sampler[1-3]D|"
                        "samplerCube|sampler[1-2]DShadow|uniform|attribute|varying|const|in|out|inout)\\b");*/
                s = Highlighter::getSyntaxKeyWords(extension(),"dt");
                if(!s.isEmpty())
                    rule.pattern = QRegExp("\\b("+s+")\\b");
		rules.append(rule);

		rule.type = Highlighter::BuiltinVar;
                /*rule.pattern = QRegExp(
			"\\b(gl_(Position|PointSize|ClipVertex|FragCoord|FragFacing|FragColor|"
			"FragData|FragDepth|Color|SecondaryColor|Normal|Vertex|MultiTexCoord[0-7]|FogColor|"
			"MaxLights|MaxClipPlanes|MaxTextureUnits|MaxTextureCoords|MaxVertexAttributes|"
			"MaxVertexUniformComponents|MaxVaryingFloats|MaxVertexTextureImageUnits|MaxCombinedTextureImageUnits|"
			"MaxTextureImageUnits|MaxFragmentUniformComponents|MaxDrawBuffers|ModelViewMatrix|ProjectionMatrix|"
			"ModelViewProjectionMatrix|TextureMatrix|NormalMatrix|ModelViewMatrixInverse|ProjectionMatrixInverse|"
			"ModelViewProjectionMatrixInverse|TextureMatrixInverse|ModelViewMatrixTranspose|ProjectionMatrixTranspose|"
			"ModelViewProjectionMatrixTranspose|TextureMatrixTranspose|ModelViewMatrixInverseTranspose|ProjectionMatrixInverseTranspose|"
			"ModelViewProjectionMatrixInverseTranspose|TextureMatrixInverseTranspose|NormScale|DepthRangeParameters|DepthRange|"
			"ClipPlane|PointParameters|Point|MaterialParameters|FrontMaterial|BackMaterial|LightSourceParameters|LightSource|"
			"LightModelParameters|LightModel|LightModelProducts|FrontLightModelProduct|BackLightModelProduct|LightProducts|"
			"FrontLightProduct|BackLightProduct|TextureEnvColor|EyePlaneS|EyePlaneT|EyePlaneR|EyePlaneQ|ObjectPlaneS|ObjectPlaneT|"
			"ObjectPlaneR|ObjectPlaneQ|FogParameters|Fog|FrontColor|BackColor|FrontSecondaryColor|BackSecondaryColor|TexCoord|FogFragCoord|Color|"
                        "SecondaryColor))\\b");*/
                s = Highlighter::getSyntaxKeyWords(extension(),"bv");
                if(!s.isEmpty())
                    rule.pattern = QRegExp("\\b("+s+")\\b");
		rules.append(rule);

		rule.type = Highlighter::BuiltinFunction;
                /*rule.pattern = QRegExp(
			"\\b(radians|degrees|sin|cos|tan|asin|acos|atan|pow|exp|log|exp2|log2|sqrt|inversesqrt|"
			"abs|sign|floor|ceil|fract|mod|min|max|clamp|mix|step|smoothstep|length|distance|dot|cross|normalize|ftransform|"
			"faceforward|reflect|refract|matrixCompMult|lessThan|lessThenEqual|greaterThan|greaterThanEqual|equal|notEqual|"
			"any|all|not|texture1D|texture1DProj|texture1DLod|texture1DProjLod|texture2D|texture2DProj|texture2DLod|"
			"texture2DProjLod|texture3D|texture3DProj|texture3DLod|texture3DProjLod|textureCube|textureCubeLod|shadow1D|"
			"shadow2D|shadow1DProj|shadow2DProj|shadow1DLod|shadow2DLod|shadow1DProjLod|shadow2DProjLod|dFdx|dFdy|fwidth|"
                        "noise1|noise2|noise3|noise4)\\b");*/

                s = Highlighter::getSyntaxKeyWords(extension(),"bf");
                if(!s.isEmpty())
                    rule.pattern = QRegExp("\\b("+s+")\\b");

		rules.append(rule);

		rule.type = Highlighter::Number;
		rule.pattern = QRegExp("\\b[-+]?\\d*\\.?\\d+([eE][-+]?\\d+)?\\b");

		rules.append(rule);

		rule.type = Highlighter::Comment;
		rule.pattern = QRegExp("//.*$");
		rules.append(rule);

		return rules;
	}

	QString multiLineCommentStart() const
	{
		return "/*";
	}

	QString multiLineCommentEnd() const
	{
		return "*/";
	}
};

REGISTER_EFFECT_FACTORY(GLSLEffectFactory);

