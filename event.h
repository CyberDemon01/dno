#ifndef _event_h
#define _event_h

enum class event_type {none, keyboard, mouse};
enum class mouse_event_type {position, wheel, button};

struct keyboard_event {
  int key = {};
  int action = {};
  keyboard_event() = default;
  keyboard_event(int _key, int _action);
};

struct mouse_position_event {
  double xpos = {};
  double ypos = {};
  mouse_position_event() = default;
  mouse_position_event(double _xpos, double _ypos);
};

struct mouse_wheel_event {
  double xoffset = {};
  double yoffset = {};
  mouse_wheel_event() = default;
  mouse_wheel_event(double _xoffset, double _yoffset);
};

struct mouse_button_event {
  int button = {};
  int action = {};
  mouse_button_event() = default;
  mouse_button_event(int _button, int _action);
};

struct mouse_event {
  mouse_event_type type = {};
  mouse_position_event position = {};
  mouse_button_event button = {};
  mouse_wheel_event wheel = {};
  mouse_event() = default;
  mouse_event(mouse_position_event _position);
  mouse_event(mouse_button_event _button);
  mouse_event(mouse_wheel_event _wheel);
};

struct event {
  event_type type = {event_type::none};
  keyboard_event keyboard = {};
  mouse_event mouse = {};
  event() = default;
  event(const keyboard_event& evt);
  event(const mouse_event& evt);
};

#endif
