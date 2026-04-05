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
#include <gadgets/checkbox.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/checkbox.h>

/* vbcc proto headers used here expect global library bases. */
struct IntuitionBase *IntuitionBase = NULL;
struct Library *WindowBase = NULL;
struct Library *LayoutBase = NULL;
struct Library *CheckBoxBase = NULL;

/* Local prototype needed by this build setup. */
ULONG DoMethodA(Object *obj, Msg msg);

#define GID_ENABLED  1

struct App
{
    struct Library *IntuitionLib;
    struct Library *WindowLib;
    struct Library *LayoutLib;
    struct Library *CheckBoxLib;

    Object *check_obj;
    Object *root_layout;
    Object *win_obj;

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

    struct TagItem check_tags[] =
    {
        { GA_ID,        GID_ENABLED },
        { GA_RelVerify, TRUE },
        { GA_Text,      (ULONG)"_Enabled" },
        { GA_Selected,  FALSE },
        { TAG_DONE,     0 }
    };

    struct TagItem root_layout_tags[] =
    {
        { LAYOUT_Orientation, LAYOUT_ORIENT_VERT },
        { LAYOUT_SpaceOuter,  TRUE },
        { LAYOUT_AddChild,    0 },   /* checkbox */
        { TAG_DONE,           0 }
    };

    struct TagItem window_tags[] =
    {
        { WA_Title,        (ULONG)"ReXamples - 04_CheckBox [OFF]" },
        { WA_Activate,     TRUE },
        { WA_DepthGadget,  TRUE },
        { WA_DragBar,      TRUE },
        { WA_CloseGadget,  TRUE },
        { WA_SizeGadget,   TRUE },
        { WA_IDCMP,        IDCMP_CLOSEWINDOW },
        { WINDOW_Position, WPOS_CENTERSCREEN },
        { WINDOW_Layout,   0 },   /* filled after root layout exists */
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

    app->CheckBoxLib = OpenLibrary("checkbox.gadget", 47);
    if (app->CheckBoxLib == NULL)
        goto out;
    CheckBoxBase = app->CheckBoxLib;

    app->check_obj = NewObjectA(CHECKBOX_GetClass(), NULL, check_tags);
    if (app->check_obj == NULL)
        goto out;

    root_layout_tags[2].ti_Data = (ULONG)app->check_obj;

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

    UI_UpdateTitle(app);

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
        app->check_obj = NULL;
    }
    else
    {
        if (app->root_layout != NULL)
        {
            DisposeObject(app->root_layout);
            app->root_layout = NULL;
            app->check_obj = NULL;
        }
        else if (app->check_obj != NULL)
        {
            DisposeObject(app->check_obj);
            app->check_obj = NULL;
        }
    }

    if (app->CheckBoxLib != NULL)
    {
        CloseLibrary(app->CheckBoxLib);
        app->CheckBoxLib = NULL;
        CheckBoxBase = NULL;
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

static void App_DispatchEnabled(struct App *app)
{
    ULONG selected;

    GetAttr(GA_Selected, app->check_obj, &selected);
    app->enabled_state = (selected != 0);

    UI_UpdateTitle(app);
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
                            case GID_ENABLED:
                                App_DispatchEnabled(app);
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
