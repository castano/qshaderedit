
#include "effect.h"
#include "messagepanel.h"
#include "outputparser.h"
#include "parameter.h"

#include <math.h>

#include <QtCore/QCoreApplication>
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
		"SUB viewVec, cp, iVertex;\n\n"
		
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
	
	enum Stage {
		Stage_Vertex,
		Stage_Fragment,
	};
	enum Namespace {
		Namespace_Local,
		Namespace_Environment,
	};	
	
	// ARB Parameter.
	class ArbParameter : public Parameter
	{
	private:
		Stage m_stage;
		Namespace m_nameSpace;
		GLint m_location;

	public:
		ArbParameter(): m_location(-1)
		{
		}
		
		ArbParameter(const QString & name): m_location(-1)
		{
			setName(name);
			
			if( name.contains("color", Qt::CaseInsensitive) ) {
				setWidget(Widget_Color);
			}
		}
		
		virtual int rows() const { return 4; }
		virtual int columns() const { return 1; }
		
		Stage stage() const { return m_stage; }
		void setStage(Stage stage) { m_stage = stage; }
		
		Namespace nameSpace() const { return m_nameSpace; }
		void setNameSpace(Namespace nspace) { m_nameSpace = nspace; }
		
		GLint location() const { return m_location; }
		void setLocation(GLint location) { m_location = location; }
	};
	
} // namespace


class ArbEffect : public Effect
{
private:
	
	QByteArray m_vertexProgramText;
	QByteArray m_fragmentProgramText;

	QTime m_time;
	
	QVector<ArbParameter *> m_parameterArray;

	GLuint m_vp;
	GLuint m_fp;
	
	OutputParser* m_outputParser;

public:
	
	// Ctor.
	ArbEffect(const EffectFactory * factory) : Effect(factory),
		m_vertexProgramText(s_vertexProgramText),
		m_fragmentProgramText(s_fragmentProgramText),
		m_outputParser(NULL)
	{
		m_time.start();

		const char* vendor = (const char*)glGetString(GL_VENDOR);
		if (strcmp(vendor, "ATI Technologies Inc.") == 0)
			m_outputParser = new AtiAsmOutputParser;
		else if (strcmp(vendor, "NVIDIA Corporation") == 0)
			m_outputParser = new NvidiaAsmOutputParser;
	}
	
	virtual ~ArbEffect()
	{
		deletePrograms();
		delete m_outputParser;
		qDeleteAll(m_parameterArray);
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
		resetParameters();
		
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
			parseProgram(m_vertexProgramText, Stage_Vertex);
		}
		glDisable( GL_VERTEX_PROGRAM_ARB );
		
		QCoreApplication::processEvents();
		
