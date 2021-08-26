#include "OS/InputSystem.h"
#include <math.h>
#include <Xinput.h>

std::list<InputSystem::ListData> InputSystem::keyStack;
const int InputSystem::saveFrame;

size_t operator|=(size_t& s, const KeyCode k) {
    return s |= static_cast<const size_t>(k);
}
size_t operator|=(size_t& s, const MouseButton k) {
    return s |= static_cast<const size_t>(k);
}


KeyBoardState::KeyBoardState()
    :keyboard_{ std::make_unique<DirectX::Keyboard>()} {
}

bool KeyBoardState::kGetValue(const KeyCode keycode)const {
    return keyValue_.test(static_cast<size_t>(keycode));
}

bool KeyBoardState::kGetValue(const size_t keycode)const {
    return keyValue_.test(keycode);
}

ButtonState KeyBoardState::GetState(const KeyCode keycode)const {
    if (keyValue_.test(static_cast<size_t>(keycode))) {
        if (oldKeyValue_.test(static_cast<size_t>(keycode))) {
            return ButtonState::kHold;
        }
            return ButtonState::kPress;
    }

    if (oldKeyValue_.test(static_cast<size_t>(keycode))) {
        return ButtonState::kRelease;
    }

    return ButtonState::kNone;
}

ButtonState KeyBoardState::kGetState(const size_t keycode)const {
    InputSystem::keyStack.emplace_back(keycode);
    return ButtonState::kPress;
}




MouseState::MouseState(const HWND hwnd)
    :mouse_{std::make_unique<DirectX::Mouse>()} {
    mouse_->SetWindow(hwnd);
}

bool MouseState::mGetValue(const MouseButton button)const {
    return buttonValue_.test(static_cast<size_t>(button));
}

bool MouseState::mGetValue(const size_t button)const {
    return buttonValue_.test(button);
}

ButtonState MouseState::GetState(const MouseButton button)const {
    if (buttonValue_.test(static_cast<size_t>(button))) {
        if (oldButtonValue_.test(static_cast<size_t>(button))) {
            return ButtonState::kHold;
        }
        return ButtonState::kPress;
    }

    if (oldButtonValue_.test(static_cast<size_t>(button))) {
        return ButtonState::kRelease;
    }

    return ButtonState::kNone;
}

ButtonState MouseState::mGetState(const size_t button)const {
    InputSystem::keyStack.emplace_back(button);
    return ButtonState::kPress;
}

int MouseState::DeltaX()const {
    return x_;
}

int MouseState::DeltaY()const {
    return y_;
}

int MouseState::scrollWheelValue()const {
    return scrollWheelValue_;
}




GamePadState::GamePadState()
{}

bool GamePadState::kGetValue(const GamePadButton keycode)const {
    return buttonValue_.test(static_cast<size_t>(keycode));
}

bool GamePadState::kGetValue(const size_t keycode)const {
    return buttonValue_.test(keycode);
}

ButtonState GamePadState::GetState(const GamePadButton keycode)const {
    if (buttonValue_.test(static_cast<size_t>(keycode))) {
        if (oldButtonValue_.test(static_cast<size_t>(keycode))) {
            return ButtonState::kHold;
        }
        return ButtonState::kPress;
    }

    if (oldButtonValue_.test(static_cast<size_t>(keycode))) {
        return ButtonState::kRelease;
    }
    return ButtonState::kNone;
}

ButtonState GamePadState::kGetState(const size_t keycode)const {
    InputSystem::keyStack.emplace_back(keycode);
    return ButtonState::kPress;
}


void InputSystem::Initialize(const HWND hwnd) {
    keyboard_ = std::make_unique<KeyBoardState>();
    mouse_    = std::make_unique<MouseState>(hwnd);
    gamepad_  = std::make_unique<GamePadState>();
}

