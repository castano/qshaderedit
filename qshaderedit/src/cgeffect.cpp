
#include "effect.h"


class CgEffectFactory : public EffectFactory
{
	virtual bool isSupported() const
	{
		return false;
	}
	
	virtual QString name() const
	{
		return tr("Cg Shader");
	}
	
	virtual QString namePlural() const
	{
		return tr("Cg Shaders");
	}
	
	virtual QString extension() const
	{
		return "cg";
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

REGISTER_EFFECT_FACTORY(CgEffectFactory);
