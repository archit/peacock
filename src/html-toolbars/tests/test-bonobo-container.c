#include <libbonoboui.h>
#include <libgnomeui/libgnomeui.h>

#define CONTROL_OAFIID "OAFIID:Peacock_HTML_Toolbars"

GtkWidget *
create_window ()
{
	BonoboWindow *window;
	BonoboWidget *control;
	BonoboUIContainer *ui_cont;

	window = bonobo_window_new ("df", "df");
	ui_cont = bonobo_window_get_ui_container (BONOBO_WINDOW (window));
	control = bonobo_widget_new_control (CONTROL_OAFIID,
					     BONOBO_OBJREF (ui_cont));
	bonobo_window_set_contents (BONOBO_WINDOW (window),
				    GTK_WIDGET (control));

	gtk_widget_show (GTK_WIDGET (control));
	gtk_widget_show (GTK_WIDGET (window));

	return window;
}

int
main (int argc, char **argv)
{
	GnomeProgram *program;
	GtkWidget *window;
	GError *error;
	gchar *oaf_iid = NULL;

	prg = gnome_program_init ("peacock-html-toolbars-control", "0.1",
				  NULL, argc, argv, GNOME_PARAM_HUMAN_READABLE_NAME, "Test PeacockHTMLToolbars", NULL);
/*GNOME_PARAM_APP_DATADIR, "/usr/share/*/

	if (bonobo_ui_init ("BonoboControl test", "0.1", &argc, argv) == FALSE)
		g_error ("Could not initialize Bonobo!");

	
	/*
	 * Create Main App Window.
	 */
	window = create_window ();
	gtk_widget_show (window);

	bonobo_main ();

	g_free (oaf_iid);
	return 0;
}
