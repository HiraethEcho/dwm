/* See LICENSE file for copyright and license details. */
#include "exitdwm.c"
#include "movestack.c"
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx = 3; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */

static const unsigned int tabModKey = 0x40;
static const unsigned int tabCycleKey = 0x17;

static const unsigned int gappih = 2; /* horiz inner gap between windows */
static const unsigned int gappiv = 2; /* vert inner gap between windows */
static const unsigned int gappoh = 2; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov = 2; /* vert outer gap between windows and screen edge */
static int smartgaps = 1; /* 1 means no outer gap when there is only one window */

static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0; /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display
systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray = 1;             /* 0 means no systray */
static const int showbar = 1;                 /* 0 means no bar */
static const int topbar = 1;                  /* 0 means bottom bar */
// ⭘  󰣇 󱓞 󰣇

static const double activeopacity = 0.9f; /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 0.75f; /* Window opacity when it's inactive (0 <= opacity <= 1) */

static const int focusonwheel = 0;

static const char *fonts[] = {"Maple Mono NF:size=10"};
static const char dmenufont[] = "Maple Mono NF:size=15";

static const char col_gray1[] = "#3b4252";
static const char whiten[] = "#abb2bf";
static const char gray1[] = "#a9b1d6";
static const char gray2[] = "#24283b"; // unfocused window border
static const char gray3[] = "#414868";
static const char gray4[] = "#565f89";
static const char gray5[] = "#6d8dad";
static const char col_gray2[] = "#8fbcbb";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#88c0d0";
static const char col_gray5[] = "#D8DEE9";
static const char col_gray6[] = "#d8dee9";
static const char col_cyan[] = "#005577";
static const char blue[] = "#81A1C1";
static const char bblue[] = "#5e81ac";
static const char sky[] = "#61afef";
static const char red[] = "#d57780";
static const char yellow[] = "#FFFF00";
static const char deepred[] = "#9d2933";
static const char green[] = "#89b482";
static const char antigreen[] = "#764b7d";
static const char rose[] = "#BF616A";
static const char brown[] = "#c7b89d";
static const char ggreen[] = "#99CC32";
static const char grass[] = "#A3BE8C";
static const char black[] = "#2A303C";
static const char pink[] = "#c71585";
static const char white[] = "#f8f8f2";
static const char white2[] = "#d8dee9";

static const char *tagocc[][2] = {
  {"#D08770", gray3},
  {"#A3BE8C", gray3},
  {"#B48EAD", gray3},
  {"#BF616A", gray3},
  {"#EBCB8B", gray3},
};

static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeButton] = {white, sky, col_cyan},
    [SchemeTag]    = {white , black, col_cyan},
    [SchemeSym]    = {rose, green, col_cyan},
    [SchemeNorm]   = {col_gray5, gray3, grass},
    [SchemeSel]    = {black, blue, deepred},
    [SchemeHid]    = {blue, gray2, rose},
    [SchemeStatus] = {col_gray3, gray4, col_gray2},
    [SchemeSys]    = {col_gray3, gray3, col_gray2},
};

static const char buttonbar[] = "";
static const char *tags[] = {"󰋜", "󰗚", "", "󰃨", "󰃥"};

static const unsigned int ulinepad = 6; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke = 2; /* thickness / height of the underline */
static const unsigned int ulinevoffset = 2; /* how far above the bottom of the bar the line should appear */
// static const int ulineall    = 1;  /* 1 to show underline on all tags, 0 for
// just the active ones */

/* xprop(1):
 *  WM_CLASS(STRING) = instance, class
 *  WM_NAME(STRING) = title
 */
