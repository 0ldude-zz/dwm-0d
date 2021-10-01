/* See LICENSE file for copyright and license details. */

/* include function keys */
#include <X11/XF86keysym.h>
/* function keys scripts */
static const char *upvol[]		= {
	"/usr/bin/pulsemixer", "--change-volume", "+5", NULL 
};
static const char *dwnvol[]		= {
	"/usr/bin/pulsemixer", "--change-volume", "-5", NULL
};
static const char *tglmute[]	= { 
	"/usr/bin/pulsemixer", "--toggle-mute", NULL 
};
static const char *brghtup[]	= {
	"/usr/bin/xbacklight", "-inc", "10",  NULL
};
static const char *brghtdwn[]	= {
	"/usr/bin/xbacklight", "-dec", "10",  NULL
};

/* shortcut scripts */

/* appearance */
/* border pixel of windows */
static const unsigned int borderpx  = 5;       
/* snap pixel */
static const unsigned int snap      = 32;
/* 0: tray follows selected monitor, >0: pin tray to monitor X */
static const unsigned int systraypinning = 0;
/* systray spacing */
static const unsigned int systrayspacing = 2;
/* 1: if pinning fails, display systray on the first monitor,
False: display systray on the last monitor*/
static const int systraypinningfailfirst = 1;
/* 0 means no systray */
static const int showsystray        = 1;
/* 0 means no bar */
static const int showbar            = 1;
/* 0 means bottom bar */
static const int topbar             = 1;
static const char *fonts[]          = { 
	"Terminus:style=bold:size=14:antialias=true:autohint=true" 
};
static const char dmenufont[]       = { 
	"Terminus:style=bold:size=14:antialias=true:autohint=true"
};
static const char col_bg[]          = "#282828";
static const char col_red[]         = "#cc241d";
static const char col_green[]       = "#98971a";
static const char col_yellow[]      = "#d79921";
static const char col_blue[]        = "#458588";
static const char col_purple[]      = "#b16286";
static const char col_aqua[]        = "#689d6a";
static const char col_gray[]        = "#a89984";
static const char col_darkgray[]    = "#928374";
static const char col_lightred[]    = "#fb4934";
static const char col_lightgreen[]  = "#b8bb26";
static const char col_lightyellow[] = "#fabd2f";
static const char col_lightblue[]   = "#83a598";
static const char col_lightpurple[] = "#d3869b";
static const char col_lightaqua[]   = "#8ec07c";
static const char col_fg[]          = "#ebdbb2";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_fg, col_bg,  col_darkgray },
	[SchemeSel]  = { col_fg, col_blue,  col_blue },
 };

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class	instance title				tags mask	isfloating	monitor	scratch key */
	{ "Gimp",	NULL,	NULL,       	 	0,     		1,          -1,     0  },
	{ "firefox",NULL,	NULL,       	 	1 << 8,		0,          -1,     0  },
	{ "teams",	NULL,	NULL,       	 	1 << 8,		0,          -1,     0  },
	{ NULL,		NULL,	"scratchpad",		0,			1,          -1,     's'},
	{ NULL,		NULL,	"pulsemixer",		0,			1,          -1,     'v'},
	{ NULL,		NULL,	"nmtui",	 	   	0,			1,          -1,     'n'},
	{ NULL,		NULL,	"sfeed",	 	   	0,			1,          -1,     'u'},
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
	"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg, "-nf", col_fg, \
	"-sb", col_blue, "-sf", col_fg, NULL
};
static const char *termcmd[]  = { "st", NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] 		= {
	"s", "st", "-t", "scratchpad", "-g", "149x39" "+180+160", NULL
}; 
static const char *scratchpadvolume[] 	= {
	"v", "st", "-t", "pulsemixer", "-g", "149x39" "+180+160", "-e", "pulsemixer", NULL
};	
static const char *scratchpadnmtui[] 	= {
	"n", "st", "-t", "nmtui",	   "-g", "149x39" "+180+160", "-e", "nmtui", NULL
};
static const char *scratchpadrss[] 		= {
	"u", "st", "-t", "sfeed",	   "-g", "149x39" "+180+160", "-e", "sfeed_scratchpad", NULL
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_s,	   togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_v,	   togglescratch,  {.v = scratchpadvolume } },
	{ MODKEY,                       XK_n,	   togglescratch,  {.v = scratchpadnmtui } },
	{ MODKEY,                       XK_u,	   togglescratch,  {.v = scratchpadrss } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {0} },
	{ 0,		 XF86XK_AudioRaiseVolume,	   spawn,	       {.v = upvol } },
	{ 0,		 XF86XK_AudioLowerVolume,	   spawn,	       {.v = dwnvol } },
	{ 0,		 XF86XK_AudioMute,			   spawn,	       {.v = tglmute } },
	{ 0,		 XF86XK_MonBrightnessUp,	   spawn,	       {.v = brghtup } },
	{ 0,		 XF86XK_MonBrightnessDown,	   spawn,	       {.v = brghtdwn } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

