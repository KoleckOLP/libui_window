// controls.c

// System headers
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>
#endif

// External dependencies (submodules)
#include "ui.h"

typedef struct {
    uiWindow *dialog;
    int result;
} DialogContext;

// Callback for Yes button
static void on_yes_clicked(uiButton *b, void *data) {
    (void)b;  // Prevent unused parameter warning
    DialogContext *ctx = (DialogContext *)data;
    ctx->result = 1;
    uiControlDestroy(uiControl(ctx->dialog));
}

// Callback for No button
static void on_no_clicked(uiButton *b, void *data) {
    (void)b;  // Prevent unused parameter warning
    DialogContext *ctx = (DialogContext *)data;
    ctx->result = 0;
    uiControlDestroy(uiControl(ctx->dialog));
}

// Function to show a modal yes/no dialog, returns 1 for Yes, 0 for No

int show_yes_no_dialog(const char *title, const char *message) {
    int windowWidth = 200;
    int windowHeight = 50;

    uiWindow *dialog = uiNewWindow(title, windowWidth, windowHeight, 0);

    #ifdef _WIN32
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    #endif

    // TODO: Test this code on macOS !!!
    #ifdef __APPLE__
        CGDirectDisplayID display = CGMainDisplayID();
        int screenWidth = (int)CGDisplayPixelsWide(display);
        int screenHeight = (int)CGDisplayPixelsHigh(display);
    #endif

    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    uiWindowSetPosition(dialog, posX, posY);

    uiBox *box = uiNewVerticalBox();
    uiBoxSetPadded(box, 1);
    uiWindowSetChild(dialog, uiControl(box));

    uiLabel *label = uiNewLabel(message);
    uiBoxAppend(box, uiControl(label), 0);

    uiBox *buttonBox = uiNewHorizontalBox();
    uiBoxSetPadded(buttonBox, 1);
    uiBoxAppend(box, uiControl(buttonBox), 0);

    uiButton *yesButton = uiNewButton("Yes");
    uiButton *noButton = uiNewButton("No");

    uiBoxAppend(buttonBox, uiControl(yesButton), 1);
    uiBoxAppend(buttonBox, uiControl(noButton), 1);

    DialogContext ctx = { .dialog = dialog, .result = -1 };

    uiButtonOnClicked(yesButton, on_yes_clicked, &ctx);
    uiButtonOnClicked(noButton,  on_no_clicked,  &ctx);

    uiControlShow(uiControl(dialog));

    while (ctx.result == -1) {
        uiMainStep(1);
    }

    return ctx.result;
}
