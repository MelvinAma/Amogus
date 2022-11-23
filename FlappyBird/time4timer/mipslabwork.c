/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr(void)
{
  return;
}

/* Lab-specific initialization goes here */
void labinit(void)
{
  // init timer 2 for 100ms timeout (10 timeouts per second)

  int outputBits = ~0xFF;
  volatile int *trise = (volatile int *)0xbf886100;
  *trise &= outputBits;

  int inputBits = ~0xFF0;
  TRISD = inputBits;

  T2CON = 0x70;
  PR2 = ((80000000 / 256) / 10);
  TMR2 = 0;
  T2CONSET = 0x8000;

  return;
}

int timeoutcount = 0;
/* This function is called repetitively from the main program */
void labwork(void)
{
  volatile int *portE = (volatile int *)0xbf886110;

  if (IFS(0) & 0x100)
  {
    IFS(0) = 0;
    timeoutcount++;
    if (timeoutcount == 10)
    {

      time2string(textstring, mytime);
      display_string(3, textstring);
      display_update();
      display_image(96, icon);
      tick(&mytime);
      *portE += 1;
      timeoutcount = 0;
    }
  }



  switch (getbtns())
  {
  case 1:
    mytime = (getsw() << 4) + (mytime & 0xFF0F);
    break;
  case 2:
    mytime = (getsw() << 8) + (mytime & 0xF0FF);
    break;
  case 3:
    mytime = (getsw() << 4) + (mytime & 0xFF0F);
    mytime = (getsw() << 8) + (mytime & 0xF0FF);
    break;
  case 4:
    mytime = (getsw() << 12) + (mytime & 0x0FFF);
    break;
  case 5:
    mytime = (getsw() << 4) + (mytime & 0xFF0F);
    mytime = (getsw() << 12) + (mytime & 0xFF0F);
    break;
  case 6:
    mytime = (getsw() << 8) + (mytime & 0xF0FF);
    mytime = (getsw() << 12) + (mytime & 0x0FFF);
    break;
  case 7:
    mytime = (getsw() << 4) + (mytime & 0xFF0F);
    mytime = (getsw() << 8) + (mytime & 0xF0FF);
    mytime = (getsw() << 12) + (mytime & 0x0FFF);
    break;
  }
}
