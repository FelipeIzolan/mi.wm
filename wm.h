#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>

#include <X11/keysym.h>

#include <xcb/xcb.h>
#include <xcb/xproto.h>

typedef struct {
  uint16_t mod;
  xcb_keysym_t keysym;
  void (*func)(char **args);
  char **args;
} key;


static void spawn(char ** args);
static void die(char * err);
