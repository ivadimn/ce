/*******************************
Урок 1.3
Пример b2 - Вывод таблицы в GTK+
********************************/
#include <gtk/gtk.h>

GtkWidget       *window;    // Окно программы
GtkListStore    *buffer;    // Буфер таблицы
GtkWidget       *table;     // Таблица
GtkTreeViewColumn *column;  // Отображаемая колонка
GtkTreeIter     iter;       // Итератор таблицы (текущая строка)
GtkCellRenderer *renderer;  // Рендер таблицы (текущая ячейка)

// Обозначения полей
enum
{
    C_NUM, C_NAME, C_OKLAD, N_COL
};

void ShowTable()
{
    // 1. Создание буфера таблицы
    buffer = gtk_list_store_new(N_COL, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_UINT);

    // 2. Вставка строк в буфер
    gtk_list_store_append (buffer, &iter);
    gtk_list_store_set (buffer, &iter,
                       C_NUM, 1,
                       C_NAME, "Иванов",
                       C_OKLAD, (guint) 20000,
                      -1);

    gtk_list_store_append (buffer, &iter);
    gtk_list_store_set (buffer, &iter,
                       C_NUM, 2,
                       C_NAME, "Петров",
                       C_OKLAD, (guint) 25000,
                      -1);

    gtk_list_store_append (buffer, &iter);
    gtk_list_store_set (buffer, &iter,
                       C_NUM, 3,
                       C_NAME, "Сидоров",
                       C_OKLAD, (guint) 30000,
                      -1);

    // 3. Создание таблицы
    table = gtk_tree_view_new_with_model(GTK_TREE_MODEL(buffer));

    // 4. Добавление столбцов в таблицу
    renderer = gtk_cell_renderer_text_new();

    column = gtk_tree_view_column_new_with_attributes
                ("№", renderer,"text", C_NUM, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (table), column);

    column = gtk_tree_view_column_new_with_attributes
                ("Фамилия", renderer,"text", C_NAME, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (table), column);

    column = gtk_tree_view_column_new_with_attributes
                ("Оклад", renderer,"text", C_OKLAD, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (table), column);

}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 300);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    ShowTable();

    gtk_container_add(GTK_CONTAINER(window), table);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
