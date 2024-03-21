#include "file_info.h"
#include "tv.h"

static GtkTreeStore *store;

static void update_tree(file_info_t* dir, GtkTreeIter *parent_iter) {

  GtkTreeIter iter;
  char buff[12];
    
  gtk_window_set_title (GTK_WINDOW (window), dir->full_name);

  get_file_list(dir);

  printf(" - %ld\n", dir->size);
  
  for (size_t i = 0; i < dir->size; i++) {
    strftime(buff, 12, "%d.%m.%Y", localtime(&dir->flist[i].date));
    printf(" - %s\n", dir->flist[i].name);
    gtk_tree_store_append(store, &iter, parent_iter);
    gtk_tree_store_set(store, &iter,
                      NAME_COLUMN, dir->flist[i].name,
                      SIZE_COLUMN, dir->flist[i].size,
                      DATE_COLUMN, buff,
                      DATA_COLUMN, &dir->flist[i], -1);
  }
}

static void selection_changed(GtkTreeSelection *selection, gpointer data) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  file_info_t *finfo;

  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, DATA_COLUMN, &finfo, -1);
    g_print ("You selected a file: %s, : %d\n", finfo->full_name, finfo->type);
    if (finfo->type == TYPE_DIR && finfo->flist == NULL) {
      update_tree(finfo, &iter);  
    }
  }
}

static GtkWidget* init_tree(file_info_t* dir) {
  
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

  update_tree(dir, NULL);

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
  gtk_tree_view_column_set_resizable(column, TRUE);                                                  
  gtk_tree_view_column_set_alignment(column, 0.0);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  column = gtk_tree_view_column_new_with_attributes("Date edidting", renderer, 
                                                    "text", DATE_COLUMN, 
                                                    NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);                                                  
  gtk_tree_view_column_set_alignment(column, 0.0);
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
  gint rwidth, rheight;

  window = gtk_application_window_new (app);
  
  gtk_window_set_title (GTK_WINDOW (window), "...");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);
  
  getcwd(cur_dir, MAX_NAME - 1);
  dir = create_dir(cur_dir);
  tree = init_tree(dir);
  gtk_window_set_child(GTK_WINDOW(window), tree);

  gtk_window_present (GTK_WINDOW (window));
}
