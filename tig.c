static size_t utf8_length(const char *string, int *width, size_t max_width, int *trimmed, bool reserve);
#define TAB_SIZE	8
	unsigned int head:1;	/* Is it the current HEAD? */
	unsigned int tracked:1;	/* Is it the remote for the current HEAD? */
	REQ_(MAXIMIZE,		"Maximize the current view"), \
static bool opt_line_graphics		= TRUE;
static int opt_tab_size			= TAB_SIZE;
static char opt_remote[SIZEOF_REF]	= "";
	if (!isatty(STDIN_FILENO)) {
		opt_request = REQ_VIEW_PAGER;
		opt_pipe = stdin;
		return TRUE;
	}

LINE(DATE,         "",			COLOR_BLUE,	COLOR_DEFAULT,	0), \
LINE(LINE_NUMBER,  "",			COLOR_CYAN,	COLOR_DEFAULT,	0), \
LINE(MAIN_LOCAL_TAG,"",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
LINE(MAIN_REMOTE,  "",			COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(MAIN_TRACKED, "",			COLOR_YELLOW,	COLOR_DEFAULT,	A_BOLD), \
LINE(MAIN_REF,     "",			COLOR_CYAN,	COLOR_DEFAULT,	0), \
LINE(MAIN_HEAD,    "",			COLOR_CYAN,	COLOR_DEFAULT,	A_BOLD), \
LINE(BLAME_ID,     "",			COLOR_MAGENTA,	COLOR_DEFAULT,	0)
	LINE_INFO,
	LINE_NONE
	{ KEY_F(5),	REQ_REFRESH },
	{ 'O',		REQ_MAXIMIZE },
		} else if (!string_enum_compare(argv[0], "main-date", strlen("main-date"))) {
			info = get_line_info("date");

	if (!strcmp(argv[0], "line-graphics")) {
		opt_line_graphics = parse_bool(argv[2]);
		return OK;
	}

	bool git_dir;		/* Whether the view requires a git directory. */
	/* Drawing */
	struct line *curline;	/* Line currently being drawn. */
	enum line_type curtype;	/* Attribute currently used for drawing. */

	bool (*draw)(struct view *view, struct line *line, unsigned int lineno);
#define VIEW_STR(name, cmd, env, ref, ops, map, git) \
	{ name, cmd, #env, ref, ops, map, git }
#define VIEW_(id, name, ops, git, ref) \
	VIEW_STR(name, TIG_##id##_CMD,  TIG_##id##_CMD, ref, ops, KEYMAP_##id, git)
	VIEW_(MAIN,   "main",   &main_ops,   TRUE,  ref_head),
	VIEW_(DIFF,   "diff",   &pager_ops,  TRUE,  ref_commit),
	VIEW_(LOG,    "log",    &pager_ops,  TRUE,  ref_head),
	VIEW_(TREE,   "tree",   &tree_ops,   TRUE,  ref_commit),
	VIEW_(BLOB,   "blob",   &blob_ops,   TRUE,  ref_blob),
	VIEW_(BLAME,  "blame",  &blame_ops,  TRUE,  ref_commit),
	VIEW_(HELP,   "help",   &help_ops,   FALSE, ""),
	VIEW_(PAGER,  "pager",  &pager_ops,  FALSE, "stdin"),
	VIEW_(STATUS, "status", &status_ops, TRUE,  ""),
	VIEW_(STAGE,  "stage",	&stage_ops,  TRUE,  ""),
#define VIEW(req) 	(&views[(req) - REQ_OFFSET - 1])
#define VIEW_REQ(view)	((view) - views + REQ_OFFSET + 1)

enum line_graphic {
	LINE_GRAPHIC_VLINE,
};

static int line_graphics[] = {
	/* LINE_GRAPHIC_VLINE: */ '|'
};

static inline void
set_view_attr(struct view *view, enum line_type type)
{
	if (!view->curline->selected && view->curtype != type) {
		wattrset(view->win, get_line_attr(type));
		wchgat(view->win, -1, 0, type, NULL);
		view->curtype = type;
	}
}

draw_text(struct view *view, enum line_type type, const char *string,
	  int max_len, bool use_tilde)
	int col = 0;
		len = utf8_length(string, &col, max_len, &trimmed, use_tilde);
		col = len = strlen(string);
			col = len = max_len;
	set_view_attr(view, type);
		set_view_attr(view, LINE_DELIMITER);
		col++;
	}

	return col;
}

static int
draw_space(struct view *view, enum line_type type, int max, int spaces)
{
	static char space[] = "                    ";
	int col = 0;

	spaces = MIN(max, spaces);

	while (spaces > 0) {
		int len = MIN(spaces, sizeof(space) - 1);

		col += draw_text(view, type, space, spaces, FALSE);
		spaces -= len;
	}

	return col;
}

static int
draw_lineno(struct view *view, unsigned int lineno, int max)
{
	static char fmt[] = "%1ld";
	char number[10] = "          ";
	int digits3 = view->digits < 3 ? 3 : view->digits;
	int max_number = MIN(digits3, STRING_SIZE(number));
	bool showtrimmed = FALSE;
	int col;

	lineno += view->offset + 1;
	if (lineno == 1 || (lineno % opt_num_interval) == 0) {
		if (view->digits <= 9)
			fmt[1] = '0' + digits3;

		if (!string_format(number, fmt, lineno))
			number[0] = 0;
		showtrimmed = TRUE;
	}

	if (max < max_number)
		max_number = max;

	col = draw_text(view, LINE_LINE_NUMBER, number, max_number, showtrimmed);
	if (col < max) {
		set_view_attr(view, LINE_DEFAULT);
		waddch(view->win, line_graphics[LINE_GRAPHIC_VLINE]);
		col++;
	}
	if (col < max) {
		waddch(view->win, ' ');
		col++;
	}

	return col;
}

static int
draw_graphic(struct view *view, enum line_type type, chtype graphic[], size_t size, size_t max)
{
	int i;
	int col;

	if (max < size)
		size = max;

	set_view_attr(view, type);
	/* Using waddch() instead of waddnstr() ensures that
	 * they'll be rendered correctly for the cursor line. */
	for (i = 0; i < size; i++)
		waddch(view->win, graphic[i]);

	col = size;
	if (size < max) {
		waddch(view->win, ' ');
		col++;
	return col;
}

static int
draw_field(struct view *view, enum line_type type, char *text, int len, int max_len, bool trim)
{
	int max = MIN(max_len, len);
	int col;

	if (text)
		col = draw_text(view, type, text, max - 1, trim);
	else
		col = draw_space(view, type, max - 1, max - 1);

	col += draw_space(view, LINE_DEFAULT, max - col, max - col);
	return col;
}

static int
draw_date(struct view *view, struct tm *time, int max)
{
	char buf[DATE_COLS];
	char *date;
	int timelen = 0;

	if (time)
		timelen = strftime(buf, sizeof(buf), DATE_FORMAT, time);
	date = timelen ? buf : NULL;

	return draw_field(view, LINE_DATE, date, DATE_COLS, max, FALSE);
	wmove(view->win, lineno, 0);
	view->curline = line;
	view->curtype = LINE_NONE;
	line->selected = FALSE;

		set_view_attr(view, LINE_CURSOR);
	draw_ok = view->ops->draw(view, line, lineno);
	OPEN_NOMAXIMIZE = 8,	/* Do not maximize the current view. */
	bool nomaximize = !!(flags & OPEN_NOMAXIMIZE);
	if (view->git_dir && !opt_git_dir[0]) {
		report("The %s view is disabled in pager view", view->name);
	} else if (!nomaximize) {
	if (view->ops->open) {
		if (!view->ops->open(view)) {
			report("Failed to load %s view", view->name);
			return;
		}

	} else if ((reload || strcmp(view->vid, view->id)) &&
		   !begin_update(view)) {
		report("Failed to load %s view", view->name);
		return;
	}

		werase(view->win);
		/* FIXME: When all views can refresh always do this. */
		if (view == VIEW(REQ_VIEW_STATUS) ||
		    view == VIEW(REQ_VIEW_STAGE))
			request = REQ_REFRESH;
		else
			return TRUE;
	case REQ_MAXIMIZE:
		if (displayed_views() == 2)
			open_view(view, VIEW_REQ(view), OPEN_DEFAULT);
		break;

pager_draw(struct view *view, struct line *line, unsigned int lineno)
	int col = 0;
	if (opt_line_number) {
		col += draw_lineno(view, lineno, view->width);
		if (col >= view->width)
			return TRUE;
	draw_text(view, line->type, text, view->width - col, TRUE);
		blame->header = !group;
		else if (!view->parent)
			die("No blame exist for %s", view->vid);
		return add_line_data(view, blame, LINE_BLAME_ID) != NULL;
blame_draw(struct view *view, struct line *line, unsigned int lineno)
	struct tm *time = NULL;
	char *id = NULL, *author = NULL;
	if (blame->commit && *blame->commit->filename) {
		id = blame->commit->id;
		author = blame->commit->author;
		time = &blame->commit->time;
		col += draw_date(view, time, view->width);
		int max = view->width - col;
		col += draw_field(view, LINE_MAIN_AUTHOR, author, AUTHOR_COLS, max, TRUE);
		int max = view->width - col;
		col += draw_field(view, LINE_BLAME_ID, id, ID_COLS, max, FALSE);
	col += draw_lineno(view, lineno, view->width - col);
	col += draw_text(view, LINE_DEFAULT, blame->text, view->width - col, TRUE);
#define MATCH(text, on)							\
	(on && *text && regexec(view->regex, text, 1, &pmatch, 0) != REG_NOMATCH)
		if (MATCH(commit->title, 1) ||
		    MATCH(commit->author, opt_author) ||
		    MATCH(commit->id, opt_date))
		    MATCH(buf, 1))
	return MATCH(blame->text, 1);
	system("git update-index -q --refresh >/dev/null 2>/dev/null");
	while (prev_lineno > 0 && !view->line[prev_lineno].data)
		prev_lineno--;
	if (view->lineno < view->offset)
		view->offset = view->lineno;
	else if (view->offset + view->height <= view->lineno)
		view->offset = view->lineno - view->height + 1;

status_draw(struct view *view, struct line *line, unsigned int lineno)
	enum line_type type;
	char *text;
	int col = 0;
			type = LINE_STAT_SECTION;
			type = LINE_STAT_SECTION;
			type = LINE_STAT_SECTION;
			type = LINE_DEFAULT;
			type = LINE_STAT_HEAD;
	} else {
		char buf[] = { status->status, ' ', ' ', ' ', 0 };
		col += draw_text(view, line->type, buf, view->width, TRUE);
		type = LINE_DEFAULT;
		text = status->new.name;
	draw_text(view, type, text, view->width - col, TRUE);
	enum open_flags split;
	split = view_is_displayed(view) ? OPEN_SPLIT : 0;
	open_view(view, REQ_VIEW_STAGE, OPEN_RELOAD | split);
status_exists(struct status *status, enum line_type type)
{
	struct view *view = VIEW(REQ_VIEW_STATUS);
	struct line *line;

	for (line = view->line; line < view->line + view->lines; line++) {
		struct status *pos = line->data;

		if (line->type == type && pos &&
		    !strcmp(status->new.name, pos->new.name))
			return TRUE;
	}

	return FALSE;
}


static FILE *
status_update_prepare(enum line_type type)
		return NULL;

	switch (type) {
	case LINE_STAT_STAGED:
		string_add(cmd, cmdsize, "git update-index -z --index-info");
		break;

	case LINE_STAT_UNSTAGED:
	case LINE_STAT_UNTRACKED:
		string_add(cmd, cmdsize, "git update-index -z --add --remove --stdin");
		break;

	default:
		die("line type %d not handled in switch", type);
	}

	return popen(cmd, "w");
}

static bool
status_update_write(FILE *pipe, struct status *status, enum line_type type)
{
	char buf[SIZEOF_STR];
	size_t bufsize = 0;
	size_t written = 0;
					status->old.mode,
	return written == bufsize;
}

static bool
status_update_file(struct status *status, enum line_type type)
{
	FILE *pipe = status_update_prepare(type);
	bool result;

	if (!pipe)
		return FALSE;

	result = status_update_write(pipe, status, type);
	return result;
}

static bool
status_update_files(struct view *view, struct line *line)
{
	FILE *pipe = status_update_prepare(line->type);
	bool result = TRUE;
	struct line *pos = view->line + view->lines;
	int files = 0;
	int file, done;
	if (!pipe)
	for (pos = line; pos < view->line + view->lines && pos->data; pos++)
		files++;

	for (file = 0, done = 0; result && file < files; line++, file++) {
		int almost_done = file * 100 / files;

		if (almost_done > done) {
			done = almost_done;
			string_format(view->ref, "updating file %u of %u (%d%% done)",
				      file, files, done);
			update_view_title(view);
		}
		result = status_update_write(pipe, line->data, line->type);
	}

	pclose(pipe);
	return result;
		/* This should work even for the "On branch" line. */
		if (line < view->line + view->lines && !line[1].data) {
			report("Nothing to update");
			return FALSE;
		if (!status_update_files(view, line + 1)) {
			report("Failed to update file status");
	} else if (!status_update_file(line->data, line->type)) {
		return FALSE;
static bool
stage_diff_write(FILE *pipe, struct line *line, struct line *end)
	while (line < end) {
		if (!stage_diff_line(pipe, line++))
			return FALSE;
		if (line->type == LINE_DIFF_CHUNK ||
		    line->type == LINE_DIFF_HEADER)
			break;
	}
	return TRUE;
}
static struct line *
stage_diff_find(struct view *view, struct line *line, enum line_type type)
{
	for (; view->line < line; line--)
		if (line->type == type)
			return line;
stage_update_chunk(struct view *view, struct line *chunk)
	struct line *diff_hdr;
	diff_hdr = stage_diff_find(view, chunk, LINE_DIFF_HEADER);
				"git apply --whitespace=nowarn --cached %s - && "
	if (!stage_diff_write(pipe, diff_hdr, chunk) ||
	    !stage_diff_write(pipe, chunk, view->line + view->lines))
		chunk = NULL;
	return chunk ? TRUE : FALSE;
static bool
	struct line *chunk = NULL;

	if (!opt_no_head && stage_line_type != LINE_STAT_UNTRACKED)
		chunk = stage_diff_find(view, line, LINE_DIFF_CHUNK);

	if (chunk) {
		if (!stage_update_chunk(view, chunk)) {
			return FALSE;
		}

	} else if (!stage_status.status) {
		view = VIEW(REQ_VIEW_STATUS);

		for (line = view->line; line < view->line + view->lines; line++)
			if (line->type == stage_line_type)
				break;

		if (!status_update_files(view, line + 1)) {
			report("Failed to update files");
			return FALSE;
	} else if (!status_update_file(&stage_status, stage_line_type)) {
		return FALSE;
	return TRUE;
		if (!stage_update(view, line))
			return REQ_NONE;
	case REQ_REFRESH:
		/* Reload everything ... */
		break;

		return pager_request(view, request, line);
	open_view(view, REQ_VIEW_STATUS, OPEN_RELOAD | OPEN_NOMAXIMIZE);

	/* Check whether the staged entry still exists, and close the
	 * stage view if it doesn't. */
	if (!status_exists(&stage_status, stage_line_type))
		return REQ_VIEW_CLOSE;

	if (stage_line_type == LINE_STAT_UNTRACKED)
		opt_pipe = fopen(stage_status.new.name, "r");
	else
		string_copy(opt_cmd, view->cmd);
	open_view(view, REQ_VIEW_STAGE, OPEN_RELOAD | OPEN_NOMAXIMIZE);

		{ ' ',	'|' },
	if (opt_line_graphics)
		fillers[DEFAULT].line = line_graphics[LINE_GRAPHIC_VLINE];

main_draw(struct view *view, struct line *line, unsigned int lineno)
		col += draw_date(view, &commit->time, view->width);
		int max = view->width - col;
		col += draw_field(view, LINE_MAIN_AUTHOR, commit->author, AUTHOR_COLS, max, TRUE);
		col += draw_graphic(view, LINE_MAIN_REVGRAPH,
				    commit->graph, commit->graph_size,
				    view->width - col);
			enum line_type type;

			if (commit->refs[i]->head)
				type = LINE_MAIN_HEAD;
				type = LINE_MAIN_LOCAL_TAG;
				type = LINE_MAIN_TAG;
			else if (commit->refs[i]->tracked)
				type = LINE_MAIN_TRACKED;
				type = LINE_MAIN_REMOTE;
				type = LINE_MAIN_REF;

			col += draw_text(view, type, "[", view->width - col, TRUE);
			col += draw_text(view, type, commit->refs[i]->name, view->width - col, TRUE);
			col += draw_text(view, type, "]", view->width - col, TRUE);

			col += draw_text(view, LINE_DEFAULT, " ", view->width - col, TRUE);
	draw_text(view, LINE_DEFAULT, commit->title, view->width - col, TRUE);
		if (!view->lines && !view->parent)
			die("No revisions match the given arguments.");
static bool
grep_refs(struct ref **refs, regex_t *regex)
{
	regmatch_t pmatch;
	size_t i = 0;

	if (!refs)
		return FALSE;
	do {
		if (regexec(regex, refs[i]->name, 1, &pmatch, 0) != REG_NOMATCH)
			return TRUE;
	} while (refs[i++]->next);

	return FALSE;
}

	enum { S_TITLE, S_AUTHOR, S_DATE, S_REFS, S_END } state;
		case S_AUTHOR:
			if (!opt_author)
				continue;
			text = commit->author;
			break;
			if (!opt_date)
				continue;
		case S_REFS:
			if (!opt_show_refs)
				continue;
			if (grep_refs(commit->refs, view->regex) == TRUE)
				return TRUE;
			continue;
utf8_length(const char *string, int *width, size_t max_width, int *trimmed, bool reserve)
	size_t last_ucwidth = 0;
	*width = 0;
		*width  += ucwidth;
		if (*width > max_width) {
			*width -= ucwidth;
			if (reserve && *width == max_width) {
				*width -= last_ucwidth;
		last_ucwidth = ucwidth;

	TABSIZE = opt_tab_size;
	if (opt_line_graphics) {
		line_graphics[LINE_GRAPHIC_VLINE] = ACS_VLINE;
	}
	bool tracked = FALSE;
		tracked  = !strcmp(opt_remote, name);
	ref->head = head;
	ref->tracked = tracked;
	/* branch.<head>.remote */
	if (*opt_head &&
	    !strncmp(name, "branch.", 7) &&
	    !strncmp(name + 7, opt_head, strlen(opt_head)) &&
	    !strcmp(name + 7 + strlen(opt_head), ".remote"))
		string_ncopy(opt_remote, value, valuelen);

	if (*opt_head && *opt_remote &&
	    !strncmp(name, "branch.", 7) &&
	    !strncmp(name + 7, opt_head, strlen(opt_head)) &&
	    !strcmp(name + 7 + strlen(opt_head), ".merge")) {
		size_t from = strlen(opt_remote);

		if (!strncmp(value, "refs/heads/", STRING_SIZE("refs/heads/"))) {
			value += STRING_SIZE("refs/heads/");
			valuelen -= STRING_SIZE("refs/heads/");
		}

		if (!string_format_from(opt_remote, &from, "/%s", value))
			opt_remote[0] = 0;
	}

load_git_config(void)
	FILE *pipe = popen("(git rev-parse --git-dir --is-inside-work-tree "
			   " --show-cdup; git symbolic-ref HEAD) 2>/dev/null", "r");
	if (load_git_config() == ERR)
	if (!opt_git_dir[0] && opt_request != REQ_VIEW_PAGER)
	if (*opt_git_dir && load_refs() == ERR)