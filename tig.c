#ifdef HAVE_NCURSESW_NCURSES_H
#include <ncursesw/ncurses.h>
#else
#ifdef HAVE_NCURSES_NCURSES_H
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif
#endif
static bool prompt_yesno(const char *prompt);
#define SIZEOF_REV	41	/* Holds a SHA-1 and an ending NUL. */
#define SIZEOF_ARG	32	/* Default argument array size. */
#define GIT_CONFIG "config"
	"git ls-remote . 2>/dev/null"
#define TIG_MAIN_BASE \
	"git log --no-color --pretty=raw --parents --topo-order"

	TIG_MAIN_BASE " %s 2>/dev/null"
	REQ_(STATUS_CHECKOUT,	"Checkout file"), \
	REQ_(STAGE_NEXT,	"Find next chunk to stage"), \
static int opt_author_cols		= AUTHOR_COLS-1;
static enum request
	enum request request = REQ_VIEW_MAIN;
		return REQ_VIEW_PAGER;
		return REQ_VIEW_MAIN;
		return REQ_VIEW_STATUS;
		return REQ_VIEW_BLAME;
		request = REQ_VIEW_DIFF;
		request = subcommand[0] == 'l' ? REQ_VIEW_LOG : REQ_VIEW_DIFF;
		string_copy(opt_cmd, TIG_MAIN_BASE);
			return REQ_NONE;
			return REQ_NONE;
	return request;
get_line_type(const char *line)
get_line_info(const char *name)
	{ '!',		REQ_STATUS_CHECKOUT },
	{ '@',		REQ_STAGE_NEXT },
add_run_request(enum keymap keymap, int key, int argc, const char **argv)
	struct run_request *req;
	char cmd[SIZEOF_STR];
		if (!string_format_from(cmd, &bufpos, "%s ", *argv))
	req = realloc(run_request, (run_requests + 1) * sizeof(*run_request));
	if (!req)
	run_request = req;
	req = &run_request[run_requests++];
	string_copy(req->cmd, cmd);
	req->keymap = keymap;
	req->key = key;
		const char *argv[1];
option_color_command(int argc, const char *argv[])
static int
parse_int(const char *s, int default_value, int min, int max)
{
	int value = atoi(s);

	return (value < min || value > max) ? default_value : value;
}

