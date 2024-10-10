#include <gtk/gtk.h>

// Function to handle the file upload button click
void on_file_upload_clicked(GtkWidget *widget, gpointer user_data) {
    GtkFileChooserDialog *dialog;
    GtkFileChooser *chooser;
    char *filename;

    // Create a file chooser dialog
    dialog = GTK_FILE_CHOOSER_DIALOG(gtk_file_chooser_dialog_new("Open File",
                            GTK_WINDOW(user_data),
                            GTK_FILE_CHOOSER_ACTION_OPEN,
                            "_Cancel", GTK_RESPONSE_CANCEL,
                            "_Open", GTK_RESPONSE_ACCEPT,
                            NULL));

    // Show the dialog and wait for a response
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        g_print("File selected: %s\n", filename); // Print the selected filename to console
        g_free(filename); // Free the filename string
    }

    // Destroy the dialog
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

// Function to handle the process button click
void on_process_clicked(GtkWidget *widget, gpointer user_data) {
    g_print("Processing the uploaded file...\n");
    // Here you would add your processing logic
}

// Function to handle the get results button click
void on_get_results_clicked(GtkWidget *widget, gpointer user_data) {
    g_print("Getting results...\n");
    // Here you would add your logic to retrieve results
}

// Main function
int main(int argc, char **argv) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *file_upload_button;
    GtkWidget *process_button;
    GtkWidget *get_results_button;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "File Upload Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    // Connect the delete event (close the application)
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to hold the buttons
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create buttons
    file_upload_button = gtk_button_new_with_label("File Upload");
    process_button = gtk_button_new_with_label("Process");
    get_results_button = gtk_button_new_with_label("Get Results");

    // Connect button signals to their respective handlers
    g_signal_connect(file_upload_button, "clicked", G_CALLBACK(on_file_upload_clicked), window);
    g_signal_connect(process_button, "clicked", G_CALLBACK(on_process_clicked), NULL);
    g_signal_connect(get_results_button, "clicked", G_CALLBACK(on_get_results_clicked), NULL);

    // Add buttons to the box
    gtk_box_pack_start(GTK_BOX(vbox), file_upload_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), process_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), get_results_button, TRUE, TRUE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the main loop
    gtk_main();

    return 0;
}