#define MOD1    XCB_MOD_MASK_4       // WIN or SUPER Key
#define MOD2    XCB_MOD_MASK_1       // ALT Key
#define SHIFT   XCB_MOD_MASK_SHIFT   // SHITFT Key
#define CONTROL XCB_MOD_MASK_CONTROL // CONTROL Key

static char *termcmd[] = { "st", NULL };
static key keys[] = {
  { MOD1,      XK_space, spawn,      termcmd },
};
