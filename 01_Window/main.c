#include <string.h>

#include <exec/types.h>
#include <exec/libraries.h>
#include <utility/tagitem.h>
#include <dos/dosextens.h>

#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/intuitionbase.h>

#include <classes/window.h>
#include <gadgets/layout.h>

#include <clib/intuition_protos.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>

/* vbcc proto headers used here expect global library bases. */
struct IntuitionBase *IntuitionBase = NULL;
struct Library *WindowBase = NULL;
struct Library *LayoutBase = NULL;

/* Needed because the current header combination does not provide a visible
 * prototype for DoMethodA() in this build setup.
 */
ULONG DoMethodA(Object *obj, Msg msg);

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
    struct Library *IntuitionLib;
    struct Library *WindowLib;
    struct Library *LayoutLib;

    Object *layout_obj;
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

    struct TagItem layout_tags[] =
    {
        { TAG_DONE, 0 }
    };

    struct TagItem window_tags[] =
    {
        { WA_Title,        (ULONG)"ReXamples - 01_Window" },
        { WA_Activate,     TRUE },
        { WA_DepthGadget,  TRUE },
        { WA_DragBar,      TRUE },
        { WA_CloseGadget,  TRUE },
        { WA_SizeGadget,   TRUE },
        { WA_IDCMP,        IDCMP_CLOSEWINDOW },
        { WINDOW_Position, WPOS_CENTERSCREEN },
        { WINDOW_Layout,   0 },   /* filled after layout object exists */
        { TAG_DONE,        0 }
    };

    ok = 0;

    app->IntuitionLib = OpenLibrary("intuition.library", 39);
    if (app->IntuitionLib == NULL)
        goto out;
    IntuitionBase = (struct IntuitionBase *)app->IntuitionLib;

    app->WindowLib = OpenLibrary("window.class", 47);
    if (app->WindowLib == NULL)
        goto out;
    WindowBase = app->WindowLib;

    app->LayoutLib = OpenLibrary("layout.gadget", 47);
    if (app->LayoutLib == NULL)
        goto out;
    LayoutBase = app->LayoutLib;

    app->layout_obj = NewObjectA(LAYOUT_GetClass(), NULL, layout_tags);
    if (app->layout_obj == NULL)
        goto out;

    window_tags[8].ti_Data = (ULONG)app->layout_obj;

    app->win_obj = NewObjectA(WINDOW_GetClass(), NULL, window_tags);
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
    ULONG open_msg[2];

    open_msg[0] = WM_OPEN;
    open_msg[1] = 0;

    app->win = (struct Window *)DoMethodA(app->win_obj, (Msg)open_msg);
    if (app->win == NULL)
        return 0;

    app->win_sigmask = 1UL << app->win->UserPort->mp_SigBit;
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
    ULONG close_msg[2];

    if (app->win != NULL)
    {
        close_msg[0] = WM_CLOSE;
        close_msg[1] = 0;

        DoMethodA(app->win_obj, (Msg)close_msg);
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
        app->layout_obj = NULL;   /* owned by window object now */
    }
    else if (app->layout_obj != NULL)
    {
        DisposeObject(app->layout_obj);
        app->layout_obj = NULL;
    }

    if (app->LayoutLib != NULL)
    {
        CloseLibrary(app->LayoutLib);
        app->LayoutLib = NULL;
        LayoutBase = NULL;
    }

    if (app->WindowLib != NULL)
    {
        CloseLibrary(app->WindowLib);
        app->WindowLib = NULL;
        WindowBase = NULL;
    }

    if (app->IntuitionLib != NULL)
    {
        CloseLibrary(app->IntuitionLib);
        app->IntuitionLib = NULL;
        IntuitionBase = NULL;
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
    WORD code;
    ULONG waitmask;
    struct wmHandle handle_msg;

    waitmask = app->win_sigmask | SIGBREAKF_CTRL_C;

    while (app->running)
    {
        signals = Wait(waitmask);

        if (signals & SIGBREAKF_CTRL_C)
            app->running = 0;

        if (signals & app->win_sigmask)
        {
            handle_msg.MethodID = WM_HANDLEINPUT;
            handle_msg.wmh_Code = &code;

            while ((result = DoMethodA(app->win_obj, (Msg)&handle_msg)) != WMHI_LASTMSG)
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

