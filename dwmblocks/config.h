/* macro for conveniently specifying pathu and pathc below */
#define PATH(name)                      "/home/wyz/suckless/dwm/scripts/"name

/* buffer size for capturing output of the programs used for updating blocks */
#define CMDOUTLENGTH                    50

/* DELIMITERENDCHAR must be less than 32.
 * At max, DELIMITERENDCHAR - 1 number of clickable blocks are allowed.
 * Raw characters larger than DELIMITERENDCHAR and smaller than ' ' in ASCII
   character set can be used for signaling color change in status.
 * The character corresponding to DELIMITERENDCHAR + 1 ('\x0b' when
   DELIMITERENDCHAR is 10) will switch the active colorscheme to the first one
   defined in colors array in dwm's config.h and so on.
 * If you wish to change DELIMITERENDCHAR, don't forget to update its value in
   dwm.c and color codes in your pathu programs. */
#define DELIMITERENDCHAR                10

/* delimiter specified as an array of characters
 * (don't remove DELIMITERENDCHAR at the end) */
/* static const char delimiter[] = {']', '[', DELIMITERENDCHAR }; */
static const char delimiter[] = { DELIMITERENDCHAR };

#include "block.h"

/* If interval of a block is set to 0, the block will only be updated once at
   startup.
 * If interval is set to a negative value, the block will never be updated in
   the main loop.
 * Set pathc to NULL if clickability is not required for the block.
 * Set signal to 0 if both clickability and signaling are not required for the
   block.
 * Signal must be less than DELIMITERENDCHAR for clickable blocks.
 * If multiple signals are pending, then the lowest numbered one will be
   delivered first. */

/* pathu - path of the program whose output is used as status text of the block
           (output of the program should be a single line of text with at most
            one newline character at the end)
 * pathc - path of the program used for handling clicks on the block */

/* 1 interval = INTERVALs seconds, INTERVALn nanoseconds */
#define INTERVALs                       0
#define INTERVALn                       100000000

static Block blocks[] = {
  /*      pathu                           pathc          interval        signal */
  /* { PATH("test.sh")           , PATH("test_button.sh")       , 1  , 7} , */
  /* { PATH("resource.sh")   , PATH("resource_button.sh")   , 1  , 1} , */
  /* { PATH("storage.sh")    , PATH("storage_button.sh")    , 1  , 9} , */
  /* { PATH("battery.sh")    , PATH("battery_button.sh")    , 30 , 8} , */
  /* { PATH("internet.sh")   , PATH("internet_button.sh")   , 1  , 7} , */
  /* { PATH("bluetooth.sh")  , PATH("bluetooth_button.sh")  , 1  , 4} , */
  /* { PATH("xbacklight.sh") , PATH("xbacklight_button.sh") , 1  , 3} , */
  /* { PATH("power1.sh")   , NULL                       , 1  , NULL } , */
  { PATH("mpc.sh")      , PATH("mpc_button.sh")      , 1  , 4    } ,
  /* { PATH("power2.sh")   , NULL                       , 1  , NULL } , */
  { PATH("pamixer.sh")  , PATH("pamixer_button.sh")  , 1  , 3    } ,
  /* { PATH("power3.sh")   , NULL                       , 1  , NULL } , */
  { PATH("calendar.sh") , PATH("calendar_button.sh") , 30 , 2    } ,
  /* { PATH("power4.sh")   , NULL                       , 1  , NULL } , */
  { PATH("time.sh")     , NULL                       , 10 , 1    } ,
  { PATH("left.sh")     , NULL                       , 1  , NULL } ,
  { PATH("right.sh")    , NULL                       , 1  , NULL } ,
  { NULL } /* just to mark the end of the array */
};
