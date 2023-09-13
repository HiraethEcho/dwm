/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "gaplessgrid.c"
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;   /* 0 means no systray */
static const int colorfultag        = 1;        /* 0 means use SchemeSel for selected non vacant tag */
static const double activeopacity   = 0.9f;     /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 0.7f;     /* Window opacity when it's inactive (0 <= opacity <= 1) */

static const char *fonts[]          = { "Maple Mono NF:size=10" };
static const char dmenufont[]       = "Maple Mono NF:size=15";

#include "themes/tokyonight.h"
static const char col_gray0[] = "#111111";
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_gray5[] = "#D8DEE9";
static const char col_cyan[]  = "#005577";
static const char col_red[]   = "#BF616A";
static const char col_green[] = "#89b482";

static const unsigned int baralpha = 0.7*0xffU;
static const unsigned int sysalpha = 0.9*0xffU;
static const unsigned int nonealpha= 0x01U;
static const unsigned int borderalpha = OPAQUE;

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	// [SchemeNorm] = { col_gray3, col_gray5, col_gray2 },
	// [SchemeSel]  = { col_gray4, col_cyan,  col_red  },
    [SchemeStatus]  = { red   , col_gray0, col_cyan },
    [SchemeNorm]    = { yellow, col_gray2    , gray2    },
    [SchemeSel]     = { col_green , blue     , red     },
    [SchemeNone]    = { gray4, gray4    , gray4    },
    [SchemeTagNorm] = { yellow, gray3    , black    },
    [SchemeTagSel]  = { blue  , black    , black    },
    [SchemeTag]     = { white , black    , black    },
    [SchemeTag1]    = { blue  , black    , black    },
    [SchemeTag2]    = { red   , black    , black    },
    [SchemeTag3]    = { orange, black    , black    },
    [SchemeTag4]    = { green , black    , black    },
    [SchemeTag5]    = { pink  , black    , black    },
    [SchemeHid]     = { red   , col_gray1, col_cyan },
    [SchemeSystray] = { col_gray1   , col_gray0, col_cyan },
    [SchemeSymbol]  = { col_gray1   , col_green, col_cyan },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
  [SchemeStatus]  = { OPAQUE, OPAQUE, borderalpha },
  [SchemeNorm]    = { OPAQUE, baralpha, borderalpha },
  [SchemeSel]     = { OPAQUE, OPAQUE, borderalpha },
  [SchemeNone]    = { nonealpha, nonealpha, nonealpha },
  [SchemeTagNorm] = { OPAQUE, baralpha, borderalpha },
  [SchemeTagSel]  = { OPAQUE, baralpha, borderalpha },
  [SchemeTag]     = { OPAQUE, baralpha, borderalpha },
  [SchemeTag1]    = { baralpha, baralpha, borderalpha },
  [SchemeTag2]    = { baralpha, baralpha, borderalpha },
  [SchemeTag3]    = { baralpha, baralpha, borderalpha },
  [SchemeTag4]    = { baralpha, baralpha, borderalpha },
  [SchemeTag5]    = { baralpha, baralpha, borderalpha },
  [SchemeHid]     = { OPAQUE, nonealpha, nonealpha },
  [SchemeSystray] = { OPAQUE, OPAQUE, OPAQUE },
  [SchemeSymbol]  = { OPAQUE, OPAQUE, OPAQUE },
};

/* tagging */
/*   󰈦 󰇩          󰨞   󰗚  󰓓   󰏊 󰖳 󰿎  󰮯   󰍖  󰊨 󱉺  󰉋  󰂤  󰇮 󰖟          󰑴  ☭ ☯ ⚛    󰏬 󰴓 󱓷 󱓧 */
static const char *tags[] = { "󰋜", "󰗚", "","","" };

static const int tagschemes[] = {
    SchemeTag1, SchemeTag2, SchemeTag3, SchemeTag4, SchemeTag5
};
static const unsigned int ulinepad      = 5; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke   = 2; /* thickness / height of the underline */
static const unsigned int ulinevoffset  = 0; /* how far above the bottom of the bar the line should appear */
static const int ulineall               = 0; /* 1 to show underline on all tags, 0 for just the active ones */

#include "shift-tools.c"
static const char scratchpadname[] = "scratchpad";
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   focusopacity    unfocusopacity     monitor */
	// { "Gimp",     NULL,       NULL,       0,            1,           1.0,            inactiveopacity,   -1 },
	{ "weixin"      , NULL , NULL           , 0 , 1 , 0.8 , inactiveopacity , -1 } ,
	{ "OneDriveGUI" , NULL , NULL           , 0 , 1 , 0.8 , inactiveopacity , -1 } ,
	{ "QQ"          , NULL , NULL           , 0 , 1 , 0.8 , inactiveopacity , -1 } ,
  { "st"          , NULL , scratchpadname , 0 , 1 , 0.7 , 0.3             , -1 } ,
};

/* layout(s) */
static const float mfact        = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
  { "[#]",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]      = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]       = { "alacritty", NULL };
static const char *rofidrun[]      = {"rofi","-show","drun"};
static const char *rofirun[]       = {"rofi","-show","run"};
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "150x40", NULL };

