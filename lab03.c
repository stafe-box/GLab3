#include "lab03_sqlite.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define UI_FILE "lab03.glade"

enum
{
    LECTURERID,
    LECTURERNAME,
    DISCIPLINEID,
    DISCIPLINENAME,
    LECTURES,
    PRACTICES,
    LABORATORY,
    CONTROLID,
    CONTROLNAME
};

struct MainWindowObjects
{
    GtkWindow           *main_window;
    GtkTreeView         *treeview;
    GtkListStore        *liststore;
    GtkTreeViewColumn   *cln_lecturer_id;
    GtkTreeViewColumn   *cln_lecturer_name;
    GtkTreeViewColumn   *cln_discipline_id;
    GtkTreeViewColumn   *cln_discipline_name;
    GtkTreeViewColumn   *cln_lectures;
    GtkTreeViewColumn   *cln_practices;
    GtkTreeViewColumn   *cln_laboratory;
    GtkTreeViewColumn   *cln_control_id;
    GtkTreeViewColumn   *cln_control_name;
    GtkDialog           *addDialog;
    GtkBox              *databox;
    GtkComboBoxText     *lecturer_name_entry;
    GtkEntry            *lecturer_name_entry_text;
    GtkEntry            *discipline_name_entry;
    GtkSpinButton       *lectures_spin;
    GtkSpinButton       *practices_spin;
    GtkSpinButton       *laboratory_spin;
    GtkEntry            *control_entry;
    GtkDialog           *delDialog;
} mainWindowObjects;


int callback(void *not_used, int argc, char **argv, char **col_names)
{
    GtkTreeIter iter;
    if (argc == 9)
    {
        gtk_list_store_append(GTK_LIST_STORE(mainWindowObjects.liststore), &iter);
        gtk_list_store_set(GTK_LIST_STORE(mainWindowObjects.liststore), &iter, 
                           LECTURERID, atoi(argv[LECTURERID]), LECTURERNAME, argv[LECTURERNAME], DISCIPLINEID, atoi(argv[DISCIPLINEID]),
                           DISCIPLINENAME, argv[DISCIPLINENAME], LECTURES, atoi(argv[LECTURES]), PRACTICES, atoi(argv[PRACTICES]),
                           LABORATORY, atoi(argv[LABORATORY]), CONTROLID, atoi(argv[CONTROLID]), CONTROLNAME, argv[CONTROLNAME], -1);
    }
    return 0;
}

/*void price_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model,
                          GtkTreeIter *iter, gpointer user_data)
{
    gfloat price;
    gchar buf[30];
    gtk_tree_model_get(model, iter, PRICE, &price, -1);
    g_snprintf(buf, sizeof(buf), "%.2f", price);
    g_object_set(renderer, "text", buf, NULL);
}*/

int main(int argc, char **argv)
{
    GtkBuilder *builder;
    GError *error = NULL;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    if (!gtk_builder_add_from_file(builder, UI_FILE, &error))
    {
        g_warning("%s\n", error->message);
        g_free(error);
        return (1);
    }

    mainWindowObjects.main_window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    mainWindowObjects.treeview =
        GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview_components"));
    mainWindowObjects.liststore =
        GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore_components"));

    /*mainWindowObjects.lectures_adjustment =
        GTK_ADJUSTMENT(gtk_builder_get_object(builder, "lectures_adjustment"));
    mainWindowObjects.practices_adjustment =
        GTK_ADJUSTMENT(gtk_builder_get_object(builder, "practices_adjustment"));
    mainWindowObjects.laboratory_adjustment =
        GTK_ADJUSTMENT(gtk_builder_get_object(builder, "laboratory_adjustment"));*/

    mainWindowObjects.cln_lecturer_id =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_lecturer_id"));
    mainWindowObjects.cln_lecturer_name =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_lecturer_name"));
    mainWindowObjects.cln_discipline_id =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_discipline_id"));
    mainWindowObjects.cln_discipline_name =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_discipline_name"));
    mainWindowObjects.cln_lectures =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_lectures"));
    mainWindowObjects.cln_practices =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_practices"));
    mainWindowObjects.cln_laboratory =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_laboratory"));
    mainWindowObjects.cln_control_id =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_control_id"));
    mainWindowObjects.cln_control_name =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_control_name"));
    

    gtk_builder_connect_signals(builder, &mainWindowObjects);

    g_object_unref(G_OBJECT(builder));
    gtk_widget_show_all(GTK_WIDGET(mainWindowObjects.main_window));

    sqlite_get_data();

    gtk_main();
}

