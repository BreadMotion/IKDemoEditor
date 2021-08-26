#pragma warning( disable : 4996 )

#include "pch.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include "FrameWork/Input/InputDevice.h"

// ライブラリリンク
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


XINPUT xInput[PAD_NUM_MAX];
DINPUT dInput[PAD_NUM_MAX];

XINPUT xInputDummy;

#define DEADZONE 150

// #define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
// #define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
// #define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30


// 1つのデバイスごとに呼び出されるコールバック関数
BOOL PASCAL EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	//DEBUG("コールバック呼び出し\n");

	LPDIRECTINPUTDEVICE8   _lpJoystick = NULL;

	// ジョイスティックデバイスの作成
	HRESULT ret = PAD.lpDI->CreateDevice(lpddi->guidInstance, &_lpJoystick, NULL);
	if (FAILED(ret)) {
		//DEBUG("デバイス作成失敗\n");
		return DIENUM_STOP;
	}

	// 入力データ形式のセット
	ret = _lpJoystick->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(ret)) {
		//DEBUG("入力データ形式のセット失敗\n");
		_lpJoystick->Release();
		return DIENUM_STOP;
	}

	// 排他制御のセット
	ret = _lpJoystick->SetCooperativeLevel(PAD.hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(ret)) {
		//DEBUG("排他制御のセット失敗\n");
		_lpJoystick->Release();
		return DIENUM_STOP;
	}

	// 入力範囲のセット
	DIPROPRANGE	diprg;
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMax = 1000;
	diprg.lMin = -1000;

	// X軸
	diprg.diph.dwObj = DIJOFS_X;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// Y軸
	diprg.diph.dwObj = DIJOFS_Y;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// Z軸
	diprg.diph.dwObj = DIJOFS_Z;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RX軸
	diprg.diph.dwObj = DIJOFS_RX;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RY軸
	diprg.diph.dwObj = DIJOFS_RY;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RZ軸
	diprg.diph.dwObj = DIJOFS_RZ;
	_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// 起動準備完了
	_lpJoystick->Poll();

	// 構築完了なら
	char inputDeviceName[260];
	WideCharToMultiByte(CP_ACP, 0, lpddi->tszProductName, -1, inputDeviceName, sizeof(inputDeviceName), NULL, NULL);

	if (strcmp(inputDeviceName, "Controller (Xbox One For Windows)") != 0)
	{
		PAD.lpJoystick.emplace_back(_lpJoystick);
	}

	// 最初の1つのみで終わる
	return DIENUM_CONTINUE;			// 次のデバイスを列挙するにはDIENUM_CONTINUEを返す
}

void Joystick::InitInputDevice()
{
	lpJoystick.clear();

	// IDirectInput8の作成
	HRESULT ret = DirectInput8Create(PAD.instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&lpDI, NULL);
	if (FAILED(ret))
	{
		// 作成に失敗
		//assert(!ret);
	}

	// ジョイスティックの列挙
	ret = lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyDeviceProc, NULL, /*DIEDFL_FORCEFEEDBACK |*/ DIEDFL_ATTACHEDONLY);
	if (FAILED(ret))
	{
		lpDI->Release();
		//assert(!ret);
	}

	for (auto& it : lpJoystick)
	{
		if (!it)
		{
			// ジョイスティックが1つも見つからない
			//MessageBoxW(PAD.hwnd, L"ジョイスティックが1つも接続されていません", L"エラー", MB_OK | MB_ICONHAND);
			lpDI->Release();
			//assert(!ret && "ジョイスティックが1つも見つからない");
		}
		else
		{
			// デバイス情報
			dc.dwSize = sizeof(dc);
			it->GetCapabilities(&dc);

			// 動作開始
			it->Acquire();
		}
	}
}

