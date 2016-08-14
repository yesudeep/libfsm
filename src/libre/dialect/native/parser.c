/*
 * Automatically generated from the files:
 *	src/libre/dialect/native/parser.sid
 * and
 *	src/libre/parser.act
 * by:
 *	sid
 */

/* BEGINNING OF HEADER */

#line 112 "src/libre/parser.act"


	#include <assert.h>
	#include <limits.h>
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <errno.h>
	#include <ctype.h>

	#include "libfsm/internal.h" /* XXX */

	#include <adt/bitmap.h>

	#include <fsm/fsm.h>

	#include <re/re.h>

	#include "libre/internal.h"

	#ifndef DIALECT
	#error DIALECT required
	#endif

	#define PASTE(a, b) a ## b
	#define CAT(a, b)   PASTE(a, b)

	#define LX_PREFIX CAT(lx_, DIALECT)

	#define LX_TOKEN  CAT(LX_PREFIX, _token)
	#define LX_STATE  CAT(LX_PREFIX, _lx)
	#define LX_NEXT   CAT(LX_PREFIX, _next)
	#define LX_INIT   CAT(LX_PREFIX, _init)

	#define DIALECT_COMP  CAT(comp_, DIALECT)
	#define DIALECT_GROUP CAT(group_, DIALECT)

	/* XXX: get rid of this; use same %entry% for all grammars */
	#define DIALECT_ENTRY       CAT(p_re__, DIALECT)
	#define DIALECT_GROUP_ENTRY CAT(p_group_H, DIALECT)

	#include "parser.h"
	#include "lexer.h"

	#include "../comp.h"
	#include "../group.h"

	typedef char     t_char;
	typedef unsigned t_unsigned;
	typedef unsigned t_pred; /* TODO */

	typedef struct lx_pos t_pos;
	typedef struct fsm_state * t_fsm__state;
	typedef struct re_grp t_grp;

	struct act_state {
		enum LX_TOKEN lex_tok;
		enum LX_TOKEN lex_tok_save;
		struct re_grp *g; /* for <stash-group> */
		int overlap; /* permit overlap in groups */

		/*
		 * Lexical position stored for syntax errors.
		 */
		struct re_pos synstart;
		struct re_pos synend;

		/*
		 * Lexical positions stored for errors which describe multiple tokens.
		 * We're able to store these without needing a stack, because these are
		 * non-recursive productions.
		 */
		struct re_pos groupstart; struct re_pos groupend;
		struct re_pos rangestart; struct re_pos rangeend;
		struct re_pos countstart; struct re_pos countend;
	};

	struct lex_state {
		struct LX_STATE lx;
		struct lx_dynbuf buf; /* XXX: unneccessary since we're lexing from a string */

		int (*f)(void *opaque);
		void *opaque;

		/* for lgetc()'s character-shoving */
		enum { SHOVE_PRE, SHOVE_MID, SHOVE_NONE } shove;

		/* TODO: use lx's generated conveniences for the pattern buffer */
		char a[512];
		char *p;
	};

	#define CURRENT_TERMINAL (act_state->lex_tok)
	#define ERROR_TERMINAL   (TOK_ERROR)
	#define ADVANCE_LEXER    do { mark(&act_state->synstart, &lex_state->lx.start); \
	                              mark(&act_state->synend,   &lex_state->lx.end);   \
	                              act_state->lex_tok = LX_NEXT(&lex_state->lx); } while (0)
	#define SAVE_LEXER(tok)  do { act_state->lex_tok_save = act_state->lex_tok; \
	                              act_state->lex_tok = tok;                     } while (0)
	#define RESTORE_LEXER    do { act_state->lex_tok = act_state->lex_tok_save; } while (0)

	static void
	mark(struct re_pos *r, const struct lx_pos *pos)
	{
		assert(r != NULL);
		assert(pos != NULL);

		r->byte = pos->byte;
	}

	/* TODO: centralise perhaps */
	static void
	snprintdots(char *s, size_t sz, const char *msg)
	{
		size_t n;

		assert(s != NULL);
		assert(sz > 3);
		assert(msg != NULL);

		n = sprintf(s, "%.*s", (int) sz - 3 - 1, msg);
		if (n == sz - 3 - 1) {
			strcpy(s + sz, "...");
		}
	}

	static void
	group_add(t_grp *g, char c)
	{
		assert(g != NULL);
		assert((unsigned char) c != FSM_EDGE_EPSILON);

		if (bm_get(&g->set, (unsigned char) c)) {
			bm_set(&g->dup, (unsigned char) c);
			return;
		}

		bm_set(&g->set, (unsigned char) c);
	}

	/* TODO: centralise */
	/* XXX: escaping really depends on dialect */
	static const char *
	escchar(char *s, size_t sz, int c)
	{
		size_t i;

		const struct {
			int c;
			const char *s;
		} a[] = {
			{ '\\', "\\\\" },

			{ '^',  "\\^"  },
			{ '-',  "\\-"  },
			{ ']',  "\\]"  },
			{ '[',  "\\["  },

			{ '\f', "\\f"  },
			{ '\n', "\\n"  },
			{ '\r', "\\r"  },
			{ '\t', "\\t"  },
			{ '\v', "\\v"  }
		};

		assert(s != NULL);
		assert(sz >= 5);

		(void) sz;

		for (i = 0; i < sizeof a / sizeof *a; i++) {
			if (a[i].c == c) {
				return a[i].s;
			}
		}

		if (!isprint((unsigned char) c)) {
			sprintf(s, "\\x%02X", (unsigned char) c);
			return s;
		}

		sprintf(s, "%c", c);
		return s;
	}

	static int
	escputc(int c, FILE *f)
	{
		char s[5];

		return fputs(escchar(s, sizeof s, c), f);
	}

#line 207 "src/libre/dialect/native/parser.c"


#ifndef ERROR_TERMINAL
#error "-s no-numeric-terminals given and ERROR_TERMINAL is not defined"
#endif

/* BEGINNING OF FUNCTION DECLARATIONS */

static void p_anchor(fsm, flags, lex_state, act_state, err, t_fsm__state, t_fsm__state);
static void p_expr_C_Clist_Hof_Halts_C_Calt(fsm, flags, lex_state, act_state, err, t_fsm__state, t_fsm__state);
static void p_159(fsm, flags, lex_state, act_state, err, t_fsm__state, t_fsm__state, t_fsm__state *, t_fsm__state *);
static void p_164(fsm, flags, lex_state, act_state, err, t_grp *);
static void p_176(fsm, flags, lex_state, act_state, err, t_grp *, t_char *, t_pos *);
static void p_group(fsm, flags, lex_state, act_state, err, t_fsm__state, t_fsm__state);
static void p_180(fsm, flags, lex_state, act_state, err, t_fsm__state *, t_fsm__state *, t_pos *, t_unsigned *);
static void p_group_C_Cgroup_Hbody(fsm, flags, lex_state, act_state, err, t_grp *);
static void p_expr(fsm, flags, lex_state, act_state, err, t_fsm__state, t_fsm__state);
extern void p_re__native(fsm, flags, lex_state, act_state, err);
static void p_group_C_Clist_Hof_Hterms(fsm, flags, lex_state, act_state, err, t_grp *);
static void p_expr_C_Clist_Hof_Hatoms(fsm, flags, lex_state, act_state, err, t_fsm__state, t_fsm__state);
static void p_group_C_Cgroup_Hbm(fsm, flags, lex_state, act_state, err, t_grp *);
static void p_expr_C_Clist_Hof_Halts(fsm, flags, lex_state, act_state, err, t_fsm__state, t_fsm__state);
extern void p_group_Hnative(fsm, flags, lex_state, act_state, err);
static void p_expr_C_Catom(fsm, flags, lex_state, act_state, err, t_fsm__state, t_fsm__state *);
static void p_literal(fsm, flags, lex_state, act_state, err, t_fsm__state, t_fsm__state);