/*G_MODULE_EXPORT void on_btnsave_clicked(GtkWidget *button, gpointer data)//переделать
{
    GtkTreeIter iter;
    gboolean reader =
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter);
    while (reader)
    {
        gint compid;
        gchar *compname;
        gfloat price;
        gtk_tree_model_get(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter, COMPID, &compid,
                           COMPNAME, &compname, PRICE, &price, -1);
        sqlite_update(compid, compname, price);
        reader = gtk_tree_model_iter_next(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter);
    }
    gtk_list_store_clear(mainWindowObjects.liststore);
    sqlite_get_data();
}

G_MODULE_EXPORT void on_cellrenderertext_compname_edited(GtkCellRendererText *renderer, gchar *path,
                                                         gchar *new_text, gpointer data)//переделать
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, COMPNAME, new_text, -1);
    }
}*/

G_MODULE_EXPORT void on_clnrender_lectures_edited(GtkCellRendererText *renderer, gchar *path,
                                                      gchar *new_text, gpointer data)//переделать
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, LECTURES, atof(new_text), -1);
    }
}

G_MODULE_EXPORT void on_clnrender_practices_edited(GtkCellRendererText *renderer, gchar *path,
                                                      gchar *new_text, gpointer data)//переделать
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, PRACTICES, atof(new_text), -1);
    }
}

G_MODULE_EXPORT void on_clnrender_laboratory_edited(GtkCellRendererText *renderer, gchar *path,
                                                      gchar *new_text, gpointer data)//переделать
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, LABORATORY, atof(new_text), -1);
    }
}

G_MODULE_EXPORT void on_show_hidden_toggled(GtkToggleButton *button, gpointer data)//переделать
{
    gboolean visible = gtk_toggle_button_get_active(button);
    gtk_tree_view_column_set_visible(mainWindowObjects.cln_lecturer_id, visible);
    gtk_tree_view_column_set_visible(mainWindowObjects.cln_discipline_id, visible);
    gtk_tree_view_column_set_visible(mainWindowObjects.cln_control_id, visible);
}

G_MODULE_EXPORT void on_btnabout_clicked(GtkButton *button, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "О программе", mainWindowObjects.main_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "_OK", GTK_RESPONSE_NONE, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 15);
    GtkWidget *label = gtk_label_new("\nЭто как ваша лабораторная работа №3, только уже она и есть\n");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show(label);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

G_MODULE_EXPORT void on_window_destroy(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}

