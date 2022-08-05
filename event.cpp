#include "event.h"

keyboard_event::keyboard_event(int _key, int _action) :
  key(_key), action(_action) {}

mouse_position_event::mouse_position_event(double _xpos, double _ypos) :
  xpos(_xpos), ypos(_ypos) {}

mouse_wheel_event::mouse_wheel_event(double _xoffset, double _yoffset) :
  xoffset(_xoffset), yoffset(_yoffset) {}

mouse_button_event::mouse_button_event(int _button, int _action) :
  button(_button), action(_action) {}

mouse_event::mouse_event(mouse_position_event _position) :
  type(mouse_event_type::position), position(_position) {}

mouse_event::mouse_event(mouse_button_event _button) :
  type(mouse_event_type::button), button(_button) {}

mouse_event::mouse_event(mouse_wheel_event _wheel) :
  type(mouse_event_type::wheel), wheel(_wheel) {}

event::event(const keyboard_event& evt) :
  type(event_type::keyboard), keyboard(evt) {}

event::event(const mouse_event& evt) :
  type(event_type::mouse), mouse(evt) {}
