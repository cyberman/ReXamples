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
#include <proto/fuelgauge.h>
#include <proto/button.h>

#define GID_STEP   1

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;
    struct Library *FuelGaugeBase;
    struct Library *ButtonBase;

    Object *win_obj;
    Object *gad_gauge;
    Object *gad_step;

    struct Window *win;

    ULONG win_sigmask;
    int running;
    ULONG level;
};

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
    app->level = 0;
}

static void UI_UpdateTitle(struct App *app)
{
    char title[96];

    sprintf(title, "ReXamples - 16_Gauge [%lu%%]", app->level);

    if (app->win != NULL)
        SetWindowTitles(app->win, (UBYTE *)title, (UBYTE *)~0);
}

static void UI_UpdateGauge(struct App *app)
{
    if (app->gad_gauge != NULL)
    {
        SetAttrs(app->gad_gauge,
            FUELGAUGE_Level, app->level,
            TAG_DONE);
    }

    UI_UpdateTitle(app);
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

    app->FuelGaugeBase = OpenLibrary("fuelgauge.gadget", 47);
    if (app->FuelGaugeBase == NULL)
        goto out;

    app->ButtonBase = OpenLibrary("button.gadget", 47);
    if (app->ButtonBase == NULL)
        goto out;

    app->gad_gauge =
        FuelGaugeObject,
            GA_RelVerify, TRUE,
            FUELGAUGE_Min, 0,
            FUELGAUGE_Max, 100,
            FUELGAUGE_Level, app->level,
            FUELGAUGE_Percent, TRUE,
        End;

    if (app->gad_gauge == NULL)
        goto out;

    app->gad_step =
        ButtonObject,
            GA_ID, GID_STEP,
            GA_Text, (ULONG)"_Step",
        End;

    if (app->gad_step == NULL)
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 16_Gauge [0%]",
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
                    StartMember, app->gad_gauge, EndMember,
                    StartMember, app->gad_step, EndMember,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    if (!ok && app->gad_step != NULL)
    {
        DisposeObject(app->gad_step);
        app->gad_step = NULL;
    }

    if (!ok && app->gad_gauge != NULL)
    {
        DisposeObject(app->gad_gauge);
        app->gad_gauge = NULL;
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

    UI_UpdateGauge(app);

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
        app->gad_gauge = NULL;
        app->gad_step = NULL;
    }
    else
    {
        if (app->gad_step != NULL)
        {
            DisposeObject(app->gad_step);
            app->gad_step = NULL;
        }

        if (app->gad_gauge != NULL)
        {
            DisposeObject(app->gad_gauge);
            app->gad_gauge = NULL;
        }
    }

    if (app->ButtonBase != NULL)
    {
        CloseLibrary(app->ButtonBase);
        app->ButtonBase = NULL;
    }

    if (app->FuelGaugeBase != NULL)
    {
        CloseLibrary(app->FuelGaugeBase);
        app->FuelGaugeBase = NULL;
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
                    case GID_STEP:
                        app->level += 10;
                        if (app->level > 100)
                            app->level = 0;
                        UI_UpdateGauge(app);
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

