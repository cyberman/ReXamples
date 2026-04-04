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

/*****************************************************************************
 *
 *  struct App
 *
 *  Purpose:
 *      Top-level application state for the lesson.
 *
 *  Notes:
 *      This lesson keeps the state intentionally small:
 *      required class bases, the window object, the opened Intuition window,
 *      and the signal mask used by the event loop.
 *
 *****************************************************************************/

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;

    Object *win_obj;
    struct Window *win;

    ULONG win_sigmask;
    int running;
};

/*****************************************************************************
 *
 *  App_Clear
 *
 *  Purpose:
 *      Reset application state to a known empty state.
 *
 *  Inputs:
 *      app - Pointer to application state structure.
 *
 *  Side effects:
 *      Clears all fields in the application state structure.
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
 *      Open required classes and create the minimal window object tree.
 *
 *  Inputs:
 *      app - Pointer to application state structure.
 *
 *  Returns:
 *      Non-zero on success, zero on failure.
 *
 *  Ownership:
 *      On success, the application state owns the opened class libraries
 *      and the created window object.
 *
 *  Failure behaviour:
 *      On failure, no partial ownership is retained by this function.
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

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 01_Window",
            WA_Activate,    TRUE,
            WA_DepthGadget, TRUE,
            WA_DragBar,     TRUE,
            WA_CloseGadget, TRUE,
            WA_SizeGadget,  TRUE,
            WA_IDCMP,       IDCMP_CLOSEWINDOW,
            WINDOW_Position, WPOS_CENTERSCREEN,
            WINDOW_Layout,
                LayoutObject,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    return ok;
}

/*****************************************************************************
 *
 *  UI_Open
 *
 *  Purpose:
 *      Open the lesson window and capture the signal mask needed for Wait().
 *
 *  Inputs:
 *      app - Pointer to application state structure.
 *
 *  Returns:
 *      Non-zero on success, zero on failure.
 *
 *  Ownership:
 *      On success, the application state owns an open Intuition window.
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

/*****************************************************************************
 *
 *  UI_Close
 *
 *  Purpose:
 *      Close the opened Intuition window if present.
 *
 *  Inputs:
 *      app - Pointer to application state structure.
 *
 *  Side effects:
 *      Clears the open window pointer and signal mask.
 *
 *****************************************************************************/

static void UI_Close(struct App *app)
{
    if (app->win != NULL)
    {
        RA_CloseWindow(app->win_obj);
        app->win = NULL;
    }

    app->win_sigmask = 0;
}

/*****************************************************************************
 *
 *  UI_Destroy
 *
 *  Purpose:
 *      Dispose the object tree and close class libraries.
 *
 *  Inputs:
 *      app - Pointer to application state structure.
 *
 *  Ownership:
 *      Releases all UI resources still owned by the application state.
 *
 *****************************************************************************/

static void UI_Destroy(struct App *app)
{
    if (app->win_obj != NULL)
    {
        DisposeObject(app->win_obj);
        app->win_obj = NULL;
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
 *  App_Run
 *
 *  Purpose:
 *      Run the minimal event loop until the window is closed or Ctrl-C arrives.
 *
 *  Inputs:
 *      app - Pointer to application state structure.
 *
 *****************************************************************************/

static void App_Run(struct App *app)
{
    ULONG signals;
    ULONG result;
    UWORD code;
    ULONG waitmask;

    waitmask = app->win_sigmask | SIGBREAKF_CTRL_C;

    while (app->running)
    {
        signals = Wait(waitmask);

        if (signals & SIGBREAKF_CTRL_C)
        {
            app->running = 0;
        }

        if (signals & app->win_sigmask)
        {
            while ((result = RA_HandleInput(app->win_obj, &code)) != WMHI_LASTMSG)
            {
                switch (result & WMHI_CLASSMASK)
                {
                    case WMHI_CLOSEWINDOW:
                        app->running = 0;
                        break;

                    default:
                        break;
                }
            }
        }
    }
}

/*****************************************************************************
 *
 *  main
 *
 *  Purpose:
 *      Entry point for lesson 01_Window.
 *
 *****************************************************************************/

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

