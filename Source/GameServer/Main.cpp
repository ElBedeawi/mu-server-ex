#include "StdAfx.H"
#include "main.H"
#include "ClosePlayer.H"
#include "Functions.H"
#include "resource.H"
#include "MapServerManager.H"
#include "AcceptIP.H"
#include "GameMain.H"
#include "CashShop.H"
#include "Notice.H"
#include "DBSockMng.H"
#include "Defines.H"
#include "ChaosCastle.H"
#include "BloodCastle.H"
#include "GoldenEvent.H"
#include "IllusionTemple.H"
#include "CastleSiegeSync.H"
#include "EventManagement.H"
#include "Kanturu.H"
#include "User.H"
#include "TMonsterAI.H"
#include "CWhatsUpDummyServer.H"
#include "EDSProtocol.H"
#include "JSProtocol.H"
#include "DuelSystem.h"
#include "ImperialFort.h"
#include "DoubleGoer.h"
#include "Raklion.h"
#include "RedDragon.h"
#include "CastleSiege.h"
#include "CastleDeepEvent.h"
#include "Weapon.h"
#include "Crywolf.h"
#include "MoveSystem.H"
#include "LogProc.H"
#include "Configs.H"
#include "Defines.H"
#include "MiniDumper.H"

DWORD StartTime;

