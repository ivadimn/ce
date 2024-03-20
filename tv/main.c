#include "file_info.h"
#include "tv.h"


int main(int argc,char *argv[]){

    //dir_list("/home/vadim/ci", 0);
  //char *cur_dir_name[MAX_PATH];
  GtkApplication *app;
  int status;
  
  app = gtk_application_new ("gtk.treeview.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;

}