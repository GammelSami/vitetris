#include <string.h>
#include "menu.h"
#include "internal.h"
#include "../input/input.h"
#include "../textgfx/textgfx.h"
#include "../draw/draw.h"
#include "../lang.h"

#if TWOPLAYER
int player_ = 0;
#endif

/* left column: rows 0-5, right column: rows 6-12 */
#define INP_LEFTCOL 6
#define INP_NITEMS 13
#define inp_row(i) ((i) < INP_LEFTCOL ? (i) : (i)-INP_LEFTCOL)

static int inp_keypr(int i)
{
	const char keypr[INP_NITEMS] = {
				MVUP, MVDOWN, MVLEFT, MVRIGHT, A_BTN, B_BTN,
				MVUP, A_BTN, B_BTN, HARDDROP, MVDOWN,
				SPEEDUP, SPEEDDOWN};
	int k = keypr[i];
	if (player_)
		k |= player_==1 ? PLAYER_1 : PLAYER_2;
	if (i >= INP_LEFTCOL)
		k |= IN_GAME;
	return k;
}

void inp_printkeys(int dev, int x, int y)
{
	const char *k;
	int i;
	x += 6;
	setcurs(x, y);
	for (i = 0; i < INP_NITEMS; i++) {
		k = getkeyfor_str(dev, inp_keypr(i));
		printstr(k);
		putnchars(' ', 6-strlen(k));
		if (i == INP_LEFTCOL-1) {
			x += 17;
			setcurs(x, y);
		} else
			newln(x);
	}
}

static int inp_setcurs(int x, int y, int i)
{
	if (i >= INP_LEFTCOL)
		x += 12;
	setcurs(x, y+inp_row(i));
	return x;
}

void inputsetup_menu(int pl, int x, int y)
{
	const char *menu[INP_NITEMS] = {
		" Up", "Dwn", "Lft", "Rgt", "  A", "  B",
		" Rotate ", " Rot cw ", NULL, "Harddrop", "Softdrop",
		" Speed +", " Speed -"
	};
	char rot_acw[9] = " Rot acw";
	int dev = 0;
	int i = 0;
	int xx, k;
#ifdef JOYSTICK
	int devlist = num_joyst;
	if (devlist) {
		i = -1;
		setcurs(x, y);
		newln(x);
		y++;
	}
#endif
	spellword(rot_acw+5);
	menu[8] = rot_acw;
	drawmenu(menu, INP_LEFTCOL, i, x, y, NULL);
	drawmenu(menu+INP_LEFTCOL, INP_NITEMS-INP_LEFTCOL, -1, x+12, y, NULL);
#ifdef TWOPLAYER
	player_ = pl;
#endif
	while (1) {
#ifdef JOYSTICK
		if (devlist && i==-1) {
			if (!inp_devlist(&dev, x, y-1))
				return;
			printmenuitem(menu[0], 1);
			i = 0;
		}
#endif
		inp_printkeys(dev, x, y);
		xx = inp_setcurs(x, y, i);
		refreshwin(-1);
		k = setkeymapping(dev, inp_keypr(i));
#ifdef JOYSTICK
		if (devlist && (k==MVUP && !i || k=='\t' && i==INP_NITEMS-1)) {
			printmenuitem(menu[i], 0);
			i = -1;
			continue;
		}
#endif
		if (k==MVUP && !i)
			k = MVDOWN;
		else if (k==MVDOWN && i==INP_NITEMS-1) {
			printmenuitem(menu[INP_NITEMS-1], 0);
			inp_setcurs(x, y, i=0);
			printmenuitem(menu[0], 1);
			continue;
		}
		switch (handle_menuitem_2cols(menu, INP_NITEMS, &i, INP_LEFTCOL,
					      xx, y+inp_row(i), NULL, k)) {
		case 0:
		case 2:
			return;
		case 3:
			inp_setcurs(x, y, i);
			printmenuitem(menu[i], 1);
		}
	}
}

#ifndef NO_MENU
static void printhelp(int x, int y)
{
	setcurs(x, y);
	printstr("ESC to clear, TAB to skip,");  newln(x);
	printstr("ENTER to return");
}

void inputsetup_box(int pl, int x, int y)
{
	/* total drawbox height; interior holds the taller of the two columns */
	int h = 9;
#ifdef JOYSTICK
	if (num_joyst)
		h++;
#endif
	clearbox(x-1, y, 0, h+4);
	printhelp(x+2, y+h+2);
	setcurs(x, y);
	printmenuitem("Input Setup", 1);
#ifdef TWOPLAYER
	if (!pl)
		printstr(" single");
	printstr(" player");
	if (pl)
		putch(pl+'0');
#endif
	drawbox(x, y+1, 31, h, NULL);
	inputsetup_menu(pl, x+1, y+2);
	clearbox(x, y, 0, h+4);
}
#endif
