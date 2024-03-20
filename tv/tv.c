#include "file_info.h"
#include "tv.h"


static void selection_changed(GtkTreeSelection *selection, gpointer data) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *name;
  file_info_t *finfo;

  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, NAME_COLUMN, &name, -1);
    gtk_tree_model_get(model, &iter, DATA_COLUMN, &finfo, -1);
    g_print ("You selected a file: %s\n", name);
    g_print ("You selected a file: %s\n", finfo->full_name);
    g_free (name);  
  }
}

static void refresh_tree_model(GtkTreeStore *store, file_info_t* dir, GtkTreeIter *parent_iter) {

  GtkTreeIter iter;
  
  gtk_window_set_title (GTK_WINDOW (window), dir->full_name);

  get_file_list(dir);

  printf(" - %ld\n", dir->size);

  
  for (size_t i = 0; i < dir->size; i++) {
    printf(" - %s\n", dir->flist[i].name);
    gtk_tree_store_append(store, &iter, NULL);
    gtk_tree_store_set(store, &iter,
                      NAME_COLUMN, dir->flist[i].name,
                      SIZE_COLUMN, dir->flist[i].size,
                      DATA_COLUMN, &dir->flist[i], -1);
  }

}

static GtkWidget* init_tree(file_info_t* dir) {
  GtkTreeStore *store;
  GtkWidget *tree;
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;
  GtkTreeSelection *select;

  store = gtk_tree_store_new(N_COLUMNS, 
                              G_TYPE_STRING,
                              G_TYPE_ULONG,
                              G_TYPE_POINTER
                              );

  refresh_tree_model(store, dir, NULL);

  tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

  g_object_unref(G_OBJECT(store));
  renderer = gtk_cell_renderer_text_new();  
  
  
  column = gtk_tree_view_column_new_with_attributes("File name", renderer, 
                                                    "text", NAME_COLUMN,  
                                                    NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
  
  column = gtk_tree_view_column_new_with_attributes("Size", renderer, 
                                                    "text", SIZE_COLUMN, 
                                                     NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  column = gtk_tree_view_column_new();                                                   
  gtk_tree_view_column_set_visible(column, FALSE);                                                  
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
  gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
  g_signal_connect(G_OBJECT(select), "changed", 
                            G_CALLBACK (selection_changed), NULL);
  
  return tree;

}

void activate (GtkApplication* app, gpointer user_data)   {

  GtkWidget *tree;
  file_info_t* dir;
  char cur_dir[MAX_NAME];

  //создаём главное окно  
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "...");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

  getcwd(cur_dir, MAX_NAME - 1);
  dir = create_dir(cur_dir);
  tree = init_tree(dir);
  gtk_window_set_child(GTK_WINDOW(window), tree);

  gtk_window_present (GTK_WINDOW (window));
}
