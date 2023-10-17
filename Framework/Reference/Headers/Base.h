#pragma once

#include "Engine_Defines.h"

/* ��� Ŭ�������� �θ�Ŭ������ �ȴ�. */
/* ���۷���ī��Ʈ�� ����(AddRef, Release)�Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* ���۷��� ī��Ʈ�� ������Ų��. */
	unsigned long AddRef();

	/* ���۷��� ī��Ʈ�� ���ҽ�Ų��. or �����Ѵ�.  */
	unsigned long Release();

protected:
	unsigned long			m_dwRefCnt = { 0 };

public:
	virtual void Free() = 0;

};


END