static const Rule rules[] = {
    /* class      instance    title       tags mask     isfloating focusopacity
       unfocusopacity     monitor */
    {"Gimp", NULL, NULL, 0, 1, 1.0, inactiveopacity, -1},
    // { "weixin"      , NULL , NULL           , 0 , 1 , 0.8 , inactiveopacity ,
    // -1 } , { "OneDriveGUI" , NULL , NULL           , 0 , 1 , 0.8 ,
    // inactiveopacity , -1 } , { "QQ"          , NULL , NULL           , 0 , 1
    // , 0.8 , inactiveopacity , -1 } ,
    {"st", NULL, "scratchpad", 0, 1, 0.8, 0.7, -1},
    {"st", NULL, "tasks", 0, 1, 0.8, 0.7, -1},
};
/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1
/* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"[M]", monocle},
    {"H[]", deck},
    {"TTT", bstack},
    {"HHH", grid},
    // {"---", horizgrid},
    {"|M|", centeredmaster},
    {"><>", NULL}, /* no layout function means floating behavior */
    {NULL, NULL},
};
/*
    {">M>", centeredfloatingmaster},
    {"[@]", spiral},
    {"[\\]", dwindle},
    {":::", gaplessgrid},
    {"###", nrowgrid},
    {"===", bstackhoriz},
*/
/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY, TAG) \
{MODKEY,                           KEY, view,       {.ui = 1 << TAG}}, \
{MODKEY | ControlMask,             KEY, toggleview, {.ui = 1 << TAG}}, \
{MODKEY | ShiftMask,               KEY, tag,        {.ui = 1 << TAG}}, \
{MODKEY | ControlMask | ShiftMask, KEY, toggletag,  {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL};
static const char *termcmd[] = {"alacritty", NULL};

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "150x40", NULL };