static const char *edge[]      = { "microsoft-edge-stable", NULL };
static const char *pavu[]      = { "pavucontrol", NULL };
static const char *upvol[]     = { "pamixer", "-i",  "5",     NULL };
static const char *downvol[]   = { "pamixer", "-d", "5",     NULL };
static const char *mutevol[]   = { "pamixer", "-t",  NULL };
static const char *uplight[]   = {"xbacklight", "-inc", "5", NULL};
static const char *downlight[] = {"xbacklight", "-dec", "5", NULL};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,           XK_p,      spawn,          {.v = dmenucmd } },
  { MODKEY,           XK_space,  spawn,          {.v = rofidrun } },
	{ MODKEY,           XK_Return, spawn,          {.v = termcmd } },
  { Mod4Mask,         XK_grave,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,           XK_b,      togglebar,      {0} },
	{ MODKEY,           XK_j,      focusstackvis,  {.i = +1 } },
	{ MODKEY,           XK_k,      focusstackvis,  {.i = -1 } },
	{ MODKEY|ShiftMask, XK_j,      focusstackhid,  {.i = +1 } },
	{ MODKEY|ShiftMask, XK_k,      focusstackhid,  {.i = -1 } },
	{ MODKEY,           XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,           XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,           XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,           XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask, XK_Return, zoom,           {0} },
	{ MODKEY,           XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask, XK_c,      killclient,     {0} },
	{ MODKEY,           XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,           XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask, XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,           XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,           XK_f,      fullscreen,     {0} },
	{ MODKEY|ShiftMask, XK_space,  togglefloating, {0} },
	{ MODKEY,           XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask, XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,           XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,           XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask, XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask, XK_period, tagmon,         {.i = +1 } },


	{ MODKEY,           XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,           XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask, XK_equal,  setgaps,        {.i = 0  } },
  { MODKEY|ShiftMask, XK_s,      changefocusopacity,   {.f = +0.025}},
  { MODKEY|ShiftMask, XK_a,      changefocusopacity,   {.f = -0.025}},
  { MODKEY|ShiftMask, XK_x,      changeunfocusopacity, {.f = +0.025}},
  { MODKEY|ShiftMask, XK_z,      changeunfocusopacity, {.f = -0.025}},
	// { MODKEY,           XK_s,      showall,           {0} },
	{ MODKEY|ShiftMask, XK_q,      show,        {0} },
	{ MODKEY,           XK_q,      hide,           {0} },

  { Mod4Mask,             XK_e,      spawn,  SHCMD("st ranger") },
  { Mod4Mask,             XK_space,  spawn,  SHCMD("xinput enable 8") },
  { Mod4Mask|ShiftMask,   XK_space,  spawn,  SHCMD("xinput disable 8") },
	{ Mod4Mask|ControlMask, XK_Right,	shiftview,      { .i = +1 } },
	{ Mod4Mask|ControlMask, XK_Left,	shiftview,      { .i = -1 } },
	{ Mod4Mask|ShiftMask,   XK_Right, shiftboth,      { .i = +1 }	},
	{ Mod4Mask|ShiftMask,	  XK_Left,  shiftboth,      { .i = -1 }	},
	// { MODKEY|ControlMask,		XK_w,      shiftswaptags,  { .i = -1 }	},
	// { MODKEY|ControlMask,		XK_e,      shiftswaptags,  { .i = +1 }	},


	{0,                 XF86XK_AudioRaiseVolume, spawn, {.v = upvol}},
  {0,                 XF86XK_AudioLowerVolume, spawn, {.v = downvol}},
	{0,                 XF86XK_AudioMute,        spawn, {.v = mutevol }},
	{0,                 XF86XK_MonBrightnessUp,  spawn, {.v = uplight}},
	{0,                 XF86XK_MonBrightnessDown,spawn, {.v = downlight}},

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

  { MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, 
	{ MODKEY|ControlMask,           XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button1,        togglewin,          {0} },
  { ClkWinTitle,          0,              Button2,        killclient,         {0} },
  { ClkWinTitle,          0,              Button3,        zoom,               {0} },
  { ClkWinTitle,          0,              Button4,        changefocusopacity, {.f = +0.025} },
  { ClkWinTitle,          0,              Button5,        changefocusopacity, {.f = -0.025} },
  { ClkWinTitle,          MODKEY,         Button4,        changeunfocusopacity, {.f = +0.025} },
  { ClkWinTitle,          MODKEY,         Button5,        changeunfocusopacity, {.f = -0.025} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = termcmd } },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = mutevol } },
	{ ClkStatusText,        MODKEY,         Button2,        spawn,          {.v = pavu }},
	{ ClkStatusText,        0,              Button3,        spawn,          {.v = edge } },
	{ ClkStatusText,        0,              Button4,        spawn,          {.v = upvol } },
	{ ClkStatusText,        0,              Button5,        spawn,          {.v = downvol } },
	{ ClkStatusText,        MODKEY,         Button4,        spawn,          {.v = uplight } },
	{ ClkStatusText,        MODKEY,         Button5,        spawn,          {.v = downlight } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },

	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

