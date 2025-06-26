// main.c

// System headers
#include <stdio.h>  // for printf (for debugging)
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// External dependencies (submodules)
#include "ui.h"  // libui-ng headder

// Local dependencies (this project)
#include "controls.h"


void onButtonClicked(uiButton *b, void *data) {
    (void)b;  // Prevent unused parameter warning
    uiWindow *win = (uiWindow *)data;
    uiMsgBox(win, "Hello", "You clicked the button!");

// Windows only code to return focus to the original window when when Message Box is closed
// not needed for macOS it retuns focus just fine
#ifdef _WIN32
    HWND hwnd = (HWND) uiControlHandle(uiControl(win));
    if (IsWindow(hwnd))
        SetActiveWindow(hwnd);
#endif
}


int onWindowClosing(uiWindow *sender, void *data) {
    (void)sender;  // Prevent unused parameter warning
    (void)data;  // Prevent unused parameter warning
    int answer = show_yes_no_dialog("Exit?", "Are you sure you want to exit?");
    //fprintf(stderr, "%d\n", answer);  // debug prints user choice
    if (answer == 1) {  // User clicked Yes
        uiQuit();  // Stop main loop and allow app to exit
        return 1;  // Allow the window to close
    }
    return 0;  // Prevent window closing, user clicked No
}


int main(void) {
    uiInitOptions o = {0};  // Initialize option struct with zeroed-out default
    const char *err = uiInit(&o);  // Initialize the libui library
    if (err != NULL) {
        fprintf(stderr, "libui init failed: %s\n", err);  // debug prints why inicialization failed 
        return 1;  // Return failure if inicialization failed
    }

    printf("Console output enabled!\n");  // debug Console print

    // Create main application window with title and dimensions
    uiWindow *win = uiNewWindow("Hello libui", 300, 150, 0);

    // Create a vertical box layout container to hold other UI elements
    uiBox *box = uiNewVerticalBox();
    uiBoxSetPadded(box, 1);  // Enable padding between elements inside the box

    // Create a label and it to the box layout
    uiControl *label = (uiControl *)uiNewLabel("This is a label above the button");
    uiBoxAppend(box, label, 0);  // Append lable to the box; 0 means it won't stretch

    // Create a button and add it to the box layout
    uiButton *btn = uiNewButton("Click me");
    uiBoxAppend(box, uiControl(btn), 0);  // Append button to the box; cast required because box takes generic controls

    // Set up the callback for when the button is clicked
    // 'win' is passed as user data so th callback knows which windows it belongs to
    uiButtonOnClicked(btn, onButtonClicked, win);

    // Set the box layout as the content of the window
    uiWindowSetChild(win, uiControl(box));

    // Set up the callback for when the window is closed
    uiWindowOnClosing(win, onWindowClosing, NULL);

    // Show the window on screen
    uiControlShow(uiControl(win));

    // Start the main UI event loop
    uiMain();

    // Clean up libui resources when the app exits
    uiUninit();

    return 0; // Return success
}
