#include <gtk/gtk.h>

GtkWidget *barredemenu;
GtkWidget *lemenu; 
GtkWidget *litem;

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Puissance 4");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_window_present (GTK_WINDOW (window));
  barredemenu = gtk_menu_bar_new();  
  lemenu = gtk_menu_new();
  litem = gtk_menu_item_new_with_label("Fichier");
  gtk_menu_shell_append(GTK_MENU_SHELL(lemenu), litem);  
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(litem), lemenu);
  gtk_window_set_child (GTK_WINDOW (window), barredemenu);
  
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
