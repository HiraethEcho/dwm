/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "exitdwm.c"
/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int gappx     = 1;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int extrabar           = 1;        /* 0 means no extra bar */
static const char statussep         = ']';      /* separator between statuses */
static const double activeopacity   = 0.95f;     /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 0.7f;     /* Window opacity when it's inactive (0 <= opacity <= 1) */

/* static const char *fonts[]        = { "monospace:size                = 10" }; */
static const char *fonts[]           = { "Code New Roman Nerd Font:size = 12" };
/* static const char dmenufont[]     = "monospace:size                  = 10"; */
static const char dmenufont[]        = "Code New Roman Nerd Font:size   = 12";

static const char col1[]     = "#222222";
static const char col2[]     = "#444444";
static const char col3[]     = "#bbbbbb";
static const char col4[]     = "#eeeeee";
static const char col5[]     = "#9370db";
static const char col6[]     = "#018274";
static const char col7[]     = "#89cff0";
static const char nord1[]    = "#D8DEE9";
static const char nord2[]    = "#5E81AC";
static const char nord3[]    = "#B48EAD";
static const char nord4[]    = "#434C5E";
static const char nord5[]    = "#A3BE8C";
static const char nord6[]    = "#2E3440";
static const char nord8[]    = "#D8DEE9";
static const char nord9[]    = "#E5E9F0";
static const char nord10[]   = "#3B4252";
static const char nord11[]   = "#81A1C1";
static const char nord12[]   = "#BF616A";
static const char nord13[]   = "#EBCB8B";
static const char nord14[]   = "#D08770";
static const char nord15[]   = "#B48EAD";

enum {
  Scheme0b,
  Scheme0c,
  Scheme0d,
  Scheme0e,
  Scheme0f,
  Scheme10,
  Scheme11,
  Scheme12,

  SchemeSel,
  SchemeNorm, 
  SchemeTagNorm,
  SchemeTagSel,
  SchemeHid,
  SchemeSymbol,
  SchemeStatus,
  SchemeEl,
  SchemeEm,
  SchemeEr,
}; /* color schemes */

static const char *colors[][3]      = {
  /*               fg         bg         border   */
  [SchemeTagNorm] = { nord14         , col4     , col2    } ,
  [SchemeTagSel]  = { col1          , nord11    , col1  } ,

  [SchemeHid]     = { nord1      , col1 , col4     } ,

  [SchemeSymbol]  = { nord10       , nord13     , col2    } ,
  
  [SchemeStatus]  = { col2     , nord10    , col2    } ,

  [SchemeNorm]    = { nord12 , nord1     , col3 } ,
  [SchemeSel]     = { nord10 , nord11    , nord12  } ,

  [Scheme0b]      = { nord5        , nord1      , col2    } ,
  [Scheme0c]      = { nord12        , nord5    , col2    } ,

  [Scheme0d]      = { nord14        , nord5     , col2    } ,
  [Scheme0e]      = { col4          , nord14      , col2    } ,

  [Scheme0f]      = { nord11         , nord14     , col2    } ,
  [Scheme10]      = { nord4         , nord11     , col2    } ,

  [Scheme11]      = { nord8        , nord11     , col2    } ,
  [Scheme12]      = { col1     , nord8     , col2    } ,

  [SchemeEl]      = { nord12        , nord10    , col2    } ,
  [SchemeEm]      = { nord12        , nord8     , col2    } ,
  [SchemeEr]      = { nord6         , nord3     , col2    } ,
};

/* tagging */
/*   󰈦 󰇩               ﬐    */
static const char *tags[] = { "","","","󰃨","" };
/* static const char *tags[] = { "", "", "", "", "", "", "", "﬏", "", "" }; */

