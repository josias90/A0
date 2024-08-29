#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

GtkListStore *store;


// Funktion fürs Einlesen der CSV Datei
void read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Fehler während der Öffnung der Datei CSV");
        return;
    }

    char line[MAX_LINE_LENGTH];
    GtkTreeIter iter;

    // Einlesen jeder Zeile der CSV Datei
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        gtk_list_store_append(store, &iter);

        int column = 0;
        while (token != NULL) {
            // Löschen der Zeilensüberspung
            token[strcspn(token, "\r\n")] = 0;
            gtk_list_store_set(store, &iter, column, token, -1);
            token = strtok(NULL, ",");
            column++;
        }
    }

    fclose(file);
}

// Callback für die Bearbeitung der Zelle
void on_cell_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data) {
    GtkTreeIter iter;
    GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
    int column = GPOINTER_TO_INT(user_data);

    gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path);
    gtk_list_store_set(store, &iter, column, new_text, -1);

    gtk_tree_path_free(path);
}

// Funktion für die Bearbeitung der Spalten der Tabelle
void make_columns_editable(GtkTreeView *tree_view) {
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    for (int i = 0; i < 4; i++) { // Einnahme 4 Spalten zum Beispeil
        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "editable", TRUE, NULL);
        g_signal_connect(renderer, "edited", G_CALLBACK(on_cell_edited), GINT_TO_POINTER(i));

        column = gtk_tree_view_column_new_with_attributes(
            g_strdup_printf("Colonne %d", i + 1), renderer,
            "text", i, NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    }
}

// Funktion um Data in der CSV Datei zu speichern
void save_csv(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Fehler während der Öffnung der Datei CSV, um zu scheiben");
        return;
    }

    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);

    while (valid) {
        gchar *value;
        for (int i = 0; i < 3; i++) { //Einnahme 4 Spalten zum Beispeil
            gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, i, &value, -1);
            fprintf(file, "%s", value);
            g_free(value);

            if (i < 2) fprintf(file, ",");
        }
        fprintf(file, "\n");
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }

    fclose(file);
    g_print("Datei CSV Erfolgreich gespeichert.\n");
}

// Callback für das Speicher-Button
void on_save_button_clicked(GtkButton *button, gpointer user_data) {
    save_csv("Speicher.csv");
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                               "Datei wurden Erfolgreich gespeichert !");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Editor CSV");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *tree_view = gtk_tree_view_new();
    gtk_box_pack_start(GTK_BOX(vbox), tree_view, TRUE, TRUE, 0);

    // Initialisierung der Modele der Datei (store)
    store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING); //Einnahme 3 Spalten zum Beispeil
    gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(store));

    make_columns_editable(GTK_TREE_VIEW(tree_view));

    GtkWidget *button_save = gtk_button_new_with_label("Sauvegarder");
    g_signal_connect(button_save, "clicked", G_CALLBACK(on_save_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button_save, FALSE, FALSE, 0);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    read_csv("GTK.csv");

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

