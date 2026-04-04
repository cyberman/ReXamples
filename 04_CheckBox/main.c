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
#include <proto/checkbox.h>

#define GID_ENABLED  1

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;
    struct Library *CheckBoxBase;

    Object *win_obj;
    Object *gad_enabled;

    struct Window *win;

    ULONG win_sigmask;
    int running;
    int enabled_state;
};

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
}

static void UI_UpdateTitle(struct App *app)
{
    if (app->win != NULL)
    {
        if (app->enabled_state)
            SetWindowTitles(app->win, (UBYTE *)"ReXamples - 04_CheckBox [ON]", (UBYTE *)~0);
        else
            SetWindowTitles(app->win, (UBYTE *)"ReXamples - 04_CheckBox [OFF]", (UBYTE *)~0);
    }
}

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

    app->CheckBoxBase = OpenLibrary("checkbox.gadget", 47);
    if (app->CheckBoxBase == NULL)
        goto out;

    app->enabled_state = 0;

    app->gad_enabled =
        CheckBoxObject,
            GA_ID, GID_ENABLED,
            GA_Text, (ULONG)"_Enabled",
            GA_Selected, FALSE,
        End;

    if (app->gad_enabled == NULL)
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 04_CheckBox [OFF]",
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
                    StartMember, app->gad_enabled, EndMember,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    if (!ok && app->gad_enabled != NULL)
    {
        DisposeObject(app->gad_enabled);
        app->gad_enabled = NULL;
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

    UI_UpdateTitle(app);

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
        app->gad_enabled = NULL;
    }
    else if (app->gad_enabled != NULL)
    {
        DisposeObject(app->gad_enabled);
        app->gad_enabled = NULL;
    }

    if (app->CheckBoxBase != NULL)
    {
        CloseLibrary(app->CheckBoxBase);
        app->CheckBoxBase = NULL;
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
    ULONG selected;

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
                    case GID_ENABLED:
                        GetAttr(GA_Selected, app->gad_enabled, &selected);
                        app->enabled_state = (selected != 0);
                        UI_UpdateTitle(app);
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

