#include "common.h"
#include "file_info.h"
#include <gtk/gtk.h>

static void print_hello(GtkApplication *app, gpointer data) {
    g_print("Hello World\n");
}

static void quit_cb(GtkWindow *window) {
    gtk_window_close(window);
}

static void activate (GtkApplication* app, gpointer user_data)   {

  //создаём билдер и загружаем UI описание
  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "builder.xml", NULL);

 //создаём главное окно  
  GObject *window = gtk_builder_get_object(builder, "window");
  gtk_window_set_application(GTK_WINDOW (window), app);

  GObject* button = gtk_builder_get_object(builder, "button1");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
  
  button = gtk_builder_get_object(builder, "button2");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
  
  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(quit_cb), window);

  gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

  g_object_unref(builder);

}


int main(int argc,char *argv[]){

    //dir_list("/home/vadim/ci", 0);
  GtkApplication *app;
  int status;
  #ifdef GTK_SRCDIR
    g_chdir(GTK_SRCDIR)
  #endif

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run(G_APPLICATION (app), argc, argv);
  printf("Status %d\n", status);
  g_object_unref(app);

  return status;

}