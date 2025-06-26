#include "ui.h"

typedef struct {
    uiWindow *dialog;
    int result;
} DialogContext;

// Callback for Yes button
static void on_yes_clicked(uiButton *b, void *data) {
    DialogContext *ctx = (DialogContext *)data;
    ctx->result = 1;
    uiControlDestroy(uiControl(ctx->dialog));
}

// Callback for No button
static void on_no_clicked(uiButton *b, void *data) {
    DialogContext *ctx = (DialogContext *)data;
    ctx->result = 0;
    uiControlDestroy(uiControl(ctx->dialog));
}

// Function to show a modal yes/no dialog, returns 1 for Yes, 0 for No

int show_yes_no_dialog(const char *title, const char *message) {
    uiWindow *dialog = uiNewWindow(title, 200, 50, 0);
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