static const char scratchpadname[] = "scratchpad";
static const Rule rules[] = {
  /* xprop(1):
   *	WM_CLASS(STRING) = instance, class
   *	WM_NAME(STRING) = title
   */
  /* class   instance    title       tags mask  isfloating focusopacity    unfocusopacity     monitor */
  { "st"       , NULL , scratchpadname , 1 << 8 , 1 ,       0.7 ,          0.3             , -1 } ,
  /* { "st"       , NULL , "ranger"       , 1 << 8 , 1 ,       0.9 ,          inactiveopacity , -1 } , */
  /* { "weixin"   , NULL , NULL           , 1 << 8 , 1 , 0.9 , 0.7             , -1 } , */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
  /* symbol arrange function */
  { "[]=" , tile    } , /* first entry is default */
  { "><>" , NULL    } , /* no layout function means floating behavior */
  { "[M]" , monocle } ,
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

static const char *dmenucmd[]      = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col1, "-nf", col3, "-sb", col5, "-sf", col4, NULL };
static const char *termcmd[]       = { "st", NULL };
static const char *rofidrun[]      = {"rofi","-show","drun"};
static const char *rofirun[]       = {"rofi","-show","run"};
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "150x40", NULL };


static const char *upvol[]      = { "/usr/bin/pactl"      , "set-sink-volume" , "0" , "+5%"    , NULL };
static const char *downvol[]    = { "/usr/bin/pactl"      , "set-sink-volume" , "0" , "-5%"    , NULL };
static const char *mutevol[]    = { "/usr/bin/pactl"      , "set-sink-mute"   , "0" , "toggle" , NULL };
static const char *light_up[]   = { "/usr/bin/xbacklight" , "-inc"            , "5" , NULL};
static const char *light_down[] = { "/usr/bin/xbacklight" , "-dec"            , "5" , NULL};

