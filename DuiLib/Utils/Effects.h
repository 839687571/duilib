#ifndef __UIEFFECTS_H__
#define __UIEFFECTS_H__

#include <list>
namespace DuiLib
{
    enum
    {
        EFFECT_NULL,
        EFFECT_BLUR,
    };

    typedef struct BlurEffectParam
	{
		BlurEffectParam() : dSigma(1.0), nRadius(1){}
		double dSigma;
		int nRadius;
	}BlurEffectParam;

class Effects;

class UIEffect
{
public:
    UIEffect();
    ~UIEffect();

    virtual int GetEffectType(){return m_nType;}
protected:

    bool SetParameters(const void *params, const UINT size);

    bool GetParameters(UINT *size, void *params);

protected:
    void* m_pData;
    UINT m_uSize;
    int m_nType;
};

class BlurEffect : public UIEffect
{
public:
    BlurEffect();
    bool SetParameters(const BlurEffectParam *parameters);

    bool GetParameters(UINT *size, BlurEffectParam *parameters);
};


class Effects
{
public:
    Effects();
    ~Effects();

    bool AddEffect(UIEffect* pEffect);
    bool RemoveEffect(int nEffect);
    UIEffect* FindEffect(int nEffect);

private:
    std::list<UIEffect* > m_lEffects;
};

}

#endif  // __UIEFFECTS_H__
