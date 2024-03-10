#include 
#include <gtk/gtk.h>

enum
{
   TITLE_COLUMN,
   AUTHOR_COLUMN,
   CHECKED_COLUMN,
   N_COLUMNS
};

static void refresh_tree_model(GtkTreeStore *store) {

  GtkTreeIter iter1;
  GtkTreeIter iter2;

  gtk_tree_store_append(store, &iter1, NULL);
  gtk_tree_store_set(store, &iter1,
                      TITLE_COLUMN, "The Art of Computer Programming",
                      AUTHOR_COLUMN, "Donald E. Knuth",
                      CHECKED_COLUMN, FALSE, -1);

  gtk_tree_store_append(store, &iter2, &iter1);  
  gtk_tree_store_set(store, &iter2,
                      TITLE_COLUMN, "Volumn 1: Fundamental Algorithms",
                      -1);                  
  
  gtk_tree_store_append(store, &iter2, &iter1);  
  gtk_tree_store_set(store, &iter2,
                      TITLE_COLUMN, "Volumn 2: Seminumerical Algorithms",
                      -1);                  
  gtk_tree_store_append(store, &iter2, &iter1);  
  gtk_tree_store_set(store, &iter2,
                      TITLE_COLUMN, "Volumn 3: Sorting and Searching",
                      -1);                                      
}

static GtkWidget* init_tree(void) {
  GtkTreeStore *store;
  GtkWidget *tree;
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  store = gtk_tree_store_new(N_COLUMNS, 
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_BOOLEAN
                              );

  refresh_tree_model(store);

  tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

  g_object_unref(G_OBJECT(store));
  renderer = gtk_cell_renderer_text_new();
  g_object_set(G_OBJECT(renderer), "foreground", "red", NULL);

  column = gtk_tree_view_column_new_with_attributes("Author", renderer, 
                                                    "text", AUTHOR_COLUMN, NULL);

  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  column = gtk_tree_view_column_new_with_attributes("Title", renderer, 
                                                    "text", TITLE_COLUMN, NULL);

  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
  renderer = gtk_cell_renderer_toggle_new();

  column = gtk_tree_view_column_new_with_attributes("Checked out", renderer, 
                                                    "active", CHECKED_COLUMN, NULL);

  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  return tree;

}


static void activate (GtkApplication* app, gpointer user_data)   {

  GtkWidget *window;
  GtkWidget *tree;
  //создаём главное окно  
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Tree View");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

  tree = init_tree();
  gtk_window_set_child(GTK_WINDOW(window), tree);

  gtk_window_present (GTK_WINDOW (window));
}


int main(int argc,char *argv[]){

    //dir_list("/home/vadim/ci", 0);
  char *cur_dir_name[MAX_PATH];
  GtkApplication *app;
  int status;
  


  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;

}