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
#include <proto/scroller.h>
#include <proto/space.h>

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;
    struct Library *ScrollerBase;
    struct Library *SpaceBase;

    Object *win_obj;

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
 *  UI_Create
 *
 *  Purpose:
 *      Demonstrate a larger-than-visible layout region in a native window.
 *
 *  Notes:
 *      This is a structural lesson, not a document viewer.
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

    app->ScrollerBase = OpenLibrary("scroller.gadget", 47);
    if (app->ScrollerBase == NULL)
        goto out;

    app->SpaceBase = OpenLibrary("space.gadget", 47);
    if (app->SpaceBase == NULL)
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 18_ScrollableLayout",
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

                    StartMember,
                        ScrollerObject,
                            GA_RelVerify, TRUE,
                            SCROLLER_Top, 0,
                            SCROLLER_Total, 100,
                            SCROLLER_Visible, 20,
                        End,
                    EndMember,

                    StartMember,
                        VLayoutObject,
                            LAYOUT_SpaceOuter, TRUE,

                            StartMember,
                                SpaceObject, SPACE_MinHeight, 20, End,
                            EndMember,
                            StartMember,
                                SpaceObject, SPACE_MinHeight, 20, End,
                            EndMember,
                            StartMember,
                                SpaceObject, SPACE_MinHeight, 20, End,
                            EndMember,
                            StartMember,
                                SpaceObject, SPACE_MinHeight, 20, End,
                            EndMember,
                            StartMember,
                                SpaceObject, SPACE_MinHeight, 20, End,
                            EndMember,
                        End,
                    EndMember,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
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
    }

    if (app->SpaceBase != NULL)
    {
        CloseLibrary(app->SpaceBase);
        app->SpaceBase = NULL;
    }

    if (app->ScrollerBase != NULL)
    {
        CloseLibrary(app->ScrollerBase);
        app->ScrollerBase = NULL;
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

