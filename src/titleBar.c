#include <gtk/gtk.h>

static void onFileChooserResponse(GtkNativeDialog *fileChooser, int response,
                                  char *text) {
    if (response == GTK_RESPONSE_ACCEPT) {
        GFile *chosenFile =
            gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));

        g_file_replace_contents(chosenFile, text, strlen(text), NULL, FALSE,
                                G_FILE_CREATE_NONE, NULL, NULL, NULL);

        g_object_unref(chosenFile);
    }

    g_object_unref(fileChooser);
}

static void saveButtonClicked(GtkWidget *saveButton, GtkWidget *window) {
    // TODO: cycle through all widgets and find the GtkTextView.
    GtkWidget *textView = gtk_widget_get_first_child(window);
    GtkTextBuffer *textBuffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(textBuffer, &start, &end);

    char *text = gtk_text_buffer_get_text(textBuffer, &start, &end, true);

    GtkFileChooserNative *fileChooser = gtk_file_chooser_native_new(
        "Save File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, "Save",
        "Cancel");

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(fileChooser),
                                      "Untitled Document");

    g_signal_connect(fileChooser, "response", G_CALLBACK(onFileChooserResponse),
                     text);

    gtk_native_dialog_show(GTK_NATIVE_DIALOG(fileChooser));
}

void buildTitleBar(GtkWidget *window) {
    GtkWidget *headerBar = gtk_header_bar_new();
    GtkWidget *saveButton = gtk_button_new_with_label("Save");
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClicked),
                     window);

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), saveButton);

    gtk_window_set_titlebar(GTK_WINDOW(window), headerBar);
}