void InputSystem::UpdateStates() {
    {//ゲームパッド
        gamepad_->axisLx   = gamepad_->axisLy = 0.0f;
        gamepad_->axisRx   = gamepad_->axisRy = 0.0f;
        gamepad_->triggerL = gamepad_->triggerR = 0.0f;

        gamepad_->oldButtonValue_ = gamepad_->buttonValue_;
        gamepad_->buttonValue_    = 0;
        gamepad_->exist           = false;
        XINPUT_STATE xinputState;
        if (XInputGetState(gamepad_->slot, &xinputState) == ERROR_SUCCESS) {
            XINPUT_GAMEPAD& gp = xinputState.Gamepad;
            auto& v = gamepad_->buttonValue_;

            if (gp.wButtons & XINPUT_GAMEPAD_DPAD_UP)		    v.set(static_cast<size_t>(GamePadButton::ButtonUP));
            if (gp.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)	    v.set(static_cast<size_t>(GamePadButton::ButtonRIGHT));
            if (gp.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)		    v.set(static_cast<size_t>(GamePadButton::ButtonDOWN));
            if (gp.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)		    v.set(static_cast<size_t>(GamePadButton::ButtonLEFT));
            if (gp.wButtons & XINPUT_GAMEPAD_A)				    v.set(static_cast<size_t>(GamePadButton::ButtonA));
            if (gp.wButtons & XINPUT_GAMEPAD_B)				    v.set(static_cast<size_t>(GamePadButton::ButtonB));
            if (gp.wButtons & XINPUT_GAMEPAD_X)				    v.set(static_cast<size_t>(GamePadButton::ButtonX));
            if (gp.wButtons & XINPUT_GAMEPAD_Y)				    v.set(static_cast<size_t>(GamePadButton::ButtonY));
            if (gp.wButtons & XINPUT_GAMEPAD_START)			    v.set(static_cast<size_t>(GamePadButton::ButtonSTART));
            if (gp.wButtons & XINPUT_GAMEPAD_BACK)			    v.set(static_cast<size_t>(GamePadButton::ButtonBACK));
            if (gp.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)		v.set(static_cast<size_t>(GamePadButton::ButtonLEFT_THUMB));
            if (gp.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)       v.set(static_cast<size_t>(GamePadButton::ButtonRIGHT_THUMB));
            if (gp.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)     v.set(static_cast<size_t>(GamePadButton::ButtonLEFT_SHOULDER));
            if (gp.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)    v.set(static_cast<size_t>(GamePadButton::ButtonRIGHT_SHOULDER));

            if (gp.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)  v.set(static_cast<size_t>(GamePadButton::ButtonLEFT_TRIGGER));
            if (gp.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) v.set(static_cast<size_t>(GamePadButton::ButtonRIGHT_TRIGGER));

            if ((gp.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && gp.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
                (gp.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && gp.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
            {
                gp.sThumbLX = 0;
                gp.sThumbLY = 0;
            }

            if ((gp.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && gp.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
                (gp.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && gp.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
            {
                gp.sThumbRX = 0;
                gp.sThumbRY = 0;
            }

            gamepad_->triggerL = static_cast<float>(gp.bLeftTrigger) / 255.0f;
            gamepad_->triggerR = static_cast<float>(gp.bRightTrigger) / 255.0f;
            gamepad_->axisLx = static_cast<float>(gp.sThumbLX) / static_cast<float>(0x8000);
            gamepad_->axisLy = static_cast<float>(gp.sThumbLY) / static_cast<float>(0x8000);
            gamepad_->axisRx = static_cast<float>(gp.sThumbRX) / static_cast<float>(0x8000);
            gamepad_->axisRy = static_cast<float>(gp.sThumbRY) / static_cast<float>(0x8000);
            gamepad_->exist  = true;
        }


        {//キーボード
            keyboard_->oldKeyValue_ = keyboard_->keyValue_;
            keyboard_->keyValue_ = 0;
            if (keyboard_->keyboard_->IsConnected()) {
                const auto s = keyboard_->keyboard_->GetState();
                auto& v = keyboard_->keyValue_;

                if (s.NumPad0)   v.set(static_cast<size_t>(KeyCode::k0));
                if (s.NumPad1)   v.set(static_cast<size_t>(KeyCode::k1));
                if (s.NumPad2)   v.set(static_cast<size_t>(KeyCode::k2));
                if (s.NumPad3)   v.set(static_cast<size_t>(KeyCode::k3));
                if (s.NumPad4)   v.set(static_cast<size_t>(KeyCode::k4));
                if (s.NumPad5)   v.set(static_cast<size_t>(KeyCode::k5));
                if (s.NumPad6)   v.set(static_cast<size_t>(KeyCode::k6));
                if (s.NumPad7)   v.set(static_cast<size_t>(KeyCode::k7));
                if (s.NumPad8)   v.set(static_cast<size_t>(KeyCode::k8));
                if (s.NumPad9)   v.set(static_cast<size_t>(KeyCode::k9));
                if (s.Q)         v.set(static_cast<size_t>(KeyCode::kQ));
                if (s.W)         v.set(static_cast<size_t>(KeyCode::kW));
                if (s.E)         v.set(static_cast<size_t>(KeyCode::kE));
                if (s.R)         v.set(static_cast<size_t>(KeyCode::kR));
                if (s.T)         v.set(static_cast<size_t>(KeyCode::kT));
                if (s.Y)         v.set(static_cast<size_t>(KeyCode::kY));
                if (s.U)         v.set(static_cast<size_t>(KeyCode::kU));
                if (s.I)         v.set(static_cast<size_t>(KeyCode::kI));
                if (s.O)         v.set(static_cast<size_t>(KeyCode::kO));
                if (s.P)         v.set(static_cast<size_t>(KeyCode::kP));
                if (s.A)         v.set(static_cast<size_t>(KeyCode::kA));
                if (s.S)         v.set(static_cast<size_t>(KeyCode::kS));
                if (s.D)         v.set(static_cast<size_t>(KeyCode::kD));
                if (s.F)         v.set(static_cast<size_t>(KeyCode::kF));
                if (s.G)         v.set(static_cast<size_t>(KeyCode::kG));
                if (s.H)         v.set(static_cast<size_t>(KeyCode::kH));
                if (s.J)         v.set(static_cast<size_t>(KeyCode::kJ));
                if (s.K)         v.set(static_cast<size_t>(KeyCode::kK));
                if (s.L)         v.set(static_cast<size_t>(KeyCode::kL));
                if (s.Z)         v.set(static_cast<size_t>(KeyCode::kZ));
                if (s.X)         v.set(static_cast<size_t>(KeyCode::kX));
                if (s.C)         v.set(static_cast<size_t>(KeyCode::kC));
                if (s.V)         v.set(static_cast<size_t>(KeyCode::kV));
                if (s.B)         v.set(static_cast<size_t>(KeyCode::kB));
                if (s.N)         v.set(static_cast<size_t>(KeyCode::kN));
                if (s.M)         v.set(static_cast<size_t>(KeyCode::kM));

                if (s.Left)      v.set(static_cast<size_t>(KeyCode::kLeft));
                if (s.Up)        v.set(static_cast<size_t>(KeyCode::kUp));
                if (s.Right)     v.set(static_cast<size_t>(KeyCode::kRight));
                if (s.Down)      v.set(static_cast<size_t>(KeyCode::kDown));

                if (s.Escape)    v.set(static_cast<size_t>(KeyCode::kEsc));
                if (s.Space)     v.set(static_cast<size_t>(KeyCode::kSpace));
                if (s.Enter)     v.set(static_cast<size_t>(KeyCode::kEnter));
                if (s.LeftShift) v.set(static_cast<size_t>(KeyCode::kLShift));
            }
        }

        {//マウス
            mouse_->oldButtonValue_ = mouse_->buttonValue_;
            mouse_->buttonValue_ = 0;
            if (mouse_->mouse_->IsConnected()) {
                const auto s = mouse_->mouse_->GetState();
                auto& v = mouse_->buttonValue_;
                if (s.leftButton)    v.set(static_cast<size_t>(MouseButton::kLeft));
                if (s.middleButton)  v.set(static_cast<size_t>(MouseButton::kMiddle));
                if (s.rightButton)   v.set(static_cast<size_t>(MouseButton::kRight));
                mouse_->x_ = s.x;
                mouse_->y_ = s.y;
                mouse_->scrollWheelValue_ = s.scrollWheelValue;
            }
        }
    }

    for (auto& data : keyStack) data.frame++;

    keyStack.erase(
        remove_if(keyStack.begin(), keyStack.end(),
            [](ListData data) { return data.frame >= saveFrame; }),
        keyStack.end());
}
