
#include "effect.h"
#include "messagepanel.h"
#include "outputparser.h"

#include <math.h>

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QTime>
#include <QtCore/QVariant>

#include <GL/glew.h>

#include <QtOpenGL/QGLContext>

namespace {

	// Default vertex program.
	static const char * s_vertexProgramText =
		"!!ARBvp1.0\n\n"
		
		"PARAM mvp[4]  = { state.matrix.mvp };\n"
		"PARAM mvi[4] = { state.matrix.modelview.inverse };\n\n"
		
		"ATTRIB iVertex = vertex.position;\n"
		"ATTRIB iNormal = vertex.normal;\n\n"
		
		"OUTPUT oPos     = result.position;\n"
		"OUTPUT normal   = result.texcoord[0];\n"
		"OUTPUT viewVec  = result.texcoord[1];\n\n"
		
		"DP4 oPos.x, mvp[0], iVertex;\n"
		"DP4 oPos.y, mvp[1], iVertex;\n"
		"DP4 oPos.z, mvp[2], iVertex;\n"
		"DP4 oPos.w, mvp[3], iVertex;\n\n"
		
		"MOV normal, iNormal;\n\n"
		
		"PARAM cameraPos = state.matrix.modelview.row[3];\n"
		"TEMP cp;\n"
		"DP4 cp.x, mvi[0], cameraPos;\n"
		"DP4 cp.y, mvi[1], cameraPos;\n"
		"DP4 cp.z, mvi[2], cameraPos;\n"
		"SUB	viewVec, cp, iVertex;\n\n"
		
		"END";
	

	// Default fragment program.
	static const char * s_fragmentProgramText =
		"!!ARBfp1.0\n\n"
		
		"PARAM specExp = 8;\n"
		"PARAM color = { 1, 0, 0, 0 };\n"
		"#PARAM lightVec = state.light[0].position;\n"
		"PARAM lightVec = {0.577, 0.577, 0.577, 0};\n\n"
		
		"TEMP normal, viewVec, reflVec, diffuse, specular, light;\n\n"
		
		"DP3		normal.w, fragment.texcoord[0], fragment.texcoord[0];\n"
		"RSQ		normal.w, normal.w;\n"
		"MUL		normal, fragment.texcoord[0], normal.w;\n\n"
		
		"DP3		viewVec.w, fragment.texcoord[1], fragment.texcoord[1];\n"
		"RSQ		viewVec.w, viewVec.w;\n"
		"MUL		viewVec, fragment.texcoord[1], viewVec.w;\n\n"
		
		"DP3		reflVec, viewVec, normal;\n"
		"MUL		reflVec, reflVec, normal;\n"
		"MAD		reflVec, reflVec, 2, -viewVec;\n\n"
		
		"DP3_SAT	diffuse, lightVec, normal;\n"
		"DP3_SAT	specular, reflVec, lightVec;\n"
		"POW		specular, specular.x, specExp.x;\n\n"
		
		"MAD		light, diffuse, color, specular;\n"
		"MAD		result.color, color, 0.1, light;\n\n"
		
		"END";

	
	// ARB shader file tags.
	static const char * s_vertexProgramTag = "[VertexProgram]\n";
	static const char * s_fragmentProgramTag = "[FragmentProgram]\n";
	static const char * s_parametersTag = "[Parameters]\n";
	
	// ARB Parameter.
	struct ARBParameter
	{
		QString name;
		QVariant value;
		GLint location;
		GLenum type;
	};
	
	// ARB texture unit.
	struct ARBTexUnit
	{
		int parameter;
		GLuint texObject;
	};

} // namespace


class ARBEffect : public Effect
{
private:
	
	QByteArray m_vertexProgramText;
	QByteArray m_fragmentProgramText;

	QTime m_time;
	
	QVector<ARBParameter> m_parameterArray;
	QVector<ARBParameter> m_oldParameterArray;
	
	GLuint m_vp;
	GLuint m_fp;
	
