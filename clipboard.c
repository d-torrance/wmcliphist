#include <wmcliphist.h>
#include <time.h>

/* when true, clipboard will be automatically taken up by wmcliphist */
gint	auto_take_up = 1;

/* number of items to keep (may be overriden from command line) */
gint	num_items_to_keep = 10;

/* number if items kept */
gint	num_items = 0;

/* current number of locked items */
gint	locked_count;

/* list of clipboard history items */
GList	*history_items = NULL;

/* selected item */
HISTORY_ITEM	*selected = NULL;



/*
 * dumps history list to stderr
 * for debugging purposes only
 */
void
dump_history_list_fn(char *header)
{
	gint		indent = 1, i;
	GList		*node = history_items;
	HISTORY_ITEM	*data;
	
	fprintf(stderr, "%s\n", header);
	while (node) {
		data = (HISTORY_ITEM *)node->data;
		for (i = 0; i < indent; i++)
			putc('-', stderr);
		fprintf(stderr, " %.*s\n", (int) data->content_len,
				data->content);
		indent++;
		node = node->next;
	}
	fprintf(stderr, "==========\n");
}


/*
 * get clipboard content - partialy inspired by Downloader for X
 */
gboolean
my_get_xselection(GtkWidget *window, GdkEvent *event)
{
	GdkAtom		atom;
	gint		format;
	size_t		length;
	gchar		*content = NULL;

	/* previous clipboard content */
	static size_t	old_content_len = 0;
	static gchar	*old_content = "";


	begin_func("my_get_xselection");

	length = (size_t) gdk_selection_property_get(window->window,
			(guchar **) &content,
			&atom, &format);

	if (length > 0) {
		if ((length != old_content_len ||
				memcmp(content, old_content, length) != 0) &&
				!GTK_CHECK_MENU_ITEM(menu_app_clip_ignore)->active) {
			/* new data in clipboard */
			/* store new content for future comparation */
			if (old_content_len > 0)
				g_free(old_content);
			old_content = content;
			old_content_len = length;

			/* process item */
			process_item(content, length, 0, TRUE);
		} else {
			/* no new data */
			g_free(content);
		}

		/* when clipboard is locked, set selection owener to myself */
		if (GTK_CHECK_MENU_ITEM(menu_app_clip_ignore)->active ||
				GTK_CHECK_MENU_ITEM(menu_app_clip_lock)->active) {
			if (gtk_selection_owner_set(dock_app,
						GDK_SELECTION_PRIMARY,
						GDK_CURRENT_TIME) == 0)
				selected = NULL;
		}

	}

	return_val(TRUE);
}


/*
 * clipboard conversion - inspired by Downloader for X too :)
 */
gboolean
time_conv_select()
{
	begin_func("time_conv_select");

	gtk_selection_convert(main_window,
			GDK_SELECTION_PRIMARY,
			GDK_TARGET_STRING,
			GDK_CURRENT_TIME);
	return_val(TRUE);
}


/*
 * handles request for selection from other apps
 */
gboolean
selection_handle(GtkWidget *widget, 
		GtkSelectionData *selection_data,
		guint info,
		guint time_stamp,
		gpointer data)
{
	begin_func("selection_handle");

	if (selected)
		gtk_selection_data_set(selection_data,
				GDK_SELECTION_TYPE_STRING,
				8,
				(guchar *)selected->content,
				selected->content_len);
	else
		gtk_selection_data_set(selection_data,
				GDK_SELECTION_TYPE_STRING,
				8,
				(guchar *)"",
				0);

	return_val(TRUE);
}
