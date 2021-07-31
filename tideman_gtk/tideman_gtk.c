#include <gtk/gtk.h>

// Prototypes
static void print_info(GtkWidget* widget, gpointer data);
static void activate(GtkApplication* app, gpointer user_data);

int main(int argc, char* argv[])
{
    GtkApplication* app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

static void print_info(GtkWidget* widget, gpointer data)
{
    g_print("Work in progress...\n");
}

static void activate(GtkApplication* app, gpointer user_data)
{
    GtkWidget* window;
    GtkWidget* button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tideman");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    button = gtk_button_new_with_label("Tideman");
    g_signal_connect(button, "clicked", G_CALLBACK(print_info), NULL);
    gtk_window_set_child(GTK_WINDOW(window), button);

    gtk_window_present(GTK_WINDOW(window));
}

