#include "int32t.h"

#define HISCORE_LISTLEN 10

/* Two lists sharing one array and one file format:
 * 0..9  games played with automatic levelup
 * 10..19 games played with levelup switched off
 * The list a record belongs to travels in its name terminator code. */
extern
struct hiscore {
	char name[8];
	int_least32_t score;
	char startlevel;
	char level;
	short lines;
} hiscores[2*HISCORE_LISTLEN];

extern unsigned lastscore;

/* index of the first entry of the list the current game belongs to */
#define hiscore_base() (player1.autolevelup ? 0 : HISCORE_LISTLEN)

#define ishiscore() (((game->mode & MODE_BTYPE) ? player1.lines == 0 : player1.score >= 12000) && \
		     player1.score > hiscores[hiscore_base()+HISCORE_LISTLEN-1].score)

int readhiscores(const char *filename);

/* returns 0 if highscore could not be saved */
int savehiscore(const char *name);

/* returns hiscores[i].name or buf */
const char *gethiscorename(int i, char *buf);

/* Get formatted hiscorelist for list (0 or 1) in buf, which should be of
 * size 320.  every row is 32 chars
 * returns number of rows */
int gethiscorelist(char *buf, int list);