		if(output != NULL) output->info("Compiling fragment program...");
		glGenProgramsARB( 1, &m_fp );
		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, m_fp );
		glProgramStringARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, m_fragmentProgramText.length(), m_fragmentProgramText );
		if( checkProgramError(output, 1) ) {
			succeed = false;
		}
		else {
			parseProgram(m_fragmentProgramText, Stage_Fragment);
		}
		glDisable( GL_FRAGMENT_PROGRAM_ARB );
		
		return succeed;
	}	
	
	
	// Parameter info.
	virtual int parameterCount() const
	{
		return m_parameterArray.count();
	}

	virtual const Parameter * parameterAt(int idx) const
	{
		Q_ASSERT(idx >= 0 && idx < parameterCount());
		return m_parameterArray.at(idx);
	}
	virtual Parameter * parameterAt(int idx)
	{
		return m_parameterArray.at(idx);
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
		
		// Enable programs.
		glEnable( GL_VERTEX_PROGRAM_ARB );
		glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_vp );
		
		glEnable( GL_FRAGMENT_PROGRAM_ARB );
		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, m_fp );
		
		// Set uniforms.
		setParameters();
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
		GLint position;
		glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB, &position );
		if( position != -1 ) {
			const char * error = (const char *) glGetString( GL_PROGRAM_ERROR_STRING_ARB );
			if( output ) output->log(QString(error), inputNumber, m_outputParser);
			qDebug("%s", error);
			return true;
		}
		
		return false;
	}
	
	void resetParameters()
	{
// 		qSwap(m_oldParameterArray, m_parameterArray);
		
		qDeleteAll(m_parameterArray);
		m_parameterArray.clear();
	}
	
	void parseProgram(const QByteArray & code, Stage stage)
	{
		// Get parameters. @@ Create these regular expressions only once.
		static QRegExp paramRegExp("\\s*PARAM\\s+(\\w+)(?:\\[(\\d+)\\])?\\s*=\\s*(\\S.*)");
		static QRegExp commentRegExp("#.*$");
		
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
						addVectorParameter(name, value, stage);
					}
					else {
						addMatrixParameter(name, size.toInt(), value, stage);
					}
				}
			}
			
			start = end+1;
			end = qMin(code.indexOf(';', end+1), code.indexOf('\n', end+1));
		}
	}
	
	void addVectorParameter(const QString & name, const QString & value, Stage stage)
	{
		ArbParameter * param = new ArbParameter(name);
		param->setStage(stage);

		static QRegExp localRegExp("program\\.local\\[(\\d+)\\]");
		static QRegExp envRegExp("program\\.env\\[(\\d+)\\]");
		
		//qDebug() << "name:" << name << "value:" << value;
		
		bool match = false;
		if( localRegExp.exactMatch(value) ) {
			param->setNameSpace(Namespace_Local);
			param->setLocation(localRegExp.cap(1).toInt());
			match = true;
		}
		else if( envRegExp.exactMatch(value) ) {
			param->setNameSpace(Namespace_Environment);
			param->setLocation(envRegExp.cap(1).toInt());
			match = true;
		}
		
		if( match ) {
			param->setValue(QVariantList() << 0.0f << 0.0f << 0.0f << 0.0f);
			m_parameterArray.append(param);
		}
	}
	
	void addMatrixParameter(const QString & name, int size, const QString & value, Stage stage)
	{
		// @@ Add support for parameters in the format: "program.local[0..1]"
		//qDebug() << "value:" << value;
		static QRegExp rx("[\\{\\s,\\}]+");
		QStringList values = value.split(rx, QString::SkipEmptyParts);
		if(values.count() == size) {
			for(int i = 0; i < size; i++) {
				addVectorParameter(name + "[" + QString::number(i) + "]", values.at(i), stage);
			}
		}
	}
	
	void setParameters()
	{
		foreach(const ArbParameter * p, m_parameterArray) {
			GLenum stage = p->stage() == Stage_Vertex ? GL_VERTEX_PROGRAM_ARB : GL_FRAGMENT_PROGRAM_ARB;
			
			if (p->type() == QVariant::List) {
				QVariantList list = p->value().toList();
				Q_ASSERT(list.count() == 4);
				
				if( p->stage() == Stage_Vertex ) {
					if( p->nameSpace() == Namespace_Local ) {
						glProgramLocalParameter4dARB(GL_VERTEX_PROGRAM_ARB, p->location(), list.at(0).toDouble(), list.at(1).toDouble(), list.at(2).toDouble(), list.at(3).toDouble());
					}
					else if( p->nameSpace() == Namespace_Environment ) {
						glProgramEnvParameter4dARB(GL_VERTEX_PROGRAM_ARB, p->location(), list.at(0).toDouble(), list.at(1).toDouble(), list.at(2).toDouble(), list.at(3).toDouble());
					}
				}
				else if( p->stage() == Stage_Fragment ) {
					if( p->nameSpace() == Namespace_Local ) {
						glProgramLocalParameter4dARB(GL_FRAGMENT_PROGRAM_ARB, p->location(), list.at(0).toDouble(), list.at(1).toDouble(), list.at(2).toDouble(), list.at(3).toDouble());
					}
					else if( p->nameSpace() == Namespace_Environment ) {
						glProgramEnvParameter4dARB(GL_FRAGMENT_PROGRAM_ARB, p->location(), list.at(0).toDouble(), list.at(1).toDouble(), list.at(2).toDouble(), list.at(3).toDouble());
					}
				}
			}
		} 
	}
};


class ArbEffectFactory : public EffectFactory
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
 		return new ArbEffect(this);
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

REGISTER_EFFECT_FACTORY(ArbEffectFactory);

