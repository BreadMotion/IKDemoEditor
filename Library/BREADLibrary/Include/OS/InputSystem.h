#pragma once

#include <bitset>
#include <list>
#include <map>

#include <windows.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <xinput.h>

#pragma region enums
enum class ButtonState {
    kPress,
    kRelease,
    kHold,
    kNone,
};

enum class KeyCode : size_t {
    k0,
    k1,
    k2,
    k3,
    k4,
    k5,
    k6,
    k7,
    k8,
    k9,

    kQ,
    kW,
    kE,
    kR,
    kT,
    kY,
    kU,
    kI,
    kO,
    kP,
    kA,
    kS,
    kD,
    kF,
    kG,
    kH,
    kJ,
    kK,
    kL,
    kZ,
    kX,
    kC,
    kV,
    kB,
    kN,
    kM,

    kLeft,
    kUp,
    kRight,
    kDown,

    kEsc,
    kSpace,
    kEnter,
    kLShift,

    kMax

};

enum class MouseButton : size_t {
    kLeft,
    kMiddle,
    kRight,

    kMax
};

enum class GamePadButton : size_t {
    ButtonUP,
    ButtonRIGHT,
    ButtonDOWN,
    ButtonLEFT,
    ButtonA,
    ButtonB,
    ButtonX,
    ButtonY,
    ButtonSTART,
    ButtonBACK,
    ButtonLEFT_THUMB,
    ButtonRIGHT_THUMB,
    ButtonLEFT_SHOULDER,
    ButtonRIGHT_SHOULDER,
    ButtonLEFT_TRIGGER,
    ButtonRIGHT_TRIGGER,
    kMax
};
#pragma endregion

class KeyBoardState final
{
    friend class InputSystem;

public:
    KeyBoardState();
    ~KeyBoardState() {
        keyboard_.reset();
    }

    [[nodiscard]] bool __fastcall kGetValue(const KeyCode& code)const;
    [[nodiscard]] bool __fastcall kGetValue(const size_t& code)const;
    [[nodiscard]] ButtonState __fastcall GetState(const KeyCode& keycode)const;
    ButtonState __fastcall kGetState(const size_t& keycode) const;

private:
    std::unique_ptr<DirectX::Keyboard> keyboard_ = {};

    std::bitset<static_cast<size_t>(KeyCode::kMax)> keyValue_ = {};
    std::bitset<static_cast<size_t>(KeyCode::kMax)> oldKeyValue_ = {};
};

class MouseState final {
    friend class InputSystem;

public:
    MouseState(const HWND hwnd);
    ~MouseState() {
        mouse_.reset();
    }

    [[nodiscard]] bool        __fastcall mGetValue(const MouseButton& buttons)const;
    [[nodiscard]] bool        __fastcall mGetValue(const size_t&      buttons)const;
    [[nodiscard]] ButtonState __fastcall GetState(const  MouseButton& button) const;
    ButtonState               __fastcall mGetState(const  size_t&     button) const;

    [[nodiscard]] int  DeltaX()          const;
    [[nodiscard]] int  DeltaY()          const;
    [[nodiscard]] int  scrollWheelValue()const;

private:
    std::unique_ptr<DirectX::Mouse> mouse_ = {};

    std::bitset<static_cast<size_t>(MouseButton::kMax)> buttonValue_    = {};
    std::bitset<static_cast<size_t>(MouseButton::kMax)> oldButtonValue_ = {};

    int x_                = {};
    int y_                = {};
    int scrollWheelValue_ = {};
};


class GamePadState {
public:
    friend class InputSystem;

public:
    GamePadState();
    ~GamePadState() = default;

    [[nodiscard]] bool        kGetValue(const GamePadButton& code)const;
    [[nodiscard]] bool        kGetValue(const size_t&        code)const;
    [[nodiscard]] ButtonState GetState(const GamePadButton&  keycode) const;
    ButtonState               kGetState(const size_t&        keycode) const;

    float GetAxisLX()   const { return axisLx; }
    float GetAxisLY()   const { return axisLy; }
    float GetAxisRX()   const { return axisRx; }
    float GetAxisRY()   const { return axisRy; }
    float GetTriggerL() const { return triggerL; }
    float GetTriggerR() const { return triggerR; }
    bool  GetExist()    const { return exist; }

   // std::unique_ptr<XINPUT_GAMEPAD> gamePad_ = {};
private:
    std::bitset<static_cast<size_t>(GamePadButton::kMax)> buttonValue_    = {};
    std::bitset<static_cast<size_t>(GamePadButton::kMax)> oldButtonValue_ = {};

