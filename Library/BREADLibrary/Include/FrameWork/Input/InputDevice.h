#pragma once
#include <bitset>
#include <list>
#include <map>

#include <windows.h>
#include <cassert>

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>
#include <vector>
#include <memory>
//#include <Mouse.h>
//#include <Keyboard.h>
#include <Xinput.h>
#pragma comment (lib, "xinput.lib")

#define PAD_NUM_MAX 4

#define XINPUT_DPAD_UP			      0x0001
#define XINPUT_DPAD_DOWN		  0x0002
#define XINPUT_DPAD_LEFT		      0x0004
#define XINPUT_DPAD_RIGHT		      0x0008
#define XINPUT_START			          0x0010
#define XINPUT_BACK				          0x0020
#define XINPUT_LEFT_THUMB		      0x0040
#define XINPUT_RIGHT_THUMB		  0x0080
#define XINPUT_LEFT_SHOULDER	  0x0100
#define XINPUT_RIGHT_SHOULDER	  0x0200
#define XINPUT_A				              0x1000
#define XINPUT_B				              0x2000
#define XINPUT_X				              0x4000
#define XINPUT_Y				              0x8000


struct XINPUT
{
	// state
	int bAs; // val : 0 or 1
	int bBs; // val : 0 or 1
	int bXs; // val : 0 or 1
	int bYs; // val : 0 or 1

	int bRBs; // val : 0 or 1
	int bLBs; // val : 0 or 1
	int bRTs; // val : 0 ~ 255
	int bLTs; // val : 0 ~ 255

	int    bUPs;      // val : 0 or 1
	int  bDOWNs; // val : 0 or 1
	int bRIGHTs;   // val : 0 or 1
	int  bLEFTs;     // val : 0 or 1

	int bSTARTs; // val : 0 or 1
	int  bBACKs; // val : 0 or 1

	int bR3s; // val : 0 or 1
	int bL3s; // val : 0 or 1

	// trg
	int bAt; // val : 0 or 1
	int bBt; // val : 0 or 1
	int bXt; // val : 0 or 1
	int bYt; // val : 0 or 1

	int bRBt; // val : 0 or 1
	int bLBt; // val : 0 or 1
	int bRTt; // val : 0 or 1
	int bLTt; // val : 0 or 1

	int    bUPt; // val : 0 or 1
	int  bDOWNt; // val : 0 or 1
	int bRIGHTt; // val : 0 or 1
	int  bLEFTt; // val : 0 or 1

	int bSTARTt; // val : 0 or 1
	int  bBACKt; // val : 0 or 1

	int bR3t; // val : 0 or 1
	int bL3t; // val : 0 or 1

	// stick
	int  sX; // val : -1000 ~ 1000 (-1000 : LEFT , 1000 : RIGHT)
	int  sY; // val : -1000 ~ 1000 (-1000 : UP   , 1000 : DOWN)
	int sRX; // val : -1000 ~ 1000 (-1000 : LEFT , 1000 : RIGHT)
	int sRY; // val : -1000 ~ 1000 (-1000 : UP   , 1000 : DOWN)

	// vibration
	bool isVibration;
	float rVib;
	float lVib;
	float vibCnt;
	float vibMaxCnt;

	// button cnt
	int cnt[16] = { 0 };
};

extern XINPUT xInput[PAD_NUM_MAX];
extern XINPUT xInputDummy;


struct DINPUT
{
	// state
	int   bCIRCLEs; // val : 0 or 1
	int    bCROSSs; // val : 0 or 1
	int bTRIANGLEs; // val : 0 or 1
	int     bRECTs; // val : 0 or 1

	int bR1s; // val : 0 or 1
	int bL1s; // val : 0 or 1
	int bR2s; // val : 0 ~ 255
	int bL2s; // val : 0 ~ 255

	int    bUPs; // val : 0 or 1
	int  bDOWNs; // val : 0 or 1
	int bRIGHTs; // val : 0 or 1
	int  bLEFTs; // val : 0 or 1

	int   bSHAREs; // val : 0 or 1
	int bOPTIONSs; // val : 0 or 1

	int bR3s; // val : 0 or 1
	int bL3s; // val : 0 or 1

	// trg
	int   bCIRCLEt; // val : 0 or 1
	int    bCROSSt; // val : 0 or 1
	int bTRIANGLEt; // val : 0 or 1
	int     bRECTt; // val : 0 or 1

