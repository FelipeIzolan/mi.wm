#include "wm.h"
#include <xcb/xcb_keysyms.h>
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

/*static void killclient(char **args) {*/
/*  xcb_kill_client(c, NULL);*/
/*}*/

static void die(char * err) {
  if (err) {
    fprintf(stderr, "%s\n", err);
  }
  exit(EXIT_FAILURE);
}

// // // // // // // // // // // // // // // // // // // // // // // // //

#ifdef WALLPAPER
static void wallpaper() {
  xcb_pixmap_t w;
  xcb_create_pixmap(c, s->root_depth, 321, s->root, 128, 128);
}
#endif

// // // // // // // // // // // // // // // // // // // // // // // // //

static void __event_MapRequest(xcb_map_request_event_t * ev) {
  const uint32_t values[] = {
    (s->width_in_pixels - 800) / 2,
    (s->height_in_pixels - 600) / 2,
    800,
    600
  };
  xcb_map_window(c, ev->window);
  xcb_configure_window(
    c,
    ev->window,
    XCB_CONFIG_WINDOW_X |
    XCB_CONFIG_WINDOW_Y |
    XCB_CONFIG_WINDOW_WIDTH |
    XCB_CONFIG_WINDOW_HEIGHT,
    &values
  );
}

// // // // // // // // // // // // // // // // // // // // // // // // //

static void setup() {  
  const uint32_t values[] = {
/*    XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |*/
    XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
  };
  
  xcb_change_window_attributes(
    c,
    s->root,
    XCB_CW_EVENT_MASK,
    &values
  );

  xcb_ungrab_key(c, XCB_GRAB_ANY, s->root, XCB_MOD_MASK_ANY); // clear

  int size = sizeof(keys) / sizeof(keys[0]);
  xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(c);
  
  for (int i = 0; i < size; i++) {
    key cc = keys[i];
    xcb_keycode_t *keycode = xcb_key_symbols_get_keycode(keysyms, cc.keysym);
    xcb_grab_key(
      c, 
      0,
      s->root,
      cc.mod,
      *keycode,
      XCB_GRAB_MODE_ASYNC,
      XCB_GRAB_MODE_ASYNC
    );
  }

  xcb_flush(c);
  xcb_key_symbols_free(keysyms);
}

int main(int argc, char * argv[]) {
  if (argc == 2 && strcmp("-v", argv[1]) == 0)
    die("mi.wm: v0.2");

  if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        die("mi.wm: signal");
  }

  int err = 0;
  c = xcb_connect(NULL, NULL);
  err = xcb_connection_has_error(c);

  if (err > 0) {
    die("mi.wm: xcb_connection_has_error");
  }

  s = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

  setup();

  while(!err) {
    xcb_generic_event_t *ev = xcb_wait_for_event(c);
		if (ev != NULL) {
      switch (ev->response_type & ~0x80) {
        case XCB_MAP_REQUEST:
          __event_MapRequest((xcb_map_request_event_t *)ev); 
          break;
        case XCB_KEY_PRESS:
          break;
      }
      free(ev);
    }
    xcb_flush(c);
    err = xcb_connection_has_error(c);
  }

  return 0;
}
