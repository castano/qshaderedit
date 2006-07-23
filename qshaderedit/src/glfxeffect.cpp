
#include "effect.h"


class GLFXEffectFactory : public EffectFactory
{
	virtual bool isSupported() const
	{
		return false;
	}
	
	virtual QString name() const
	{
		return tr("GLFX Effect");
	}
	
	virtual QString namePlural() const
	{
		return tr("GLFX Effects");
	}
	
	virtual QString extension() const
	{
		return "glfx";
	}
	
	virtual QIcon icon() const
	{
		return QIcon();
	}
	
	virtual Effect * createEffect() const
	{
		Q_ASSERT(isSupported());
		return NULL;
	}

	virtual QList<Highlighter::Rule> highlightingRules() const
	{
		return QList<Highlighter::Rule>();
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

REGISTER_EFFECT_FACTORY(GLFXEffectFactory);