	int bR1t; // val : 0 or 1
	int bL1t; // val : 0 or 1
	int bR2t; // val : 0 or 1
	int bL2t; // val : 0 or 1

	int    bUPt; // val : 0 or 1
	int  bDOWNt; // val : 0 or 1
	int bRIGHTt; // val : 0 or 1
	int  bLEFTt; // val : 0 or 1

	int   bSHAREt; // val : 0 or 1
	int bOPTIONSt; // val : 0 or 1

	int bR3t; // val : 0 or 1
	int bL3t; // val : 0 or 1

	// stick
	int sX;
	int sY;
	int sRX;
	int sRY;

	// button cnt
	int cnt[16] = { 0 };
};

extern DINPUT dInput[PAD_NUM_MAX];


class Joystick
{
public:
	HWND hwnd;
	HINSTANCE instance;

	int cnt = 0;

	DIDEVCAPS dc;

	LPDIRECTINPUT8          lpDI = NULL;
	std::vector<LPDIRECTINPUTDEVICE8>  lpJoystick;

	//std::vector<XINPUT_VIBRATION> vibration;
	//LPDIRECTINPUTDEVICE8    lpJoystick = NULL;      // ジョイスティックデバイス
	//LPDIRECTINPUTEFFECT     lpDIEffect = NULL;
	//DWORD					dwNumForceFeedbackAxis = NULL;		// フォースフィードバック

	 Joystick() : hwnd(), dc(), instance() {}
	~Joystick() {}

	//再接続時にも使用
	void InitInputDevice();

	static Joystick& getInstance()
	{
		static Joystick instance;
		return	instance;
	}
};

#define	PAD	Joystick::getInstance()


extern bool GetXInputState(XINPUT *xinput, int _num = 0, float elapsedTime = 0.0f);         //Xboxコントローラーの入力状態の取得
extern bool GetDInputState(DINPUT *dinput, int _num = 0, float elapsedTime = 0.0f);         //PS4コントローラーの入力状態の取得

/// <summary>
/// Xboxコントローラーのバイブレーション値の設定
/// </summary>
/// <param name="rVib"> 右の振動値 : 0.0f ~ 1.0f </param>
/// <param name="lVib"> 左の振動値 : 0.0f ~ 1.0f </param>
/// <param name="_cnt"> 振動時間(フレーム) </param>
/// <param name="_num"> コントローラー番号 : 0 ~ 3 </param>
extern void SetXInputVibration(float rVib, float lVib, float _cnt, int _num = 0);


class Mouse
{
public:
	LPDIRECTINPUT8             pDInput    = NULL;
	LPDIRECTINPUTDEVICE8 pDIMouse = NULL;

	DIMOUSESTATE diMouseState;
	POINT                mousePos;

	 Mouse() : diMouseState(), mousePos() {}
	~Mouse() {}

	bool InitDInput(HINSTANCE hInstApp, HWND hWnd);
	bool ReleaseDInput();

	bool InitDInputMouse(HWND hWnd);
	bool ReleaseDInputMouse();

	void GetMouseState(HWND hWnd);

	static Mouse& getInstance()
	{
		static Mouse instance;
		return	instance;
	}
};

#define MOUSE Mouse::getInstance()








