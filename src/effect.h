#ifndef EFFECT_H
#define EFFECT_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtGui/QIcon>
#include "highlighter.h"

//#undef Q_ASSERT
//#define Q_ASSERT(b) do { if(!(b)) __asm__ volatile ("trap"); } while(false)

class QFile;
class QByteArray;
class MessagePanel;
class EffectFactory;
class Parameter;
class OutputParser;
class QGLWidget;

class Effect : public QObject
{
	Q_OBJECT
public:
	enum EditorType {
		EditorType_None,
		EditorType_Scalar,
		EditorType_Color,
		EditorType_Vector,
		EditorType_Matrix,
		EditorType_File
	};
	
	Effect(const EffectFactory * factory) : m_factory(factory)
	{
	}
	
	const EffectFactory * factory() const
	{
		return m_factory;
	}
	
	// Load/Save the effect.
	virtual void load(QFile * file) = 0;
	virtual void save(QFile * file) = 0;
	
	virtual int getInputNum() = 0;
	virtual QString getInputName(int i) = 0;
	virtual const QByteArray & getInput(int i) const = 0;
	virtual void setInput(int i, const QByteArray & str) = 0;
	
	// Compilation.
	virtual void build(bool threaded) = 0;
	virtual bool isBuilding() const = 0; 
	
	// Parameter info.
	virtual int parameterCount() const = 0;
	virtual const Parameter * parameterAt(int idx) const = 0;
	virtual Parameter * parameterAt(int idx) = 0;

	// Effect info.
	virtual bool isValid() const = 0;
	virtual bool isAnimated() const = 0;
	
	// Technique info.
	virtual int getTechniqueNum() const = 0;
	virtual QString getTechniqueName(int t) const = 0;
	virtual void selectTechnique(int t) = 0;
	
	// Pass info.
	virtual int getPassNum() const = 0;
	
	// Rendering.
	virtual void begin() = 0;
	virtual void beginPass(int p) = 0;
	virtual void beginMaterialGroup() { }
	virtual void endPass() = 0;
	virtual void end() = 0;
	
signals:
	void infoMessage(QString msg);
	void errorMessage(QString msg);
	void buildMessage(QString msg, int input, OutputParser * parser);
	
	void built(bool succeed);
	
private:
	EffectFactory const * const m_factory;

};

class EffectFactory : public QObject
{
public:
	virtual bool isSupported() const = 0;
	virtual QString name() const = 0;
	virtual QString namePlural() const = 0;
	virtual QString extension() const = 0;
	virtual QIcon icon() const = 0;
	virtual Effect * createEffect(QGLWidget * widget) const = 0;
	virtual bool savesParameters() const = 0;
	
	virtual QList<Highlighter::Rule> highlightingRules() const = 0;
	virtual QString multiLineCommentStart() const = 0;
	virtual QString multiLineCommentEnd() const = 0;

	static const EffectFactory * factoryForExtension(const QString & ext);
	static const QList<const EffectFactory *> & factoryList();
	static void addFactory(const EffectFactory * factory);
	static void removeFactory(const EffectFactory * factory);
};


#define REGISTER_EFFECT_FACTORY(Factory) \
	namespace { \
		static Factory factory; \
		struct Factory##Registrar { \
			Factory##Registrar() { EffectFactory::addFactory(&factory); } \
			~Factory##Registrar() { EffectFactory::removeFactory(&factory); } \
		}; \
		static Factory##Registrar registrar; \
	}



#endif // EFFECT_H