/* BEGINNING OF STATIC VARIABLES */


/* BEGINNING OF FUNCTION DEFINITIONS */

static void
p_anchor(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_fsm__state ZIx, t_fsm__state ZIy)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		t_pred ZIp;

		/* BEGINNING OF INLINE: 129 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_END):
				{
					/* BEGINNING OF EXTRACT: END */
					{
#line 355 "src/libre/parser.act"

		switch (flags & RE_ANCHOR) {
		case RE_TEXT:  ZIp = RE_EOT; break;
		case RE_MULTI: ZIp = RE_EOL; break;
		case RE_ZONE:  ZIp = RE_EOZ; break;

		default:
			/* TODO: raise error */
			ZIp = 0U;
		}
	
#line 267 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: END */
					ADVANCE_LEXER;
				}
				break;
			case (TOK_START):
				{
					/* BEGINNING OF EXTRACT: START */
					{
#line 343 "src/libre/parser.act"

		switch (flags & RE_ANCHOR) {
		case RE_TEXT:  ZIp = RE_SOT; break;
		case RE_MULTI: ZIp = RE_SOL; break;
		case RE_ZONE:  ZIp = RE_SOZ; break;

		default:
			/* TODO: raise error */
			ZIp = 0U;
		}
	
#line 289 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: START */
					ADVANCE_LEXER;
				}
				break;
			default:
				goto ZL1;
			}
		}
		/* END OF INLINE: 129 */
		/* BEGINNING OF ACTION: add-pred */
		{
#line 540 "src/libre/parser.act"

		assert((ZIx) != NULL);
		assert((ZIy) != NULL);

/* TODO:
		if (!fsm_addedge_predicate(fsm, (ZIx), (ZIy), (ZIp))) {
			goto ZL1;
		}
*/
	
#line 313 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: add-pred */
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

static void
p_expr_C_Clist_Hof_Halts_C_Calt(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_fsm__state ZIx, t_fsm__state ZIy)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		t_fsm__state ZIz;

		/* BEGINNING OF ACTION: add-concat */
		{
#line 527 "src/libre/parser.act"

		(ZIz) = fsm_addstate(fsm);
		if ((ZIz) == NULL) {
			goto ZL1;
		}
	
#line 341 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: add-concat */
		/* BEGINNING OF ACTION: add-epsilon */
		{
#line 534 "src/libre/parser.act"

		if (!fsm_addedge_epsilon(fsm, (ZIx), (ZIz))) {
			goto ZL1;
		}
	
#line 352 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: add-epsilon */
		p_expr_C_Clist_Hof_Hatoms (fsm, flags, lex_state, act_state, err, ZIz, ZIy);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		}
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

static void
p_159(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_fsm__state ZI155, t_fsm__state ZI156, t_fsm__state *ZO157, t_fsm__state *ZO158)
{
	t_fsm__state ZI157;
	t_fsm__state ZI158;

ZL2_159:;
	switch (CURRENT_TERMINAL) {
	case (TOK_ALT):
		{
			ADVANCE_LEXER;
			p_expr_C_Clist_Hof_Halts_C_Calt (fsm, flags, lex_state, act_state, err, ZI155, ZI156);
			/* BEGINNING OF INLINE: 159 */
			if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
				RESTORE_LEXER;
				goto ZL1;
			} else {
				goto ZL2_159;
			}
			/* END OF INLINE: 159 */
		}
		/* UNREACHED */
	default:
		{
			ZI157 = ZI155;
			ZI158 = ZI156;
		}
		break;
	case (ERROR_TERMINAL):
		return;
	}
	goto ZL0;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
ZL0:;
	*ZO157 = ZI157;
	*ZO158 = ZI158;
}

static void
p_164(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_grp *ZIg)
{
	switch (CURRENT_TERMINAL) {
	case (TOK_RANGE):
		{
			t_char ZIb;
			t_pos ZI105;
			t_pos ZI106;

			/* BEGINNING OF EXTRACT: RANGE */
			{
#line 232 "src/libre/parser.act"

		ZIb = '-';
		ZI105 = lex_state->lx.start;
		ZI106   = lex_state->lx.end;
	
#line 425 "src/libre/dialect/native/parser.c"
			}
			/* END OF EXTRACT: RANGE */
			ADVANCE_LEXER;
			/* BEGINNING OF ACTION: group-add-char */
			{
#line 454 "src/libre/parser.act"

		group_add((ZIg), (ZIb));
	
#line 435 "src/libre/dialect/native/parser.c"
			}
			/* END OF ACTION: group-add-char */
		}
		break;
	case (ERROR_TERMINAL):
		return;
	default:
		break;
	}
}

static void
p_176(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_grp *ZIg, t_char *ZI173, t_pos *ZI174)
{
	switch (CURRENT_TERMINAL) {
	case (TOK_RANGE):
		{
			t_char ZIb;
			t_pos ZIend;

			/* BEGINNING OF INLINE: 81 */
			{
				{
					t_char ZI82;
					t_pos ZI83;
					t_pos ZI84;

					switch (CURRENT_TERMINAL) {
					case (TOK_RANGE):
						/* BEGINNING OF EXTRACT: RANGE */
						{
#line 232 "src/libre/parser.act"

		ZI82 = '-';
		ZI83 = lex_state->lx.start;
		ZI84   = lex_state->lx.end;
	
#line 473 "src/libre/dialect/native/parser.c"
						}
						/* END OF EXTRACT: RANGE */
						break;
					default:
						goto ZL3;
					}
					ADVANCE_LEXER;
				}
				goto ZL2;
			ZL3:;
				{
					/* BEGINNING OF ACTION: err-expected-range */
					{
#line 724 "src/libre/parser.act"

		if (err->e == RE_ESUCCESS) {
			err->e = RE_EXRANGE;
		}
	
#line 493 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: err-expected-range */
				}
			ZL2:;
			}
			/* END OF INLINE: 81 */
			/* BEGINNING OF INLINE: 85 */
			{
				switch (CURRENT_TERMINAL) {
				case (TOK_CHAR):
					{
						t_pos ZI89;

						/* BEGINNING OF EXTRACT: CHAR */
						{
#line 335 "src/libre/parser.act"

		assert(lex_state->buf.a[0] != '\0');
		assert(lex_state->buf.a[1] == '\0');

		ZI89 = lex_state->lx.start;
		ZIend   = lex_state->lx.end;

		ZIb = lex_state->buf.a[0];
	
#line 519 "src/libre/dialect/native/parser.c"
						}
						/* END OF EXTRACT: CHAR */
						ADVANCE_LEXER;
					}
					break;
				case (TOK_HEX):
					{
						t_pos ZI88;

						/* BEGINNING OF EXTRACT: HEX */
						{
#line 310 "src/libre/parser.act"

		unsigned long u;
		char *e;

		assert(0 == strncmp(lex_state->buf.a, "\\x", 2));
		assert(strlen(lex_state->buf.a) >= 3);

		ZI88 = lex_state->lx.start;
		ZIend   = lex_state->lx.end;

		errno = 0;

		u = strtoul(lex_state->buf.a + 2, &e, 16);

		if ((u == ULONG_MAX && errno == ERANGE) || u > UCHAR_MAX) {
			err->e = RE_EHEXRANGE;
			snprintdots(err->esc, sizeof err->esc, lex_state->buf.a);
			goto ZL1;
		}

		if ((u == ULONG_MAX && errno != 0) || *e != '\0') {
			err->e = RE_EXESC;
			goto ZL1;
		}

		ZIb = (char) (unsigned char) u;
	
#line 559 "src/libre/dialect/native/parser.c"
						}
						/* END OF EXTRACT: HEX */
						ADVANCE_LEXER;
					}
					break;
				case (TOK_OCT):
					{
						t_pos ZI86;

						/* BEGINNING OF EXTRACT: OCT */
						{
#line 282 "src/libre/parser.act"

		unsigned long u;
		char *e;

		assert(0 == strncmp(lex_state->buf.a, "\\", 1));
		assert(strlen(lex_state->buf.a) >= 2);

		ZI86 = lex_state->lx.start;
		ZIend   = lex_state->lx.end;

		errno = 0;

		u = strtoul(lex_state->buf.a + 1, &e, 8);

		if ((u == ULONG_MAX && errno == ERANGE) || u > UCHAR_MAX) {
			err->e = RE_EOCTRANGE;
			snprintdots(err->esc, sizeof err->esc, lex_state->buf.a);
			goto ZL1;
		}

		if ((u == ULONG_MAX && errno != 0) || *e != '\0') {
			err->e = RE_EXESC;
			goto ZL1;
		}

		ZIb = (char) (unsigned char) u;
	
#line 599 "src/libre/dialect/native/parser.c"
						}
						/* END OF EXTRACT: OCT */
						ADVANCE_LEXER;
					}
					break;
				case (TOK_RANGE):
					{
						t_pos ZI90;

						/* BEGINNING OF EXTRACT: RANGE */
						{
#line 232 "src/libre/parser.act"

		ZIb = '-';
		ZI90 = lex_state->lx.start;
		ZIend   = lex_state->lx.end;
	
#line 617 "src/libre/dialect/native/parser.c"
						}
						/* END OF EXTRACT: RANGE */
						ADVANCE_LEXER;
					}
					break;
				default:
					goto ZL1;
				}
			}
			/* END OF INLINE: 85 */
			/* BEGINNING OF ACTION: mark-range */
			{
#line 766 "src/libre/parser.act"

		mark(&act_state->rangestart, &(*ZI174));
		mark(&act_state->rangeend,   &(ZIend));
	
#line 635 "src/libre/dialect/native/parser.c"
			}
			/* END OF ACTION: mark-range */
			/* BEGINNING OF ACTION: group-add-range */
			{
#line 460 "src/libre/parser.act"

		int i;

		if ((unsigned char) (ZIb) < (unsigned char) (*ZI173)) {
			char a[5], b[5];

			assert(sizeof err->set >= 1 + sizeof a + 1 + sizeof b + 1 + 1);

			sprintf(err->set, "[%s-%s]",
				escchar(a, sizeof a, (*ZI173)), escchar(b, sizeof b, (ZIb)));
			err->e = RE_ENEGRANGE;
			goto ZL1;
		}

		for (i = (unsigned char) (*ZI173); i <= (unsigned char) (ZIb); i++) {
			group_add((ZIg), (char) i);
		}
	
#line 659 "src/libre/dialect/native/parser.c"
			}
			/* END OF ACTION: group-add-range */
		}
		break;
	default:
		{
			/* BEGINNING OF ACTION: group-add-char */
			{
#line 454 "src/libre/parser.act"

		group_add((ZIg), (*ZI173));
	
#line 672 "src/libre/dialect/native/parser.c"
			}
			/* END OF ACTION: group-add-char */
		}
		break;
	case (ERROR_TERMINAL):
		return;
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

static void
p_group(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_fsm__state ZIx, t_fsm__state ZIy)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		t_grp ZIg;

		/* BEGINNING OF ACTION: make-group */
		{
#line 412 "src/libre/parser.act"

		bm_clear(&(ZIg).set);
		bm_clear(&(ZIg).dup);
	
#line 702 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: make-group */
		p_group_C_Cgroup_Hbm (fsm, flags, lex_state, act_state, err, &ZIg);
		/* BEGINNING OF INLINE: 117 */
		{
			switch (CURRENT_TERMINAL) {
			case (ERROR_TERMINAL):
				RESTORE_LEXER;
				goto ZL3;
			default:
				goto ZL3;
			}
			/* UNREACHED */
		ZL3:;
			{
				/* BEGINNING OF ACTION: err-expected-closegroup */
				{
#line 730 "src/libre/parser.act"

		if (err->e == RE_ESUCCESS) {
			err->e = RE_EXCLOSEGROUP;
		}
	
#line 726 "src/libre/dialect/native/parser.c"
				}
				/* END OF ACTION: err-expected-closegroup */
			}
		}
		/* END OF INLINE: 117 */
		/* BEGINNING OF ACTION: group-to-states */
		{
#line 479 "src/libre/parser.act"

		int i;

		if (!act_state->overlap && bm_count(&(&ZIg)->dup) > 0) {
			err->e  = RE_EOVERLAP;

			/* TODO: would like to show the original spelling verbatim, too */

			if (-1 == bm_snprint(&(&ZIg)->set, err->set, sizeof err->set, 1, escputc)) {
				err->e = RE_EERRNO;
				goto ZL1;
			}

			if (-1 == bm_snprint(&(&ZIg)->dup, err->dup, sizeof err->dup, 1, escputc)) {
				err->e = RE_EERRNO;
				goto ZL1;
			}

			goto ZL1;
		}

		/* TODO: eventually libfsm will provide a neater mechanism here */

		i = -1;
		while (i = (int) bm_next(&(&ZIg)->set, i, 1), i <= UCHAR_MAX) {
			if (!fsm_addedge_literal(fsm, (ZIx), (ZIy), (char) i)) {
				goto ZL1;
			}
		}
	
#line 765 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: group-to-states */
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

static void
p_180(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_fsm__state *ZIx, t_fsm__state *ZIy, t_pos *ZI177, t_unsigned *ZI179)
{
	switch (CURRENT_TERMINAL) {
	case (TOK_CLOSECOUNT):
		{
			t_pos ZI138;
			t_pos ZIend;

			/* BEGINNING OF EXTRACT: CLOSECOUNT */
			{
#line 254 "src/libre/parser.act"

		ZI138 = lex_state->lx.start;
		ZIend   = lex_state->lx.end;
	
#line 791 "src/libre/dialect/native/parser.c"
			}
			/* END OF EXTRACT: CLOSECOUNT */
			ADVANCE_LEXER;
			/* BEGINNING OF ACTION: mark-count */
			{
#line 771 "src/libre/parser.act"

		mark(&act_state->countstart, &(*ZI177));
		mark(&act_state->countend,   &(ZIend));
	
#line 802 "src/libre/dialect/native/parser.c"
			}
			/* END OF ACTION: mark-count */
			/* BEGINNING OF ACTION: count-m-to-n */
			{
#line 596 "src/libre/parser.act"

		unsigned i;
		struct fsm_state *a;
		struct fsm_state *b;

		if ((*ZI179) < (*ZI179)) {
			err->e = RE_ENEGCOUNT;
			err->m = (*ZI179);
			err->n = (*ZI179);
			goto ZL1;
		}

		if ((*ZI179) == 0) {
			if (!fsm_addedge_epsilon(fsm, (*ZIx), (*ZIy))) {
				goto ZL1;
			}
		}

		b = (*ZIy);

		for (i = 1; i < (*ZI179); i++) {
			a = fsm_state_duplicatesubgraphx(fsm, (*ZIx), &b);
			if (a == NULL) {
				goto ZL1;
			}

			/* TODO: could elide this epsilon if fsm_state_duplicatesubgraphx()
			 * took an extra parameter giving it a m->new for the start state */
			if (!fsm_addedge_epsilon(fsm, (*ZIy), a)) {
				goto ZL1;
			}

			if (i >= (*ZI179)) {
				if (!fsm_addedge_epsilon(fsm, (*ZIy), b)) {
					goto ZL1;
				}
			}

			(*ZIy) = b;
			(*ZIx) = a;
		}
	
#line 850 "src/libre/dialect/native/parser.c"
			}
			/* END OF ACTION: count-m-to-n */
		}
		break;
	case (TOK_SEP):
		{
			t_unsigned ZIn;
			t_pos ZIend;
			t_pos ZI141;

			ADVANCE_LEXER;
			switch (CURRENT_TERMINAL) {
			case (TOK_COUNT):
				/* BEGINNING OF EXTRACT: COUNT */
				{
#line 374 "src/libre/parser.act"

		unsigned long u;
		char *e;

		u = strtoul(lex_state->buf.a, &e, 10);

		if ((u == ULONG_MAX && errno == ERANGE) || u > UINT_MAX) {
			err->e = RE_ECOUNTRANGE;
			snprintdots(err->esc, sizeof err->esc, lex_state->buf.a);
			goto ZL1;
		}

		if ((u == ULONG_MAX && errno != 0) || *e != '\0') {
			err->e = RE_EXCOUNT;
			goto ZL1;
		}

		ZIn = (unsigned int) u;
	
#line 886 "src/libre/dialect/native/parser.c"
				}
				/* END OF EXTRACT: COUNT */
				break;
			default:
				goto ZL1;
			}
			ADVANCE_LEXER;
			switch (CURRENT_TERMINAL) {
			case (TOK_CLOSECOUNT):
				/* BEGINNING OF EXTRACT: CLOSECOUNT */
				{
#line 254 "src/libre/parser.act"

		ZIend = lex_state->lx.start;
		ZI141   = lex_state->lx.end;
	
#line 903 "src/libre/dialect/native/parser.c"
				}
				/* END OF EXTRACT: CLOSECOUNT */
				break;
			default:
				goto ZL1;
			}
			ADVANCE_LEXER;
			/* BEGINNING OF ACTION: mark-count */
			{
#line 771 "src/libre/parser.act"

		mark(&act_state->countstart, &(*ZI177));
		mark(&act_state->countend,   &(ZIend));
	
#line 918 "src/libre/dialect/native/parser.c"
			}
			/* END OF ACTION: mark-count */
			/* BEGINNING OF ACTION: count-m-to-n */
			{
#line 596 "src/libre/parser.act"

		unsigned i;
		struct fsm_state *a;
		struct fsm_state *b;

		if ((ZIn) < (*ZI179)) {
			err->e = RE_ENEGCOUNT;
			err->m = (*ZI179);
			err->n = (ZIn);
			goto ZL1;
		}

		if ((*ZI179) == 0) {
			if (!fsm_addedge_epsilon(fsm, (*ZIx), (*ZIy))) {
				goto ZL1;
			}
		}

		b = (*ZIy);

		for (i = 1; i < (ZIn); i++) {
			a = fsm_state_duplicatesubgraphx(fsm, (*ZIx), &b);
			if (a == NULL) {
				goto ZL1;
			}

			/* TODO: could elide this epsilon if fsm_state_duplicatesubgraphx()
			 * took an extra parameter giving it a m->new for the start state */
			if (!fsm_addedge_epsilon(fsm, (*ZIy), a)) {
				goto ZL1;
			}

			if (i >= (*ZI179)) {
				if (!fsm_addedge_epsilon(fsm, (*ZIy), b)) {
					goto ZL1;
				}
			}

			(*ZIy) = b;
			(*ZIx) = a;
		}
	
#line 966 "src/libre/dialect/native/parser.c"
			}
			/* END OF ACTION: count-m-to-n */
		}
		break;
	case (ERROR_TERMINAL):
		return;
	default:
		goto ZL1;
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

static void
p_group_C_Cgroup_Hbody(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_grp *ZIg)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		/* BEGINNING OF INLINE: 98 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_CLOSEGROUP):
				{
					t_char ZI161;
					t_pos ZI162;
					t_pos ZI163;

					/* BEGINNING OF EXTRACT: CLOSEGROUP */
					{
#line 243 "src/libre/parser.act"

		ZI161 = ']';
		ZI162 = lex_state->lx.start;
		ZI163   = lex_state->lx.end;
	
#line 1006 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: CLOSEGROUP */
					ADVANCE_LEXER;
					/* BEGINNING OF ACTION: group-add-char */
					{
#line 454 "src/libre/parser.act"

		group_add((ZIg), (ZI161));
	
#line 1016 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: group-add-char */
					p_164 (fsm, flags, lex_state, act_state, err, ZIg);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL1;
					}
				}
				break;
			case (TOK_RANGE):
				{
					t_char ZIc;
					t_pos ZI101;
					t_pos ZI102;

					/* BEGINNING OF EXTRACT: RANGE */
					{
#line 232 "src/libre/parser.act"

		ZIc = '-';
		ZI101 = lex_state->lx.start;
		ZI102   = lex_state->lx.end;
	
#line 1040 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: RANGE */
					ADVANCE_LEXER;
					/* BEGINNING OF ACTION: group-add-char */
					{
#line 454 "src/libre/parser.act"

		group_add((ZIg), (ZIc));
	
#line 1050 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: group-add-char */
				}
				break;
			default:
				break;
			}
		}
		/* END OF INLINE: 98 */
		p_group_C_Clist_Hof_Hterms (fsm, flags, lex_state, act_state, err, ZIg);
		/* BEGINNING OF INLINE: 107 */
		{
			if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
				RESTORE_LEXER;
				goto ZL1;
			}
		}
		/* END OF INLINE: 107 */
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

static void
p_expr(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_fsm__state ZIx, t_fsm__state ZIy)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		p_expr_C_Clist_Hof_Halts (fsm, flags, lex_state, act_state, err, ZIx, ZIy);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		}
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

void
p_re__native(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		t_fsm__state ZIx;
		t_fsm__state ZIy;

		/* BEGINNING OF ACTION: make-states */
		{
#line 399 "src/libre/parser.act"

		assert(fsm != NULL);
		/* TODO: assert fsm is empty */

		(ZIx) = fsm_getstart(fsm);
		assert((ZIx) != NULL);

		(ZIy) = fsm_addstate(fsm);
		if ((ZIy) == NULL) {
			goto ZL1;
		}

		fsm_setend(fsm, (ZIy), 1);
	
#line 1122 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: make-states */
		/* BEGINNING OF INLINE: 152 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_DOT): case (TOK_OPENSUB): case (TOK_OPENGROUP): case (TOK_ESC):
			case (TOK_OCT): case (TOK_HEX): case (TOK_CHAR): case (TOK_START):
			case (TOK_END):
				{
					p_expr (fsm, flags, lex_state, act_state, err, ZIx, ZIy);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL3;
					}
				}
				break;
			default:
				{
					/* BEGINNING OF ACTION: add-epsilon */
					{
#line 534 "src/libre/parser.act"

		if (!fsm_addedge_epsilon(fsm, (ZIx), (ZIy))) {
			goto ZL3;
		}
	
#line 1149 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: add-epsilon */
				}
				break;
			}
			goto ZL2;
		ZL3:;
			{
				/* BEGINNING OF ACTION: err-expected-alts */
				{
#line 718 "src/libre/parser.act"

		if (err->e == RE_ESUCCESS) {
			err->e = RE_EXALTS;
		}
	
#line 1166 "src/libre/dialect/native/parser.c"
				}
				/* END OF ACTION: err-expected-alts */
			}
		ZL2:;
		}
		/* END OF INLINE: 152 */
		/* BEGINNING OF INLINE: 153 */
		{
			{
				switch (CURRENT_TERMINAL) {
				case (TOK_EOF):
					break;
				default:
					goto ZL5;
				}
				ADVANCE_LEXER;
			}
			goto ZL4;
		ZL5:;
			{
				/* BEGINNING OF ACTION: err-expected-eof */
				{
#line 742 "src/libre/parser.act"

		if (err->e == RE_ESUCCESS) {
			err->e = RE_EXEOF;
		}
	
#line 1195 "src/libre/dialect/native/parser.c"
				}
				/* END OF ACTION: err-expected-eof */
			}
		ZL4:;
		}
		/* END OF INLINE: 153 */
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

static void
p_group_C_Clist_Hof_Hterms(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_grp *ZIg)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
ZL2_group_C_Clist_Hof_Hterms:;
	{
		/* BEGINNING OF INLINE: 94 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_CHAR):
				{
					t_char ZI173;
					t_pos ZI174;
					t_pos ZI175;

					/* BEGINNING OF EXTRACT: CHAR */
					{
#line 335 "src/libre/parser.act"

		assert(lex_state->buf.a[0] != '\0');
		assert(lex_state->buf.a[1] == '\0');

		ZI174 = lex_state->lx.start;
		ZI175   = lex_state->lx.end;

		ZI173 = lex_state->buf.a[0];
	
#line 1238 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: CHAR */
					ADVANCE_LEXER;
					p_176 (fsm, flags, lex_state, act_state, err, ZIg, &ZI173, &ZI174);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL4;
					}
				}
				break;
			case (TOK_ESC):
				{
					t_char ZIc;

					/* BEGINNING OF EXTRACT: ESC */
					{
#line 263 "src/libre/parser.act"

		assert(lex_state->buf.a[0] == '\\');
		assert(lex_state->buf.a[1] != '\0');
		assert(lex_state->buf.a[2] == '\0');

		ZIc = lex_state->buf.a[1];

		switch (ZIc) {
		case 'f': ZIc = '\f'; break;
		case 'n': ZIc = '\n'; break;
		case 'r': ZIc = '\r'; break;
		case 't': ZIc = '\t'; break;
		case 'v': ZIc = '\v'; break;
		default:             break;
		}
	
#line 1272 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: ESC */
					ADVANCE_LEXER;
					/* BEGINNING OF ACTION: group-add-char */
					{
#line 454 "src/libre/parser.act"

		group_add((ZIg), (ZIc));
	
#line 1282 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: group-add-char */
				}
				break;
			case (TOK_HEX):
				{
					t_char ZI169;
					t_pos ZI170;
					t_pos ZI171;

					/* BEGINNING OF EXTRACT: HEX */
					{
#line 310 "src/libre/parser.act"

		unsigned long u;
		char *e;

		assert(0 == strncmp(lex_state->buf.a, "\\x", 2));
		assert(strlen(lex_state->buf.a) >= 3);

		ZI170 = lex_state->lx.start;
		ZI171   = lex_state->lx.end;

		errno = 0;

		u = strtoul(lex_state->buf.a + 2, &e, 16);

		if ((u == ULONG_MAX && errno == ERANGE) || u > UCHAR_MAX) {
			err->e = RE_EHEXRANGE;
			snprintdots(err->esc, sizeof err->esc, lex_state->buf.a);
			goto ZL4;
		}

		if ((u == ULONG_MAX && errno != 0) || *e != '\0') {
			err->e = RE_EXESC;
			goto ZL4;
		}

		ZI169 = (char) (unsigned char) u;
	
#line 1323 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: HEX */
					ADVANCE_LEXER;
					p_176 (fsm, flags, lex_state, act_state, err, ZIg, &ZI169, &ZI170);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL4;
					}
				}
				break;
			case (TOK_OCT):
				{
					t_char ZI165;
					t_pos ZI166;
					t_pos ZI167;

					/* BEGINNING OF EXTRACT: OCT */
					{
#line 282 "src/libre/parser.act"

		unsigned long u;
		char *e;

		assert(0 == strncmp(lex_state->buf.a, "\\", 1));
		assert(strlen(lex_state->buf.a) >= 2);

		ZI166 = lex_state->lx.start;
		ZI167   = lex_state->lx.end;

		errno = 0;

		u = strtoul(lex_state->buf.a + 1, &e, 8);

		if ((u == ULONG_MAX && errno == ERANGE) || u > UCHAR_MAX) {
			err->e = RE_EOCTRANGE;
			snprintdots(err->esc, sizeof err->esc, lex_state->buf.a);
			goto ZL4;
		}

		if ((u == ULONG_MAX && errno != 0) || *e != '\0') {
			err->e = RE_EXESC;
			goto ZL4;
		}

		ZI165 = (char) (unsigned char) u;
	
#line 1370 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: OCT */
					ADVANCE_LEXER;
					p_176 (fsm, flags, lex_state, act_state, err, ZIg, &ZI165, &ZI166);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL4;
					}
				}
				break;
			default:
				goto ZL4;
			}
			goto ZL3;
		ZL4:;
			{
				/* BEGINNING OF ACTION: err-expected-term */
				{
#line 700 "src/libre/parser.act"

		if (err->e == RE_ESUCCESS) {
			err->e = RE_EXTERM;
		}
	
#line 1395 "src/libre/dialect/native/parser.c"
				}
				/* END OF ACTION: err-expected-term */
			}
		ZL3:;
		}
		/* END OF INLINE: 94 */
		/* BEGINNING OF INLINE: 95 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_ESC): case (TOK_OCT): case (TOK_HEX): case (TOK_CHAR):
				{
					/* BEGINNING OF INLINE: group::list-of-terms */
					goto ZL2_group_C_Clist_Hof_Hterms;
					/* END OF INLINE: group::list-of-terms */
				}
				/* UNREACHED */
			default:
				break;
			}
		}
		/* END OF INLINE: 95 */
	}
}

static void
p_expr_C_Clist_Hof_Hatoms(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_fsm__state ZIx, t_fsm__state ZIy)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
ZL2_expr_C_Clist_Hof_Hatoms:;
	{
		t_fsm__state ZIz;

		/* BEGINNING OF ACTION: add-concat */
		{
#line 527 "src/libre/parser.act"

		(ZIz) = fsm_addstate(fsm);
		if ((ZIz) == NULL) {
			goto ZL1;
		}
	
#line 1439 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: add-concat */
		/* BEGINNING OF INLINE: 145 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_START): case (TOK_END):
				{
					p_anchor (fsm, flags, lex_state, act_state, err, ZIx, ZIz);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL1;
					}
				}
				break;
			case (TOK_DOT): case (TOK_OPENSUB): case (TOK_OPENGROUP): case (TOK_ESC):
			case (TOK_OCT): case (TOK_HEX): case (TOK_CHAR):
				{
					p_expr_C_Catom (fsm, flags, lex_state, act_state, err, ZIx, &ZIz);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL1;
					}
				}
				break;
			default:
				goto ZL1;
			}
		}
		/* END OF INLINE: 145 */
		/* BEGINNING OF INLINE: 146 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_DOT): case (TOK_OPENSUB): case (TOK_OPENGROUP): case (TOK_ESC):
			case (TOK_OCT): case (TOK_HEX): case (TOK_CHAR): case (TOK_START):
			case (TOK_END):
				{
					/* BEGINNING OF INLINE: expr::list-of-atoms */
					ZIx = ZIz;
					goto ZL2_expr_C_Clist_Hof_Hatoms;
					/* END OF INLINE: expr::list-of-atoms */
				}
				/* UNREACHED */
			default:
				{
					/* BEGINNING OF ACTION: add-epsilon */
					{
#line 534 "src/libre/parser.act"

		if (!fsm_addedge_epsilon(fsm, (ZIz), (ZIy))) {
			goto ZL1;
		}
	
#line 1492 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: add-epsilon */
				}
				break;
			}
		}
		/* END OF INLINE: 146 */
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

static void
p_group_C_Cgroup_Hbm(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_grp *ZIg)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		t_pos ZIstart;
		t_pos ZI110;

		switch (CURRENT_TERMINAL) {
		case (TOK_OPENGROUP):
			/* BEGINNING OF EXTRACT: OPENGROUP */
			{
#line 238 "src/libre/parser.act"

		ZIstart = lex_state->lx.start;
		ZI110   = lex_state->lx.end;
	
#line 1526 "src/libre/dialect/native/parser.c"
			}
			/* END OF EXTRACT: OPENGROUP */
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		/* BEGINNING OF INLINE: 111 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_INVERT):
				{
					t_char ZI112;

					/* BEGINNING OF EXTRACT: INVERT */
					{
#line 228 "src/libre/parser.act"

		ZI112 = '^';
	
#line 1547 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: INVERT */
					ADVANCE_LEXER;
					p_group_C_Cgroup_Hbody (fsm, flags, lex_state, act_state, err, ZIg);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL1;
					}
					/* BEGINNING OF ACTION: invert-group */
					{
#line 421 "src/libre/parser.act"

		/*
		 * The bm struct is limited to 0..UCHAR_MAX only; special edges up to
		 * FSM_EDGE_MAX are not present, and so do not get inverted here.
		 */
		bm_invert(&(ZIg)->set);

		/*
		 * Note we don't invert the dup set here; duplicates are always
		 * kept in the positive.
		 */
	
#line 1571 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: invert-group */
				}
				break;
			case (TOK_CLOSEGROUP): case (TOK_RANGE): case (TOK_ESC): case (TOK_OCT):
			case (TOK_HEX): case (TOK_CHAR):
				{
					p_group_C_Cgroup_Hbody (fsm, flags, lex_state, act_state, err, ZIg);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL1;
					}
				}
				break;
			default:
				goto ZL1;
			}
		}
		/* END OF INLINE: 111 */
		/* BEGINNING OF INLINE: 113 */
		{
			{
				t_char ZI114;
				t_pos ZI115;
				t_pos ZIend;

				switch (CURRENT_TERMINAL) {
				case (TOK_CLOSEGROUP):
					/* BEGINNING OF EXTRACT: CLOSEGROUP */
					{
#line 243 "src/libre/parser.act"

		ZI114 = ']';
		ZI115 = lex_state->lx.start;
		ZIend   = lex_state->lx.end;
	
#line 1608 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: CLOSEGROUP */
					break;
				default:
					goto ZL4;
				}
				ADVANCE_LEXER;
				/* BEGINNING OF ACTION: mark-group */
				{
#line 761 "src/libre/parser.act"

		mark(&act_state->groupstart, &(ZIstart));
		mark(&act_state->groupend,   &(ZIend));
	
#line 1623 "src/libre/dialect/native/parser.c"
				}
				/* END OF ACTION: mark-group */
			}
			goto ZL3;
		ZL4:;
			{
				/* BEGINNING OF ACTION: err-expected-closegroup */
				{
#line 730 "src/libre/parser.act"

		if (err->e == RE_ESUCCESS) {
			err->e = RE_EXCLOSEGROUP;
		}
	
#line 1638 "src/libre/dialect/native/parser.c"
				}
				/* END OF ACTION: err-expected-closegroup */
			}
		ZL3:;
		}
		/* END OF INLINE: 113 */
	}
	return;
