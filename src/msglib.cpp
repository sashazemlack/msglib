#include "msglib.h"

#if defined(_WIN32) || defined(_WIN64)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <string>

namespace {

std::wstring Utf8ToWide(const char* utf8)
{
    if (utf8 == nullptr || utf8[0] == '\0') {
        return std::wstring();
    }

    const int needed = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, nullptr, 0);
    if (needed <= 0) {
        return std::wstring();
    }

    std::wstring wide(static_cast<size_t>(needed - 1), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, &wide[0], needed);
    return wide;
}

UINT IconFlags(int icon)
{
    switch (icon) {
    case 1:
        return MB_ICONINFORMATION;
    case 2:
        return MB_ICONWARNING;
    case 3:
        return MB_ICONERROR;
    default:
        return 0;
    }
}

} // namespace

extern "C" MSGLIB_API int MsgBox(int icon, const char* message, const char* title)
{
    if (message == nullptr || title == nullptr) {
        return 0;
    }

    const std::wstring wmessage = Utf8ToWide(message);
    const std::wstring wtitle = Utf8ToWide(title);

    const int result = MessageBoxW(
        nullptr,
        wmessage.c_str(),
        wtitle.c_str(),
        MB_OK | IconFlags(icon));

    return (result == IDOK) ? 1 : 0;
}

#else /* Linux / GTK3 */

#include <gtk/gtk.h>

namespace {

GtkMessageType MessageTypeFromIcon(int icon)
{
    switch (icon) {
    case 1:
        return GTK_MESSAGE_INFO;
    case 2:
        return GTK_MESSAGE_WARNING;
    case 3:
        return GTK_MESSAGE_ERROR;
    default:
        return GTK_MESSAGE_OTHER;
    }
}

const char* IconNameFromIcon(int icon)
{
    switch (icon) {
    case 1:
        return "dialog-information";
    case 2:
        return "dialog-warning";
    case 3:
        return "dialog-error";
    default:
        return nullptr;
    }
}

void EnsureMessageDialogIconVisible(GtkWidget* dialog, int icon)
{
    const char* icon_name = IconNameFromIcon(icon);
    if (icon_name == nullptr) {
        return;
    }

    // Adwaita and similar themes hide GtkMessageDialog images via CSS.
    GtkCssProvider* css = gtk_css_provider_new();
    gtk_css_provider_load_from_data(
        css,
        "messagedialog image,"
        "dialog messagedialog image,"
        ".message-dialog image {"
        "  opacity: 1;"
        "  -gtk-icon-style: regular;"
        "  min-width: 48px;"
        "  min-height: 48px;"
        "  margin: 0 6px 0 0;"
        "  padding: 0;"
        "}",
        -1,
        nullptr);

    GdkScreen* screen = gtk_widget_get_screen(dialog);
    if (screen != nullptr) {
        gtk_style_context_add_provider_for_screen(
            screen,
            GTK_STYLE_PROVIDER(css),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    g_object_unref(css);

    GtkWidget* image = gtk_image_new_from_icon_name(icon_name, GTK_ICON_SIZE_DIALOG);
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    gtk_message_dialog_set_image(GTK_MESSAGE_DIALOG(dialog), image);
G_GNUC_END_IGNORE_DEPRECATIONS
    gtk_widget_show(image);

    // Halve the built-in GtkMessageDialog spacing between icon and text.
    GtkWidget* parent = gtk_widget_get_parent(image);
    if (parent != nullptr && GTK_IS_BOX(parent)) {
        const gint spacing = gtk_box_get_spacing(GTK_BOX(parent));
        gtk_box_set_spacing(GTK_BOX(parent), spacing / 2);
    }

    gtk_window_set_icon_name(GTK_WINDOW(dialog), icon_name);
}

} // namespace

extern "C" MSGLIB_API int MsgBox(int icon, const char* message, const char* title)
{
    if (message == nullptr || title == nullptr) {
        return 0;
    }

    if (!gtk_init_check(nullptr, nullptr)) {
        return 0;
    }

    // Construct explicitly so we keep a classic title bar (title visible)
    // instead of a CSD header bar that often hides it.
    GtkWidget* dialog = GTK_WIDGET(g_object_new(
        GTK_TYPE_MESSAGE_DIALOG,
        "message-type", MessageTypeFromIcon(icon),
        "buttons", GTK_BUTTONS_OK,
        "text", message,
        "title", title,
        "modal", TRUE,
        "destroy-with-parent", TRUE,
        "use-header-bar", FALSE,
        "resizable", FALSE,
        nullptr));

    if (dialog == nullptr) {
        return 0;
    }

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

    EnsureMessageDialogIconVisible(dialog, icon);

    gtk_widget_show_all(dialog);

    const gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    while (gtk_events_pending()) {
        gtk_main_iteration();
    }

    return (response == GTK_RESPONSE_OK) ? 1 : 0;
}

#endif
