#ifndef HANDLERS_H
#define HANDLERS_H

class Keyboard;
class Button;
class Slider;

class ButtonHandler 
{
public:
    virtual void OnButtonPressed(Button *pButton) = 0;
};

class KeyboardHandler
{
public:
    virtual void OnKeyboardNotePlayed(Keyboard *pKeyboard, const float &note) = 0;
};

class SliderHandler
{
public:
    virtual void OnSliderReleased(Slider *pSlider) = 0;
};

#endif