bool GetXInputState(XINPUT *xinput, int _num, float elapsedTime)
{
	// ゲームパッドの状態を取得
	XINPUT_STATE state;
	DWORD connection = XInputGetState( _num, &state );

	if (connection != 0)
	{
		xinput = &xInputDummy;
		return false;
	}


	//BUTTONS STATE
	xinput->bAs = ( state.Gamepad.wButtons & XINPUT_A ) ? 1 : 0;
	xinput->bBs = ( state.Gamepad.wButtons & XINPUT_B ) ? 1 : 0;
	xinput->bXs = ( state.Gamepad.wButtons & XINPUT_X ) ? 1 : 0;
	xinput->bYs = ( state.Gamepad.wButtons & XINPUT_Y ) ? 1 : 0;

	xinput->bLBs = ( state.Gamepad.wButtons & XINPUT_LEFT_SHOULDER  ) ? 1 : 0;
	xinput->bRBs = ( state.Gamepad.wButtons & XINPUT_RIGHT_SHOULDER ) ? 1 : 0;

	xinput->bSTARTs = ( state.Gamepad.wButtons & XINPUT_START ) ? 1 : 0;
	xinput->bBACKs  = ( state.Gamepad.wButtons & XINPUT_BACK  ) ? 1 : 0;

	xinput->bL3s = ( state.Gamepad.wButtons & XINPUT_LEFT_THUMB  ) ? 1 : 0;
	xinput->bR3s = ( state.Gamepad.wButtons & XINPUT_RIGHT_THUMB ) ? 1 : 0;

	xinput->bUPs    = ( state.Gamepad.wButtons & XINPUT_DPAD_UP    ) ? 1 : 0;
	xinput->bDOWNs  = ( state.Gamepad.wButtons & XINPUT_DPAD_DOWN  ) ? 1 : 0;
	xinput->bLEFTs  = ( state.Gamepad.wButtons & XINPUT_DPAD_LEFT  ) ? 1 : 0;
	xinput->bRIGHTs = ( state.Gamepad.wButtons & XINPUT_DPAD_RIGHT ) ? 1 : 0;


	//BUTTONS TRG
	if ( xinput->bAs )
	{
		if (xinput->cnt[0] == 0) xinput->bAt = 1;
		else xinput->bAt = 0;
		xinput->cnt[0]++;
	}
	else
	{
		xinput->bAt = 0;
		xinput->cnt[0] = 0;
	}

	if ( xinput->bBs )
	{
		if ( xinput->cnt[1] == 0 ) xinput->bBt = 1;
		else xinput->bBt = 0;
		xinput->cnt[1]++;
	}
	else
	{
		xinput->bBt = 0;
		xinput->cnt[1] = 0;
	}

	if ( xinput->bXs )
	{
		if ( xinput->cnt[2] == 0 ) xinput->bXt = 1;
		else xinput->bXt = 0;
		xinput->cnt[2]++;
	}
	else
	{
		xinput->bXt = 0;
		xinput->cnt[2] = 0;
	}

	if ( xinput->bYs )
	{
		if ( xinput->cnt[3] == 0 ) xinput->bYt = 1;
		else xinput->bYt = 0;
		xinput->cnt[3]++;
	}
	else
	{
		xinput->bYt = 0;
		xinput->cnt[3] = 0;
	}

	if ( xinput->bLBs )
	{
		if ( xinput->cnt[4] == 0 ) xinput->bLBt = 1;
		else xinput->bLBt = 0;
		xinput->cnt[4]++;
	}
	else
	{
		xinput->bLBt = 0;
		xinput->cnt[4] = 0;
	}

	if ( xinput->bRBs )
	{
		if ( xinput->cnt[5] == 0 ) xinput->bRBt = 1;
		else xinput->bRBt = 0;
		xinput->cnt[5]++;
	}
	else
	{
		xinput->bRBt = 0;
		xinput->cnt[5] = 0;
	}

	if ( xinput->bSTARTs )
	{
		if ( xinput->cnt[6] == 0 ) xinput->bSTARTt = 1;
		else xinput->bSTARTt = 0;
		xinput->cnt[6]++;
	}
	else
	{
		xinput->bSTARTt = 0;
		xinput->cnt[6] = 0;
	}

	if ( xinput->bBACKs )
	{
		if ( xinput->cnt[7] == 0 ) xinput->bBACKt = 1;
		else xinput->bBACKt = 0;
		xinput->cnt[7]++;
	}
	else
	{
		xinput->bBACKt = 0;
		xinput->cnt[7] = 0;
	}

	if ( xinput->bL3s )
	{
		if ( xinput->cnt[8] == 0 ) xinput->bL3t = 1;
		else xinput->bL3t = 0;
		xinput->cnt[8]++;
	}
	else
	{
		xinput->bL3t = 0;
		xinput->cnt[8] = 0;
	}

	if ( xinput->bR3s )
	{
		if ( xinput->cnt[9] == 0 ) xinput->bR3t = 1;
		else xinput->bR3t = 0;
		xinput->cnt[9]++;
	}
	else
	{
		xinput->bR3t = 0;
		xinput->cnt[9] = 0;
	}

	if ( xinput->bUPs )
	{
		if ( xinput->cnt[10] == 0 )xinput->bUPt = 1;
		else xinput->bUPt = 0;
		xinput->cnt[10]++;
	}
	else
	{
		xinput->bUPs = 0;
		xinput->cnt[10] = 0;
	}

	if ( xinput->bDOWNs )
	{
		if ( xinput->cnt[11] == 0 )xinput->bDOWNt = 1;
		else xinput->bDOWNt = 0;
		xinput->cnt[11]++;
	}
	else
	{
		xinput->bDOWNt = 0;
		xinput->cnt[11] = 0;
	}

	if ( xinput->bLEFTs)
	{
		if ( xinput->cnt[13] == 0 )xinput->bLEFTt = 1;
		else xinput->bLEFTt = 0;
		xinput->cnt[13]++;
	}
	else
	{
		xinput->bLEFTt = 0;
		xinput->cnt[13] = 0;
	}

	if ( xinput->bRIGHTs)
	{
		if ( xinput->cnt[12] == 0 )xinput->bRIGHTt = 1;
		else xinput->bRIGHTt = 0;
		xinput->cnt[12]++;
	}
	else
	{
		xinput->bRIGHTt = 0;
		xinput->cnt[12] = 0;
	}


	// トリガー
	xinput->bRTs = state.Gamepad.bRightTrigger;
	xinput->bLTs = state.Gamepad.bLeftTrigger;

	if ( xinput->bRTs )
	{
		if ( xinput->cnt[15] == 0 ) xinput->bRTt = 1;
		else xinput->bRTt = 0;
		xinput->cnt[15]++;
	}
	else
	{
		xinput->bRTt = 0;
		xinput->cnt[15] = 0;
	}
	if ( xinput->bLTs )
	{
		if ( xinput->cnt[14] == 0 ) xinput->bLTt = 1;
		else xinput->bLTt = 0;
		xinput->cnt[14]++;
	}
	else
	{
		xinput->bLTt = 0;
		xinput->cnt[14] = 0;
	}

	// スティック
	//XINPUT_STATE state = g_Controllers[i].state;
	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;
	float LX2 = LX * LX;
	float LY2 = LY * LY;
	float LXY = LX2 + LY2;
	//determine how far the controller is pushed
	float magnitude = sqrtf(LXY);
	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;
	float normalizedMagnitude = 0.0f;
	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{ 
		//clip the magnitude at its expected maximum value 
		if (magnitude > 32767.0f)
			magnitude = 32767.0f;
		//adjust magnitude relative to the end of the dead zone 
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		normalizedLX = LX / 32767.0f * 1000.0f;
		normalizedLY = LY / 32767.0f * 1000.0f;
		if (-DEADZONE <= normalizedLX && normalizedLX <= DEADZONE)
		{
			normalizedLX = 0.0f;
		}
		if (-DEADZONE <= normalizedLY && normalizedLY <= DEADZONE)
		{
			normalizedLY = 0.0f;
		}
		//optionally normalize the magnitude with respect to its expected range  
		//giving a magnitude value of 0.0 to 1.0 
		normalizedMagnitude = magnitude / (32767.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{	
		magnitude = 0.0f;
		normalizedLX = 0.0f;
		normalizedLY = 0.0f;
		normalizedMagnitude = 0.0f;
	}//repeat for right thumb stick

	xinput->sX = static_cast<int>(normalizedLX);
	xinput->sY = static_cast<int>(-normalizedLY);


	//XINPUT_STATE state = g_Controllers[i].state;
	float RX = state.Gamepad.sThumbRX;
	float RY = state.Gamepad.sThumbRY;
	float RX2 = RX * RX;
	float RY2 = RY * RY;
	float RXY = RX2 + RY2;
	//determine how far the controller is pushed
	magnitude = sqrtf(RXY);
	//determine the direction the controller is pushed
	float normalizedRX = RX / magnitude;
	float normalizedRY = RY / magnitude;
	normalizedMagnitude = 0.0f;
	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		//clip the magnitude at its expected maximum value 
		if (magnitude > 32767.0f)
			magnitude = 32767.0f;
		//adjust magnitude relative to the end of the dead zone 
		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		normalizedRX = RX / 32767.0f * 1000.0f;
		normalizedRY = RY / 32767.0f * 1000.0f;
		if (-DEADZONE <= normalizedRX && normalizedRX <= DEADZONE)
		{
			normalizedRX = 0.0f;
		}
		if (-DEADZONE <= normalizedRY && normalizedRY <= DEADZONE)
		{
			normalizedRY = 0.0f;
		}
		//optionally normalize the magnitude with respect to its expected range  
		//giving a magnitude value of 0.0 to 1.0 
		normalizedMagnitude = magnitude / (32767.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0f;
		normalizedRX = 0.0f;
		normalizedRY = 0.0f;
		normalizedMagnitude = 0.0f;
	}//repeat for right thumb stick

	xinput->sRX = static_cast<int>(normalizedRX);
	xinput->sRY = static_cast<int>(-normalizedRY);

	
	// バイブレーション更新
	if (xinput->isVibration)
	{
		if (xinput->vibMaxCnt <= xinput->vibCnt)
		{
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
			vibration.wLeftMotorSpeed = 0;  // val:0~65535
			vibration.wRightMotorSpeed = 0; // val:0~65535
			XInputSetState(_num, &vibration);

			xinput->vibCnt = 0.0f;
			xinput->vibMaxCnt = 0.0f;
			xinput->isVibration = false;
		}
		else
		{
			xinput->vibCnt += 1.0f * elapsedTime;
		}
	}

	return true;
}

bool GetDInputState(DINPUT *dinput, int _num, float elapsedTime)
{
	if ((int)(PAD.lpJoystick.size() - 1) != _num) return false;


	// データ取得前にPollが必要なら
	if (PAD.dc.dwFlags&DIDC_POLLEDDATAFORMAT) {
		PAD.lpJoystick[_num]->Poll();
	}


	// ジョイスティックの入力
	DIJOYSTATE joy;
	ZeroMemory(&joy, sizeof(joy));
	HRESULT ret = PAD.lpJoystick[_num]->GetDeviceState(sizeof(joy), &joy);
	if (FAILED(ret)) {
		// 失敗なら再び動作開始を行う
		PAD.lpJoystick[_num]->Acquire();
	}


	//STICK
	if (-DEADZONE <= joy.lX  && joy.lX  <= DEADZONE) joy.lX  = 0;
	if (-DEADZONE <= joy.lY  && joy.lY  <= DEADZONE) joy.lY  = 0;
	if (-DEADZONE <= joy.lZ  && joy.lZ  <= DEADZONE) joy.lZ  = 0;
	if (-DEADZONE <= joy.lRz && joy.lRz <= DEADZONE) joy.lRz = 0;

	dinput->sX = joy.lX;
	dinput->sY = joy.lY;
	dinput->sRX = joy.lZ;
	dinput->sRY = joy.lRz;


	//BUTTONS STATE
	dinput->bRECTs     = joy.rgbButtons[0] & 0x80 ? 1 : 0;
	dinput->bCROSSs    = joy.rgbButtons[1] & 0x80 ? 1 : 0;
	dinput->bCIRCLEs   = joy.rgbButtons[2] & 0x80 ? 1 : 0;
	dinput->bTRIANGLEs = joy.rgbButtons[3] & 0x80 ? 1 : 0;

	dinput->bL1s = joy.rgbButtons[4] & 0x80 ? 1 : 0;
	dinput->bR1s = joy.rgbButtons[5] & 0x80 ? 1 : 0;

	dinput->bSHAREs   = joy.rgbButtons[8] & 0x80 ? 1 : 0;
	dinput->bOPTIONSs = joy.rgbButtons[9] & 0x80 ? 1 : 0;

	dinput->bL3s = joy.rgbButtons[10] & 0x80 ? 1 : 0;
	dinput->bR3s = joy.rgbButtons[11] & 0x80 ? 1 : 0;

	if (joy.rgdwPOV[0] == 0) dinput->bUPs = 1;
	else dinput->bUPs = 0;
	if (joy.rgdwPOV[0] == 18000) dinput->bDOWNs = 1;
	else dinput->bDOWNs = 0;
	if (joy.rgdwPOV[0] == 9000) dinput->bRIGHTs = 1;
	else dinput->bRIGHTs = 0;
	if (joy.rgdwPOV[0] == 27000) dinput->bLEFTs = 1;
	else dinput->bLEFTs = 0;

#if 0
	dinput->bR2s = joy.lRx;
	dinput->bL2s = joy.lRy;
#else
	dinput->bR2s = static_cast<int>((joy.lRx + 1000) / 2000.0f * 255.0f);
	dinput->bL2s = static_cast<int>((joy.lRy + 1000) / 2000.0f * 255.0f);
#endif

	//BUTTONS TRG
	if ( dinput->bRECTs )
	{
		if ( dinput->cnt[0] == 0 ) dinput->bRECTt = 1;
		else dinput->bRECTt = 0;
		dinput->cnt[0]++;
	}
	else
	{
		dinput->bRECTt = 0;
		dinput->cnt[0] = 0;
	}

	if ( dinput->bCROSSs )
	{
		if (dinput->cnt[1] == 0) dinput->bCROSSt = 1;
		else dinput->bCROSSt = 0;
		dinput->cnt[1]++;
	}
	else
	{
		dinput->bCROSSt = 0;
		dinput->cnt[1] = 0;
	}

	if ( dinput->bCIRCLEs )
	{
		if ( dinput->cnt[2] == 0 ) dinput->bCIRCLEt = 1;
		else dinput->bCIRCLEt = 0;
		dinput->cnt[2]++;
	}
	else
	{
		dinput->bCIRCLEt = 0;
		dinput->cnt[2] = 0;
	}

	if ( dinput->bTRIANGLEs )
	{
		if ( dinput->cnt[3] == 0 ) dinput->bTRIANGLEt = 1;
		else dinput->bTRIANGLEt = 0;
		dinput->cnt[3]++;
	}
	else
	{
		dinput->bTRIANGLEt = 0;
		dinput->cnt[3] = 0;
	}

	if ( dinput->bL1s )
	{
		if ( dinput->cnt[4] == 0 ) dinput->bL1t = 1;
		else dinput->bL1t = 0;
		dinput->cnt[4]++;
	}
	else
	{
		dinput->bL1t = 0;
		dinput->cnt[4] = 0;
	}

	if ( dinput->bR1s )
	{
		if (dinput->cnt[5] == 0) dinput->bR1t = 1;
		else dinput->bR1t = 0;
		dinput->cnt[5]++;
	}
	else
	{
		dinput->bR1t = 0;
		dinput->cnt[5] = 0;
	}

	if ( dinput->bSHAREs )
	{
		if ( dinput->cnt[6] == 0 ) dinput->bSHAREt = 1;
		else dinput->bSHAREt = 0;
		dinput->cnt[6]++;
	}
	else
	{
		dinput->bSHAREt = 0;
		dinput->cnt[6] = 0;
	}

	if ( dinput->bOPTIONSs )
	{
		if ( dinput->cnt[7] == 0 ) dinput->bOPTIONSt = 1;
		else dinput->bOPTIONSt = 0;
		dinput->cnt[7]++;
	}
	else
	{
		dinput->bOPTIONSt = 0;
		dinput->cnt[7] = 0;
	}

	if ( dinput->bL3s )
	{
		if ( dinput->cnt[8] == 0 ) dinput->bL3t = 1;
		else dinput->bL3t = 0;
		dinput->cnt[8]++;
	}
	else
	{
		dinput->bL3t = 0;
		dinput->cnt[8] = 0;
	}

	if ( dinput->bR3s )
	{
		if ( dinput->cnt[9] == 0 ) dinput->bR3t = 1;
		else dinput->bR3t = 0;
		dinput->cnt[9]++;
	}
	else
	{
		dinput->bR3t = 0;
		dinput->cnt[9] = 0;
	}

	if ( dinput->bUPs )
	{
		if ( dinput->cnt[10] == 0 )dinput->bUPt = 1;
		else dinput->bUPt = 0;
		dinput->cnt[10]++;
	}
	else
	{
		dinput->bUPs = 0;
		dinput->cnt[10] = 0;
	}

	if ( dinput->bDOWNs )
	{
		if ( dinput->cnt[11] == 0 ) dinput->bDOWNt = 1;
		else dinput->bDOWNt = 0;
		dinput->cnt[11]++;
	}
	else
	{
		dinput->bDOWNt = 0;
		dinput->cnt[11] = 0;
	}

	if ( dinput->bLEFTs )
	{
		if ( dinput->cnt[13] == 0 ) dinput->bLEFTt = 1;
		else dinput->bLEFTt = 0;
		dinput->cnt[13]++;
	}
	else
	{
		dinput->bLEFTt = 0;
		dinput->cnt[13] = 0;
	}

	if ( dinput->bRIGHTs )
	{
		if ( dinput->cnt[12] == 0 ) dinput->bRIGHTt = 1;
		else dinput->bRIGHTt = 0;
		dinput->cnt[12]++;
	}
	else
	{
		dinput->bRIGHTt = 0;
		dinput->cnt[12] = 0;
	}

	if ( dinput->bR2s )
	{
		if ( dinput->cnt[15] == 0 ) dinput->bR2t = 1;
		else dinput->bR2t = 0;
		dinput->cnt[15]++;
	}
	else
	{
		dinput->bR2t = 0;
		dinput->cnt[15] = 0;
	}

	if ( dinput->bL2s )
	{
		if ( dinput->cnt[14] == 0 ) dinput->bL2t = 1;
		else dinput->bL2t = 0;
		dinput->cnt[14]++;
	}
	else
	{
		dinput->bL2t = 0;
		dinput->cnt[14] = 0;
	}

	return true;
}

void SetXInputVibration(float rVib, float lVib, float _cnt, int _num)
{
	xInput[_num].isVibration = true;
	xInput[_num].rVib = rVib;
	xInput[_num].lVib = lVib;
	xInput[_num].vibMaxCnt = _cnt;

	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = (int)(lVib * 65535.0f);  // val:0~65535
	vibration.wRightMotorSpeed = (int)(rVib * 65535.0f); // val:0~65535
	XInputSetState(_num, &vibration);
}

bool GetKeyboardState(int input_num)
{
	if (GetAsyncKeyState(input_num) < 0) return true;
	return false;
}

bool GetKeyboardTrg(int input_num)
{
	bool trg = false;
	if (GetKeyState(input_num) < 0)
	{
		if (PAD.cnt == 0) trg = true;
		PAD.cnt++;
	}
	else
	{
		PAD.cnt = 0;
	}
	return trg;
}


bool Mouse::InitDInput(HINSTANCE hInstApp, HWND hWnd)
{
	HRESULT ret = DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& pDInput, NULL);
	if (FAILED(ret)) {
		return false;	// 作成に失敗
	}
	return true;
}

bool Mouse::ReleaseDInput()
{
	// DirectInputのデバイスを開放
	if (pDInput) {
		pDInput->Release();
		pDInput = NULL;
	}

	return true;
}

bool Mouse::InitDInputMouse(HWND hWnd)
{
	HRESULT ret = S_FALSE;
	if (pDInput == NULL) {
		return false;
	}

	// マウス用にデバイスオブジェクトを作成
	ret = pDInput->CreateDevice(GUID_SysMouse, &pDIMouse, NULL);
	if (FAILED(ret)) {
		// デバイスの作成に失敗
		return false;
	}

	// データフォーマットを設定
	ret = pDIMouse->SetDataFormat(&c_dfDIMouse);	// マウス用のデータ・フォーマットを設定
	if (FAILED(ret)) {
		// データフォーマットに失敗
		return false;
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	ret = pDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret)) {
		// モードの設定に失敗
		return false;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	ret = pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) {
		// デバイスの設定に失敗
		return false;
	}

	// 入力制御開始
	pDIMouse->Acquire();

	return true;
}

