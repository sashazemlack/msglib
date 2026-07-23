#ifndef MSGLIB_H
#define MSGLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
#  ifdef MSGLIB_EXPORTS
#    define MSGLIB_API __declspec(dllexport)
#  else
#    define MSGLIB_API __declspec(dllimport)
#  endif
#else
#  if defined(__GNUC__) && __GNUC__ >= 4
#    define MSGLIB_API __attribute__((visibility("default")))
#  else
#    define MSGLIB_API
#  endif
#endif

/**
 * Show a modal message box.
 *
 * @param icon    0 = none, 1 = information, 2 = warning, 3 = error
 * @param message UTF-8 message text (must not be NULL)
 * @param title   UTF-8 window title (must not be NULL)
 * @return        1 if the user dismissed the dialog with OK, 0 on failure
 */
MSGLIB_API int MsgBox(int icon, const char* message, const char* title);

#ifdef __cplusplus
}
#endif

#endif /* MSGLIB_H */