MainClass gMainApp;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
APP_MAIN WinMain(APP_ARGS) 
{
	StartTime	= GetTickCount();

	LoadString(LOAD_APP_WIN);
	// -----
	gMainApp.MyRegisterClass(hInstance);
	// -----
	CUnknown.Unknown872();
	// -----
	if (!gMainApp.Initialize(APP_ARGS_PUSH)) return FALSE;
	// -----
	ShowWindow(gMainApp.m_hWnd, SW_SHOWMAXIMIZED);
	// -----
	if ( gNet.Initialize() )
	{
		CLog.Info("[NETWORK] Created & Initialized Successfully.");
	}
	// -----
	RFile.RFSetInfo( gLanguage );
	// -----
	MCfg.Initialize();
	// -----
	RFile.GetString((LPSTR)IPAdress[0],  IP_ADR_HDR,	"JoinServerIP",		RFile.szRFileName[0]);
	RFile.GetString((LPSTR)IPAdress[1],  IP_ADR_HDR,	"DataServer1IP",	RFile.szRFileName[0]);
	RFile.GetString((LPSTR)IPAdress[2],  IP_ADR_HDR,	"DataServer2IP",	RFile.szRFileName[0]);
	RFile.GetString((LPSTR)IPAdress[3],  IP_ADR_HDR,	"ExDBIP",			RFile.szRFileName[0]);
	// -----
	Port[0]		= RFile.GetInt(PORT_HDR,	"GameServerPort",		RFile.szRFileName[0]);
	Port[1]		= RFile.GetInt(PORT_HDR,	"JoinServerPort",		RFile.szRFileName[0]);
	Port[2]		= RFile.GetInt(PORT_HDR,	"DataServer1Port",		RFile.szRFileName[0]);
	Port[3]		= RFile.GetInt(PORT_HDR,	"DataServer2Port",		RFile.szRFileName[0]);
	Port[4]		= RFile.GetInt(PORT_HDR,	"ExDBPort",				RFile.szRFileName[0]);
	Port[5]		= RFile.GetInt(PORT_HDR,	"RankingServerPort",	RFile.szRFileName[0]);
	Port[6]		= RFile.GetInt(PORT_HDR,	"EventChipServerPort",	RFile.szRFileName[0]);
	// -----
	gLanguage	= RFile.GetInt(GS_INFO_HDR, "Protocol",			RFile.szRFileName[0]);
	// -----
	GameMainInit(gMainApp.m_hWnd);
	// -----
	gWhatsUpDummyServer.Start(gMainApp.m_hWnd, Port[0]+1);
	// -----
	gMainApp.StartNetwork();
	// -----
	//if ( Unknown592 == false ) ExitProcess(0);
	// -----
	CLog.Debug("GameServer v%s Successfully Initialized in %d seconds.", SOFT_VER,((GetTickCount() - StartTime)/1000));
	// -----
	while(GetMessage(&gMainApp.Msg, NULL, 0, 0) != NULL)
	{
		if(!TranslateAccelerator(gMainApp.Msg.hwnd, 0, &gMainApp.Msg))
		{
		    TranslateMessage(&gMainApp.Msg);
		    DispatchMessage(&gMainApp.Msg);
		}
	}
	// -----
	return (int) gMainApp.Msg.wParam;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
APP_LPRESULTCB WndProc(WNDPROC_ARGS)
{
	// -----
	switch(uMsg) 
	{
		case WM_GM_SERVER_MSG_PROC:
		{
			GMServerMsgProc(wParam, lParam);
		} break;
		// ------
		case WM_GM_CLIENT_MSG_PROC:
		{
			GMClientMsgProc(wParam, lParam);
		}
		break;
		// ------
		case WM_GM_JOIN_CLIENT_MSG_PROC:
		{
			GMJoinClientMsgProc(wParam, lParam);
		} break;
		// ------
		case WM_GM_RANKING_CLIENT_MSG_PROC:
		{
			GMRankingClientMsgProc(wParam, lParam);
		} break;
		// ------
		case WM_GM_EVENTCHIP_CLIENT_MSG_PROC:
		{
			GMEventChipClientMsgProc(wParam, lParam);
		} break;
		// ------
		case WM_GM_DATA1_CLIENT_MSG_PROC:
		{
			cDBSMng.MsgProc(0, wParam, lParam);
		} break;
		// ------
		case WM_GM_DATA2_CLIENT_MSG_PROC:
		{
			cDBSMng.MsgProc(1, wParam, lParam);
		} break;
		// ------
		case WM_GM_EXDATA_CLIENT_MSG_PROC:
		{
			ExDataClientMsgProc(wParam, lParam);
		} break;
		// ------
		case WM_START_SERVER:
		{
			if ( gMainApp.bServerReady == 2 ) // If JoinServer & DataServer Connected!
			{
				gMainApp.GameServerStart();
			} 
		} break;
		// ------
		case WM_TIMER:
			switch ( wParam )
			{
				case WM_GUILD_LIST_REQUEST:
				{
					DGGuildListRequest();
					KillTimer(gMainApp.m_hWnd, WM_GUILD_LIST_REQUEST);
				} break;
				// ------
				case WM_LOG_PAINT:
				{
					RECT rect;
					HDC hdc = GetDC(hWnd);
					GetClientRect(hWnd, &rect);
					COLORREF Color	= TColor.Black();
					FillRect(hdc, & rect, CreateSolidBrush(Color));
					ReleaseDC(hWnd, hdc);
					// ------
					if ( gMainApp.bCurPaintType == 0)
					{
							gMainApp.PaintWindow(hWnd);
					}
					// ------
					gObjViewportPaint(hWnd, gMainApp.bCurPaintPlayer);
				} break;
				// ------
				case WM_MONSTER_AND_MSG_PROC:
					MonsterAndMsgProc();
					break;
				// ------
				case WM_MOVE_MONSTER_PROC:
					MoveMonsterProc();
				#if(!GS_CASTLE)
					g_Kanturu.UserMonsterCountCheck();
				#endif
					break;
				// ------
				case WM_EVENT_RUN_PROC:
				#if(!GS_CASTLE)
					g_ChaosCastle.Run();
					g_BloodCastle.Run();
					g_Kanturu.Run();
					g_IllusionTemple.Run();
					gImperialFort.Run();
					gRaklion.Run();
					gDoubleGoer.Run();
					gGoldenEvent.Run();
					gRedDragon.Run();
					gDuel.Run();
				#else
					g_CastleSiege.Run();
					g_CastleDeepEvent.Run();
					g_CsNPC_Weapon.WeaponAttackProc();
					g_Crywolf.Run();
				#endif
					gEventManager.Run();
					break;
				// ------
				case WM_AI_MONSTER_MOVE_PROC:
					TMonsterAI::MonsterMoveProc();
					break;
				// ------
				case WM_AI_MONSTER_PROC:
					TMonsterAI::MonsterAIProc();
					break;
				// ------
				case WM_FIRST_MSG_PROCESS:
					for ( int n=0;n<OBJMAX;n++)
					{
						gObjStateSetCreate(n);
					}
					for ( int n=0;n<OBJMAX;n++)
					{
						gObjViewportListDestroy(n);
					}
					for ( int n=0;n<OBJMAX;n++)
					{
						gObjViewportListCreate(n);
					}
					for ( int n=0;n<OBJMAX;n++)
					{
						gObjViewportListProtocol(n);
					}

					if ( gDoPShopOpen != FALSE )
					{
						for ( int n= OBJ_STARTUSERINDEX;n<OBJMAX;n++)
						{
							PShop_ViewportListRegenarate(n);
						}
						for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
						{
							if (gObjIsConnected(n) != PLAYER_EMPTY )
							{
								gObj[n].m_bPShopItemChange = false;
							}
						}
					}
					for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
					{
						gObjUnionUpdateProc(n);
					}
					gObjSetState();
					GameServerInfoSend();
					break;
					// ------
				case WM_SECOND_MSG_PROCESS:
					for (int n = 0;n<MAX_NUMBER_MAP;n++)
					{
						MapC[n].WeatherVariationProcess();
					}
					wsGServer.m_SendSec = wsGServer.m_SendSecTmp;
					wsGServer.m_RecvSec = wsGServer.m_RecvSecTmp;
					wsGServer.m_SendSecTmp = 0;
					wsGServer.m_RecvSecTmp = 0;
					break;
				// ------
				case WM_GS_CLOSE:
					if ( gMainApp.bCloseMsg != 0 )
					{
						gMainApp.bCloseMsgTime--;

						if ( gMainApp.bCloseMsgTime <= 1 )
						{
							if ( gMainApp.bCloseMsgTime == 1 )
							{
								SendMsg.MessageOutAll(0x00, "GameServer Closed." );
								CLog.LogAddC(TColor.Red(), "GameServer Closed.");
							}
						}
						else
						{
							if ( (gMainApp.bCloseMsgTime % 10) == 0 )
							{
								SendMsg.MessageOutAll(0x00, "Server will be ShutDown Automatically in %d Second(s).", gMainApp.bCloseMsgTime);
								CLog.LogAddC(TColor.Red(), "Server will be ShutDown Automatically in %d Second(s).", gMainApp.bCloseMsgTime);
							}
						}

						if ( gMainApp.bCloseMsgTime < 0 )
						{
							KillTimer(gMainApp.m_hWnd, WM_GS_CLOSE);
							LogOutAllUsers();
							gMainApp.bCloseMsgTime	= 0;
							gMainApp.bCloseMsg		= 0;
						}
					}
					gObjSecondProc();

					#if(GS_CASTLE)
						if( cDBSMng.m_ActiveServer > DS_UNINITIALIZED && g_CastleSiege.GetDataLoadState() == 2 )
						{
							g_CastleSiege.DataRequest();
						}
					
						g_CastleSiege.SendCastleStateSync();
						g_CastleSiegeSync.AdjustTributeMoney();
						g_Crywolf.CrywolfSecondAct();
					#else
						g_CastleSiegeSync.AdjustTributeMoney();
					#endif
					break;
				// ------
				case WM_SET_DATE:
					gSetDate();
					WhisperCash.TimeCheckCashDelete();
					break;
				// ------
				case WM_CONNECT_DATASERVER:
					cDBSMng.Connect();
					gObjCheckAllUserDuelStop();
					break;
				// ------
			}
			break;
		// ------
		case WM_CLOSE:
		{
			gMainApp.ExitWindow(hWnd);
		} break;
		// ------
		case WM_KEYDOWN:
			switch ( wParam )
			{
				case VK_F1: // RELOAD CONFIGURATION [ F1 ]
					break;
				case VK_F2: // INCREASE USER LIMIT -10 [ F2 ]
					gServerMaxUser -= 10;
					if ( gServerMaxUser < 10 )
						gServerMaxUser = 10;
					break;
				case VK_F3: // INCREASE USER LIMIT + 10 [ F3 ]
					gServerMaxUser += 10;
					if ( gServerMaxUser > 1500 )
						gServerMaxUser = 1500;
					break;
				// ------
			}
			break;
		// ------
		case WM_DESTROY:
			GameMainFree();
			gNet.CloseNetwork();
			PostQuitMessage(0);
			break;
		default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
   }
   return FALSE;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool MainClass :: Initialize(HINSTANCE hInstance, int nCmdShow)
{
	this->hInst = hInstance;

	wsprintf(this->szTitle, "GameServer Emulator v%s", SOFT_VER);

	this->m_hWnd = CreateWindow(szWindowClass, TEXT (szTitle), WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!this->m_hWnd) 
		return false;
	ShowWindow(this->m_hWnd, nCmdShow);
	UpdateWindow(this->m_hWnd);
	return true;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void MainClass :: MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)CreateSolidBrush(BLACK_BRUSH);
	wcex.lpszClassName	= this->szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));

	RegisterClassEx(&wcex);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool MainClass :: ExitWindow(HWND hWnd)
{
	if ( MessageBox(gMainApp.m_hWnd, "Are You Sure Close GameServer?", "Terminate GameServer", MB_YESNO|MB_APPLMODAL) == IDYES )
	{
		DestroyWindow(hWnd);
	}
	return true;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool MainClass :: PaintWindow(HWND hWnd)
{
	CLog.Run();
	return true;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool MainClass :: GameServerStart() 
{
	if( GameMainServerCreate(WM_GM_SERVER_MSG_PROC, WM_GM_CLIENT_MSG_PROC) == FALSE )
	{
		CLog.MsgBox("GameServer","GameServer Run Failed!", MB_OK);
		ExitProcess(0);
	}
	gNet.CreateSrvWorker(Port[0]);

	SetTimer(m_hWnd, WM_LOG_PAINT, 2000, NULL);
	SetTimer(m_hWnd, WM_FIRST_MSG_PROCESS, 1000, NULL);
	SetTimer(m_hWnd, WM_SET_DATE, 60000, NULL);
	SetTimer(m_hWnd, WM_LOG_DATE_CHANGE, 60000, NULL);
	SetTimer(m_hWnd, WM_CONNECT_DATASERVER, 10000, NULL);
	SetTimer(m_hWnd, WM_SECOND_MSG_PROCESS, 1000, NULL);
	SetTimer(m_hWnd, WM_GS_CLOSE, 1000, NULL);
	SetTimer(m_hWnd, WM_MONSTER_AND_MSG_PROC, 500, NULL);
	SetTimer(m_hWnd, WM_MOVE_MONSTER_PROC, 300, NULL);
	SetTimer(m_hWnd, WM_EVENT_RUN_PROC, 100, NULL);
	SetTimer(m_hWnd, WM_AI_MONSTER_MOVE_PROC, 100, NULL);
	SetTimer(m_hWnd, WM_AI_MONSTER_PROC, 1011, NULL);
	SetTimer(m_hWnd, WM_GUILD_LIST_REQUEST, 1000, NULL);

	return true;
}
bool MainClass :: StartNetwork()
{	
	if( Connected[0] == FALSE )
	{
		if(GMJoinServerConnect((LPSTR)IPAdress[0], WM_GM_JOIN_CLIENT_MSG_PROC) == NULL)
		{	
			CLog.MsgBox("GameServer", "GameServer Couldn't Connect to DataServer!", MB_OK);
			ExitProcess(0);
		}
	}
	if( Connected[1] == FALSE )
	{
		if(GMDataServerConnect((LPSTR)IPAdress[1], WM_GM_DATA1_CLIENT_MSG_PROC) == NULL)
		{
			if(GMDataServerConnect((LPSTR)IPAdress[2], WM_GM_DATA2_CLIENT_MSG_PROC) == NULL)
			{
				CLog.MsgBox("GameServer", "GameServer Couldn't Connect to DataServer!", MB_OK);
				ExitProcess(0);
			}
		}
	}
	if( (Connected[2] != FALSE) && (Connected[3] == FALSE) )
	{
		if(GMRankingServerConnect(gDevilSquareEventServerIp, WM_GM_RANKING_CLIENT_MSG_PROC) == NULL)
		{
			CLog.MsgBox("GameServer", "GameServer Couldn't Connect to DataServerEx!", MB_OK);
			return false;
		}
		Connected[3] = true;
	}
	if( (Connected[4] != FALSE) && (Connected[5] == FALSE) )
	{
		if (GMEventChipServerConnect(gEventChipServerIp, WM_GM_EVENTCHIP_CLIENT_MSG_PROC) == NULL)
		{
			CLog.MsgBox("GameServer", "GameServer Couldn't Connect to DataServerEx!", MB_OK);
			return false;
		}
		Connected[5] = true;
	}
	cDBSMng.Connect();
	return true;
}