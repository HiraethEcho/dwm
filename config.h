/* See LICENSE file for copyright and license details. */
#include "exitdwm.c"
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */

static const unsigned int gappih = 3; /* horiz inner gap between windows */
static const unsigned int gappiv = 3; /* vert inner gap between windows */
static const unsigned int gappoh = 3; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov = 3; /* vert outer gap between windows and screen edge */
static int smartgaps = 1; /* 1 means no outer gap when there is only one window */

static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor
X */
static const unsigned int systrayonleft = 0; /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False:
display systray on the last monitor*/
static const int showsystray = 1; /* 0 means no systray */
static const int showbar = 1;     /* 0 means no bar */
static const int topbar = 1;      /* 0 means bottom bar */
// ⭘  󰣇 󱓞
static const char buttonbar[]       = "󰣇";

static const double activeopacity = 0.9f; /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 0.7f; /* Window opacity when it's inactive (0 <= opacity <= 1) */

static const int focusonwheel = 0;

static const char *fonts[] = {"Maple Mono NF:size=10"};
static const char dmenufont[] = "Maple Mono NF:size=15";

static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_gray5[] = "#D8DEE9";
static const char col_cyan[] = "#005577";
static const char col_red[] = "#BF616A";
static const char col_green[] = "#89b482";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel] = {col_gray4, col_cyan, col_cyan},
    [SchemeHid] = {col_cyan, col_gray1, col_cyan},
};

/* tagging */
// static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
/* 󰃨   󰈦 󰇩          󰨞   󰗚
 *  󰓓   󰏊 󰖳 󰨇 󰿎  󰮯   󰍖  󰊨 󱉺 
 * 󰉋  󰂤  󰇮 󰖟  󰨇         󰑴 
 * ☭ ☯ ⚛    󰏬 󰴓 󱓷 󱓧 */
static const char *tags[] = {"󰋜", "󰗚", "", "󰃨", "󰃥"};

static const char *tagsel[][2] = {
    {"#ffffff", "#ff0000"}, {"#ffffff", "#ff7f00"}, {"#000000", "#ffff00"},
    {"#000000", "#00ff00"}, {"#ffffff", "#0000ff"}, {"#ffffff", "#4b0082"},
    {"#ffffff", "#9400d3"}, {"#000000", "#ffffff"}, {"#ffffff", "#000000"},
};

static const unsigned int ulinepad = 6;/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke = 2; /* thickness / height of the underline */
static const unsigned int ulinevoffset = 2; /* how far above the bottom of the bar the line should appear */
// static const int ulineall    = 1;  /* 1 to show underline on all tags, 0 for just the active ones */

	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   focusopacity    unfocusopacity     monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           1.0,            inactiveopacity,   -1 },
	// { "weixin"      , NULL , NULL           , 0 , 1 , 0.8 , inactiveopacity , -1 } ,
	// { "OneDriveGUI" , NULL , NULL           , 0 , 1 , 0.8 , inactiveopacity , -1 } ,
	// { "QQ"          , NULL , NULL           , 0 , 1 , 0.8 , inactiveopacity , -1 } ,
  { "st"          , NULL , "scratchpad" , 0 , 1 , 0.7 , 0.3             , -1 } ,
};
/* layout(s) */
static const float mfact = 0.55;                 /* factor of master area size [0.05..0.95] */
static const int nmaster = 1; /* number of clients in master area */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT  1 
    /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
{"[]="   , tile}                    , /* first entry is default */
{"[M]"   , monocle}                 ,
{ "[@]"  , spiral }                 ,
{ "[\\]" , dwindle }                ,
{ "H[]"  , deck }                   ,
{ "TTT"  , bstack }                 ,
{ "==="  , bstackhoriz }            ,
{ "HHH"  , grid }                   ,
{ "###"  , nrowgrid }               ,
{ "---"  , horizgrid }              ,
{ ":::"  , gaplessgrid }            ,
{ "|M|"  , centeredmaster }         ,
{ ">M>"  , centeredfloatingmaster } ,
{ "><>"  , NULL }                   , /* no layout function means floating behavior */
{ NULL   , NULL }                   ,
};

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
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m",      dmenumon, "-fn",    dmenufont, "-nb",     col_gray1, "-nf",       col_gray3, "-sb",    col_cyan, "-sf",     col_gray4, NULL};
static const char *termcmd[] = {"alacritty", NULL};

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "150x40", NULL };

