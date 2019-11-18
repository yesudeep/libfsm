/* Generated by libfsm */

#include LF_HEADER

#include <assert.h>
#include <stddef.h>

#include <fsm/fsm.h>

int
class_any_fsm(struct fsm *fsm, struct fsm_state *x, struct fsm_state *y)
{
	struct fsm_state *s[2];
	size_t i;

	assert(x != NULL);
	assert(y != NULL);

	for (i = 0; i < 2; i++) {
		if (i == 0) {
			s[0] = x;
			continue;
		}

		if (i == 1) {
			s[1] = y;
			continue;
		}

		s[i] = fsm_addstate(fsm);
		if (s[i] == NULL) {
			return 0;
		}
	}

	if (!fsm_addedge_any(fsm, s[0], s[1])) { return 0; }


	return 1;
}

