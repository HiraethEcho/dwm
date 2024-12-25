#include "exitdwm.c"
#include "movestack.c"
#include <X11/X.h>
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int snap = 15;    /* snap pixel */
static const  int ebarpad   = 0;
static const  int barpad   = 0;

static const unsigned int altModKey = 0x40; //alt
static const unsigned int winModKey = 0x85; //win
static const unsigned int tabCycleKey = 0x17;
static const unsigned int tabCycleKeyInv = 0x32;

static const unsigned int gappih = 3; /* horiz inner gap between windows */
static const unsigned int gappiv = 3; /* vert inner gap between windows */
static const unsigned int gappoh = 3; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov = 3; /* vert outer gap between windows and screen edge */
static int smartgaps = 1; /* 1 means no outer gap when there is only one window */


static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 0; /* systray spacing */
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray = 1;             /* 0 means no systray */

static const int showbar = 1;                 /* 0 means no bar */
static const int showextrabar = 1;                 /* 0 means no extrabar */
static const int topbar = 1;                  /* 0 means bottom bar */

static const int isfixedtabwidth = 1; 
static const char *tabWidth = "                   ";

static const char statussep         = ';';      /* separator between status bars */


static const int startontag         = 1;        /* 0 means no tag active on start */
static const int scalepreview       = 2;        /* preview scaling (display w and h / scalepreview) */
static const int previewbar         = 1;        /* show the bar in the preview window */
// ⭘  󰣇 󱓞 󰣇

static const double activeopacity = 0.9f; /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 0.75f; /* Window opacity when it's inactive (0 <= opacity <= 1) */

static const int focusonwheel = 1;

static const char *fonts[] = {"Maple Mono NF:size=12"};
static const char dmenufont[] = "Maple Mono NF:size=12";


#define OPAQUE                  0xffU
static const unsigned int launcheralpha = 0.6*0xffU;
static const unsigned int selalpha = 0.9*0xffU;
static const unsigned int normalpha = 0.8*0xffU;
static const unsigned int hidealpha = 0.5*0xffU;
static const unsigned int etyalpha = 0.1*0xffU;
static const unsigned int sysalpha = 0.9*0xffU;
static const unsigned int statusalpha = 0.7*0xffU;

static const unsigned int symalpha = 0.8*0xffU;
static const unsigned int tagalpha = 0.9*0xffU;
static const unsigned int titlealpha = 0.6*0xffU;
static const unsigned int borderalpha = OPAQUE;

static const char gray1[] = "#bbbbbb";
static const char gray2[] = "#88c0d0";
static const char gray3[] = "#d8dee9";
static const char cyan[] = "#005577";


static const char *tagn[][2] = {
  {"#91a1c1", "#ecb7dc"},
  {"#8798c4", "#dcb7dc"},
  {"#8395c7", "#ccb7dc"},
  {"#7992ca", "#bcb7dc"},
  {"#7589ce", "#acb7dc"},
};

static const char *tagy[][2] = {
  {"#ef616a","#61afef" },
  {"#de617a","#98fb98" },
  {"#cd618a","#9c8eff" },
  {"#bc619a","#ffec2b" },
  {"#ab61aa","#6ce0d6" },
};
static const unsigned int tagalphas[][2]      = {
  { OPAQUE, tagalpha },
  { OPAQUE, tagalpha },
  { OPAQUE, tagalpha },
  { OPAQUE, tagalpha },
  { OPAQUE, tagalpha },
};

static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeLauncher] = { "#61afef", "#5a5255", cyan},
    [SchemeSel]      = { "#2a303c", "#81a1c1", "#cc241d"},
    [SchemeNorm]     = { "#d8dee9", "#414868", "#d8dee9"},
    [SchemeHid]      = { "#81a1c1", "#24283b", cyan},
    [SchemeEty]      = { "#d8dee9", "#000000", "#d8dee9"},
    [SchemeSys]      = { "#ffffff", "#81a1c1", cyan},
    [SchemeStatus]   = { "#ffffff", "#49688e", cyan},
    [SchemeSym]      = { "#dfe3ee", "#3b5998", cyan},
    [SchemeTag]      = { "#83848e", "#213438", cyan},
    [SchemeTitle]    = { "#2a303c", "#ffeaba", "#d8dee9"},
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeLauncher]  = { OPAQUE, launcheralpha, borderalpha },
	[SchemeSel]       = { OPAQUE, selalpha,      borderalpha },
	[SchemeNorm]      = { OPAQUE, normalpha,     borderalpha },
	[SchemeHid]       = { OPAQUE, hidealpha,     borderalpha },
	[SchemeEty]       = { OPAQUE, etyalpha,      borderalpha },
	[SchemeSys]       = { OPAQUE, sysalpha,      borderalpha },
	[SchemeStatus]    = { OPAQUE, statusalpha,   borderalpha },
	[SchemeSym]       = { OPAQUE, symalpha,      borderalpha },
	[SchemeTag]       = { OPAQUE, tagalpha,      borderalpha },
	[SchemeTitle]     = { OPAQUE, titlealpha,    borderalpha },
};

