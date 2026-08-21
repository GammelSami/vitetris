#include <stdlib.h>
#include "menu.h"
#include "../input/input.h"
#include "../textgfx/textgfx.h"
#include "../draw/draw.h"
#include "../hiscore.h"

void show_hiscorelist(int x, int y)
{
	char buf[320];
	char *p;
	int list = 0;
	int n, k;
	readhiscores(NULL);
	while (1) {
		setcurs(x, y);
		/* title doubles as the list selector; 12+11+8 == 31 chars,
		 * the exact width of the column header below */
		printmenuitem("Highscores", 1);
		printstr("  Level Up ");
		printmenuitem_options("auto off", list);
		newln(x);
		setcolorpair(MAGENTA_FG);
		printstr("    Name      Score  Lvl  Lines"); newln(x);
		setattr_normal();
		/* the other list may have been longer */
		clearbox(x, y+2, 0, HISCORE_LISTLEN);
		setcurs(x, y+2);
		if (!hiscores[list*HISCORE_LISTLEN].score) {
			printstr("    No saved scores");
			newln(x);
		} else {
			n = gethiscorelist(buf, list);
			p = buf;
			while (n) {
				p[30] = '\0';
				printstr(p);
				newln(x);
				p += 32;
				n--;
			}
		}
		refreshwin(-1);
		k = getkeypress_block(SINGLE_PL);
		if (k == 'q')
			exit(0);
		if (k != MVLEFT && k != MVRIGHT && k != '\t')
			return;
		list = !list;
	}
}