static const Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,                       XK_space,  spawn,          {.v = rofidrun } },
  { MODKEY,                       XK_p,      spawn,          {.v = rofirun } },
  { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
  { Mod4Mask,                     XK_grave,  togglescratch,  {.v = scratchpadcmd } },
  /* { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } }, */

  { MODKEY|ControlMask,           XK_b,      togglebar,      {0} },
	{ MODKEY,             XK_e,      toggleextrabar, {0} },
  { MODKEY,                       XK_j,      focusstackvis,  {.i = +1 } },
  { MODKEY,                       XK_k,      focusstackvis,  {.i = -1 } },
  { MODKEY|ShiftMask,             XK_j,      focusstackhid,  {.i = +1 } },
  { MODKEY|ShiftMask,             XK_k,      focusstackhid,  {.i = -1 } },
  { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
  { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_Return, zoom,           {0} },
  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY|ShiftMask,             XK_c,      killclient,     {0} },

  /* { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} }, */
  { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_f,      fullscreen,     {0} },
  { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },

  { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  
  /* { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } }, */
  /* { MODKEY,                       XK_period, focusmon,       {.i = +1 } }, */

  { MODKEY,                       XK_comma,  changefocusopacity,   {.f = -0.025}},
  { MODKEY,                       XK_period, changefocusopacity,   {.f = +0.025}},
  { MODKEY|ShiftMask,             XK_comma,  changeunfocusopacity, {.f = -0.025}},
  { MODKEY|ShiftMask,             XK_period, changeunfocusopacity, {.f = +0.025}},

  { MODKEY,                       XK_s,      show,           {0} },
  { MODKEY|ShiftMask,             XK_s,      showall,        {0} },
  { MODKEY|ShiftMask,             XK_h,      hide,           {0} },
  { MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
  { MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
  { MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },

  {0,               XF86XK_AudioLowerVolume, spawn,          {.v = downvol}},
  {0,               XF86XK_AudioMute,        spawn,          {.v = mutevol }},
  {0,               XF86XK_AudioRaiseVolume, spawn,          {.v = upvol}},
  {0,               XF86XK_MonBrightnessUp,  spawn,          {.v = light_up}},
  {0,               XF86XK_MonBrightnessDown,spawn,          {.v = light_down}},

    TAGKEYS(                      XK_1,                      0)
    TAGKEYS(                      XK_2,                      1)
    TAGKEYS(                      XK_3,                      2)
    TAGKEYS(                      XK_4,                      3)
    TAGKEYS(                      XK_5,                      4)
    TAGKEYS(                      XK_6,                      5)
    TAGKEYS(                      XK_7,                      6)
    TAGKEYS(                      XK_8,                      7)
    TAGKEYS(                      XK_9,                      8)

  { MODKEY|ControlMask,           XK_q,      quit,           {0} },
  { MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, 
  { MODKEY|ShiftMask,             XK_q,      exitdwm,        {0} },
};

/* XK_braceleft XK_braceright XK_colon  XK_semicolon XK_slash XK_bracketleft XK_backslash   XK_bracketright XK_grave XK_quoteleft */   
/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,          {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,          {.v = &layouts[2]} },

  { ClkWinTitle,          0,              Button1,        togglewin,          {0} },
  { ClkWinTitle,          0,              Button2,        killclient,         {0} },
  { ClkWinTitle,          0,              Button3,        zoom,               {0} },
  { ClkWinTitle,          0,              Button4,        changefocusopacity, {.f = +0.025} },
  { ClkWinTitle,          0,              Button5,        changefocusopacity, {.f = -0.025} },

  { ClkStatusText,        0,              Button1,        sigdwmblocks,       {.i = 1} },
  { ClkStatusText,        0,              Button2,        sigdwmblocks,       {.i = 2} },
  { ClkStatusText,        0,              Button3,        sigdwmblocks,       {.i = 3} },
  { ClkStatusText,        0,              Button4,        sigdwmblocks,       {.i = 4} },
  { ClkStatusText,        0,              Button5,        sigdwmblocks,       {.i = 5} },

	{ ClkExBarLeftStatus,   0,              Button1,        spawn,          SHCMD("st ranger") },
	{ ClkExBarLeftStatus,   0,              Button2,        spawn,          SHCMD("st htop") },
	{ ClkExBarLeftStatus,   0,              Button3,        spawn,          SHCMD("thunar") },
	/* { ClkExBarLeftStatus,   0,              Button4,        spawn,          {.v = rofirun } }, */
	/* { ClkExBarLeftStatus,   0,              Button5,        spawn,          {.v = rofirun } }, */
	{ ClkExBarMiddle,       0,              Button1,        spawn,          {.v = termcmd } },
	{ ClkExBarMiddle,       0,              Button2,        spawn,          {.v = rofidrun } },
	{ ClkExBarMiddle,       0,              Button3,        spawn,          SHCMD("microsoft-edge-beta")} ,
	/* { ClkExBarMiddle,       0,              Button4,        spawn,          {.v = termcmd } }, */
	/* { ClkExBarMiddle,       0,              Button5,        spawn,          {.v = termcmd } }, */
	{ ClkExBarRightStatus,  0,              Button1,        spawn,          SHCMD("nm-connection-editor") },
	{ ClkExBarRightStatus,  0,              Button2,        spawn,          SHCMD("tlpui") },
	{ ClkExBarRightStatus,  0,              Button3,        spawn,          SHCMD("blueberry")},
	{ ClkExBarRightStatus,  0,              Button4,        spawn,          SHCMD("xbacklight -inc 5") },
	{ ClkExBarRightStatus,  0,              Button5,        spawn,          SHCMD("xbacklight -dec 5") },

  { ClkClientWin,         MODKEY,         Button1,        movemouse,          {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating,     {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,        {0} },
  { ClkClientWin,         MODKEY,         Button4,        changefocusopacity, {.f = +0.025} },
  { ClkClientWin,         MODKEY,         Button5,        changefocusopacity, {.f = -0.025} },

  { ClkTagBar,            0,              Button1,        view,               {0} },
  { ClkTagBar,            0,              Button3,        toggleview,         {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,                {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,          {0} },
};

