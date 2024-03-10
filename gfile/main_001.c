#include "common.h"
#include "file_info.h"
#include <gtk/gtk.h>

static void print_hello(GtkApplication *app, gpointer data) {
    g_print("Hello World\n");
}


static void activate (GtkApplication* app, gpointer user_data)   {

  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;
//создаём главное окно  
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  //gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  grid = gtk_grid_new();
  gtk_window_set_child(GTK_WINDOW(window), grid);

  button = gtk_button_new_with_label("Button 1");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label("Button 2");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);
  
  button = gtk_button_new_with_label("Quit");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), window);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1); 

  gtk_window_present (GTK_WINDOW (window));
}


int main(int argc,char *argv[]){

    //dir_list("/home/vadim/ci", 0);
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;

}