/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "exitdwm.c"
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 1;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int extrabar           = 1;        /* 0 means no extra bar */
static const char statussep         = ';';      /* separator between statuses */
static const double activeopacity   = 0.9f;     /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 0.8f;     /* Window opacity when it's inactive (0 <= opacity <= 1) */

/* static const char *fonts[]        = { "monospace:size                = 10" }; */
static const char *fonts[]           = { "Code New Roman Nerd Font:size = 12" };
/* static const char dmenufont[]     = "monospace:size                  = 10"; */
static const char dmenufont[]        = "Code New Roman Nerd Font:size   = 12";

static const char col_gray1[]        = "#222222";
static const char col_gray2[]        = "#444444";
static const char col_gray3[]        = "#bbbbbb";
static const char col_gray4[]        = "#eeeeee";
static const char col_cyan[]         = "#777777";
static const char col_red[]          = "#770000";
static const char col_AliceBlue[]    = "#F0F8FF";
static const char col_Aquamarine[]   = "#7FFFD4";
static const char col_AntiqueWhite[] = "#FAEBD7";
static const char col_PaleGreen[]    = "#98fb98";
static const char col_LimeGreen[]    = "#32CD32";
static const char col_slateblue[]    = "#6A5ACD";
static const char col_awesome[]      = "#ff2052";
static const char col_azure[]        = "#007fff";
static const char col_BabyBlue[]     = "#89cff0";
static const char col1[]             = "#ffffff";
static const char col2[]             = "#dfffff";
static const char col3[]             = "#8a2be2";
static const char col4[]             = "#9370db";
static const char col5[]             = "#a020f0";
static const char col6[]             = "#018274";
static const char nord1[]="#D8DEE9";
static const char nord2[]="#81A1C1";
static const char nord3[]="#B48EAD";
static const char nord4[]="#434C5E";
static const char nord5[]="#A3BE8C";
static const char nord6[]="#2E3440";
static const char nord8[]="#D8DEE9";
static const char nord9[]="#E5E9F0";
static const char nord10[]="#3B4252";
static const char nord11[]="#81A1C1";
static const char nord12[]="#BF616A";
static const char nord13[]="#EBCB8B";
static const char nord14[]="#D08770";
static const char nord15[]="#B48EAD";

enum {
  Scheme0b,
  Scheme0c,
  Scheme0d,
  Scheme0e,
  Scheme0f,
  Scheme10,
  Scheme11,
  Scheme12,
  Scheme13,
  Scheme14,
  Scheme15,
  Scheme16,
  Scheme17,
  Scheme18,
  Scheme19,
  Scheme1a,
  Scheme1b,
  Scheme1c,
  SchemeSel,
  SchemeNorm, 
  SchemeTagNorm,
  SchemeTagSel,
  SchemeHid,
  SchemeSymbol,
  SchemeStatus,
}; /* color schemes */

static const char *colors[][3]      = {
  /*               fg         bg         border   */
  [SchemeNorm] = { col_slateblue, nord1, col_gray2 },
  [SchemeSel]  = { col_AliceBlue, nord11,  col_PaleGreen  },
  [SchemeTagNorm] = { nord3, nord6, col_gray2 },
  [SchemeTagSel]  = { col1, nord11,  col_awesome},
  [SchemeHid]  = { col_cyan,  col_gray1, col_cyan  },
  [SchemeSymbol] = { col_red,   nord5, col_gray2 },
  [SchemeStatus] = { col_gray2, nord10, col_gray2 },
  [Scheme0b] = { col1,      col4, col_gray2 },
  [Scheme0c] = { nord12,      nord10, col_gray2 },
  [Scheme0d] = { nord14,      nord1, col_gray2 },
  [Scheme0e] = { col4,      col2, col_gray2 },
  [Scheme0f] = { nord1,      nord2, col_gray2 },
  [Scheme10] = { nord4,      nord5, col_gray2 },
  [Scheme11] = { nord6,      col_BabyBlue, col_gray2 },
  [Scheme12] = { col_gray1,      nord8, col_gray2 },
  [Scheme13] = { nord12,      nord1, col_gray2 },
  [Scheme14] = { col3,      col_BabyBlue, col_gray2 },
  [Scheme15] = { col4,      col3, col_gray2 },
  [Scheme16] = { col5,      col2, col_gray2 },
  [Scheme17] = { col3,      col4, col_gray2 },
  [Scheme18] = { col3,      col4, col_gray2 },
  [Scheme19] = { col3,      col4, col_gray2 },
  [Scheme1a] = { col4,      col3, col_gray2 },
  [Scheme1b] = { col5,      col2, col_gray2 },
  [Scheme1c] = { col6,      col3, col_gray2 },
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
  /* class      instance    title       tags mask     isfloating   focusopacity    unfocusopacity     monitor */
  { "st"       , NULL , scratchpadname , 1 << 8 , 1 , 0.7 , 0.3             , -1 } ,
  { "st"       , NULL , "ranger"       , 1 << 8 , 1 , 0.9 , inactiveopacity , -1 } ,
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

static const char *dmenucmd[]      = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
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

	{ ClkExBarLeftStatus,   0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkExBarLeftStatus,   0,              Button1,        spawn,          {.v = rofirun } },
	{ ClkExBarMiddle,       0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkExBarRightStatus,  0,              Button2,        spawn,          {.v = termcmd } },

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

