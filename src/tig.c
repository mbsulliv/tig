/* Copyright (c) 2006-2014 Jonas Fonseca <jonas.fonseca@gmail.com>
#include "request.h"
#include "line.h"
#include "keys.h"
#include "view.h"
#include "repo.h"
#include "options.h"
#include "draw.h"
 */
static FILE *opt_tty			= NULL;
vertical_split_is_enabled(void)
	if (opt_vertical_split == VERTICAL_SPLIT_AUTO) {
		int height, width;
		getmaxyx(stdscr, height, width);
		return width * VSPLIT_SCALE > (height - 1) * 2;
	}
	return opt_vertical_split == VERTICAL_SPLIT_VERTICAL;
/*
 * The viewer
 */
/* The display array of active views and the index of the current view. */
static struct view *display[2];
static unsigned int current_view;
#define foreach_displayed_view(view, i) \
	for (i = 0; i < ARRAY_SIZE(display) && (view = display[i]); i++)
#define displayed_views()	(display[1] != NULL ? 2 : 1)
struct view_env view_env = { "HEAD", "HEAD" };
#define VIEW_OPS(id, name) name##_ops
static struct view_ops VIEW_INFO(VIEW_OPS);
static struct view views[] = {
#define VIEW_DATA(id, name) \
	{ #name, &name##_ops, &view_env }
	VIEW_INFO(VIEW_DATA)
};
#define VIEW(req) 	(&views[(req) - REQ_OFFSET - 1])
#define foreach_view(view, i) \
	for (i = 0; i < ARRAY_SIZE(views) && (view = &views[i]); i++)
#define view_is_displayed(view) \
	(view == display[0] || view == display[1])
#define view_has_line(view, line_) \
	((view)->line <= (line_) && (line_) < (view)->line + (view)->lines)
forward_request_to_child(struct view *child, enum request request)
	return displayed_views() == 2 && view_is_displayed(child) &&
		!strcmp(child->vid, child->ops->id);
static enum request
view_request(struct view *view, enum request request)
	if (!view || !view->lines)
		return request;
	if (request == REQ_ENTER && !opt_focus_child &&
	    view_has_flags(view, VIEW_SEND_CHILD_ENTER)) {
		struct view *child = display[1];
	    	if (forward_request_to_child(child, request)) {
			view_request(child, request);
			return REQ_NONE;
		}
	if (request == REQ_REFRESH && view->unrefreshable) {
		report("This view can not be refreshed");
		return REQ_NONE;
	return view->ops->request(view, request, &view->line[view->pos.lineno]);
	WINDOW *window = view->title;
	view->height  = apply_step(opt_split_view_height, base->height);
	view->width  = apply_step(VSPLIT_SCALE, base->width);
static WINDOW *display_win[2];
static WINDOW *display_title[2];
static WINDOW *display_sep;

		view->title = display_title[i];
	_(LINENO,    '.', "line numbers",      &opt_show_line_numbers, NULL, VIEW_NO_FLAGS), \
	_(DATE,      'D', "dates",             &opt_show_date, date_map, VIEW_NO_FLAGS), \
	_(AUTHOR,    'A', "author",            &opt_show_author, author_map, VIEW_NO_FLAGS), \
	_(REV_GRAPH, 'g', "revision graph",    &opt_show_rev_graph, NULL, VIEW_LOG_LIKE), \
	_(FILENAME,  '#', "file names",        &opt_show_filename, filename_map, VIEW_NO_FLAGS), \
	_(FILE_SIZE, '*', "file sizes",        &opt_show_file_size, file_size_map, VIEW_NO_FLAGS), \
	_(TITLE_OVERFLOW, '$', "commit title overflow display", &opt_title_overflow, NULL, VIEW_NO_FLAGS), \
	_(UNTRACKED_DIRS, 'd', "untracked directory info", &opt_status_untracked_dirs, NULL, VIEW_STATUS_LIKE), \
	} else if (menu[i].data == &opt_title_overflow) {
		int *option = menu[i].data;

		*option = *option ? -*option : 50;
		string_format_size(msg, SIZEOF_STR,
			"%sabling %s", *option > 0 ? "En" : "Dis", menu[i].text);

		lines = opt_mouse_scroll;
		if (view->pos.col <= apply_step(opt_horizontal_scroll, view->width))
			view->pos.col -= apply_step(opt_horizontal_scroll, view->width);
		view->pos.col += apply_step(opt_horizontal_scroll, view->width);
		if (!*view->env->search)
	regex_err = regcomp(view->regex, view->env->search, REG_EXTENDED | regex_flags);
	string_copy(view->grep, view->env->search);
		FORMAT_VAR("%(directory)",	view_env.directory,	"."),
		FORMAT_VAR("%(file)",		view_env.file,		""),
		FORMAT_VAR("%(ref)",		view_env.ref,		"HEAD"),
		FORMAT_VAR("%(head)",		view_env.head,		""),
		FORMAT_VAR("%(commit)",		view_env.commit,	""),
		FORMAT_VAR("%(blob)",		view_env.blob,		""),
		FORMAT_VAR("%(branch)",		view_env.branch,	""),
		FORMAT_VAR("%(stash)",		view_env.stash,		""),
		if (vars[i].value == view_env.file && !format->file_filter)
			if (!format_append_argv(&format, dst_argv, opt_diff_options))
			if (!format_append_argv(&format, dst_argv, opt_blame_options))
	if (!view->prev && view->env->lineno && view->env->lineno <= view->lines) {
		select_view_line(view, view->env->lineno);
		view->env->lineno = 0;
	if ((!reload && !strcmp(view->vid, view->ops->id)) ||
		/* Put the current view ref value to the view title ref
		string_copy_rev(view->ref, view->ops->id);
		setup_update(view, view->ops->id);
			if (opt_show_line_numbers || view_has_flags(view, VIEW_ALWAYS_LINENO))
	if (!view_has_flags(view, VIEW_NO_GIT_DIR) && !repo.git_dir[0]) {
	open_external_viewer(mergetool_argv, repo.cdup, TRUE, "");
	if (lineno && opt_editor_line_number && string_format(lineno_cmd, "+%u", lineno))
	if (!open_external_viewer(editor_argv, repo.cdup, TRUE, EDITOR_LINENO_MSG))
		opt_editor_line_number = FALSE;
		string_copy_rev_from_commit_line(view->env->commit, line->data);
			string_copy_rev(view->ref, view->env->commit);
	string_copy_rev(view->env->commit, view->ref);
	"",
	view_env.head,
		const int len = get_line_info(LINE_DIFF_HEADER)->linelen;
	if (!io_run(&io, IO_RD, repo.cdup, opt_env, blame_argv))
	if (!prefixcmp(line, "@@ -"))
		line += STRING_SIZE("@@ -");
	else if (!prefixcmp(line, "@@@ -") &&
		 (line = strchr(line + STRING_SIZE("@@@ -"), '-')))
		line += 1;
	else
		if (chunk->type != LINE_DIFF_DEL &&
		    chunk->type != LINE_DIFF_DEL2)
	string_ncopy(view->env->file, commit.filename, strlen(commit.filename));
	string_copy(view->env->ref, header.id);
	view->env->lineno = header.orig_lineno - 1;
	bool has_path = file && string_format(path, "%s%s", repo.cdup, file);
			string_format(view->env->file, "%s", file);
			view->env->blob[0] = 0;
			string_ncopy(view->ref, view->ops->id, strlen(view->ops->id));
	view_env.commit,
struct help_request_iterator {
	struct view *view;
	struct keymap *keymap;
	bool add_title;
	const char *group;
};
static bool
help_open_keymap(void *data, const struct request_info *req_info, const char *group)
{
	struct help_request_iterator *iterator = data;
	struct view *view = iterator->view;
	struct keymap *keymap = iterator->keymap;
	const char *key = get_keys(keymap, req_info->request, TRUE);
	if (req_info->request == REQ_NONE || !key || !*key)
		return TRUE;
	if (iterator->add_title && help_open_keymap_title(view, keymap))
		return FALSE;
	iterator->add_title = FALSE;
	if (iterator->group != group) {
		add_line_text(view, group, LINE_HELP_GROUP);
		iterator->group = group;
	add_line_format(view, LINE_DEFAULT, "    %-25s %-20s %s", key,
			enum_name(*req_info), req_info->help);
	return TRUE;
}

static void
help_open_keymap_run_requests(struct help_request_iterator *iterator)
{
	struct view *view = iterator->view;
	struct keymap *keymap = iterator->keymap;
	char buf[SIZEOF_STR];
	const char *group = "External commands:";
	int i;
	for (i = 0; TRUE; i++) {
		if (!req)
			break;

		if (req->keymap != keymap)
		if (iterator->add_title && help_open_keymap_title(view, keymap))
		iterator->add_title = FALSE;
	for (keymap = get_keymaps(); keymap; keymap = keymap->next) {
		struct help_request_iterator iterator = { view, keymap, TRUE };

		if (foreach_request(help_open_keymap, &iterator))
			help_open_keymap_run_requests(&iterator);
	}
	"",
push_tree_stack_entry(struct view *view, const char *name, struct position *position)
	size_t pathlen = strlen(view->env->directory);
	char *path_position = view->env->directory + pathlen;
	if (!string_format_from(view->env->directory, &pathlen, "%s/", name)) {
				"--cc", "--raw", view->ops->id, "--", "%(directory)", NULL
			tree_entry(view, LINE_TREE_HEAD, view->env->directory, NULL, NULL, 0);
		if (!begin_update(view, repo.cdup, log_file, OPEN_EXTRA)) {
		pos = strrchr(text, '\t');
		if (*view->env->directory && !strncmp(text, view->env->directory, strlen(view->env->directory)))
			text += strlen(view->env->directory);
	    !tree_entry(view, LINE_TREE_HEAD, view->env->directory, NULL, NULL, 0))
	if (*view->env->directory) {
		size_t striplen = strlen(view->env->directory);
	for (line = &view->line[1 + !!*view->env->directory]; line < entry; line++) {
		string_copy(view->env->ref, view->vid);
			open_editor(view->env->file, 0);
		if (!*view->env->directory) {
	if (!*view->env->directory)
		if (line == &view->line[1] && *view->env->directory) {
			push_tree_stack_entry(view, basename, &view->pos);
		mkauthor(entry->author, opt_author_width, opt_show_author),
		mkdate(&entry->time, opt_show_date),
		string_format(view->ref, "Files in /%s", view->env->directory);
		view->env->blob[0] = 0;
		string_copy_rev(view->env->blob, entry->id);
		string_format(view->env->file, "%s%s", view->env->directory, tree_path(line));
	if (string_rev_is_null(view->env->commit)) {
	if (view->lines == 0 && repo.prefix[0]) {
		char *pos = repo.prefix;
			push_tree_stack_entry(view, pos, &view->pos);
	} else if (strcmp(view->vid, view->ops->id)) {
		view->env->directory[0] = 0;
	return begin_update(view, repo.cdup, tree_argv, flags);
	view_env.commit,
	if (!view->env->blob[0] && view->env->file[0]) {
		const char *commit = view->env->commit[0] ? view->env->commit : "HEAD";
		if (!string_format(blob_spec, "%s:%s", commit, view->env->file) ||
		    !io_run_buf(rev_parse_argv, view->env->blob, sizeof(view->env->blob))) {
	if (!view->env->blob[0]) {
	view->encoding = get_path_encoding(view->env->file, default_encoding);
			string_copy(view->env->ref, view->parent->vid);
	view_env.blob,
 *  1. File content is read either using view_env.file from the
	if (opt_blame_options) {
		for (i = 0; opt_blame_options[i]; i++) {
			if (prefixcmp(opt_blame_options[i], "-C"))
	const char *file_argv[] = { repo.cdup, view->env->file , NULL };
	if (!view->env->file[0]) {
	if (!view->prev && *repo.prefix && !(flags & (OPEN_RELOAD | OPEN_REFRESH))) {
		string_copy(path, view->env->file);
		if (!string_format(view->env->file, "%s%s", repo.prefix, path)) {
	if (*view->env->ref || !begin_update(view, repo.cdup, file_argv, flags)) {
		if (!begin_update(view, repo.cdup, blame_cat_file_argv, flags))
	string_copy_rev(state->history_state.id, view->env->ref);
	state->history_state.filename = get_path(view->env->file);
	string_format(view->vid, "%s", view->env->file);
	string_format(view->ref, "%s ...", view->env->file);
				*view->env->ref ? view->env->ref : "--incremental", "--", view->env->file, NULL
		if (view->lines == 0 || !begin_update(view, repo.cdup, blame_argv, OPEN_EXTRA)) {
		if (view->env->lineno > 0) {
			select_view_line(view, view->env->lineno);
			view->env->lineno = 0;
	if (draw_id_custom(view, id_type, id, opt_id_width))
	if (!string_format(from, "%s:%s", view->env->ref, view->env->file) ||
	string_ncopy(view->env->ref, id, sizeof(commit->id));
	string_ncopy(view->env->file, filename, strlen(filename));
	view->env->lineno = blame->lineno;
	string_copy(view->env->ref, history_state.id);
	string_ncopy(view->env->file, history_state.filename, strlen(history_state.filename));
	view->env->lineno = view->pos.lineno;
		commit ? mkauthor(commit->author, opt_author_width, opt_show_author) : "",
		commit ? mkdate(&commit->time, opt_show_date) : "",
		string_ncopy(view->env->commit, "HEAD", 4);
		string_copy_rev(view->env->commit, commit->id);
	view_env.commit,
			if (!strncasecmp(branch->ref->id, view->env->search, strlen(view->env->search))) {
		mkauthor(branch->author, opt_author_width, opt_show_author),
	string_copy_rev(view->env->commit, branch->ref->id);
	string_copy_rev(view->env->head, branch->ref->id);
	string_copy_rev(view->env->branch, branch->ref->name);
	view_env.head,
	if (!io_run(&io, IO_RD, repo.cdup, opt_env, argv))
	"git", "ls-files", "-z", "--others", "--exclude-standard", repo.prefix, NULL, NULL,
		char *head = repo.head;
		if (!string_format(buf, "%s/%s", repo.git_dir, paths[i][0]) ||
		if (!*repo.head) {
			if (io_open(&io, "%s/rebase-merge/head-name", repo.git_dir) &&
			string_copy(status_onbranch, repo.head);
	if (repo.is_inside_work_tree == FALSE) {
		opt_status_untracked_dirs ? NULL : "--directory";
		return io_run(io, IO_WR, repo.cdup, opt_env, staged_argv);
		return io_run(io, IO_WR, repo.cdup, opt_env, others_argv);
			if (!io_run_fg(reset_argv, repo.cdup))
		return io_run_fg(checkout_argv, repo.cdup);
			view->env->ref[0] = 0;
	status_stage_info(view->env->status, line->type, status);
		string_copy(view->env->file, status->new.name);
	"status",
	if (!io_run(&io, IO_WR, repo.cdup, opt_env, apply_argv))
			string_copy(view->env->file, stage_status.new.name);
				string_copy(view->env->file, file);
		view->env->ref[0] = 0;
		view->env->lineno = diff_get_lineno(view, line);
		if (view->env->lineno > 0)
			view->env->lineno--;
	static const char *file_argv[] = { repo.cdup, stage_status.new.name, NULL };
	view->dir = repo.cdup;
	view_env.status,
	state->with_graph = opt_show_rev_graph &&
		if (!state->added_changes_commits && opt_show_changes && repo.is_inside_work_tree)
			if (!strncasecmp(commit->id, view->env->search, strlen(view->env->search))) {
		report("Unable to find commit '%s'", view->env->search);
		mkauthor(commit->author, opt_author_width, opt_show_author),
		mkdate(&commit->time, opt_show_date),
			string_copy_rev(view->env->branch, branch->name);
	string_copy_rev(view->env->commit, commit->id);
	view_env.head,
	string_format(view->env->stash, "stash@{%d}", line->lineno - 1);
	string_copy(view->ref, view->env->stash);
	view_env.stash,
	update_options_from_argv(argv);
			opt_blame_options = flags;
				int lineno = atoi(opt + 1);

				view_env.lineno = lineno > 0 ? lineno - 1 : 0;
			string_ncopy(view_env.ref, opt_rev_argv[0], strlen(opt_rev_argv[0]));
		string_ncopy(view_env.file, opt_file_argv[0], strlen(opt_file_argv[0]));
		string_ncopy(view->env->search, cmd, strlen(cmd));
	if (!repo.git_dir[0] && request != REQ_VIEW_PAGER)
				string_ncopy(view_env.search, search, strlen(search));
			else if (*view_env.search)