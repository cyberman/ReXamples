#include <string.h>
#include <stdio.h>

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
#include <gadgets/button.h>
#include <gadgets/fuelgauge.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/button.h>
#include <proto/fuelgauge.h>

/* vbcc proto headers used here expect global library bases. */
struct IntuitionBase *IntuitionBase = NULL;
struct Library *WindowBase = NULL;
struct Library *LayoutBase = NULL;
struct Library *ButtonBase = NULL;
struct Library *FuelGaugeBase = NULL;

/* Local prototype needed by this build setup. */
ULONG DoMethodA(Object *obj, Msg msg);

#define GID_STEP  1

struct App
{
    struct Library *IntuitionLib;
    struct Library *WindowLib;
    struct Library *LayoutLib;
    struct Library *ButtonLib;
    struct Library *FuelGaugeLib;

    Object *gauge_obj;
    Object *button_obj;
    Object *root_layout;
    Object *win_obj;

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
    SetAttrs(app->gauge_obj,
        FUELGAUGE_Level, app->level,
        TAG_DONE);

    UI_UpdateTitle(app);
}

static int UI_Create(struct App *app)
{
    int ok;

    struct TagItem gauge_tags[] =
    {
        { FUELGAUGE_Min,     0 },
        { FUELGAUGE_Max,     100 },
        { FUELGAUGE_Level,   0 },
        { FUELGAUGE_Percent, TRUE },
        { TAG_DONE,          0 }
    };

    struct TagItem button_tags[] =
    {
        { GA_ID,        GID_STEP },
        { GA_RelVerify, TRUE },
        { GA_Text,      (ULONG)"_Step" },
        { TAG_DONE,     0 }
    };

    struct TagItem root_layout_tags[] =
    {
        { LAYOUT_Orientation, LAYOUT_ORIENT_VERT },
        { LAYOUT_SpaceOuter,  TRUE },
        { LAYOUT_AddChild,    0 },   /* gauge */
        { LAYOUT_AddChild,    0 },   /* button */
        { TAG_DONE,           0 }
    };

    struct TagItem window_tags[] =
    {
        { WA_Title,        (ULONG)"ReXamples - 16_Gauge [0%]" },
        { WA_Activate,     TRUE },
        { WA_DepthGadget,  TRUE },
        { WA_DragBar,      TRUE },
        { WA_CloseGadget,  TRUE },
        { WA_SizeGadget,   TRUE },
        { WA_IDCMP,        IDCMP_CLOSEWINDOW },
        { WINDOW_Position, WPOS_CENTERSCREEN },
        { WINDOW_Layout,   0 },
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

    app->ButtonLib = OpenLibrary("button.gadget", 47);
    if (app->ButtonLib == NULL)
        goto out;
    ButtonBase = app->ButtonLib;

    app->FuelGaugeLib = OpenLibrary("fuelgauge.gadget", 47);
    if (app->FuelGaugeLib == NULL)
        goto out;
    FuelGaugeBase = app->FuelGaugeLib;

    app->gauge_obj = NewObjectA(FUELGAUGE_GetClass(), NULL, gauge_tags);
    if (app->gauge_obj == NULL)
        goto out;

    app->button_obj = NewObjectA(BUTTON_GetClass(), NULL, button_tags);
    if (app->button_obj == NULL)
        goto out;

    root_layout_tags[2].ti_Data = (ULONG)app->gauge_obj;
    root_layout_tags[3].ti_Data = (ULONG)app->button_obj;

    app->root_layout = NewObjectA(LAYOUT_GetClass(), NULL, root_layout_tags);
    if (app->root_layout == NULL)
        goto out;

    window_tags[8].ti_Data = (ULONG)app->root_layout;

    app->win_obj = NewObjectA(WINDOW_GetClass(), NULL, window_tags);
    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    return ok;
}

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

    UI_UpdateGauge(app);

    return 1;
}

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

static void UI_Destroy(struct App *app)
{
    if (app->win_obj != NULL)
    {
        DisposeObject(app->win_obj);
        app->win_obj = NULL;

        app->root_layout = NULL;
        app->button_obj = NULL;
        app->gauge_obj = NULL;
    }
    else
    {
        if (app->root_layout != NULL)
        {
            DisposeObject(app->root_layout);
            app->root_layout = NULL;
            app->button_obj = NULL;
            app->gauge_obj = NULL;
        }
        else
        {
            if (app->button_obj != NULL)
            {
                DisposeObject(app->button_obj);
                app->button_obj = NULL;
            }

            if (app->gauge_obj != NULL)
            {
                DisposeObject(app->gauge_obj);
                app->gauge_obj = NULL;
            }
        }
    }

    if (app->FuelGaugeLib != NULL)
    {
        CloseLibrary(app->FuelGaugeLib);
        app->FuelGaugeLib = NULL;
        FuelGaugeBase = NULL;
    }

    if (app->ButtonLib != NULL)
    {
        CloseLibrary(app->ButtonLib);
        app->ButtonLib = NULL;
        ButtonBase = NULL;
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

static void App_DispatchStep(struct App *app)
{
    app->level += 10;
    if (app->level > 100)
        app->level = 0;

    UI_UpdateGauge(app);
}

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

                    case WMHI_GADGETUP:
                        switch (result & WMHI_GADGETMASK)
                        {
                            case GID_STEP:
                                App_DispatchStep(app);
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
