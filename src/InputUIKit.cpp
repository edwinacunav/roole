#include "Platform.hpp"
#if defined(ROOLE_IS_IPHONE)

#import "Input.hpp"
#import "TextInput.hpp"
#import <functional>
#import <UIKit/UIKit.h>
using namespace std;

struct Roole::Input::Impl
{
  UIView* view = nil;
  TextInput* text_input = nullptr;
  float mouse_x, mouse_y;
  float scale_x, scale_y;
  float update_interval;
  NSMutableSet* current_touches_set;
  unique_ptr<Roole::Touches> current_touches_vector;
  
  Touch translate_touch(UITouch* ui_touch)
  {
    CGPoint point = [ui_touch locationInView:view];
    return (Touch) {
      .id = (__bridge void*)ui_touch,
      .x  = (float)point.x * scale_x,
      .y  = (float)point.y * scale_y,
    };
  }
};

Roole::Input::Input(void* view, float update_interval)
: pimpl(new Impl)
{
  pimpl->view = (__bridge UIView*) view;
  pimpl->update_interval = update_interval;
  pimpl->current_touches_set = [NSMutableSet new];
  pimpl->mouse_x = pimpl->mouse_y = -1000;
  set_mouse_factors(1, 1);
}

Roole::Input::~Input()
{
}

void Roole::Input::feed_touch_event(function<void (Touch)>& callback, void* touches)
{
    NSSet* ui_touches = (__bridge NSSet*) touches;
    
    pimpl->current_touches_vector.reset();
    
    if (&callback == &on_touch_began) {
        [pimpl->current_touches_set unionSet:ui_touches];
    }
    else if (&callback == &on_touch_ended || &callback == &on_touch_cancelled) {
        [pimpl->current_touches_set minusSet:ui_touches];
    }
    
    if (callback) {
        for (UITouch* ui_touch in ui_touches) {
            callback(pimpl->translate_touch(ui_touch));
        }
    }
}

string Roole::Input::id_to_char(Button btn)
{
    return string();
}

Roole::Button Roole::Input::char_to_id(string ch)
{
    return NO_BUTTON;
}

bool Roole::Input::down(Button btn)
{
    return false;
}

double Roole::Input::mouse_x() const
{
    return pimpl->mouse_x;
}

double Roole::Input::mouse_y() const
{
    return pimpl->mouse_y;
}

void Roole::Input::set_mouse_position(double x, double y)
{
    pimpl->mouse_x = x;
    pimpl->mouse_y = y;
}

void Roole::Input::set_mouse_factors(double scale_x, double scale_y, double offset_x,
    double offset_y)
{
    pimpl->scale_x = scale_x;
    pimpl->scale_y = scale_y;
    
    // TODO - use offset
}

const Roole::Touches& Roole::Input::current_touches() const
{
    if (!pimpl->current_touches_vector.get()) {
        pimpl->current_touches_vector.reset(new Roole::Touches);
        for (UITouch* ui_touch in pimpl->current_touches_set) {
            pimpl->current_touches_vector->push_back(pimpl->translate_touch(ui_touch));
        }
    }
    return *pimpl->current_touches_vector;
}

double Roole::Input::accelerometer_x() const
{
    return 0;
}

double Roole::Input::accelerometer_y() const
{
    return 0;
}

double Roole::Input::accelerometer_z() const
{
    return 0;
}

void Roole::Input::update()
{
    // Check for dead touches and remove from vector if necessary.
    NSMutableSet* dead_touches = nil;

    for (UITouch* touch in pimpl->current_touches_set) {
        UITouchPhase phase = [touch phase];
        if (phase == UITouchPhaseBegan ||
                phase == UITouchPhaseMoved ||
                phase == UITouchPhaseStationary) {
            continue;
        }
        
        // Something was deleted, we will need the set.
        if (!dead_touches) dead_touches = [NSMutableSet new];
        [dead_touches addObject:touch];
    }
    
    // Has something been deleted?
    if (dead_touches) {
        pimpl->current_touches_vector.reset();
        [pimpl->current_touches_set minusSet:dead_touches];
        
        if (on_touch_ended) {
            for (UITouch* ui_touch in dead_touches) {
                on_touch_ended(pimpl->translate_touch(ui_touch));
            }
        }
    }
}

Roole::TextInput* Roole::Input::text_input() const
{
    return pimpl->text_input;
}

void Roole::Input::set_text_input(TextInput* text_input)
{
    if (text_input) {
        pimpl->text_input = text_input;
        [pimpl->view becomeFirstResponder];
    }
    else {
        [pimpl->view resignFirstResponder];
        pimpl->text_input = nullptr;
    }
}

#endif