	OutputParser* m_outputParser;

public:
	
	// Ctor.
	ARBEffect(const EffectFactory * factory) : Effect(factory),
		m_vertexProgramText(s_vertexProgramText),
		m_fragmentProgramText(s_fragmentProgramText)
	{
		m_time.start();

		const char* vendor = (const char*)glGetString(GL_VENDOR);
		if (strcmp(vendor, "ATI Technologies Inc.") == 0)
			m_outputParser = new AtiAsmOutputParser;
		else if (strcmp(vendor, "NVIDIA Corporation") == 0)
			m_outputParser = new NvidiaAsmOutputParser;
	}
	
	virtual ~ARBEffect()
	{
		deletePrograms();
		delete m_outputParser;
	}
	
	
	// Load the effect from the given file.
	virtual void load(QFile * file)
	{
		Q_ASSERT(file != NULL);
		
		m_vertexProgramText.clear();
		m_fragmentProgramText.clear();
		
		QByteArray line;
		while (!file->atEnd()) {
			
			if( line.startsWith(s_vertexProgramTag) ) {
				// Read vertex shader.
				while (!file->atEnd()) {
					line = file->readLine();
					if( line.startsWith('[') ) {
						break;
					}
					m_vertexProgramText.push_back(line);
				}
				
				continue;
			}
			
			if( line.startsWith(s_fragmentProgramTag) ) {
				// Read fragment shader.
				while (!file->atEnd()) {
					line = file->readLine();
					if( line.startsWith('[') ) {
						break;
					}
					m_fragmentProgramText.push_back(line);
				}
				
				continue;
			}
			
			/*
			if( line.startsWith(s_parametersTag) ) {
				// Parse parameters.
				while (!file->atEnd()) {
					line = file->readLine();
					if( line.startsWith('[') ) {
						break;
					}
					// Parse parameter.
					parseParameter(line);
				}
				continue;
			}
			*/
			
			line = file->readLine();
		}
		
		m_time.start();
	}
	
	// Save the effect in the given file.
	virtual void save(QFile * file)
	{
		Q_ASSERT(file != NULL);
		
		// [VertexProgram]
		file->write(s_vertexProgramTag, strlen(s_vertexProgramTag));
		file->write(m_vertexProgramText);
		if(!m_vertexProgramText.endsWith('\n')) {
			file->write("\n");
		}
		
		// [FragmentProgram]
		file->write(s_fragmentProgramTag, strlen(s_fragmentProgramTag));
		file->write(m_fragmentProgramText);
		if(!m_fragmentProgramText.endsWith('\n')) {
			file->write("\n");
		}
		
		/*
		if( this->getParameterNum() > 0 ) {
			// [Parameters]
			file->write(s_parametersTag, strlen(s_parametersTag));
			
			foreach(GLSLParameter p, m_parameterArray) {
				QString str = getParameterAssignment(p);
				file->write(str.toLatin1());
			}
		}
		*/
	}
	
	virtual int getInputNum()
	{
		return 2;
	}
	
	virtual QString getInputName(int i)
	{
		Q_ASSERT(i == 0 || i == 1);
		
		if( i == 0 ) {
			return tr("Vertex Program");
		}
		else {
			return tr("Fragment Program");
		}
	}
	
	virtual const QByteArray & getInput(int i) const
	{
		Q_ASSERT(i == 0 || i == 1);
		
		if( i == 0 ) {
			return m_vertexProgramText;
		}
		else {
			return m_fragmentProgramText;
		}		
	}	
	
	virtual void setInput(int i, const QByteArray & txt)
	{
		Q_ASSERT(i == 0 || i == 1);
		
		if( i == 0 ) {
			m_vertexProgramText = txt;
		}
		else {
			m_fragmentProgramText = txt;
		}		
	}
	
