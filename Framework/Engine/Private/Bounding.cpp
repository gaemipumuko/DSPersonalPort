#include "..\Public\Bounding.h"

CBounding::CBounding()
{
}



#ifdef _DEBUG

_vector CBounding::Compute_Color()
{
	return m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);
}

#endif

void CBounding::Free()
{
}