G_MODULE_EXPORT void on_btnexit_clicked(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void remove_row_ld (GtkTreeRowReference *ref, GtkTreeModel *model)
{
    GtkTreeIter iter;
    GtkTreePath *path;
    path = gtk_tree_row_reference_get_path (ref);
    gtk_tree_model_get_iter (model, &iter, path);
    gint lecid;
    gint disid;
    gtk_tree_model_get(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter, LECTURERID, &lecid,
                           DISCIPLINEID, &disid, -1);
    sqlite_delete_ld(lecid, disid);
    gtk_widget_destroy(mainWindowObjects.delDialog);
    gtk_list_store_clear(mainWindowObjects.liststore);
    sqlite_get_data();
}

G_MODULE_EXPORT void on_lec_to_dis_btn_clicked(GtkButton *lec_to_dis_btn, gpointer data)
{
    GtkTreeSelection *selection;
    GtkTreeRowReference *ref;
    GtkTreeModel *model;
    GList *rows, *ptr, *references = NULL;
    selection = gtk_tree_view_get_selection ( mainWindowObjects.treeview );
    model = gtk_tree_view_get_model ( mainWindowObjects.treeview );
    rows = gtk_tree_selection_get_selected_rows (selection, &model);

    ptr = rows;
    while (ptr != NULL)
    {
        ref = gtk_tree_row_reference_new (model, (GtkTreePath*) ptr->data);
        references = g_list_prepend (references, gtk_tree_row_reference_copy (ref));
        gtk_tree_row_reference_free (ref);
        ptr = ptr->next;
    }
    
    g_list_foreach ( references, (GFunc) remove_row_ld, model );
    g_list_foreach ( references, (GFunc) gtk_tree_row_reference_free, NULL );
    g_list_foreach ( rows, (GFunc) gtk_tree_path_free, NULL );
    g_list_free ( references );
    g_list_free ( rows ); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void remove_row_cd (GtkTreeRowReference *ref, GtkTreeModel *model)
{
    GtkTreeIter iter;
    GtkTreePath *path;
    path = gtk_tree_row_reference_get_path (ref);
    gtk_tree_model_get_iter (model, &iter, path);
    gint disid;
    gint conid;
    gtk_tree_model_get(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter,
                           DISCIPLINEID, &disid, CONTROLID, &conid, -1);
    sqlite_delete_cd(disid, conid);
    gtk_widget_destroy(mainWindowObjects.delDialog);
    gtk_list_store_clear(mainWindowObjects.liststore);
    sqlite_get_data();

}

G_MODULE_EXPORT void on_con_to_dis_btn_clicked(GtkButton *lec_to_dis_btn, gpointer data)
{
    GtkTreeSelection *selection;
    GtkTreeRowReference *ref;
    GtkTreeModel *model;
    GList *rows, *ptr, *references = NULL;
    selection = gtk_tree_view_get_selection ( mainWindowObjects.treeview );
    model = gtk_tree_view_get_model ( mainWindowObjects.treeview );
    rows = gtk_tree_selection_get_selected_rows (selection, &model);

    ptr = rows;
    while (ptr != NULL)
    {
        ref = gtk_tree_row_reference_new (model, (GtkTreePath*) ptr->data);
        references = g_list_prepend (references, gtk_tree_row_reference_copy (ref));
        gtk_tree_row_reference_free (ref);
        ptr = ptr->next;
    }
    
    g_list_foreach ( references, (GFunc) remove_row_cd, model );
    g_list_foreach ( references, (GFunc) gtk_tree_row_reference_free, NULL );
    g_list_foreach ( rows, (GFunc) gtk_tree_path_free, NULL );
    g_list_free ( references );
    g_list_free ( rows ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////


G_MODULE_EXPORT void delbtn_cliked (GtkButton *delbtn, gpointer data)
{
    GtkBuilder *builder;
    GError *error = NULL;
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, UI_FILE, &error))
    {
        g_warning("%s\n", error->message);
        g_free(error);
        printf("%s\n", error->message);
        g_print("%s\n", error->message);
    }


    mainWindowObjects.delDialog = GTK_DIALOG(gtk_builder_get_object(builder, "delDialog"));

    gtk_builder_connect_signals(builder, &mainWindowObjects);
    g_object_unref(G_OBJECT(builder));
    gtk_widget_show_all(GTK_WIDGET(mainWindowObjects.delDialog));

}


G_MODULE_EXPORT void on_nodelbtn_clicked(GtkButton *nodelbtn, gpointer data)
{
    gtk_widget_destroy(mainWindowObjects.delDialog);
}

G_MODULE_EXPORT void on_gousebtn_clicked (GtkButton *gousebtn, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Гусь", mainWindowObjects.main_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "_OK", GTK_RESPONSE_NONE, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 15);
    GtkWidget *image = gtk_image_new_from_file("pepehonk-goose.gif");
    gtk_container_add(GTK_CONTAINER(content_area), image);
    gtk_widget_show(image);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


G_MODULE_EXPORT void btnadd_clicked (GtkButton *btnadd, gpointer data)
{
    GtkBuilder *builder;
    GError *error = NULL;
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, UI_FILE, &error))
    {
        g_warning("%s\n", error->message);
        g_free(error);
        printf("%s\n", error->message);
        g_print("%s\n", error->message);
    }
    
    mainWindowObjects.addDialog = GTK_DIALOG(gtk_builder_get_object(builder, "addDialog"));
    mainWindowObjects.databox = GTK_BOX(gtk_builder_get_object(builder, "databox"));
    mainWindowObjects.lecturer_name_entry = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "lecturer_name_entry"));
    mainWindowObjects.lecturer_name_entry = GTK_ENTRY(gtk_builder_get_object(builder, "lecturer_name_entry_text"));
    mainWindowObjects.discipline_name_entry = GTK_ENTRY(gtk_builder_get_object(builder, "discipline_name_entry"));
    mainWindowObjects.lectures_spin = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "lectures_spin"));
    mainWindowObjects.practices_spin = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "practices_spin"));
    mainWindowObjects.laboratory_spin = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "laboratory_spin"));
    mainWindowObjects.control_entry = GTK_ENTRY(gtk_builder_get_object(builder, "control_entry"));

    gtk_builder_connect_signals(builder, &mainWindowObjects);
    g_object_unref(G_OBJECT(builder));
    gtk_widget_show_all(GTK_WIDGET(mainWindowObjects.addDialog));
    
}

/*G_MODULE_EXPORT void on_newbtn_clicked (GtkButton *savebtn, gpointer data)
{
    gchar *shopname = gtk_entry_get_text(GTK_ENTRY(mainWindowObjects.storebox));
    gchar *compname = gtk_entry_get_text(GTK_ENTRY(mainWindowObjects.namebox));
    gfloat price = gtk_spin_button_get_value(GTK_SPIN_BUTTON(mainWindowObjects.pricebox));
    //gtk_entry_get_text(GTK_ENTRY(mainWindowObjects.namebox), COMPNAME, &compname);
    //gtk_entry_get_text(GTK_ENTRY(mainWindowObjects.storebox), SHOPNAME, &shopname);
    //gtk_spin_button_get_value(GTK_SPIN_BUTTON(mainWindowObjects.pricebox), PRICE, &price);

    printf("%s, %s, %f\n", shopname, compname, price);
    sqlite_add(shopname, compname, price);
    gtk_list_store_clear(mainWindowObjects.liststore);
    sqlite_get_data();

}*/

G_MODULE_EXPORT void on_cancelbtn_clicked(GtkButton *cancelbtn, gpointer data)
{
    gtk_widget_destroy(mainWindowObjects.addDialog);
}