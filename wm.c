#include "wm.h"
#include "wm_config.h"
#include <xcb/xcb.h>
#include <xcb/xproto.h>

static xcb_connection_t * c;
static xcb_screen_t * s;

static void spawn(char ** args) {
  if (fork() == 0) { 
    if (c)
      xcb_disconnect(c);

    setsid();
    execvp(args[0], args);
    
    _exit(0);
  }
}

static void die(char * err) {
  if (err) {
    fprintf(stderr, "%s\n", err);
  }
  exit(EXIT_FAILURE);
}

// // // // // // // // // // // // // // // // // // // // // // // // //

static void __event_MapRequest(xcb_map_request_event_t * ev) {
  xcb_map_window(c, ev->window);
  xcb_flush(c);
}


// // // // // // // // // // // // // // // // // // // // // // // // //

static void setup() {
  const uint32_t values[] = {
    XCB_EVENT_MASK_STRUCTURE_NOTIFY |
    XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
    XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT 
  };
  
  xcb_configure_window(
    c,
    s->root,
    XCB_CW_EVENT_MASK,
    &values
  );

  xcb_ungrab_key(c, XCB_GRAB_ANY, s->root, XCB_MOD_MASK_ANY); // clear

  int size = sizeof(keys) / sizeof(keys[0]);
  for (int i = 0; i < size; i++) {
    key cc = keys[i];
    xcb_grab_key(c, 0, s->root, cc.mod, cc.keysym, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
  }

  xcb_flush(c);
}

int main() {
  printf("mi.wm v0.1");

  int err = 0;
  c = xcb_connect(NULL, NULL);
  err = xcb_connection_has_error(c);

  if (err > 0) {
    die("xcb_connection_has_error");
  }

  s = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

  setup();

  while(!err) {
    xcb_generic_event_t *ev = xcb_wait_for_event(c);
		if (ev != NULL) {
      switch (ev->response_type) {
        case XCB_MAP_REQUEST:
          __event_MapRequest((xcb_map_request_event_t *)ev); 
          break;   
      }
      free(ev);
    }
    xcb_flush(c);
  }

  
  return 0;
}
