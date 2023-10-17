#pragma once

#include "Engine_Defines.h"

/* 모든 클래스들의 부모클래스가 된다. */
/* 레퍼런스카운트를 관리(AddRef, Release)한다. */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* 레퍼런스 카운트를 증가시킨다. */
	unsigned long AddRef();

	/* 레퍼런스 카운트를 감소시킨다. or 삭제한다.  */
	unsigned long Release();

protected:
	unsigned long			m_dwRefCnt = { 0 };

public:
	virtual void Free() = 0;

};


END