	virtual bool build(MessagePanel * output)
	{
		deletePrograms();
		
		bool succeed = true;
		
		if(output != NULL) output->clear();
		
		if(output != NULL) output->info("Compiling vertex program...");
		glGenProgramsARB( 1, &m_vp );
		glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_vp );
		glProgramStringARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, m_vertexProgramText.length(), m_vertexProgramText );
		if( checkProgramError(output, 0) ) {
			succeed = false;
		}
		else {
			ParseProgram(m_vertexProgramText);
		}
		glDisable( GL_VERTEX_PROGRAM_ARB );
		
		if(output != NULL) output->info("Compiling fragment program...");
		glGenProgramsARB( 1, &m_fp );
		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, m_fp );
		glProgramStringARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, m_fragmentProgramText.length(), m_fragmentProgramText );
		if( checkProgramError(output, 1) ) {
			succeed = false;
		}
		else {
			ParseProgram(m_fragmentProgramText);
		}
		glDisable( GL_FRAGMENT_PROGRAM_ARB );
		
		return succeed;
	}	
	
	
	// Parameter info.
	virtual int getParameterNum() const
	{
		return m_parameterArray.count();
	}
	
	virtual QString getParameterName(int idx) const
	{
		Q_ASSERT(idx >= 0 && idx < m_parameterArray.count());
		return m_parameterArray[idx].name;
	}
	
	virtual QVariant getParameterValue(int idx) const
	{
		Q_ASSERT(idx >= 0 && idx < m_parameterArray.count());
		return m_parameterArray[idx].value;
	}
	
	virtual void setParameterValue(int idx, const QVariant & value)
	{
		Q_ASSERT(idx >= 0 && idx < m_parameterArray.count());
		m_parameterArray[idx].value = value;
	}
	
	virtual EditorType getParameterEditor(int idx) const
	{
		Q_ASSERT(idx >= 0 && idx < m_parameterArray.count());
		return EditorType_Vector;
	}
	
	virtual int getParameterRows(int idx) const 
	{
		Q_ASSERT(idx >= 0 && idx < m_parameterArray.count());
		return 4;
	}
	
	virtual int getParameterColumns(int idx) const
	{
		Q_ASSERT(idx >= 0 && idx < m_parameterArray.count());
		return 1;
	}
	
	virtual bool isValid() const
	{
		return true;
	}
	
	virtual bool isAnimated() const
	{
		return false;
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
	//	Q_ASSERT(m_program != 0);
		
		glEnable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		
		// Setup ligh parameters
		float light_vector[4] = {1.0f/sqrt(3.0f), 1.0f/sqrt(3.0f), 1.0f/sqrt(3.0f), 0.0f};
		glLightfv( GL_LIGHT0, GL_POSITION, light_vector );
		
		// Enable programs.
		glEnable( GL_VERTEX_PROGRAM_ARB );
		glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_vp );
		
		glEnable( GL_FRAGMENT_PROGRAM_ARB );
		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, m_fp );
		
		// Set uniforms.
	//	setParameters();
	}
	
	virtual void beginPass(int )
	{
		// nothing to do.
	}
	
	virtual void endPass()
	{
	}
	
	virtual void end()
	{
		glDisable( GL_FRAGMENT_PROGRAM_ARB );
		glDisable( GL_VERTEX_PROGRAM_ARB );
	}

