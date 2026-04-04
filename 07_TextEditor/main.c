#include <string.h>

#include <exec/types.h>
#include <exec/libraries.h>
#include <intuition/intuition.h>
#include <intuition/icclass.h>

#include <reaction/reaction.h>
#include <reaction/reaction_macros.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/texteditor.h>

#define GID_EDITOR   1

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;
    struct Library *TextEditorBase;

    Object *win_obj;
    Object *gad_editor;

    struct Window *win;

    ULONG win_sigmask;
    int running;
};

/*****************************************************************************
 *
 *  App_Clear
 *
 *  Purpose:
 *      Reset lesson state to a known empty baseline.
 *
 *****************************************************************************/

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
}

/*****************************************************************************
 *
 *  UI_Create
 *
 *  Purpose:
 *      Create a minimal multiline editor lesson.
 *
 *  Notes:
 *      This lesson intentionally focuses on editor presence, ownership,
 *      and lifecycle. It does not yet teach document architecture.
 *
 *****************************************************************************/

static int UI_Create(struct App *app)
{
    int ok;

    ok = 0;

    app->WindowBase = OpenLibrary("window.class", 47);
    if (app->WindowBase == NULL)
        goto out;

    app->LayoutBase = OpenLibrary("layout.gadget", 47);
    if (app->LayoutBase == NULL)
        goto out;

    app->TextEditorBase = OpenLibrary("texteditor.gadget", 47);
    if (app->TextEditorBase == NULL)
        goto out;

    app->gad_editor =
        TextEditorObject,
            GA_ID, GID_EDITOR,
            GA_RelVerify, TRUE,
            GA_TabCycle, TRUE,
        End;

    if (app->gad_editor == NULL)
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 07_TextEditor",
            WA_Activate,    TRUE,
            WA_DepthGadget, TRUE,
            WA_DragBar,     TRUE,
            WA_CloseGadget, TRUE,
            WA_SizeGadget,  TRUE,
            WA_IDCMP,       IDCMP_CLOSEWINDOW,
            WINDOW_Position, WPOS_CENTERSCREEN,
            WINDOW_Layout,
                VLayoutObject,
                    LAYOUT_SpaceOuter, TRUE,
                    StartMember, app->gad_editor, EndMember,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    if (!ok && app->gad_editor != NULL)
    {
        DisposeObject(app->gad_editor);
        app->gad_editor = NULL;
    }

    return ok;
}

/*****************************************************************************
 *
 *  UI_Open
 *
 *  Purpose:
 *      Open the editor lesson window and prepare signal handling.
 *
 *****************************************************************************/

static int UI_Open(struct App *app)
{
    app->win = (struct Window *)RA_OpenWindow(app->win_obj);
    if (app->win == NULL)
        return 0;

    app->win_sigmask = RA_HandleInput(app->win_obj, NULL);
    app->running = 1;

    return 1;
}

static void UI_Close(struct App *app)
{
    if (app->win != NULL)
    {
        RA_CloseWindow(app->win_obj);
        app->win = NULL;
    }

    app->win_sigmask = 0;
}

static void UI_Destroy(struct App *app)
{
    if (app->win_obj != NULL)
    {
        DisposeObject(app->win_obj);
        app->win_obj = NULL;
        app->gad_editor = NULL;
    }
    else if (app->gad_editor != NULL)
    {
        DisposeObject(app->gad_editor);
        app->gad_editor = NULL;
    }

    if (app->TextEditorBase != NULL)
    {
        CloseLibrary(app->TextEditorBase);
        app->TextEditorBase = NULL;
    }

    if (app->LayoutBase != NULL)
    {
        CloseLibrary(app->LayoutBase);
        app->LayoutBase = NULL;
    }

    if (app->WindowBase != NULL)
    {
        CloseLibrary(app->WindowBase);
        app->WindowBase = NULL;
    }
}

/*****************************************************************************
 *
 *  App_HandleInput
 *
 *  Purpose:
 *      Keep event flow explicit even though this lesson has no rich editor
 *      command set yet.
 *
 *****************************************************************************/

static void App_HandleInput(struct App *app)
{
    ULONG result;
    UWORD code;

    while ((result = RA_HandleInput(app->win_obj, &code)) != WMHI_LASTMSG)
    {
        switch (result & WMHI_CLASSMASK)
        {
            case WMHI_CLOSEWINDOW:
                app->running = 0;
                break;

            case WMHI_GADGETUP:
                switch (result & WMHI_GADGETMASK)
                {
                    case GID_EDITOR:
                        /*
                         * This lesson does not yet process editor content.
                         * It exists to establish multiline editor lifecycle
                         * and native placement in the object tree.
                         */
                        break;

                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }
}

static void App_Run(struct App *app)
{
    ULONG signals;
    ULONG waitmask;

    waitmask = app->win_sigmask | SIGBREAKF_CTRL_C;

    while (app->running)
    {
        signals = Wait(waitmask);

        if (signals & SIGBREAKF_CTRL_C)
            app->running = 0;

        if (signals & app->win_sigmask)
            App_HandleInput(app);
    }
}

int main(void)
{
    struct App app;
    int rc;

    rc = 20;
    App_Clear(&app);

    if (!UI_Create(&app))
        goto out;

    if (!UI_Open(&app))
        goto out;

    App_Run(&app);
    rc = 0;

out:
    UI_Close(&app);
    UI_Destroy(&app);

    return rc;
}

