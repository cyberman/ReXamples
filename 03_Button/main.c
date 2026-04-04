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
#include <proto/button.h>
#include <proto/requester.h>

/*****************************************************************************
 *
 *  Lesson gadget IDs
 *
 *  Purpose:
 *      Visible gadget identity for lesson dispatch.
 *
 *****************************************************************************/

#define GID_HELLO  1

/*****************************************************************************
 *
 *  struct App
 *
 *  Purpose:
 *      Application state for the button lesson.
 *
 *****************************************************************************/

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;
    struct Library *ButtonBase;
    struct Library *RequesterBase;

    Object *win_obj;
    Object *gad_hello;

    struct Window *win;

    ULONG win_sigmask;
    int running;
};

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
}

/*****************************************************************************
 *
 *  UI_ShowInfo
 *
 *  Purpose:
 *      Show a simple native requester as visible feedback for the button action.
 *
 *  Inputs:
 *      app - Pointer to application state structure.
 *
 *****************************************************************************/

static void UI_ShowInfo(struct App *app)
{
    if (app->win_obj != NULL)
    {
        RA_Request(app->win_obj, NULL,
            "ReXamples|03_Button",
            "Ok",
            "The button event was dispatched clearly.");
    }
}

/*****************************************************************************
 *
 *  UI_Create
 *
 *  Purpose:
 *      Create a minimal window with one button gadget.
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

    app->ButtonBase = OpenLibrary("button.gadget", 47);
    if (app->ButtonBase == NULL)
        goto out;

    app->RequesterBase = OpenLibrary("requester.class", 47);
    if (app->RequesterBase == NULL)
        goto out;

    app->gad_hello =
        ButtonObject,
            GA_ID, GID_HELLO,
            GA_Text, (ULONG)"_Hello",
        End;

    if (app->gad_hello == NULL)
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 03_Button",
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
                    StartMember, app->gad_hello, EndMember,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    if (!ok && app->gad_hello != NULL)
    {
        DisposeObject(app->gad_hello);
        app->gad_hello = NULL;
    }

    return ok;
}

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
        app->gad_hello = NULL;
    }
    else if (app->gad_hello != NULL)
    {
        DisposeObject(app->gad_hello);
        app->gad_hello = NULL;
    }

    if (app->RequesterBase != NULL)
    {
        CloseLibrary(app->RequesterBase);
        app->RequesterBase = NULL;
    }

    if (app->ButtonBase != NULL)
    {
        CloseLibrary(app->ButtonBase);
        app->ButtonBase = NULL;
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
 *      Decode window messages and dispatch button actions explicitly.
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
                    case GID_HELLO:
                        UI_ShowInfo(app);
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
        {
            app->running = 0;
        }

        if (signals & app->win_sigmask)
        {
            App_HandleInput(app);
        }
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

