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
#include <proto/checkbox.h>

#define GID_ENABLE   1
#define GID_ACTION   2

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;
    struct Library *ButtonBase;
    struct Library *CheckBoxBase;

    Object *win_obj;
    Object *gad_enable;
    Object *gad_action;

    struct Window *win;

    ULONG win_sigmask;
    int running;
    int enabled_state;
    int action_count;
};

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
    app->enabled_state = 0;
    app->action_count = 0;
}

static void UI_UpdateState(struct App *app)
{
    char title[96];

    if (app->gad_action != NULL)
    {
        SetAttrs(app->gad_action,
            GA_Disabled, app->enabled_state ? FALSE : TRUE,
            TAG_DONE);
    }

    sprintf(title,
        "ReXamples - 20_EventDispatch [enabled=%d count=%d]",
        app->enabled_state,
        app->action_count);

    if (app->win != NULL)
        SetWindowTitles(app->win, (UBYTE *)title, (UBYTE *)~0);
}

/*****************************************************************************
 *
 *  App_DispatchEnable
 *
 *  Purpose:
 *      Update state from the checkbox and propagate that state to the button.
 *
 *****************************************************************************/

static void App_DispatchEnable(struct App *app)
{
    ULONG selected;

    GetAttr(GA_Selected, app->gad_enable, &selected);
    app->enabled_state = (selected != 0);
    UI_UpdateState(app);
}

/*****************************************************************************
 *
 *  App_DispatchAction
 *
 *  Purpose:
 *      Execute the action path only when enabled and reflect the result visibly.
 *
 *****************************************************************************/

static void App_DispatchAction(struct App *app)
{
    if (!app->enabled_state)
        return;

    app->action_count++;
    UI_UpdateState(app);
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

    app->ButtonBase = OpenLibrary("button.gadget", 47);
    if (app->ButtonBase == NULL)
        goto out;

    app->CheckBoxBase = OpenLibrary("checkbox.gadget", 47);
    if (app->CheckBoxBase == NULL)
        goto out;

    app->gad_enable =
        CheckBoxObject,
            GA_ID, GID_ENABLE,
            GA_Text, (ULONG)"_Enable action",
            GA_Selected, FALSE,
        End;

    if (app->gad_enable == NULL)
        goto out;

    app->gad_action =
        ButtonObject,
            GA_ID, GID_ACTION,
            GA_Text, (ULONG)"_Action",
            GA_Disabled, TRUE,
        End;

    if (app->gad_action == NULL)
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 20_EventDispatch",
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
                    StartMember, app->gad_enable, EndMember,
                    StartMember, app->gad_action, EndMember,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    if (!ok && app->gad_action != NULL)
    {
        DisposeObject(app->gad_action);
        app->gad_action = NULL;
    }

    if (!ok && app->gad_enable != NULL)
    {
        DisposeObject(app->gad_enable);
        app->gad_enable = NULL;
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

    UI_UpdateState(app);

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
        app->gad_enable = NULL;
        app->gad_action = NULL;
    }
    else
    {
        if (app->gad_action != NULL)
        {
            DisposeObject(app->gad_action);
            app->gad_action = NULL;
        }

        if (app->gad_enable != NULL)
        {
            DisposeObject(app->gad_enable);
            app->gad_enable = NULL;
        }
    }

    if (app->CheckBoxBase != NULL)
    {
        CloseLibrary(app->CheckBoxBase);
        app->CheckBoxBase = NULL;
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
                    case GID_ENABLE:
                        App_DispatchEnable(app);
                        break;

                    case GID_ACTION:
                        App_DispatchAction(app);
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

