/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include "pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

// http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf page 44 0xBF8 (SRFs special function register) + pic32mx.h for remaining

/* Interrupt Service Routine */
void user_isr(void)
{
  return;
}

/* Lab-specific initialization goes here */
void labinit(void)
{

}

/* This function is called repetitively from the main program */
void labwork(void)
{
  delay(10);

  switch (getbtns()) {

  }

  display_update();
  tick(&mytime);
  display_image(96, icon);
}
