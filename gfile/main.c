#include "common.h"
#include "file_info.h"
#include <gtk/gtk.h>

enum
{
   NAME_COLUMN,
   SIZE_COLUMN,
   DATE_COLUMN,
   DATA_COLUMN,
   N_COLUMNS
};

static GtkTreeStore *store;


static void print_hello(GtkApplication *app, gpointer data) {
    g_print("Hello World\n");
}

static GtkWidget* init_tree() {
  
  GtkWidget *tree;
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;
  GtkTreeSelection *select;

  store = gtk_tree_store_new(N_COLUMNS, 
                              G_TYPE_STRING,
                              G_TYPE_ULONG,
                              G_TYPE_STRING,
                              G_TYPE_POINTER
                              );
  tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

  
  renderer = gtk_cell_renderer_text_new();  
    
  column = gtk_tree_view_column_new_with_attributes("File name", renderer, 
                                                    "text", NAME_COLUMN,  
                                                    NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
  
  column = gtk_tree_view_column_new_with_attributes("Size", renderer, 
                                                    "text", SIZE_COLUMN, 
                                                    NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);                                                  
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  column = gtk_tree_view_column_new_with_attributes("Date edidting", renderer, 
                                                    "text", DATE_COLUMN, 
                                                    NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);                                                  
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  column = gtk_tree_view_column_new();                                                   
  gtk_tree_view_column_set_visible(column, FALSE);                                                  
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
  gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
  
  return tree;

}


static void activate (GtkApplication* app, gpointer user_data)   {

  GtkWidget *window;
  GtkWidget *scrolled_window;
  GtkWidget *box;
  GtkWidget *button_box;
  GtkWidget *tree;
  GtkWidget *button;

//создаём главное окно  
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), box);

  button = gtk_button_new_with_label("Button 1");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  button_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_append(GTK_BOX(button_box), button);
  
  gtk_box_append(GTK_BOX(box), button_box);
  

  scrolled_window = gtk_scrolled_window_new ();
  tree = init_tree();
    
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW (scrolled_window), tree);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled_window),
                                                      GTK_POLICY_AUTOMATIC,
                                                      GTK_POLICY_AUTOMATIC);
  gtk_box_append(GTK_BOX(box), scrolled_window);
  gtk_widget_set_vexpand (scrolled_window, TRUE);
    
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