static const Key keys[] = {
    /* modifier                     key        function        argument */
    // scratchpads
    {MODKEY, XK_minus, scratchpad_show, {0}},
    {MODKEY | ShiftMask, XK_minus, scratchpad_hide, {0}},
    {MODKEY, XK_equal, scratchpad_remove, {0}},
    {Mod4Mask, XK_grave, togglescratch, {.v = scratchpadcmd}},

    // spawn and kill client
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_c, killclient, {0}},

    // focus and hide clients
    {MODKEY, XK_j, focusstackvis, {.i = +1}},
    {MODKEY, XK_k, focusstackvis, {.i = -1}},
    {MODKEY | ShiftMask, XK_j, focusstackhid, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, focusstackhid, {.i = -1}},
    {MODKEY, XK_m, togglehide, {0}},
    // {MODKEY                     , XK_s                     , show , {0}} ,
    {MODKEY | ShiftMask         , XK_s                     , showall , {0}} ,
    // {MODKEY                     , XK_h                     , hide , {0}} ,

    // choose clients
    {MODKEY, XK_o, winview, {0}},
    {Mod1Mask, XK_Tab, alttab, {0}},
    // move clients
    {Mod4Mask, XK_j, movestack, {.i = +1}},
    {Mod4Mask, XK_k, movestack, {.i = -1}},
    {MODKEY | ShiftMask, XK_Return, zoom, {0}},
    // opacity
    {MODKEY | ShiftMask, XK_a, changefocusopacity, {.f = +0.025}},
    {MODKEY | ShiftMask, XK_s, changefocusopacity, {.f = -0.025}},
    {MODKEY | ShiftMask, XK_z, changeunfocusopacity, {.f = +0.025}},
    {MODKEY | ShiftMask, XK_x, changeunfocusopacity, {.f = -0.025}},
    // move and resize float clients
    { MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
    { MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
    { MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
    { MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
    { MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
    { MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
    { MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
    { MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
    { Mod4Mask,           XK_Up,     moveresizeedge, {.v = "t"} },
    { Mod4Mask,           XK_Down,   moveresizeedge, {.v = "b"} },
    { Mod4Mask,           XK_Left,   moveresizeedge, {.v = "l"} },
    { Mod4Mask,           XK_Right,  moveresizeedge, {.v = "r"} },

    // change layouts
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    // { MODKEY|ShiftMask,          XK_comma,  cyclelayout,    {.i =
    // -1 } }, { MODKEY|ShiftMask,           XK_period, cyclelayout,    {.i = +1
    // } },
    // {MODKEY, XK_Tab, view, {0}},
    // {MODKEY, XK_a, setlayout, {.v = &layouts[2]}},
    // { MODKEY                 , XK_space                 , setlayout , {0} } ,

    // modify layouts
    {MODKEY | ShiftMask, XK_h, setcfact, {.f = +0.25}},
    {MODKEY | ShiftMask, XK_l, setcfact, {.f = -0.25}},
    {MODKEY | ShiftMask, XK_o, setcfact, {.f = 0.00}},

    {MODKEY | Mod4Mask, XK_u, incrgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_u, incrgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_i, incrigaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_i, incrigaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_o, incrogaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_o, incrogaps, {.i = -1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_0, defaultgaps, {0}},

    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_b, togglebar, {0}},

    // monitors
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},

    // tags
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3) TAGKEYS(XK_5, 4){MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}}, {MODKEY, XK_0, view, {.ui = ~0}},

    // quit
    {MODKEY | ShiftMask, XK_q, quit, {0}},
    {Mod4Mask | ShiftMask, XK_q, quit, {1}},
    {MODKEY | ShiftMask, XK_e, exitdwm, {0}},

    // Functions
    {0, XF86XK_AudioRaiseVolume, spawn, SHCMD("pamixer -i 5 ; pkill -SIGUSR1 dwmblocks ")},
    {0, XF86XK_AudioLowerVolume, spawn, SHCMD("pamixer -d 5; pkill -SIGUSR1 dwmblocks")},
    {0, XF86XK_AudioMute, spawn, SHCMD("pamixer -t; pkill -SIGUSR1 dwmblocks")},
    {0, XF86XK_MonBrightnessUp, spawn, SHCMD("xbacklight -inc 5; pkill -SIGUSR1 dwmblocks")},
    {0, XF86XK_MonBrightnessDown, spawn, SHCMD("xbacklight -dec 5; pkill -SIGUSR1 dwmblocks")},
    {0, XF86XK_WLAN, spawn, SHCMD("iwgtknm-connection-editornm-connection-editor")},
    {0, XF86XK_Tools, spawn, SHCMD("rofi_allapps")},
    {0, XK_F5, spawn, SHCMD("block_update")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkButton, 0, Button1, spawn, SHCMD("rofi_quickapps")},
    {ClkButton, 0, Button2, spawn, SHCMD("rofi_powermenu")},
    {ClkButton, 0, Button3, spawn, SHCMD("rofi_allapps")},

    // {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button2, setlayout, {.v = &layouts[2]}},
    {ClkLtSymbol, 0, Button1, cyclelayout, {.i = -1}},
    {ClkLtSymbol, 0, Button3, cyclelayout, {.i = +1}},

    {ClkWinTitle, 0, Button1, togglewin, {0}},
    {ClkWinTitle, 0, Button2, killclient, {0}},
    {ClkWinTitle, 0, Button3, zoom, {0}},
    {ClkWinTitle, 0, Button4, changefocusopacity, {.f = +0.025}},
    {ClkWinTitle, 0, Button5, changefocusopacity, {.f = -0.025}},
    {ClkWinTitle, MODKEY, Button4, changeunfocusopacity, {.f = +0.025}},
    {ClkWinTitle, MODKEY, Button5, changeunfocusopacity, {.f = -0.025}},

    {ClkStatusText, 0, Button1, sigstatusbar, {.i = 1}},
    {ClkStatusText, 0, Button2, sigstatusbar, {.i = 2}},
    {ClkStatusText, 0, Button3, sigstatusbar, {.i = 3}},
    {ClkStatusText, 0, Button4, sigstatusbar, {.i = 4}},
    {ClkStatusText, 0, Button5, sigstatusbar, {.i = 5}},
    {ClkStatusText, MODKEY, Button1, sigstatusbar, {.i = 6}},

    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},

    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