bool Mouse::ReleaseDInputMouse()
{
	// DirectInputのデバイスを開放
	if (pDIMouse) {
		pDIMouse->Release();
		pDIMouse = NULL;
	}

	return true;
}

void Mouse::GetMouseState(HWND hWnd)
{
	if (pDIMouse == NULL) {
		// オブジェクト生成前に呼ばれたときはここで生成させる
		InitDInputMouse(hWnd);
	}

	// 読取前の値を保持します
	DIMOUSESTATE diMouseState_bak;	// マウス情報(変化検知用)
	memcpy(&diMouseState_bak, &diMouseState, sizeof(diMouseState_bak));

	// ここから、DirectInputで必要なコード -->
		// マウスの状態を取得します
	HRESULT	hr = pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &diMouseState);
	if (hr == DIERR_INPUTLOST) {
		pDIMouse->Acquire();
		hr = pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &diMouseState);
	}
	// --> ここまで、DirectInputで必要なコード

	GetCursorPos(&mousePos);

	if (memcmp(&diMouseState_bak, &diMouseState, sizeof(diMouseState_bak)) != 0) {
		// 確認用の処理、ここから -->
				// 値が変わったら表示します
		/*char buf[128];
		wsprintf(buf, "(%5d, %5d, %5d) %s %s %s\n",
			g_zdiMouseState.lX, g_zdiMouseState.lY, g_zdiMouseState.lZ,
			(g_zdiMouseState.rgbButtons[0] & 0x80) ? "Left" : "--",
			(g_zdiMouseState.rgbButtons[1] & 0x80) ? "Right" : "--",
			(g_zdiMouseState.rgbButtons[2] & 0x80) ? "Center" : "--");
		OutputDebugString(buf);*/
		// --> ここまで、確認用の処理
	}
}