//#pragma region myInput
//
//
//
//#pragma region enums
//enum class ButtonState {
//    kPress,
//    kRelease,
//    kHold,
//    kNone,
//};
//
//enum class KeyCode : size_t {
//    k0,
//    k1,
//    k2,
//    k3,
//    k4,
//    k5,
//    k6,
//    k7,
//    k8,
//    k9,
//
//    kQ,
//    kW,
//    kE,
//    kR,
//    kT,
//    kY,
//    kU,
//    kI,
//    kO,
//    kP,
//    kA,
//    kS,
//    kD,
//    kF,
//    kG,
//    kH,
//    kJ,
//    kK,
//    kL,
//    kZ,
//    kX,
//    kC,
//    kV,
//    kB,
//    kN,
//    kM,
//
//    kLeft,
//    kUp,
//    kRight,
//    kDown,
//
//    kEsc,
//    kSpace,
//    kEnter,
//    kLShift,
//
//    kMax
//
//};
//
//enum class MouseButton : size_t {
//    kLeft,
//    kMiddle,
//    kRight,
//
//    kMax
//};
//
//enum class GamePadButton : size_t {
//    ButtonUP,
//    ButtonRIGHT,
//    ButtonDOWN,
//    ButtonLEFT,
//    ButtonA,
//    ButtonB,
//    ButtonX,
//    ButtonY,
//    ButtonSTART,
//    ButtonBACK,
//    ButtonLEFT_THUMB,
//    ButtonRIGHT_THUMB,
//    ButtonLEFT_SHOULDER,
//    ButtonRIGHT_SHOULDER,
//    ButtonLEFT_TRIGGER,
//    ButtonRIGHT_TRIGGER,
//    kMax
//};
//#pragma endregion
//
//class KeyBoardState final
//{
//    friend class InputSystem;
//
//public:
//    KeyBoardState();
//    ~KeyBoardState() {
//        keyboard_.reset();
//    }
//
//    [[nodiscard]] bool kGetValue(const KeyCode code)const;
//    [[nodiscard]] bool kGetValue(const size_t code)const;
//    [[nodiscard]] ButtonState GetState(const KeyCode keycode)const;
//    ButtonState kGetState(const size_t keycode) const;
//
//private:
//    std::unique_ptr<DirectX::Keyboard> keyboard_ = {};
//
//    std::bitset<static_cast<size_t>(KeyCode::kMax)> keyValue_ = {};
//    std::bitset<static_cast<size_t>(KeyCode::kMax)> oldKeyValue_ = {};
//};
//
//class MouseState final {
//    friend class InputSystem;
//
//public:
//    MouseState(const HWND hwnd);
//    ~MouseState() {
//        mouse_.reset();
//    }
//
//    [[nodiscard]] bool             mGetValue(const MouseButton buttons)const;
//    [[nodiscard]] bool             mGetValue(const size_t buttons)             const;
//    [[nodiscard]] ButtonState GetState(const  MouseButton button)     const;
//    ButtonState                      mGetState(const  size_t button)               const;
//
//    [[nodiscard]] int  DeltaX()                 const;
//    [[nodiscard]] int  DeltaY()                 const;
//    [[nodiscard]] int  scrollWheelValue()const;
//
//private:
//    std::unique_ptr<DirectX::Mouse>                     mouse_ = {};
//
//    std::bitset<static_cast<size_t>(MouseButton::kMax)> buttonValue_ = {};
//    std::bitset<static_cast<size_t>(MouseButton::kMax)> oldButtonValue_ = {};
//
//    int x_ = {};
//    int y_ = {};
//    int scrollWheelValue_ = {};
//};
//
//
//class GamePadState {
//public:
//    friend class InputSystem;
//
//public:
//    GamePadState();
//    ~GamePadState() = default;
//
//    [[nodiscard]] bool             kGetValue(const GamePadButton code)       const;
//    [[nodiscard]] bool             kGetValue(const size_t        code)                  const;
//    [[nodiscard]] ButtonState GetState(const GamePadButton  keycode)   const;
//    ButtonState                      kGetState(const size_t        keycode)             const;
//
//    float GetAxisLX()   const { return axisLx; }
//    float GetAxisLY()   const { return axisLy; }
//    float GetAxisRX()   const { return axisRx; }
//    float GetAxisRY()   const { return axisRy; }
//    float GetTriggerL() const { return triggerL; }
//    float GetTriggerR() const { return triggerR; }
//    bool  GetExist()      const { return exist; }
//
//    // std::unique_ptr<XINPUT_GAMEPAD> gamePad_ = {};
//private:
//    std::bitset<static_cast<size_t>(GamePadButton::kMax)> buttonValue_ = {};
//    std::bitset<static_cast<size_t>(GamePadButton::kMax)> oldButtonValue_ = {};
//
//    float				axisLx = 0.0f;
//    float				axisLy = 0.0f;
//    float				axisRx = 0.0f;
//    float				axisRy = 0.0f;
//    float				triggerL = 0.0f;
//    float				triggerR = 0.0f;
//    int					slot = 0;
//
//    bool               exist = false;
//};
//
//class InputSystem final {
//public:
//    ~InputSystem() {
//        keyboard_.reset();
//        mouse_.reset();
//        gamepad_.reset();
//    }
//
//    [[nodiscard]] static InputSystem* Instance() {
//        static InputSystem instance{};
//        return &instance;
//    };
//
//    void Initialize(const HWND hwnd);
//    void UpdateStates();
//
//    [[nodiscard]] const KeyBoardState& Keyboard() const { return *keyboard_; }
//    [[nodiscard]] const MouseState& Mouse()     const { return *mouse_; }
//    [[nodiscard]] const GamePadState& Gamepad() const { return *gamepad_; }
//
//private:
//    explicit InputSystem() = default;
//
//private:
//    std::unique_ptr<KeyBoardState>   keyboard_ = {};
//    std::unique_ptr<MouseState>        mouse_ = {};
//    std::unique_ptr<GamePadState>   gamepad_ = {};
//
//
//public://先行入力
//    struct ListData {
//        size_t      codeData;
//        int         frame;
//        ListData(const size_t code)
//        {
//            codeData = code;
//            frame = 0;
//        }
//
//    };
//    static std::list<ListData> keyStack;
//    static const int           saveFrame = 18;
//
//public:
//    static std::list<size_t> GetKeyList(size_t delay) {
//        std::list<size_t> findList;
//        for (auto data : keyStack)
//            findList.emplace_back(data.codeData);
//
//        return findList;
//    }
//
//    static void DeleteKeyDataFirst(const size_t code) {
//        keyStack.erase(
//            remove_if(keyStack.begin(), keyStack.end(),
//                [code](ListData data) {return data.codeData == code; }),
//            keyStack.end());
//    }
//
//};
//
//
//#define KeyCast(k)      static_cast<size_t>(KeyCode::k)
//#define MouseCast(k)    static_cast<size_t>(MouseButton::k)
//#define PadCast  (k)    static_cast<size_t>(GamePadButton::k)
//
//#define bkey(k)              InputSystem::Instance()->Keyboard().kGetState(k)
//#define bmouse(m)       InputSystem::Instance()->Mouse()   .mGetState(m)
//#define bmouse(m)       InputSystem::Instance()->Gamepad() .kGetState(m)
//
//#define keyPress(k)       InputSystem::Instance()->Keyboard().GetState(KeyCode::k)       == ButtonState::kPress
//#define keyState(k)       InputSystem::Instance()->Keyboard().GetState(KeyCode::k)       == ButtonState::kHold
//#define keyRelease(k)   InputSystem::Instance()->Keyboard().GetState(KeyCode::k)       == ButtonState::kRelease
//#define keyNone(k)      InputSystem::Instance()->Keyboard().GetState(KeyCode::k)       == ButtonState::kNone
//
//#define mouse(m)             InputSystem::Instance()->Mouse().   GetState(MouseButton::m)   == ButtonState::kPress
//#define mouseState(m)     InputSystem::Instance()->Mouse().   GetState(MouseButton::m)   == ButtonState::kHold
//#define mouseRelease(m) InputSystem::Instance()->Mouse().   GetState(MouseButton::m)   == ButtonState::kRelease
//#define mouseNone(m)    InputSystem::Instance()->Mouse().   GetState(MouseButton::m)   == ButtonState::kNone
//
//
//#define padPress(p)       InputSystem::Instance()->Gamepad(). GetState(GamePadButton::p) ==ButtonState::kPress
//#define padState(p)       InputSystem::Instance()->Gamepad(). GetState(GamePadButton::p) ==ButtonState::kHold
//#define padRelease(p)   InputSystem::Instance()->Gamepad(). GetState(GamePadButton::p) ==ButtonState::kRelease
//#define padNone(p)       InputSystem::Instance()->Gamepad(). GetState(GamePadButton::p) ==ButtonState::kNone
//#define padAxisY_LStick InputSystem::Instance()->Gamepad(). GetAxisLY()
//#define padAxisX_LStick InputSystem::Instance()->Gamepad(). GetAxisLX()
//#define padAxisY_RStick InputSystem::Instance()->Gamepad(). GetAxisRY()
//#define padAxisX_RStick InputSystem::Instance()->Gamepad(). GetAxisRX()
//#define padExist             InputSystem::Instance()->Gamepad(). GetExist()
//
//#define inputListData   InputSystem::Instance()->keyStack
//
//#define Torad(angle)      DirectX::XMConvertToRadians(angle)
//#define ToDeg(angle)    DirectX::XMConvertToDegrees(angle)
//
//#pragma endregion 