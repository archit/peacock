/*
 * Small test program to test the PeacockHTMLToolbars widget.
 * Helps me eliminate bugs later if it is in the Gtk+ stuff
 * or bonobo stuff (see test-bonobo-container.c for that.)
 *
 * *cough* Xtreme Programming *cough* :-)
 */

#include <stdio.h>
#include <libgnomeui/libgnomeui.h>
#include "peacock-html-toolbars.h"

static void
new_html_tag_ready_cb (GtkWidget *widget,
		       const gchar *prefix,
		       const gchar *suffix,
		       gpointer data)
{
	fprintf (stderr, "Prefix: %s\n", prefix);
	fprintf (stderr, "Suffix: %s\n", suffix);
}

static void
hide_unhide_cb (GtkWidget *widget, gpointer user_data)
{
	gboolean state;

	g_object_get (G_OBJECT (user_data), "view-common-htmlbar",
		      &state, NULL);
	state = (state ? FALSE : TRUE);
	g_object_set (G_OBJECT (user_data), "view-common-htmlbar",
		      state, NULL);
}

int main (int argc, char **argv)
{
	GtkWidget *win;
	GtkWidget *wid;
	GtkWidget *contents;
	GtkWidget *button;
	GnomeProgram *prg;

	gtk_init (&argc, &argv);
	prg = gnome_program_init ("peacock-html-toolbars-control", "0.1",
				  NULL, argc, argv, GNOME_PARAM_HUMAN_READABLE_NAME, "Test PeacockHTMLToolbars", NULL);
/*GNOME_PARAM_APP_DATADIR, "/usr/share/*/

	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	contents = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (win), contents);
	gtk_widget_show (contents);

	wid = peacock_html_toolbars_new ();
	gtk_container_add (GTK_CONTAINER (contents), wid);
	g_signal_connect (G_OBJECT (wid), "new_html_tag_ready",
			  G_CALLBACK (new_html_tag_ready_cb), NULL);
	gtk_widget_show (wid);

	button = gtk_button_new_with_label ("Hide/Unhide Toolbar");
	gtk_container_add (GTK_CONTAINER (contents), button);
	g_signal_connect (G_OBJECT (button), "clicked",
			  G_CALLBACK (hide_unhide_cb), wid);
	gtk_widget_show (button);

	gtk_widget_show (win);

	gtk_main ();

	return 0;
}