//#pragma region myInput
//
////***************************************************************************
//std::list<InputSystem::ListData> InputSystem::keyStack;
//const int InputSystem::saveFrame;
//
//size_t operator|=(size_t& s, const KeyCode k) {
//	return s |= static_cast<const size_t>(k);
//}
//size_t operator|=(size_t& s, const MouseButton k) {
//	return s |= static_cast<const size_t>(k);
//}
//
//
//KeyBoardState::KeyBoardState()
//	:keyboard_{ std::make_unique<DirectX::Keyboard>() } {
//}
//
//bool KeyBoardState::kGetValue(const KeyCode keycode)const {
//	return keyValue_.test(static_cast<size_t>(keycode));
//}
//
//bool KeyBoardState::kGetValue(const size_t keycode)const {
//	return keyValue_.test(keycode);
//}
//
//ButtonState KeyBoardState::GetState(const KeyCode keycode)const {
//	if (keyValue_.test(static_cast<size_t>(keycode))) {
//		if (oldKeyValue_.test(static_cast<size_t>(keycode))) {
//			return ButtonState::kHold;
//		}
//		return ButtonState::kPress;
//	}
//
//	if (oldKeyValue_.test(static_cast<size_t>(keycode))) {
//		return ButtonState::kRelease;
//	}
//
//	return ButtonState::kNone;
//}
//
//ButtonState KeyBoardState::kGetState(const size_t keycode)const {
//	InputSystem::keyStack.emplace_back(keycode);
//	return ButtonState::kPress;
//}
//
//
//
//
//MouseState::MouseState(const HWND hwnd)
//	:mouse_{ std::make_unique<DirectX::Mouse>() } {
//	mouse_->SetWindow(hwnd);
//}
//
//bool MouseState::mGetValue(const MouseButton button)const {
//	return buttonValue_.test(static_cast<size_t>(button));
//}
//
//bool MouseState::mGetValue(const size_t button)const {
//	return buttonValue_.test(button);
//}
//
//ButtonState MouseState::GetState(const MouseButton button)const {
//	if (buttonValue_.test(static_cast<size_t>(button))) {
//		if (oldButtonValue_.test(static_cast<size_t>(button))) {
//			return ButtonState::kHold;
//		}
//		return ButtonState::kPress;
//	}
//
//	if (oldButtonValue_.test(static_cast<size_t>(button))) {
//		return ButtonState::kRelease;
//	}
//
//	return ButtonState::kNone;
//}
//
//ButtonState MouseState::mGetState(const size_t button)const {
//	InputSystem::keyStack.emplace_back(button);
//	return ButtonState::kPress;
//}
//
//int MouseState::DeltaX()const {
//	return x_;
//}
//
//int MouseState::DeltaY()const {
//	return y_;
//}
//
//int MouseState::scrollWheelValue()const {
//	return scrollWheelValue_;
//}
//
//
//
//
//GamePadState::GamePadState()
//{}
//
//bool GamePadState::kGetValue(const GamePadButton keycode)const {
//	return buttonValue_.test(static_cast<size_t>(keycode));
//}
//
//bool GamePadState::kGetValue(const size_t keycode)const {
//	return buttonValue_.test(keycode);
//}
//
//ButtonState GamePadState::GetState(const GamePadButton keycode)const {
//	if (buttonValue_.test(static_cast<size_t>(keycode))) {
//		if (oldButtonValue_.test(static_cast<size_t>(keycode))) {
//			return ButtonState::kHold;
//		}
//		return ButtonState::kPress;
//	}
//
//	if (oldButtonValue_.test(static_cast<size_t>(keycode))) {
//		return ButtonState::kRelease;
//	}
//	return ButtonState::kNone;
//}
//
//ButtonState GamePadState::kGetState(const size_t keycode)const {
//	InputSystem::keyStack.emplace_back(keycode);
//	return ButtonState::kPress;
//}
//
//
//void InputSystem::Initialize(const HWND hwnd) {
//	keyboard_ = std::make_unique<KeyBoardState>();
//	mouse_ = std::make_unique<MouseState>(hwnd);
//	gamepad_ = std::make_unique<GamePadState>();
//}
//
//void InputSystem::UpdateStates() {
//	{//ゲームパッド
//		gamepad_->axisLx = gamepad_->axisLy = 0.0f;
//		gamepad_->axisRx = gamepad_->axisRy = 0.0f;
//		gamepad_->triggerL = gamepad_->triggerR = 0.0f;
//
//		gamepad_->oldButtonValue_ = gamepad_->buttonValue_;
//		gamepad_->buttonValue_ = 0;
//		gamepad_->exist = false;
//		XINPUT_STATE xinputState;
//		if (XInputGetState(gamepad_->slot, &xinputState) == ERROR_SUCCESS) {
//			XINPUT_GAMEPAD& gp = xinputState.Gamepad;
//			auto& v = gamepad_->buttonValue_;
//
//			if (gp.wButtons & XINPUT_GAMEPAD_DPAD_UP)		    v.set(static_cast<size_t>(GamePadButton::ButtonUP));
//			if (gp.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)	    v.set(static_cast<size_t>(GamePadButton::ButtonRIGHT));
//			if (gp.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)		    v.set(static_cast<size_t>(GamePadButton::ButtonDOWN));
//			if (gp.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)		    v.set(static_cast<size_t>(GamePadButton::ButtonLEFT));
//			if (gp.wButtons & XINPUT_GAMEPAD_A)				    v.set(static_cast<size_t>(GamePadButton::ButtonA));
//			if (gp.wButtons & XINPUT_GAMEPAD_B)				    v.set(static_cast<size_t>(GamePadButton::ButtonB));
//			if (gp.wButtons & XINPUT_GAMEPAD_X)				    v.set(static_cast<size_t>(GamePadButton::ButtonX));
//			if (gp.wButtons & XINPUT_GAMEPAD_Y)				    v.set(static_cast<size_t>(GamePadButton::ButtonY));
//			if (gp.wButtons & XINPUT_GAMEPAD_START)			    v.set(static_cast<size_t>(GamePadButton::ButtonSTART));
//			if (gp.wButtons & XINPUT_GAMEPAD_BACK)			    v.set(static_cast<size_t>(GamePadButton::ButtonBACK));
//			if (gp.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)		v.set(static_cast<size_t>(GamePadButton::ButtonLEFT_THUMB));
//			if (gp.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)       v.set(static_cast<size_t>(GamePadButton::ButtonRIGHT_THUMB));
//			if (gp.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)     v.set(static_cast<size_t>(GamePadButton::ButtonLEFT_SHOULDER));
//			if (gp.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)    v.set(static_cast<size_t>(GamePadButton::ButtonRIGHT_SHOULDER));
//
//			if (gp.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)  v.set(static_cast<size_t>(GamePadButton::ButtonLEFT_TRIGGER));
//			if (gp.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) v.set(static_cast<size_t>(GamePadButton::ButtonRIGHT_TRIGGER));
//
//			if ((gp.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && gp.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
//				(gp.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && gp.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
//			{
//				gp.sThumbLX = 0;
//				gp.sThumbLY = 0;
//			}
//
//			if ((gp.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && gp.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
//				(gp.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && gp.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
//			{
//				gp.sThumbRX = 0;
//				gp.sThumbRY = 0;
//			}
//
//			gamepad_->triggerL = static_cast<float>(gp.bLeftTrigger) / 255.0f;
//			gamepad_->triggerR = static_cast<float>(gp.bRightTrigger) / 255.0f;
//			gamepad_->axisLx = static_cast<float>(gp.sThumbLX) / static_cast<float>(0x8000);
//			gamepad_->axisLy = static_cast<float>(gp.sThumbLY) / static_cast<float>(0x8000);
//			gamepad_->axisRx = static_cast<float>(gp.sThumbRX) / static_cast<float>(0x8000);
//			gamepad_->axisRy = static_cast<float>(gp.sThumbRY) / static_cast<float>(0x8000);
//			gamepad_->exist = true;
//		}
//
//
//		{//キーボード
//			keyboard_->oldKeyValue_ = keyboard_->keyValue_;
//			keyboard_->keyValue_ = 0;
//			if (keyboard_->keyboard_->IsConnected()) {
//				const auto s = keyboard_->keyboard_->GetState();
//				auto& v = keyboard_->keyValue_;
//
//				if (s.NumPad0)   v.set(static_cast<size_t>(KeyCode::k0));
//				if (s.NumPad1)   v.set(static_cast<size_t>(KeyCode::k1));
//				if (s.NumPad2)   v.set(static_cast<size_t>(KeyCode::k2));
//				if (s.NumPad3)   v.set(static_cast<size_t>(KeyCode::k3));
//				if (s.NumPad4)   v.set(static_cast<size_t>(KeyCode::k4));
//				if (s.NumPad5)   v.set(static_cast<size_t>(KeyCode::k5));
//				if (s.NumPad6)   v.set(static_cast<size_t>(KeyCode::k6));
//				if (s.NumPad7)   v.set(static_cast<size_t>(KeyCode::k7));
//				if (s.NumPad8)   v.set(static_cast<size_t>(KeyCode::k8));
//				if (s.NumPad9)   v.set(static_cast<size_t>(KeyCode::k9));
//				if (s.Q)         v.set(static_cast<size_t>(KeyCode::kQ));
//				if (s.W)         v.set(static_cast<size_t>(KeyCode::kW));
//				if (s.E)         v.set(static_cast<size_t>(KeyCode::kE));
//				if (s.R)         v.set(static_cast<size_t>(KeyCode::kR));
//				if (s.T)         v.set(static_cast<size_t>(KeyCode::kT));
//				if (s.Y)         v.set(static_cast<size_t>(KeyCode::kY));
//				if (s.U)         v.set(static_cast<size_t>(KeyCode::kU));
//				if (s.I)         v.set(static_cast<size_t>(KeyCode::kI));
//				if (s.O)         v.set(static_cast<size_t>(KeyCode::kO));
//				if (s.P)         v.set(static_cast<size_t>(KeyCode::kP));
//				if (s.A)         v.set(static_cast<size_t>(KeyCode::kA));
//				if (s.S)         v.set(static_cast<size_t>(KeyCode::kS));
//				if (s.D)         v.set(static_cast<size_t>(KeyCode::kD));
//				if (s.F)         v.set(static_cast<size_t>(KeyCode::kF));
//				if (s.G)         v.set(static_cast<size_t>(KeyCode::kG));
//				if (s.H)         v.set(static_cast<size_t>(KeyCode::kH));
//				if (s.J)         v.set(static_cast<size_t>(KeyCode::kJ));
//				if (s.K)         v.set(static_cast<size_t>(KeyCode::kK));
//				if (s.L)         v.set(static_cast<size_t>(KeyCode::kL));
//				if (s.Z)         v.set(static_cast<size_t>(KeyCode::kZ));
//				if (s.X)         v.set(static_cast<size_t>(KeyCode::kX));
//				if (s.C)         v.set(static_cast<size_t>(KeyCode::kC));
//				if (s.V)         v.set(static_cast<size_t>(KeyCode::kV));
//				if (s.B)         v.set(static_cast<size_t>(KeyCode::kB));
//				if (s.N)         v.set(static_cast<size_t>(KeyCode::kN));
//				if (s.M)         v.set(static_cast<size_t>(KeyCode::kM));
//
//				if (s.Left)      v.set(static_cast<size_t>(KeyCode::kLeft));
//				if (s.Up)        v.set(static_cast<size_t>(KeyCode::kUp));
//				if (s.Right)     v.set(static_cast<size_t>(KeyCode::kRight));
//				if (s.Down)      v.set(static_cast<size_t>(KeyCode::kDown));
//
//				if (s.Escape)    v.set(static_cast<size_t>(KeyCode::kEsc));
//				if (s.Space)     v.set(static_cast<size_t>(KeyCode::kSpace));
//				if (s.Enter)     v.set(static_cast<size_t>(KeyCode::kEnter));
//				if (s.LeftShift) v.set(static_cast<size_t>(KeyCode::kLShift));
//			}
//		}
//
//		{//マウス
//			mouse_->oldButtonValue_ = mouse_->buttonValue_;
//			mouse_->buttonValue_ = 0;
//			if (mouse_->mouse_->IsConnected()) {
//				const auto s = mouse_->mouse_->GetState();
//				auto& v = mouse_->buttonValue_;
//				if (s.leftButton)    v.set(static_cast<size_t>(MouseButton::kLeft));
//				if (s.middleButton)  v.set(static_cast<size_t>(MouseButton::kMiddle));
//				if (s.rightButton)   v.set(static_cast<size_t>(MouseButton::kRight));
//				mouse_->x_ = s.x;
//				mouse_->y_ = s.y;
//				mouse_->scrollWheelValue_ = s.scrollWheelValue;
//			}
//		}
//	}
//
//	for (auto& data : keyStack) data.frame++;
//
//	keyStack.erase(
//		remove_if(keyStack.begin(), keyStack.end(),
//			[](ListData data) { return data.frame >= saveFrame; }),
//		keyStack.end());
//}
//
//
//#pragma endregion 