option_set_command(int argc, const char *argv[])
		opt_num_interval = parse_int(argv[2], opt_num_interval, 1, 1024);
		return OK;
	}

	if (!strcmp(argv[0], "author-width")) {
		opt_author_cols = parse_int(argv[2], opt_author_cols, 0, 1024);
		opt_tab_size = parse_int(argv[2], opt_tab_size, 1, 1024);
		const char *arg = argv[2];
		int arglen = strlen(arg);
		switch (arg[0]) {
			if (arglen == 1 || arg[arglen - 1] != arg[0]) {
				config_msg = "Unmatched quotation";
				return ERR;
			}
			arg += 1; arglen -= 2;
option_bind_command(int argc, const char *argv[])
	const char *argv[SIZEOF_ARG];
	unsigned long col;	/* Column when drawing. */
	LINE_GRAPHIC_VLINE
draw_chars(struct view *view, enum line_type type, const char *string,
	   int max_len, bool use_tilde)
		col += draw_chars(view, type, space, spaces, FALSE);
static bool
draw_lineno(struct view *view, unsigned int lineno)
	char number[10];
	int max = view->width - view->col;
	if (max < max_number)
		max_number = max;

		static char fmt[] = "%1ld";

		col = draw_chars(view, LINE_LINE_NUMBER, number, max_number, TRUE);
	} else {
		col = draw_space(view, LINE_LINE_NUMBER, max_number, max_number);
	if (col < max)
		col += draw_space(view, LINE_DEFAULT, max - col, 1);
	view->col += col;

	return view->width - view->col <= 0;
static bool
draw_text(struct view *view, enum line_type type, const char *string, bool trim)
{
	view->col += draw_chars(view, type, string, view->width - view->col, trim);
	return view->width - view->col <= 0;
}

static bool
draw_graphic(struct view *view, enum line_type type, chtype graphic[], size_t size)
	int max = view->width - view->col;
	view->col += size;
		view->col++;
	return view->width - view->col <= 0;
static bool
draw_field(struct view *view, enum line_type type, char *text, int len, bool trim)
	int max = MIN(view->width - view->col, len);
		col = draw_chars(view, type, text, max - 1, trim);
	view->col += col + draw_space(view, LINE_DEFAULT, max - col, max - col);
	return view->width - view->col <= 0;
static bool
draw_date(struct view *view, struct tm *time)
	return draw_field(view, LINE_DATE, date, DATE_COLS, FALSE);
	view->col = 0;
end_update(struct view *view, bool force)
	while (!view->ops->read(view, NULL))
		if (!force)
			return;
	if (ferror(view->pipe) && errno != 0) {
		end_update(view, TRUE);
		end_update(view, FALSE);
	end_update(view, TRUE);
	OPEN_NOMAXIMIZE = 8	/* Do not maximize the current view. */
	if (view->pipe)
		end_update(view, TRUE);

static bool
run_confirm(const char *cmd, const char *prompt)
{
	bool confirmation = prompt_yesno(prompt);

	if (confirmation)
		system(cmd);

	return confirmation;
}

		    view == VIEW(REQ_VIEW_MAIN) ||
			end_update(view, TRUE);
	if (opt_line_number && draw_lineno(view, lineno))
		return TRUE;
	draw_text(view, line->type, text, TRUE);
	if (opt_date && draw_date(view, time))
		return TRUE;
	if (opt_author &&
	    draw_field(view, LINE_MAIN_AUTHOR, author, opt_author_cols, TRUE))
		return TRUE;
	if (draw_field(view, LINE_BLAME_ID, id, ID_COLS, FALSE))
		return TRUE;
	if (draw_lineno(view, lineno))
	draw_text(view, LINE_DEFAULT, blame->text, TRUE);
static size_t stage_chunks;
static int *stage_chunk;

/* This should work even for the "On branch" line. */
static inline bool
status_has_none(struct view *view, struct line *line)
{
	return line < view->line + view->lines && !line[1].data;
}
	"git ls-files -z --others --exclude-standard"
	"git ls-files -z --cached --exclude-standard"
	system("git update-index -q --refresh >/dev/null 2>/dev/null");
	if (opt_no_head &&
	    !status_run(view, STATUS_LIST_NO_HEAD_CMD, 'A', LINE_STAT_STAGED))
		return FALSE;
	else if (!status_run(view, STATUS_DIFF_INDEX_CMD, 0, LINE_STAT_STAGED))
	if (!status_run(view, STATUS_DIFF_FILES_CMD, 0, LINE_STAT_UNSTAGED) ||
	    !status_run(view, STATUS_LIST_OTHER_CMD, '?', LINE_STAT_UNTRACKED))
		static char buf[] = { '?', ' ', ' ', ' ', 0 };
		buf[0] = status->status;
		if (draw_text(view, line->type, buf, TRUE))
			return TRUE;
	draw_text(view, type, text, TRUE);
		stage_chunks = 0;
static bool
status_checkout(struct status *status, enum line_type type, bool has_next)
{
	if (!status || type != LINE_STAT_UNSTAGED) {
		if (has_next) {
			report("Nothing to checkout");
		} else if (type == LINE_STAT_UNTRACKED) {
			report("Cannot checkout untracked files");
		} else if (type == LINE_STAT_STAGED) {
			report("Cannot checkout staged files");
		} else {
			report("Cannot checkout multiple files");
		}
		return FALSE;

	} else {
		char cmd[SIZEOF_STR];
		char file_sq[SIZEOF_STR];

		if (sq_quote(file_sq, 0, status->old.name) >= sizeof(file_sq) ||
		    !string_format(cmd, "git checkout %s%s", opt_cdup, file_sq))
			return FALSE;

		return run_confirm(cmd, "Are you sure you want to overwrite any changes?");
	}
}

	case REQ_STATUS_CHECKOUT:
		if (!status_checkout(status, line->type, status_has_none(view, line)))
			return REQ_NONE;
		break;

static void
stage_next(struct view *view, struct line *line)
{
	int i;

	if (!stage_chunks) {
		static size_t alloc = 0;
		int *tmp;

		for (line = view->line; line < view->line + view->lines; line++) {
			if (line->type != LINE_DIFF_CHUNK)
				continue;

			tmp = realloc_items(stage_chunk, &alloc,
					    stage_chunks, sizeof(*tmp));
			if (!tmp) {
				report("Allocation failure");
				return;
			}

			stage_chunk = tmp;
			stage_chunk[stage_chunks++] = line - view->line;
		}
	}

	for (i = 0; i < stage_chunks; i++) {
		if (stage_chunk[i] > view->lineno) {
			do_scroll_view(view, stage_chunk[i] - view->lineno);
			report("Chunk %d of %d", i + 1, stage_chunks);
			return;
		}
	}

	report("No next chunk found");
}

	case REQ_STATUS_CHECKOUT:
		if (!status_checkout(&stage_status, stage_line_type, FALSE))
			return REQ_NONE;
		break;

	case REQ_STAGE_NEXT:
		if (stage_line_type == LINE_STAT_UNTRACKED) {
			report("File is untracked; press %s to add",
			       get_key(REQ_STATUS_UPDATE));
			return REQ_NONE;
		}
		stage_next(view, line);
		return REQ_NONE;

static void
clear_rev_graph(struct rev_graph *graph)
{
	graph->boundary = 0;
	graph->size = graph->pos = 0;
	graph->commit = NULL;
	memset(graph->parents, 0, sizeof(*graph->parents));
}

	clear_rev_graph(graph);
static int load_refs(void);

	if (opt_date && draw_date(view, &commit->time))
		return TRUE;
	if (opt_author &&
	    draw_field(view, LINE_MAIN_AUTHOR, commit->author, opt_author_cols, TRUE))
		return TRUE;
	if (opt_rev_graph && commit->graph_size &&
	    draw_graphic(view, LINE_MAIN_REVGRAPH, commit->graph, commit->graph_size))
		return TRUE;
			if (draw_text(view, type, "[", TRUE) ||
			    draw_text(view, type, commit->refs[i]->name, TRUE) ||
			    draw_text(view, type, "]", TRUE))
				return TRUE;
			if (draw_text(view, LINE_DEFAULT, " ", TRUE))
	draw_text(view, LINE_DEFAULT, commit->title, TRUE);
		int i;

		if (view->lines > 0) {
			commit = view->line[view->lines - 1].data;
			if (!*commit->author) {
				view->lines--;
				free(commit);
				graph->commit = NULL;
			}
		}

		for (i = 0; i < ARRAY_SIZE(graph_stacks); i++)
			clear_rev_graph(&graph_stacks[i]);
	switch (request) {
	case REQ_ENTER:
		break;
	case REQ_REFRESH:
		load_refs();
		string_copy(opt_cmd, view->cmd);
		open_view(view, REQ_VIEW_MAIN, OPEN_RELOAD);
		break;
	default:
	}
static bool
prompt_yesno(const char *prompt)
{
	enum { WAIT, STOP, CANCEL  } status = WAIT;
	bool answer = FALSE;

	while (status == WAIT) {
		struct view *view;
		int i, key;

		input_mode = TRUE;

		foreach_view (view, i)
			update_view(view);

		input_mode = FALSE;

		mvwprintw(status_win, 0, 0, "%s [Yy]/[Nn]", prompt);
		wclrtoeol(status_win);

		/* Refresh, accept single keystroke of input */
		key = wgetch(status_win);
		switch (key) {
		case ERR:
			break;

		case 'y':
		case 'Y':
			answer = TRUE;
			status = STOP;
			break;

		case KEY_ESC:
		case KEY_RETURN:
		case KEY_ENTER:
		case KEY_BACKSPACE:
		case 'n':
		case 'N':
		case '\n':
		default:
			answer = FALSE;
			status = CANCEL;
		}
	}

	/* Clear the status window */
	status_empty = FALSE;
	report("");

	return answer;
}

	if (!*opt_git_dir)
		return OK;

	while (refs_size > 0)
		free(refs[--refs_size].name);
	while (id_refs_size > 0)
		free(id_refs[--id_refs_size]);

	return read_properties(popen("git " GIT_CONFIG " --list", "r"),
	request = parse_options(argc, argv);
	if (request == REQ_NONE)
	if (!opt_git_dir[0] && request != REQ_VIEW_PAGER)
	if (load_refs() == ERR)
					request = REQ_VIEW_DIFF;
					request = REQ_VIEW_PAGER;

				/* Always reload^Wrerun commands from the prompt. */
				open_view(view, request, OPEN_RELOAD);