static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_grave, togglescratch, {.v = scratchpadcmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstackvis, {.i = +1}},
    {MODKEY, XK_k, focusstackvis, {.i = -1}},
    {MODKEY | ShiftMask, XK_j, focusstackhid, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, focusstackhid, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
	{ MODKEY|Mod4Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY|ShiftMask, XK_Return, zoom,           {0} },
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_c, killclient, {0}},
    {MODKEY, XK_s, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_a, setlayout, {.v = &layouts[2]}},
    // { MODKEY                 , XK_space                 , setlayout , {0} } ,
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    // {MODKEY                     , XK_s                     , show , {0}} ,
    // {MODKEY | ShiftMask         , XK_s                     , showall , {0}} ,
    // {MODKEY                     , XK_h                     , hide , {0}} ,
	{ MODKEY,           XK_m,      togglehide,           {0} },
    TAGKEYS(XK_1 , 0)
    TAGKEYS(XK_2 , 1)
    TAGKEYS(XK_3 , 2)
    TAGKEYS(XK_4 , 3)
    TAGKEYS(XK_5 , 4)
    {MODKEY | ShiftMask, XK_q, quit, {0}},
    {Mod4Mask | ShiftMask, XK_q, quit, {1}},
    {MODKEY | ShiftMask, XK_e, exitdwm, {0}},

    {MODKEY, XK_minus, scratchpad_show, {0}},
    {MODKEY | ShiftMask, XK_minus, scratchpad_hide, {0}},
    {MODKEY, XK_equal, scratchpad_remove, {0}},
    {MODKEY | ShiftMask, XK_a, changefocusopacity, {.f = +0.025}},
    {MODKEY | ShiftMask, XK_s, changefocusopacity, {.f = -0.025}},
    {MODKEY | ShiftMask, XK_z, changeunfocusopacity, {.f = +0.025}},
    {MODKEY | ShiftMask, XK_x, changeunfocusopacity, {.f = -0.025}},
    {MODKEY | Mod4Mask, XK_u, incrgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_u, incrgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_i, incrigaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_i, incrigaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_o, incrogaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_o, incrogaps, {.i = -1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_0, defaultgaps, {0}},

    {0, XF86XK_AudioRaiseVolume, spawn, SHCMD("pamixer -i 5")},
    {0, XF86XK_AudioLowerVolume, spawn, SHCMD("pamixer -d 5")},
    {0, XF86XK_AudioMute, spawn, SHCMD("pamixer -t")},
    {0, XF86XK_MonBrightnessUp, spawn, SHCMD("xbacklight -inc 5")},
    {0, XF86XK_MonBrightnessDown, spawn, SHCMD("xbacklight -dec 5")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
	{ ClkButton,        0,              Button1,        spawn,          SHCMD("rofi_quickapps") },
	{ ClkButton,        0,              Button2,        spawn,          SHCMD("rofi_powermenu")},
	{ ClkButton,        0,              Button3,        spawn,          SHCMD("rofi_allapps") },

    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},

  { ClkWinTitle,          0,              Button1,        togglewin,          {0} },
  { ClkWinTitle,          0,              Button2,        killclient,         {0} },
  { ClkWinTitle,          0,              Button3,        zoom,               {0} },
  { ClkWinTitle,          0,              Button4,        changefocusopacity, {.f = +0.025} },
  { ClkWinTitle,          0,              Button5,        changefocusopacity, {.f = -0.025} },
  { ClkWinTitle,          MODKEY,         Button4,        changeunfocusopacity, {.f = +0.025} },
  { ClkWinTitle,          MODKEY,         Button5,        changeunfocusopacity, {.f = -0.025} },

    {ClkStatusText, 0, Button1, sigstatusbar, {.i = 1}},
    {ClkStatusText, 0, Button2, sigstatusbar, {.i = 2}},
    {ClkStatusText, 0, Button3, sigstatusbar, {.i = 3}},
    {ClkStatusText, 0, Button4, sigstatusbar, {.i = 4}},
    {ClkStatusText, 0, Button5, sigstatusbar, {.i = 5}},

    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},

    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
