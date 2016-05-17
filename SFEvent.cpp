#include "SFEvent.h"


/**
 * Effectively wraps an SDL_Event in our custom event type.
 */
SFEvent::SFEvent(const SDL_Event & event) {
  switch (event.type) {
  case SDL_QUIT:
    code = SFEVENT_QUIT;
    break;
  case SDL_USEREVENT:
    code = SFEVENT_UPDATE;
    break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_a:
      code = SFEVENT_PLAYER_LEFT;
      break;
    case SDLK_d:
      code = SFEVENT_PLAYER_RIGHT;
      break;
    case SDLK_w:
      code = SFEVENT_PLAYER_UP;
      break;
    case SDLK_s:
      code = SFEVENT_PLAYER_DOWN;
      break;
    case SDLK_SPACE:
      code = SFEVENT_FIRE;
      break;
    case SDLK_q:
      code = SFEVENT_QUIT;
      break;
    }
    break;
  default:
    code = SFEVENT_NULL;
    break;
  }
}


SFEVENT SFEvent::GetCode() {
  return code;
}
