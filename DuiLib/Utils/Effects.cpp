#include "StdAfx.h"
#include "Effects.h"

namespace DuiLib
{
UIEffect::UIEffect()
: m_pData(NULL)
, m_uSize(0)
, m_nType(EFFECT_NULL)
{

}

UIEffect::~UIEffect()
{
    if (m_pData)
    {
        delete [] m_pData;
    }
}

bool UIEffect::SetParameters(const void *params, const UINT size)
{
    if (m_pData)
    {
        delete [] m_pData;
        m_pData = NULL;
    }

    try
    {
        m_pData = (void*)(new char [size]);
        memcpy_s(m_pData, size, params, size);
        m_uSize = size;
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool UIEffect::GetParameters(UINT *size, void *params)
{
    if (!m_pData)
    {
        return false;
    }

    if (m_uSize > *size)
    {
        return false;
    }

    *size = m_uSize;
    memcpy_s(params, *size, m_pData, m_uSize);
    return true;
}

BlurEffect::BlurEffect()
{
    m_nType = EFFECT_BLUR;
}

bool BlurEffect::SetParameters(const BlurEffectParam *parameters)
{
    UINT size = sizeof(BlurEffectParam);
    return UIEffect::SetParameters(parameters, size);
}

bool BlurEffect::GetParameters(UINT *size, BlurEffectParam *parameters)
{
    return UIEffect::GetParameters(size, (VOID*)parameters);
}

Effects::Effects()
{

}

Effects::~Effects()
{
    for (std::list<UIEffect*>::iterator it = m_lEffects.begin(); it != m_lEffects.end(); ++it)
    {
        delete *it;
    }

    m_lEffects.clear();
}

bool Effects::AddEffect(UIEffect* pEffect)
{
	RemoveEffect(pEffect->GetEffectType());

	m_lEffects.push_back(pEffect);

    return true;
}

UIEffect* Effects::FindEffect(int nEffect)
{
    for (std::list<UIEffect*>::iterator it = m_lEffects.begin(); it != m_lEffects.end(); ++it)
    {
        if ((*it)->GetEffectType() == nEffect)
        {
            return *it;
        }
    }

    return NULL;
}

bool Effects::RemoveEffect(int nEffect)
{
    for (std::list<UIEffect*>::iterator it = m_lEffects.begin(); it != m_lEffects.end(); ++it)
    {
        if ((*it)->GetEffectType() == nEffect)
        {
            delete *it;
            m_lEffects.erase(it);
            return true;
        }
    }

    return false;
}


}
