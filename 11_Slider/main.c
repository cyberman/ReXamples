#include <stdio.h>
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
#include <proto/slider.h>

#define GID_LEVEL  1

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;
    struct Library *SliderBase;

    Object *win_obj;
    Object *gad_level;

    struct Window *win;

    ULONG win_sigmask;
    int running;
    ULONG level;
};

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
    app->level = 50;
}

static void UI_UpdateTitle(struct App *app)
{
    char title[80];

    sprintf(title, "ReXamples - 11_Slider [%lu]", app->level);

    if (app->win != NULL)
        SetWindowTitles(app->win, (UBYTE *)title, (UBYTE *)~0);
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

    app->SliderBase = OpenLibrary("slider.gadget", 47);
    if (app->SliderBase == NULL)
        goto out;

    app->gad_level =
        SliderObject,
            GA_ID, GID_LEVEL,
            GA_RelVerify, TRUE,
            SLIDER_Min, 0,
            SLIDER_Max, 100,
            SLIDER_Level, app->level,
        End;

    if (app->gad_level == NULL)
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 11_Slider [50]",
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
                    StartMember, app->gad_level, EndMember,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    if (!ok && app->gad_level != NULL)
    {
        DisposeObject(app->gad_level);
        app->gad_level = NULL;
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
        app->gad_level = NULL;
    }
    else if (app->gad_level != NULL)
    {
        DisposeObject(app->gad_level);
        app->gad_level = NULL;
    }

    if (app->SliderBase != NULL)
    {
        CloseLibrary(app->SliderBase);
        app->SliderBase = NULL;
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
    ULONG level;

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
                    case GID_LEVEL:
                        GetAttr(SLIDER_Level, app->gad_level, &level);
                        app->level = level;
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

