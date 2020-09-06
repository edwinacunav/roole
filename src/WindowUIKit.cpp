#include "Platform.hpp"
#if defined(ROOLE_IS_IPHONE)

#include "RooleViewController.hpp"
#include "Roole.hpp"

using namespace std;

struct Roole::Window::Impl
{
    UIWindow* window;
    RooleViewController* controller;
    unique_ptr<Graphics> graphics;
    unique_ptr<Input> input;
    double update_interval;
    string caption;
};

Roole::Window::Window(unsigned width, unsigned height, bool fullscreen, double update_interval,
                     bool resizable)
: pimpl(new Impl)
{
    pimpl->window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    pimpl->controller = [RooleViewController new];
    pimpl->controller.gosuWindow = this;
    pimpl->window.rootViewController = pimpl->controller;
    
    // It is important to load the view before creating the Graphics instance.
    [pimpl->controller loadView];
    
    pimpl->graphics.reset(new Graphics(screen_width(), screen_height()));
    pimpl->graphics->set_resolution(width, height);
    
    pimpl->input.reset(new Input((__bridge void*) pimpl->controller.view, update_interval));
    pimpl->input->set_mouse_factors(1.0 * width / available_width(), 1.0 * height / available_height());
    
    pimpl->input->on_touch_began = [this](Roole::Touch touch) { touch_began(touch); };
    pimpl->input->on_touch_moved = [this](Roole::Touch touch) { touch_moved(touch); };
    pimpl->input->on_touch_ended = [this](Roole::Touch touch) { touch_ended(touch); };
    pimpl->input->on_touch_cancelled = [this](Roole::Touch touch) { touch_cancelled(touch); };
    
    // Now let the controller know about our Input instance.
    [pimpl->controller trackTextInput:*pimpl->input];
    
    pimpl->update_interval = update_interval;
}

Roole::Window::~Window()
{
}

unsigned Roole::Window::width() const
{
    return graphics().width();
}

unsigned Roole::Window::height() const
{
    return graphics().height();
}

bool Roole::Window::fullscreen() const
{
    return true;
}

bool Roole::Window::resizable() const
{
    return false;
}

void Roole::Window::resize(unsigned width, unsigned height, bool fullscreen)
{
    throw logic_error("Cannot resize windows on iOS");
}

double Roole::Window::update_interval() const
{
    return pimpl->update_interval;
}

void Roole::Window::set_update_interval(double update_interval)
{
    throw logic_error("Cannot change the update interval on iOS");
}

string Roole::Window::caption() const
{
    return pimpl->caption;
}

void Roole::Window::set_caption(const string& caption)
{
    pimpl->caption = caption;
}

const Roole::Graphics& Roole::Window::graphics() const
{
    return *pimpl->graphics;
}

Roole::Graphics& Roole::Window::graphics()
{
    return *pimpl->graphics;
}

const Roole::Input& Roole::Window::input() const
{
    return *pimpl->input;
}

Roole::Input& Roole::Window::input()
{
    return *pimpl->input;
}

void Roole::Window::show()
{
}

bool Roole::Window::tick()
{
    return true;
}

void Roole::Window::close()
{
    throw logic_error("Cannot close windows manually on iOS");
}

void Roole::Window::button_down(Button button)
{
}

void* Roole::Window::uikit_window() const
{
    return (__bridge void*) pimpl->window;
}

unsigned Roole::screen_width(Window*)
{
    return available_width() * [UIScreen mainScreen].scale;
}

unsigned Roole::screen_height(Window*)
{
    return available_height() * [UIScreen mainScreen].scale;
}

unsigned Roole::available_width(Window*)
{
    static CGSize screen_size = [UIScreen mainScreen].bounds.size;
    static CGFloat width = MAX(screen_size.width, screen_size.height);
    return width;
}

unsigned Roole::available_height(Window*)
{
    static CGSize screen_size = [UIScreen mainScreen].bounds.size;
    static CGFloat height = MIN(screen_size.width, screen_size.height);
    return height;
}

#endif