private:
	
	void deletePrograms()
	{
		glDeleteProgramsARB( 1, &m_vp );
		glDeleteProgramsARB( 1, &m_fp );
	}
	
	bool checkProgramError(MessagePanel * output, int inputNumber = -1)
	{
		int position;
		glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB, &position );
		if( position != -1 ) {
			const char * error = (const char *) glGetString( GL_PROGRAM_ERROR_STRING_ARB );
			if( output ) output->log(QString(error), inputNumber, m_outputParser);
			qDebug("%s", error);
			return true;
		}
		
		return false;
	}
	
	void ParseProgram(const QByteArray & code)
	{
		// Get parameters. @@ Create these regular expressions only once.
		QRegExp paramRegExp("\\s*PARAM\\s+(\\w+)(?:\\[(\\d+)\\])?\\s*=\\s*(\\S.*)");
		QRegExp commentRegExp("#.*$");
		
		int start = 0;
		int end = qMin(code.indexOf(';'), code.indexOf('\n'));
		
		while( end != -1 ) {
			QString statement = code.mid(start, end-start);
			
			if(!commentRegExp.exactMatch(statement)) {
				if(paramRegExp.exactMatch(statement)) {
					QString name = paramRegExp.cap(1);
					QString size = paramRegExp.cap(2);
					QString value = paramRegExp.cap(3);
					
					if(size.isEmpty()) {
						AddVectorParameter(name, value);
					}
					else {
						AddMatrixParameter(name, size.toInt(), value);
					}
				}
			}
			
			start = end+1;
			end = qMin(code.indexOf(';', end+1), code.indexOf('\n', end+1));
		}
	}
	
	void AddVectorParameter(const QString & name, const QString & value)
	{
		qDebug() << "Vector:" << name << "=" << value;
	}
	
	void AddMatrixParameter(const QString & name, int size, const QString & value)
	{
		qDebug() << "Matrix:" << QString(name).append("[").append(QString::number(size)).append("]") << "=" << value;
	}
	
};


class ARBEffectFactory : public EffectFactory
{
	virtual bool isSupported() const
	{
		return GLEW_ARB_vertex_program && GLEW_ARB_fragment_program;
	}
	
	virtual QString name() const
	{
		return tr("ARB Shader");
	}
	
	virtual QString namePlural() const
	{
		return tr("ARB Shaders");
	}
	
	virtual QString extension() const
	{
		return "asm";
	}
	
	virtual QIcon icon() const
	{
		return QIcon();
	}
	
	virtual Effect * createEffect() const
	{
		Q_ASSERT(isSupported());
		return new ARBEffect(this);
	}

	virtual QList<Highlighter::Rule> highlightingRules() const
	{
		QList<Highlighter::Rule> rules;
		Highlighter::Rule rule;

		rule.type = Highlighter::Keyword;
		rule.pattern = QRegExp(
			"\\b(ABS|ABS_SAT|ADD|ADD_SAT|ADDRESS|ALIAS|ARL|ATTRIB|CMP(_SAT)?|COS(_SAT)?|"
			"DP3(_SAT)?|DP4(_SAT)?|DPH(_SAT)?|DST(_SAT)?|"
			"END|EXP|EX2(_SAT)?|FLR(_SAT)?|FRC(_SAT)?|KIL|LG2(_SAT)?|"
			"LIT(_SAT)?|LOG|LRP(_SAT)?|MAD(_SAT)?|MAX(_SAT)?|"
			"MIN(_SAT)?|MOV(_SAT)?|MUL(_SAT)?|OPTION|OUTPUT|PARAM|"
			"POW(_SAT)?|RCP(_SAT)?|RSQ(_SAT)?|SIN(_SAT)|SCS(_SAT)?|"
			"SGE(_SAT)?|SLT(_SAT)?|SUB(_SAT)?|SWZ(_SAT)|"
			"TEMP|TEX(_SAT)?|TXB(_SAT)?|TXP(_SAT)?|XPD(_SAT)?|"
			"fragment|program|result|state|texture|vertex)\\b");
		rules.append(rule);
		
		rule.type = Highlighter::Misc;
		rule.pattern = QRegExp("^!!(ARBvp1\\.0|ARBfp1\\.0)");
		rules.append(rule);
		
		rule.type = Highlighter::Comment;
		rule.pattern = QRegExp("#.*$");
		rules.append(rule);

		return rules;

	}

	virtual QString multiLineCommentStart() const
	{
		return QString();
	}

	virtual QString multiLineCommentEnd() const
	{
		return QString();
	}
};

REGISTER_EFFECT_FACTORY(ARBEffectFactory);

