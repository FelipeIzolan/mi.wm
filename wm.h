#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include <X11/keysym.h>

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_keysyms.h>

typedef struct {
  uint16_t mod;
  xcb_keysym_t keysym;
  void (*func)(char **args);
  char **args;
} key;

static void spawn(char ** args);
static void die(char * err);
/*static void killclient(char ** args);*/

#ifdef WALLPAPER
#include "bitmap.h"
static void wallpaper();
#endif

//  _______  _______  _        _______ _________ _______ 
// (  ____ \(  ___  )( (    /|(  ____ \\__   __/(  ____ \
// | (    \/| (   ) ||  \  ( || (    \/   ) (   | (    \/
// | |      | |   | ||   \ | || (__       | |   | |      
// | |      | |   | || (\ \) ||  __)      | |   | | ____ 
// | |      | |   | || | \   || (         | |   | | \_  )
// | (____/\| (___) || )  \  || )      ___) (___| (___) |
// (_______/(_______)|/    )_)|/       \_______/(_______)
                                                      
#define MOD1    XCB_MOD_MASK_4       // WIN or SUPER Key
#define MOD2    XCB_MOD_MASK_1       // ALT Key
#define SHIFT   XCB_MOD_MASK_SHIFT   // SHITFT Key
#define CONTROL XCB_MOD_MASK_CONTROL // CONTROL Key

// Enable the built-in wallpaper system by uncommenting line 3
// The wallpaper must be a BMP image
// #define WALLPAPER "~/wallpaper.bmp"

static char *termcmd[] = { "st", "nvim", NULL };
static key keys[] = {
  { MOD1,      XK_Return,   spawn,      termcmd },
/*  { MOD1,      XK_q,     killclient, NULL    }*/
};