// static const char *launchers[] = {"","󰇩","","󰓓","","",""} ;
static const char *launchers[] = {"","󰀻","","󰅟"} ;
// 󰖳    󰃥    󰗚 󰨇 
static const char *tags[] = {"󰋜", "󰃥", "", "󰖳", ""};
static const int taglayouts[] = { 1, 0, 0, 0, 0, 0 }; //first for viewall tags
                                                      //

static const unsigned int ulinepad = 6; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke = 3; /* thickness / height of the underline */
static const unsigned int ulinevoffset = 2; /* how far above the bottom of the bar the line should appear */
// static const int ulineall    = 1;  /* 1 to show underline on all tags, 0 for
// just the active ones */

/* xprop(1):
 *  WM_CLASS(STRING) = instance, class
 *  WM_NAME(STRING) = title
 */

static const Rule rules[] = {
    /* class, instance, title,        tags mask, isfloating, focusopacity, unfocusopacity,  monitor */
    {"st",    NULL,     "scratchpad", 0,         1,          0.8,          0.7,             -1},
    {"st",    NULL,     "tasks",      0,         1,          0.8,          0.7,             -1},
    // {"Gimp",  NULL,     NULL,         0,         1,          1.0,          inactiveopacity, -1},
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints = 0; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1
/* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[M]", monocle},
    {":::", gaplessgrid},
    {"|M|", centeredmaster},
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"TTT", bstack},
    {NULL, NULL},
};
static const Layout tablayout = {"---", horizgrid};
/*
    {">M>", centeredfloatingmaster},
    {"[@]", spiral},
    {"[\\]", dwindle},
    {"|M|", centeredmaster},
    {"H[]", deck},
    {"###", grid},
    {":::", gaplessgrid},
    {"---", horizgrid},
    {"###", nrowgrid},
    {"===", bstackhoriz},
    {"---", horizgrid},
*/

#define MODKEY Mod1Mask
#define TAGKEYS(KEY, TAG) \
{MODKEY,                           KEY, view,       {.ui = 1 << TAG}}, \
{MODKEY | ControlMask,             KEY, toggleview, {.ui = 1 << TAG}}, \
{MODKEY | ShiftMask,               KEY, tag,        {.ui = 1 << TAG}}, \
{MODKEY | ControlMask | ShiftMask, KEY, toggletag,  {.ui = 1 << TAG}}, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", gray1, "-nf", cyan, "-sb", cyan, "-sf", gray1, NULL};
static const char *termcmd[] = {"kitty", NULL};

static const char scratchpadname[] = "pad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "150x40", NULL };

static const Key keys[] = {
    // spawn and kill client
    {Mod4Mask,           XK_grave,  togglescratch, {.v = scratchpadcmd}},
    // {Mod4Mask|ShiftMask, XK_grave,  sendtoscratch, {0}},
    {MODKEY,             XK_p,      spawn,         {.v = dmenucmd}},
    {MODKEY,             XK_Return, spawn,         {.v = termcmd}},
    {Mod4Mask,           XK_c,      killclient,    {0}},
    {MODKEY,             XK_Escape, killclient,    {0}},

    // focus and hide clients
    {MODKEY,           XK_j,   focusstackvis, {.i = +1}},
    {MODKEY,           XK_k,   focusstackvis, {.i = -1}},
    // {MODKEY,           XK_period,   focusstackvis, {.i = +1}},
    // {MODKEY,           XK_comma,   focusstackvis, {.i = -1}},
    {MODKEY,           XK_m,   togglehide,    {0}},
    {MODKEY|ShiftMask, XK_m,   showall,       {0}},
    {Mod4Mask,         XK_Tab, wintab,        {0}},
    {MODKEY,           XK_Tab, alttab,        {0}},
    // {MODKEY          ,XK_Tab,focusstackhid,{.i = +1}},
    // {MODKEY|ShiftMask,XK_Tab,focusstackhid,{.i = -1}},
    // {MODKEY | ShiftMask, XK_j, focusstackhid, {.i = +1}},
    // {MODKEY | ShiftMask, XK_k, focusstackhid, {.i = -1}},
    // {MODKEY                     , XK_s                     , show , {0}} ,
    // {MODKEY                     , XK_h                     , hide , {0}} ,

    // choose clients
    // {Mod4Mask , XK_Return   , winview , {0}} ,

    // move clients
    // {MODKEY|ShiftMask,           XK_period,      movestack,            {.i = +1}},
    // {MODKEY|ShiftMask,           XK_comma,      movestack,            {.i = -1}},
    {Mod4Mask,           XK_j,      movestack,            {.i = +1}},
    {Mod4Mask,           XK_k,      movestack,            {.i = -1}},
    {Mod4Mask,           XK_Return, movecenter,                 {0}},
    // opacity
    // {MODKEY | ShiftMask, XK_a,      changefocusopacity,   {.f = +0.025}},
    // {MODKEY | ShiftMask, XK_s,      changefocusopacity,   {.f = -0.025}},
    // {MODKEY | ShiftMask, XK_z,      changeunfocusopacity, {.f = +0.025}},
    // {MODKEY | ShiftMask, XK_x,      changeunfocusopacity, {.f = -0.025}},
    {MODKEY, XK_z,      changefocusopacity,   {.f = +0.025}},
    {MODKEY, XK_x,      changefocusopacity,   {.f = -0.025}},
    {MODKEY, XK_c,      changeunfocusopacity, {.f = +0.025}},
    {MODKEY, XK_v,      changeunfocusopacity, {.f = -0.025}},
    // move and resize float clients
    { MODKEY,            XK_Down,   moveresize,           {.v = "0x 25y 0w 0h" } },
    { MODKEY,            XK_Up,     moveresize,           {.v = "0x -25y 0w 0h" } },
    { MODKEY,            XK_Right,  moveresize,           {.v = "25x 0y 0w 0h" } },
    { MODKEY,            XK_Left,   moveresize,           {.v = "-25x 0y 0w 0h" } },
    { Mod4Mask,  XK_Down,   moveresize,           {.v = "0x 0y 0w 25h" } },
    { Mod4Mask,  XK_Up,     moveresize,           {.v = "0x 0y 0w -25h" } },
    { Mod4Mask,  XK_Right,  moveresize,           {.v = "0x 0y 25w 0h" } },
    { Mod4Mask,  XK_Left,   moveresize,           {.v = "0x 0y -25w 0h" } },
    // { Mod4Mask,          XK_Up,     moveresizeedge,       {.v = "t"} },
    // { Mod4Mask,          XK_Down,   moveresizeedge,       {.v = "b"} },
    // { Mod4Mask,          XK_Left,   moveresizeedge,       {.v = "l"} },
    // { Mod4Mask,          XK_Right,  moveresizeedge,       {.v = "r"} },

    // change layouts
    // {MODKEY,           XK_a,     setlayout,      {.v = &layouts[2]}},
    // {Mod4Mask,           XK_s,     setlayout,      {.v = &layouts[4]}},
    // {MODKEY,           XK_d,     setlayout,      {.v = &layouts[5]}},
    {MODKEY|ShiftMask, XK_space, togglefloating,   {0}},
    {Mod4Mask,         XK_f,     setlayout,        {.v = &layouts[0]}},
    {Mod4Mask,         XK_g,     setlayout,        {.v = &layouts[1]}},
    {Mod4Mask,         XK_t,     setlayout,        {.v = &layouts[3]}},
    {Mod4Mask,         XK_b,     setlayout,        {.v = &layouts[5]}},
    {MODKEY|ShiftMask, XK_f,     togglefullscreen, {0} },

    // {MODKEY|ShiftMask,  XK_f,     raisewin,       {0}},
    // { MODKEY|ShiftMask,          XK_comma,  cyclelayout,    {.i =
    // -1 } }, { MODKEY|ShiftMask,           XK_period, cyclelayout,    {.i = +1
    // } },
    // {MODKEY, XK_Tab, view, {0}},
    // {MODKEY, XK_a, setlayout, {.v = &layouts[2]}},
    // { MODKEY                 , XK_space                 , setlayout , {0} } ,

    // modify layouts
    {MODKEY|ShiftMask, XK_l,            setcfact,    {.f = +0.02}},
    {MODKEY|ShiftMask, XK_h,            setcfact,    {.f = -0.02}},
    {MODKEY,           XK_l,            setmfact,    {.f = +0.02}},
    {MODKEY,           XK_h,            setmfact,    {.f = -0.02}},

    {Mod4Mask,         XK_d,            setcfact,    {.f = 0.00}},
    {Mod4Mask,         XK_d,            view,        {0}},

    // {MODKEY,           XK_backslash,    incrgaps,    {.i = +1}},
    // {MODKEY|ShiftMask, XK_backslash,    incrgaps,    {.i = -1}},
    // {MODKEY,           XK_i,            incrigaps,   {.i = +1}},
    // {MODKEY|ShiftMask, XK_i,            incrigaps,   {.i = -1}},
    // {MODKEY,           XK_o,            incrogaps,   {.i = +1}},
    // {MODKEY|ShiftMask, XK_o,            incrogaps,   {.i = -1}},
    {MODKEY,           XK_i,            incrgaps,   {.i = +1}},
    {MODKEY,           XK_o,            incrgaps,   {.i = -1}},
    {Mod4Mask,         XK_d,            defaultgaps, {0}},

    {MODKEY,           XK_bracketleft,  incnmaster,  {.i = +1}},
    {MODKEY,           XK_bracketright, incnmaster,  {.i = -1}},

    // bars
    {MODKEY,             XK_b, togglebar,      {0}},
    {MODKEY|ControlMask, XK_b, toggleextrabar, {0}},
    {MODKEY|ShiftMask,   XK_b, toggletopbar,   {0}},

    // monitors
    // {Mod4Mask,           XK_comma,  focusmon, {.i = -1}},
    // {Mod4Mask,           XK_period, focusmon, {.i = +1}},
    // {Mod4Mask|ShiftMask, XK_comma,  tagmon,   {.i = -1}},
    // {Mod4Mask|ShiftMask, XK_period, tagmon,   {.i = +1}},
    {MODKEY,           XK_comma,  focusmon, {.i = -1}},
    {MODKEY,           XK_period, focusmon, {.i = +1}},
    {Mod4Mask, XK_comma,  tagmon,   {.i = -1}},
    {Mod4Mask, XK_period, tagmon,   {.i = +1}},
    // {MODKEY|ShiftMask, XK_comma,  tagmon,   {.i = -1}},
    // {MODKEY|ShiftMask, XK_period, tagmon,   {.i = +1}},

    // tags
    TAGKEYS(XK_1, 0) 
    TAGKEYS(XK_2, 1) 
    TAGKEYS(XK_3, 2) 
    TAGKEYS(XK_4, 3) 
    TAGKEYS(XK_5, 4)

    {MODKEY|ShiftMask, XK_0, tag,  {.ui = ~0}},
    {MODKEY,           XK_0, view, {.ui = ~0}},
    {MODKEY|ShiftMask, XK_grave, tag,  {.ui = ~0}},
    {MODKEY,           XK_grave, view, {.ui = ~0}},

    {MODKEY,                           XK_q, view,       {.ui = 1 << 0}},
    {MODKEY,                           XK_w, view,       {.ui = 1 << 1}},
    {MODKEY,                           XK_e, view,       {.ui = 1 << 2}},
    {MODKEY,                           XK_r, view,       {.ui = 1 << 3}},
    {MODKEY,                           XK_t, view,       {.ui = 1 << 4}},

    {MODKEY,                           XK_a, tag,       {.ui = 1 << 0}},
    {MODKEY,                           XK_s, tag,       {.ui = 1 << 1}},
    {MODKEY,                           XK_d, tag,       {.ui = 1 << 2}},
    {MODKEY,                           XK_f, tag,       {.ui = 1 << 3}},
    {MODKEY,                           XK_g, tag,       {.ui = 1 << 4}},

// {MODKEY,                           KEY, view,       {.ui = 1 << TAG}},
// {MODKEY | ControlMask,             KEY, toggleview, {.ui = 1 << TAG}},
// {MODKEY | ShiftMask,               KEY, tag,        {.ui = 1 << TAG}},
// {MODKEY | ControlMask | ShiftMask, KEY, toggletag,  {.ui = 1 << TAG}},

    // quit
    {MODKEY|ShiftMask,   XK_q, quit,    {0}},
    {MODKEY|ShiftMask,   XK_e, exitdwm, {0}},
    {Mod4Mask|ShiftMask, XK_q, quit,    {1}},

    // Functions
    {0, XF86XK_AudioRaiseVolume,  spawn, SHCMD("pamixer -i 5 ; pkill -SIGUSR1 dwmblocks ")},
    {0, XF86XK_AudioLowerVolume,  spawn, SHCMD("pamixer -d 5; pkill -SIGUSR1 dwmblocks")},
    {0, XF86XK_AudioMute,         spawn, SHCMD("pamixer -t; pkill -SIGUSR1 dwmblocks")},
    {0, XF86XK_MonBrightnessUp,   spawn, SHCMD("xbacklight -inc 5; pkill -SIGUSR1 dwmblocks")},
    {0, XF86XK_MonBrightnessDown, spawn, SHCMD("xbacklight -dec 5; pkill -SIGUSR1 dwmblocks")},
    {0, XF86XK_WLAN,              spawn, SHCMD("nm-connection-editor")},
    {0, XF86XK_Tools,             spawn, SHCMD("rofi_allapps")},
    // {0, XK_F5, spawn, SHCMD("block_update")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click,     event mask, button,  function argument,  */
    {ClkLtSymbol,   0,                Button1, cyclelayout,        {.i = -1}},
    {ClkLtSymbol,   0,                Button2, setlayout,          {.v = &layouts[2]}},
    {ClkLtSymbol,   0,                Button3, cyclelayout,        {.i = +1}},
    {ClkLtSymbol,   0,                Button4, setlayout,          {.v = &layouts[4]}},
    {ClkLtSymbol,   0,                Button5, setlayout,          {.v = &layouts[5]}},

    {ClkTagBar,     0,                Button1, view,               {0}},
    {ClkTagBar,     0,                Button3, toggleview,         {0}},
    {ClkTagBar,     MODKEY,           Button1, tag,                {0}},
    {ClkTagBar,     MODKEY,           Button3, toggletag,          {0}},
    {ClkTagBar,     0,                Button2, toggletag,          {0}},

    {ClkTab, 0,      Button1, togglewin,            {0}},
    {ClkTab, 0,      Button2, killclient,           {0}},
    {ClkTab, 0,      Button3, movecenter,           {0}},
    {ClkTab, 0,      Button4, changefocusopacity,   {.f = +0.025}},
    {ClkTab, 0,      Button5, changefocusopacity,   {.f = -0.025}},
    {ClkTab, MODKEY, Button4, changeunfocusopacity, {.f = +0.025}},
    {ClkTab, MODKEY, Button5, changeunfocusopacity, {.f = -0.025}},

    {ClkStatusText, 0,                Button1, sigstatusbar,       {.i = 1}},
    {ClkStatusText, 0,                Button2, sigstatusbar,       {.i = 2}},
    {ClkStatusText, 0,                Button3, sigstatusbar,       {.i = 3}},
    {ClkStatusText, 0,                Button4, sigstatusbar,       {.i = 4}},
    {ClkStatusText, 0,                Button5, sigstatusbar,       {.i = 5}},
    {ClkStatusText, ControlMask,      Button1, sigstatusbar,       {.i = 6}},
    {ClkStatusText, ControlMask,      Button2, sigstatusbar,       {.i = 7}},
    {ClkStatusText, ControlMask,      Button3, sigstatusbar,       {.i = 8}},
    {ClkStatusText, ControlMask,      Button4, sigstatusbar,       {.i = 9}},
    {ClkStatusText, ControlMask,      Button5, sigstatusbar,       {.i = 0}},

    {ClkLancher,    0,                Button1, launchf,            {.i = 1}},
    {ClkLancher,    0,                Button2, launchf,            {.i = 2}},
    {ClkLancher,    0,                Button3, launchf,            {.i = 3}},
    {ClkLancher,    0,                Button4, launchf,            {.i = 4}},
    {ClkLancher,    0,                Button5, launchf,            {.i = 5}},
    {ClkLancher,    ControlMask,      Button1, launchf,            {.i = 6}},
    {ClkLancher,    ControlMask,      Button2, launchf,            {.i = 7}},
    {ClkLancher,    ControlMask,      Button3, launchf,            {.i = 8}},
    {ClkLancher,    ControlMask,      Button4, launchf,            {.i = 9}},
    {ClkLancher,    ControlMask,      Button5, launchf,            {.i = 0}},

    {ClkTitle, 0,      Button1, togglewin,            {0}},
    {ClkTitle, 0,      Button3, movecenter,           {0}},
    {ClkTitle, 0,      Button2, killclient,           {0}},
    {ClkTitle, 0,      Button4, changefocusopacity,   {.f = +0.025}},
    {ClkTitle, 0,      Button5, changefocusopacity,   {.f = -0.025}},
    {ClkTitle, MODKEY, Button4, changeunfocusopacity, {.f = +0.025}},
    {ClkTitle, MODKEY, Button5, changeunfocusopacity, {.f = -0.025}},

    {ClkClientWin,  MODKEY,           Button1, movemouse,          {0}},
    {ClkClientWin,  MODKEY,           Button3, resizemouse,        {0}},
    {ClkClientWin,  MODKEY,           Button4, moveresize,         {.v = "0x 0y -25w 0h" } },
    {ClkClientWin,  MODKEY,           Button5, moveresize,         {.v = "0x 0y 25w 0h" } },
    {ClkClientWin,  MODKEY|ShiftMask, Button4, moveresize,         {.v = "0x 0y 0w -25h" } },
    {ClkClientWin,  MODKEY|ShiftMask, Button5, moveresize,         {.v = "0x 0y 0w 25h" } },

    {ClkEtyTitle, 0, Button1, spawn, SHCMD("dwmClkEtyTitle 1")},
    {ClkEtyTitle, 0, Button2, spawn, SHCMD("dwmClkEtyTitle 2")},
    {ClkEtyTitle, 0, Button3, spawn, SHCMD("dwmClkEtyTitle 3")},
    {ClkEtyTitle, 0, Button4, spawn, SHCMD("dwmClkEtyTitle 4")},
    {ClkEtyTitle, 0, Button5, spawn, SHCMD("dwmClkEtyTitle 5")},

    {ClkRootWin,  0, Button1, spawn, SHCMD("dwmClkRootWin 1")},
    {ClkRootWin,  0, Button2, spawn, SHCMD("dwmClkRootWin 2")},
    {ClkRootWin,  0, Button3, spawn, SHCMD("dwmClkRootWin 3")},
    {ClkRootWin,  0, Button4, spawn, SHCMD("dwmClkRootWin 4")},
    {ClkRootWin,  0, Button5, spawn, SHCMD("dwmClkRootWin 5")},

    {ClkEtyTab,   0, Button1, spawn, SHCMD("dwmClkEtyTab 1")},
    {ClkEtyTab,   0, Button2, spawn, SHCMD("dwmClkEtyTab 2")},
    {ClkEtyTab,   0, Button3, spawn, SHCMD("dwmClkEtyTab 3")},
    {ClkEtyTab,   0, Button4, spawn, SHCMD("dwmClkEtyTab 4")},
    {ClkEtyTab,   0, Button5, spawn, SHCMD("dwmClkEtyTab 5")},
    /* {ClkEtyTitle,   0,                Button1, spawn,              SHCMD("rofi_quickapps")},
    {ClkEtyTitle,   0,                Button2, spawn,              SHCMD("rofi -show run")},
    {ClkEtyTitle,   0,                Button3, spawn,              SHCMD("rofi_allapps")},
    {ClkEtyTitle,   0,                Button4, spawn,              SHCMD("xbacklight -inc 5; pkill -SIGUSR1 dwmblocks")},
    {ClkEtyTitle,   0,                Button5, spawn,              SHCMD("xbacklight -dec 5; pkill -SIGUSR1 dwmblocks")},
    {ClkRootWin,    0,                Button1, spawn,              SHCMD("wallpaper_change")},
    {ClkRootWin,    0,                Button2, spawn,              SHCMD("rofi -show run")},
    {ClkRootWin,    0,                Button3, spawn,              SHCMD("rofi_allapps")},
    {ClkRootWin,    0,                Button4, spawn,              SHCMD("dwmClkRootWin 4")},
    {ClkRootWin,    0,                Button5, spawn,              SHCMD("dwmClkRootWin 5")},
    {ClkEtyTab,     0,                Button1, spawn,              SHCMD("rofi_quickapps")},
    {ClkEtyTab,     0,                Button2, spawn,              SHCMD("rofi -show run")},
    {ClkEtyTab,     0,                Button3, spawn,              SHCMD("rofi_allapps")},
    {ClkEtyTab,     0,                Button4, spawn,              SHCMD("xbacklight -inc 5; pkill -SIGUSR1 dwmblocks")},
    {ClkEtyTab,     0,                Button5, spawn,              SHCMD("xbacklight -dec 5; pkill -SIGUSR1 dwmblocks")}, */
};
