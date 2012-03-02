//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development � 2011
// # Description:	Guardian Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#ifndef GUARDIAN_H
#define GUARDIAN_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ---------------------------------------------------------------------------------------------------------------------------------------------------
class CGuardian
{
public:
	CGuardian();
	virtual ~CGuardian();

	BOOL CreateGuardian(int iIndex);
	void GuardianAct(int iIndex);
};
// ---------------------------------------------------------------------------------------------------------------------------------------------------
extern CGuardian g_CsNPC_Guardian;
// ---------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// ---------------------------------------------------------------------------------------------------------------------------------------------------