    float axisLx   = 0.0f;
    float axisLy   = 0.0f;
    float axisRx   = 0.0f;
    float axisRy   = 0.0f;
    float triggerL = 0.0f;
    float triggerR = 0.0f;
    int	  slot     = 0;

    bool  exist    = false;
};

class InputSystem final {
public:
    ~InputSystem() {
        keyboard_.reset();
        mouse_.reset();
        gamepad_.reset();
    }

    [[nodiscard]] static InputSystem* Instance() {
        static InputSystem instance{};
        return &instance;
    };

    void Initialize(const HWND hwnd);
    void UpdateStates();

    [[nodiscard]] const KeyBoardState&  Keyboard() const { return *keyboard_; }
    [[nodiscard]] const MouseState&     Mouse()    const { return *mouse_;}
    [[nodiscard]] const GamePadState&   Gamepad() const { return *gamepad_; }

private:
    explicit InputSystem() = default;

private:
    std::unique_ptr<KeyBoardState> keyboard_ = {};
    std::unique_ptr<MouseState>    mouse_    = {};
    std::unique_ptr<GamePadState>  gamepad_  = {};


public://êÊçsì¸óÕ
    struct ListData {
        size_t      codeData;
        int         frame;
        ListData(const size_t code)
        {
            codeData = code;
            frame = 0;
        }

    };
    static std::list<ListData> keyStack;
    static const int           saveFrame = 18;

public:
    static std::list<size_t> __fastcall GetKeyList(size_t& delay) {
        std::list<size_t> findList;
        for (auto data : keyStack)
            findList.emplace_back(data.codeData);

        return findList;
    }

    static void __fastcall DeleteKeyDataFirst(const size_t& code) {
        keyStack.erase(
            remove_if(keyStack.begin(), keyStack.end(),
                [code](ListData data) {return data.codeData == code; }),
            keyStack.end());
    }

};


#define KeyCast(k)      static_cast<size_t>(KeyCode::k)
#define MouseCast(k)    static_cast<size_t>(MouseButton::k)
#define PadCast  (k)    static_cast<size_t>(GamePadButton::k)

#define bkey(k)         InputSystem::Instance()->Keyboard().kGetState(k)
#define bmouse(m)       InputSystem::Instance()->Mouse()   .mGetState(m)
#define bmouse(m)       InputSystem::Instance()->Gamepad() .kGetState(m)

#define keyPress(k)     InputSystem::Instance()->Keyboard().GetState(KeyCode::k)       == ButtonState::kPress
#define keyState(k)     InputSystem::Instance()->Keyboard().GetState(KeyCode::k)       == ButtonState::kHold
#define keyRelease(k)   InputSystem::Instance()->Keyboard().GetState(KeyCode::k)       == ButtonState::kRelease
#define keyNone(k)      InputSystem::Instance()->Keyboard().GetState(KeyCode::k)       == ButtonState::kNone

#define mouse(m)        InputSystem::Instance()->Mouse().   GetState(MouseButton::m)   == ButtonState::kPress
#define mouseState(m)   InputSystem::Instance()->Mouse().   GetState(MouseButton::m)   == ButtonState::kHold
#define mouseRelease(m) InputSystem::Instance()->Mouse().   GetState(MouseButton::m)   == ButtonState::kRelease
#define mouseNone(m)    InputSystem::Instance()->Mouse().   GetState(MouseButton::m)   == ButtonState::kNone


#define padPress(p)     InputSystem::Instance()->Gamepad(). GetState(GamePadButton::p) == ButtonState::kPress
#define padState(p)     InputSystem::Instance()->Gamepad(). GetState(GamePadButton::p) == ButtonState::kHold
#define padRelease(p)   InputSystem::Instance()->Gamepad(). GetState(GamePadButton::p) == ButtonState::kRelease
#define padNone(p)      InputSystem::Instance()->Gamepad(). GetState(GamePadButton::p) == ButtonState::kNone
#define padAxisY_LStick InputSystem::Instance()->Gamepad(). GetAxisLY()
#define padAxisX_LStick InputSystem::Instance()->Gamepad(). GetAxisLX()
#define padAxisY_RStick InputSystem::Instance()->Gamepad(). GetAxisRY()
#define padAxisX_RStick InputSystem::Instance()->Gamepad(). GetAxisRX()
#define padExist        InputSystem::Instance()->Gamepad(). GetExist()

#define inputListData   InputSystem::Instance()->keyStack

#define Torad(angle)    DirectX::XMConvertToRadians(angle)
#define ToDeg(angle)    DirectX::XMConvertToDegrees(angle)