ZL1:;
	{
		/* BEGINNING OF ACTION: err-expected-groupbody */
		{
#line 736 "src/libre/parser.act"

		if (err->e == RE_ESUCCESS) {
			err->e = RE_EXGROUPBODY;
		}
	
#line 1657 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: err-expected-groupbody */
	}
}

static void
p_expr_C_Clist_Hof_Halts(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_fsm__state ZI155, t_fsm__state ZI156)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		t_fsm__state ZI157;
		t_fsm__state ZI158;

		p_expr_C_Clist_Hof_Halts_C_Calt (fsm, flags, lex_state, act_state, err, ZI155, ZI156);
		p_159 (fsm, flags, lex_state, act_state, err, ZI155, ZI156, &ZI157, &ZI158);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		}
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

void
p_group_Hnative(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		t_pos ZIstart;
		t_grp ZIg;
		t_pos ZIend;

		/* BEGINNING OF ACTION: mark-begin */
		{
#line 745 "src/libre/parser.act"

		(ZIstart).byte = 0;
		(ZIstart).line = 1;
		(ZIstart).col  = 1;
	
#line 1705 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: mark-begin */
		/* BEGINNING OF ACTION: make-group */
		{
#line 412 "src/libre/parser.act"

		bm_clear(&(ZIg).set);
		bm_clear(&(ZIg).dup);
	
#line 1715 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: make-group */
		p_group_C_Cgroup_Hbm (fsm, flags, lex_state, act_state, err, &ZIg);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		}
		/* BEGINNING OF ACTION: mark-end */
		{
#line 755 "src/libre/parser.act"

		/*
		 * Assuming <mark-end> is called when there are no more tokens,
		 * the end of the lexical stream is the end of the current token.
		 */
		(ZIend).byte = act_state->synend.byte;
		(ZIend).line = 0; /* dummy */
		(ZIend).col  = 0; /* dummy */
	
#line 1735 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: mark-end */
		/* BEGINNING OF ACTION: mark-group */
		{
#line 761 "src/libre/parser.act"

		mark(&act_state->groupstart, &(ZIstart));
		mark(&act_state->groupend,   &(ZIend));
	
#line 1745 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: mark-group */
		/* BEGINNING OF ACTION: stash-group */
		{
#line 432 "src/libre/parser.act"

#define PARSE_GROUP

		if (!act_state->overlap && bm_count(&(&ZIg)->dup) > 0) {
			err->e  = RE_EOVERLAP;

			/* TODO: would like to show the original spelling verbatim, too */

			if (-1 == bm_snprint(&(&ZIg)->set, err->set, sizeof err->set, 1, escputc)) {
				err->e = RE_EERRNO;
				goto ZL1;
			}

			if (-1 == bm_snprint(&(&ZIg)->dup, err->dup, sizeof err->dup, 1, escputc)) {
				err->e = RE_EERRNO;
				goto ZL1;
			}

			goto ZL1;
		}

		*act_state->g = *(&ZIg);
	
#line 1774 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: stash-group */
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

static void
p_expr_C_Catom(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_fsm__state ZIx, t_fsm__state *ZIy)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		/* BEGINNING OF INLINE: 134 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_DOT):
				{
					ADVANCE_LEXER;
					/* BEGINNING OF ACTION: add-any */
					{
#line 562 "src/libre/parser.act"

		assert((ZIx) != NULL);
		assert((*ZIy) != NULL);

		if (!fsm_addedge_any(fsm, (ZIx), (*ZIy))) {
			goto ZL1;
		}
	
#line 1808 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: add-any */
				}
				break;
			case (TOK_OPENSUB):
				{
					ADVANCE_LEXER;
					p_expr (fsm, flags, lex_state, act_state, err, ZIx, *ZIy);
					switch (CURRENT_TERMINAL) {
					case (TOK_CLOSESUB):
						break;
					case (ERROR_TERMINAL):
						RESTORE_LEXER;
						goto ZL1;
					default:
						goto ZL1;
					}
					ADVANCE_LEXER;
				}
				break;
			case (TOK_OPENGROUP):
				{
					p_group (fsm, flags, lex_state, act_state, err, ZIx, *ZIy);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL1;
					}
				}
				break;
			case (TOK_ESC): case (TOK_OCT): case (TOK_HEX): case (TOK_CHAR):
				{
					p_literal (fsm, flags, lex_state, act_state, err, ZIx, *ZIy);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL1;
					}
				}
				break;
			default:
				goto ZL1;
			}
		}
		/* END OF INLINE: 134 */
		/* BEGINNING OF INLINE: 135 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_OPENCOUNT):
				{
					t_pos ZI177;
					t_pos ZI178;
					t_unsigned ZI179;

					/* BEGINNING OF EXTRACT: OPENCOUNT */
					{
#line 249 "src/libre/parser.act"

		ZI177 = lex_state->lx.start;
		ZI178   = lex_state->lx.end;
	
#line 1868 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: OPENCOUNT */
					ADVANCE_LEXER;
					switch (CURRENT_TERMINAL) {
					case (TOK_COUNT):
						/* BEGINNING OF EXTRACT: COUNT */
						{
#line 374 "src/libre/parser.act"

		unsigned long u;
		char *e;

		u = strtoul(lex_state->buf.a, &e, 10);

		if ((u == ULONG_MAX && errno == ERANGE) || u > UINT_MAX) {
			err->e = RE_ECOUNTRANGE;
			snprintdots(err->esc, sizeof err->esc, lex_state->buf.a);
			goto ZL4;
		}

		if ((u == ULONG_MAX && errno != 0) || *e != '\0') {
			err->e = RE_EXCOUNT;
			goto ZL4;
		}

		ZI179 = (unsigned int) u;
	
#line 1896 "src/libre/dialect/native/parser.c"
						}
						/* END OF EXTRACT: COUNT */
						break;
					default:
						goto ZL4;
					}
					ADVANCE_LEXER;
					p_180 (fsm, flags, lex_state, act_state, err, &ZIx, ZIy, &ZI177, &ZI179);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL4;
					}
				}
				break;
			case (TOK_PLUS):
				{
					ADVANCE_LEXER;
					/* BEGINNING OF ACTION: count-1-or-many */
					{
#line 668 "src/libre/parser.act"

		if (!fsm_addedge_epsilon(fsm, (*ZIy), (ZIx))) {
			goto ZL4;
		}

		/* isolation guard */
		/* TODO: centralise */
		{
			struct fsm_state *z;

			z = fsm_addstate(fsm);
			if (z == NULL) {
				goto ZL4;
			}

			if (!fsm_addedge_epsilon(fsm, (*ZIy), z)) {
				goto ZL4;
			}

			(*ZIy) = z;
		}
	
#line 1939 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: count-1-or-many */
				}
				break;
			case (TOK_QMARK):
				{
					ADVANCE_LEXER;
					/* BEGINNING OF ACTION: count-0-or-1 */
					{
#line 635 "src/libre/parser.act"

		if (!fsm_addedge_epsilon(fsm, (ZIx), (*ZIy))) {
			goto ZL4;
		}
	
#line 1955 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: count-0-or-1 */
				}
				break;
			case (TOK_STAR):
				{
					ADVANCE_LEXER;
					/* BEGINNING OF ACTION: count-0-or-many */
					{
#line 641 "src/libre/parser.act"

		if (!fsm_addedge_epsilon(fsm, (ZIx), (*ZIy))) {
			goto ZL4;
		}

		if (!fsm_addedge_epsilon(fsm, (*ZIy), (ZIx))) {
			goto ZL4;
		}

		/* isolation guard */
		/* TODO: centralise */
		{
			struct fsm_state *z;

			z = fsm_addstate(fsm);
			if (z == NULL) {
				goto ZL4;
			}

			if (!fsm_addedge_epsilon(fsm, (*ZIy), z)) {
				goto ZL4;
			}

			(*ZIy) = z;
		}
	
#line 1992 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: count-0-or-many */
				}
				break;
			default:
				{
					/* BEGINNING OF ACTION: count-1 */
					{
#line 691 "src/libre/parser.act"

		(void) (ZIx);
		(void) (*ZIy);
	
#line 2006 "src/libre/dialect/native/parser.c"
					}
					/* END OF ACTION: count-1 */
				}
				break;
			}
			goto ZL3;
		ZL4:;
			{
				/* BEGINNING OF ACTION: err-expected-count */
				{
#line 706 "src/libre/parser.act"

		if (err->e == RE_ESUCCESS) {
			err->e = RE_EXCOUNT;
		}
	
#line 2023 "src/libre/dialect/native/parser.c"
				}
				/* END OF ACTION: err-expected-count */
			}
		ZL3:;
		}
		/* END OF INLINE: 135 */
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

static void
p_literal(fsm fsm, flags flags, lex_state lex_state, act_state act_state, err err, t_fsm__state ZIx, t_fsm__state ZIy)
{
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		t_char ZIc;

		/* BEGINNING OF INLINE: 121 */
		{
			switch (CURRENT_TERMINAL) {
			case (TOK_CHAR):
				{
					t_pos ZI126;
					t_pos ZI127;

					/* BEGINNING OF EXTRACT: CHAR */
					{
#line 335 "src/libre/parser.act"

		assert(lex_state->buf.a[0] != '\0');
		assert(lex_state->buf.a[1] == '\0');

		ZI126 = lex_state->lx.start;
		ZI127   = lex_state->lx.end;

		ZIc = lex_state->buf.a[0];
	
#line 2066 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: CHAR */
					ADVANCE_LEXER;
				}
				break;
			case (TOK_ESC):
				{
					/* BEGINNING OF EXTRACT: ESC */
					{
#line 263 "src/libre/parser.act"

		assert(lex_state->buf.a[0] == '\\');
		assert(lex_state->buf.a[1] != '\0');
		assert(lex_state->buf.a[2] == '\0');

		ZIc = lex_state->buf.a[1];

		switch (ZIc) {
		case 'f': ZIc = '\f'; break;
		case 'n': ZIc = '\n'; break;
		case 'r': ZIc = '\r'; break;
		case 't': ZIc = '\t'; break;
		case 'v': ZIc = '\v'; break;
		default:             break;
		}
	
#line 2093 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: ESC */
					ADVANCE_LEXER;
				}
				break;
			case (TOK_HEX):
				{
					t_pos ZI124;
					t_pos ZI125;

					/* BEGINNING OF EXTRACT: HEX */
					{
#line 310 "src/libre/parser.act"

		unsigned long u;
		char *e;

		assert(0 == strncmp(lex_state->buf.a, "\\x", 2));
		assert(strlen(lex_state->buf.a) >= 3);

		ZI124 = lex_state->lx.start;
		ZI125   = lex_state->lx.end;

		errno = 0;

		u = strtoul(lex_state->buf.a + 2, &e, 16);

		if ((u == ULONG_MAX && errno == ERANGE) || u > UCHAR_MAX) {
			err->e = RE_EHEXRANGE;
			snprintdots(err->esc, sizeof err->esc, lex_state->buf.a);
			goto ZL1;
		}

		if ((u == ULONG_MAX && errno != 0) || *e != '\0') {
			err->e = RE_EXESC;
			goto ZL1;
		}

		ZIc = (char) (unsigned char) u;
	
#line 2134 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: HEX */
					ADVANCE_LEXER;
				}
				break;
			case (TOK_OCT):
				{
					t_pos ZI122;
					t_pos ZI123;

					/* BEGINNING OF EXTRACT: OCT */
					{
#line 282 "src/libre/parser.act"

		unsigned long u;
		char *e;

		assert(0 == strncmp(lex_state->buf.a, "\\", 1));
		assert(strlen(lex_state->buf.a) >= 2);

		ZI122 = lex_state->lx.start;
		ZI123   = lex_state->lx.end;

		errno = 0;

		u = strtoul(lex_state->buf.a + 1, &e, 8);

		if ((u == ULONG_MAX && errno == ERANGE) || u > UCHAR_MAX) {
			err->e = RE_EOCTRANGE;
			snprintdots(err->esc, sizeof err->esc, lex_state->buf.a);
			goto ZL1;
		}

		if ((u == ULONG_MAX && errno != 0) || *e != '\0') {
			err->e = RE_EXESC;
			goto ZL1;
		}

		ZIc = (char) (unsigned char) u;
	
#line 2175 "src/libre/dialect/native/parser.c"
					}
					/* END OF EXTRACT: OCT */
					ADVANCE_LEXER;
				}
				break;
			default:
				goto ZL1;
			}
		}
		/* END OF INLINE: 121 */
		/* BEGINNING OF ACTION: add-literal */
		{
#line 551 "src/libre/parser.act"

		assert((ZIx) != NULL);
		assert((ZIy) != NULL);

		/* TODO: check c is legal? */

		if (!fsm_addedge_literal(fsm, (ZIx), (ZIy), (ZIc))) {
			goto ZL1;
		}
	
#line 2199 "src/libre/dialect/native/parser.c"
		}
		/* END OF ACTION: add-literal */
	}
	return;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
}

/* BEGINNING OF TRAILER */

#line 1010 "src/libre/parser.act"


	static int
	lgetc(struct LX_STATE *lx)
	{
		struct lex_state *lex_state;
		int c;

		assert(lx != NULL);
		assert(lx->opaque != NULL);

		lex_state = lx->opaque;

		assert(lex_state->f != NULL);

		/* TODO: document character-shoving FSM */
		/* XXX: pos will be wrong! decrement it. as if our shoved character didn't exist */

		switch (lex_state->shove) {
		case SHOVE_PRE:
			return lex_state->shove = SHOVE_MID, '[';

		case SHOVE_MID:
			c = lex_state->f(lex_state->opaque);
			if (c == EOF) {
				return lex_state->shove = SHOVE_NONE, ']';
			}
			return c;

		case SHOVE_NONE:
			return lex_state->f(lex_state->opaque);
		}

	}

	static int
	parse(int (*f)(void *opaque), void *opaque,
		void (*entry)(struct fsm *, flags, lex_state, act_state, err),
		enum re_flags flags, int overlap,
		struct fsm *new, struct re_err *err, t_grp *g)
	{
		struct act_state  act_state_s;
		struct act_state *act_state;
		struct lex_state  lex_state_s;
		struct lex_state *lex_state;
		struct re_err dummy;

		struct LX_STATE *lx;

		assert(f != NULL);
		assert(entry != NULL);

		if (err == NULL) {
			err = &dummy;
		}

		lex_state    = &lex_state_s;
		lex_state->p = lex_state->a;

		lx = &lex_state->lx;

		LX_INIT(lx);

		lx->lgetc  = lgetc;
		lx->opaque = lex_state;

		lex_state->f       = f;
		lex_state->opaque  = opaque;

#ifdef PARSE_GROUP
/* XXX: information leak; i don't like breaking abstraction here. maybe pass in shove instead */
/* or: store start and end pointers as char *, and make an action: <shove>("[", "]"); */
		if (entry == DIALECT_GROUP_ENTRY) {
			lex_state->shove = SHOVE_PRE;
		} else
#endif
		{
			lex_state->shove = SHOVE_NONE;
		}

		lex_state->buf.a   = NULL;
		lex_state->buf.len = 0;

		/* XXX: unneccessary since we're lexing from a string */
		/* (except for pushing "[" and "]" around ::group-$dialect) */
		lx->buf   = &lex_state->buf;
		lx->push  = CAT(LX_PREFIX, _dynpush);
		lx->pop   = CAT(LX_PREFIX, _dynpop);
		lx->clear = CAT(LX_PREFIX, _dynclear);
		lx->free  = CAT(LX_PREFIX, _dynfree);

	/* XXX */
	lx->free = NULL;

		/* This is a workaround for ADVANCE_LEXER assuming a pointer */
		act_state = &act_state_s;
		act_state->g = g;

		act_state->overlap = overlap;

		err->e = RE_ESUCCESS;

		ADVANCE_LEXER;
		entry(new, flags, lex_state, act_state, err);

		if (err->e != RE_ESUCCESS) {
			/* TODO: free internals allocated during parsing (are there any?) */
			goto error;
		}

		return 0;

	error:

		assert(err->e & RE_MARK);

		/*
		 * Some errors describe multiple tokens; for these, the start and end
		 * positions belong to potentially different tokens, and therefore need
		 * to be stored statefully (in act_state). These are all from
		 * non-recursive productions by design, and so a stack isn't needed.
		 *
		 * Lexical errors describe a problem with a single token; for these,
		 * the start and end positions belong to that token.
		 *
		 * Syntax errors occur at the first point the order of tokens is known
		 * to be incorrect, rather than describing a span of bytes. For these,
		 * the start of the next token is most relevant.
		 */

		switch (err->e) {
		case RE_EOVERLAP:  err->start = act_state->groupstart; err->end = act_state->groupend; break;
		case RE_ENEGRANGE: err->start = act_state->rangestart; err->end = act_state->rangeend; break;
		case RE_ENEGCOUNT: err->start = act_state->countstart; err->end = act_state->countend; break;

		case RE_EHEXRANGE:
		case RE_EOCTRANGE:
		case RE_ECOUNTRANGE:
			/*
			 * Lexical errors: These are always generated for the current token,
			 * so lx->start/end here is correct because ADVANCE_LEXER has
			 * not been called.
			 */
			mark(&err->start, &lx->start);
			mark(&err->end,   &lx->end);
			break;

		default:
			/*
			 * Due to LL(1) lookahead, lx->start/end is the next token.
			 * This is approximately correct as the position of an error,
			 * but to be exactly correct, we store the pos for the previous token.
			 * This is more visible when whitespace exists.
			 */
			err->start = act_state->synstart;
			err->end   = act_state->synstart; /* single point */
			break;
		}

		return -1;
	}

	struct fsm *
	DIALECT_COMP(int (*f)(void *opaque), void *opaque,
		enum re_flags flags, int overlap,
		struct re_err *err)
	{
		struct fsm *new;
		struct fsm_state *start;

		assert(f != NULL);

		new = fsm_new();
		if (new == NULL) {
			return NULL;
		}

		start = fsm_addstate(new);
		if (start == NULL) {
			fsm_free(new);
			goto error;
		}

		fsm_setstart(new, start);

		if (-1 == parse(f, opaque, DIALECT_ENTRY, flags, overlap, new, err, NULL)) {
			fsm_free(new);
			return NULL;
		}

		return new;

	error:

		err->e = RE_EERRNO;

		return NULL;
	}

#ifdef PARSE_GROUP
	int
	DIALECT_GROUP(int (*f)(void *opaque), void *opaque,
		enum re_flags flags, int overlap,
		struct re_err *err, struct re_grp *g)
	{
		assert(f != NULL);
		assert(g != NULL);

		/*
		 * The ::group-$dialect production provides an entry point to the
		 * grammar where just a group is parsed and output as a bitmap,
		 * without constructing an NFA edge for each entry in the set.
		 *
		 * We use the same parse() function here so as to use the same
		 * act/lex_state setup as usual. The problem is that parse() expects
		 * the entry() callback to take no extra parameters, and so the
		 * ::group-$dialect production cannot output a :grp type. In other words,
		 * its generated function must have the same prototype as the usual
		 * grammar entry point, which of course does not output a :grp.
		 *
		 * So, we have a hacky workaround where ::group-$dialect calls a
		 * <stash-group> action which stashes the bitmap in act_state->g,
		 * which is set here to point to the caller's storage for g.
		 * This relies on the fact that ::group-$dialect need only store
		 * one group at a time, which suits us fine.
		 */

		if (-1 == parse(f, opaque, DIALECT_GROUP_ENTRY, flags, overlap, NULL, err, g)) {
			return -1;
		}

		return 0;
	}
#endif

#line 2447 "src/libre/dialect/native/parser.c"

/* END OF FILE */
