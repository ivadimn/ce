#include "common.h"
#include "file_info.h"
#include <gtk/gtk.h>

static void print_hello(GtkApplication *app, gpointer data) {
    g_print("Hello World\n");
}


static void activate (GtkApplication* app, gpointer user_data)   {

  GtkWidget *window;
  GtkWidget *scrolled_window;
  GtkWidget *grid;
  GtkWidget *box;
  GtkWidget *image;
  GtkWidget *button;

//создаём главное окно  
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

  grid = gtk_grid_new();
  gtk_window_set_child(GTK_WINDOW(window), grid);

  button = gtk_button_new_with_label("Button 1");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_append(GTK_BOX(box), button);
  
  gtk_grid_attach(GTK_GRID(grid), box, 0, 0, 1, 1);


  scrolled_window = gtk_scrolled_window_new ();
  image = gtk_image_new_from_file ("1533.jpg");
  /* And add it to the scrolled window */
  //gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), image);
  
  gtk_scrolled_window_add (GTK_SCROLLED_WINDOW (scrolled_window), image);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled_window),
                                                      GTK_POLICY_AUTOMATIC,
                                                      GTK_POLICY_AUTOMATIC);

  gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 1, 1, 1); 

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