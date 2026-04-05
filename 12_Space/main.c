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
#include <gadgets/space.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/space.h>

/* vbcc proto headers used here expect global library bases. */
struct IntuitionBase *IntuitionBase = NULL;
struct Library *WindowBase = NULL;
struct Library *LayoutBase = NULL;
struct Library *SpaceBase = NULL;

/* Local prototype needed by this build setup. */
ULONG DoMethodA(Object *obj, Msg msg);

struct App
{
    struct Library *IntuitionLib;
    struct Library *WindowLib;
    struct Library *LayoutLib;
    struct Library *SpaceLib;

    Object *space_top;
    Object *space_left;
    Object *space_center;
    Object *space_right;
    Object *space_bottom;
    Object *row_layout;
    Object *root_layout;
    Object *win_obj;

    struct Window *win;

    ULONG win_sigmask;
    int running;
};

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
}

static int UI_Create(struct App *app)
{
    int ok;

    struct TagItem top_space_tags[] =
    {
        { SPACE_MinHeight, 12 },
        { TAG_DONE,        0 }
    };

    struct TagItem side_space_tags[] =
    {
        { SPACE_MinWidth,  24 },
        { SPACE_MinHeight, 12 },
        { TAG_DONE,        0 }
    };

    struct TagItem center_space_tags[] =
    {
        { SPACE_MinWidth,  64 },
        { SPACE_MinHeight, 12 },
        { TAG_DONE,        0 }
    };

    struct TagItem bottom_space_tags[] =
    {
        { SPACE_MinHeight, 12 },
        { TAG_DONE,        0 }
    };

    struct TagItem row_layout_tags[] =
    {
        { LAYOUT_Orientation, LAYOUT_ORIENT_HORIZ },
        { LAYOUT_SpaceOuter,  TRUE },
        { LAYOUT_AddChild,    0 },   /* left */
        { LAYOUT_AddChild,    0 },   /* center */
        { LAYOUT_AddChild,    0 },   /* right */
        { TAG_DONE,           0 }
    };

    struct TagItem root_layout_tags[] =
    {
        { LAYOUT_Orientation, LAYOUT_ORIENT_VERT },
        { LAYOUT_SpaceOuter,  TRUE },
        { LAYOUT_AddChild,    0 },   /* top */
        { LAYOUT_AddChild,    0 },   /* row */
        { LAYOUT_AddChild,    0 },   /* bottom */
        { TAG_DONE,           0 }
    };

    struct TagItem window_tags[] =
    {
        { WA_Title,        (ULONG)"ReXamples - 12_Space" },
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

    app->SpaceLib = OpenLibrary("space.gadget", 47);
    if (app->SpaceLib == NULL)
        goto out;
    SpaceBase = app->SpaceLib;

    app->space_top = NewObjectA(SPACE_GetClass(), NULL, top_space_tags);
    if (app->space_top == NULL)
        goto out;

    app->space_left = NewObjectA(SPACE_GetClass(), NULL, side_space_tags);
    if (app->space_left == NULL)
        goto out;

    app->space_center = NewObjectA(SPACE_GetClass(), NULL, center_space_tags);
    if (app->space_center == NULL)
        goto out;

    app->space_right = NewObjectA(SPACE_GetClass(), NULL, side_space_tags);
    if (app->space_right == NULL)
        goto out;

    app->space_bottom = NewObjectA(SPACE_GetClass(), NULL, bottom_space_tags);
    if (app->space_bottom == NULL)
        goto out;

    row_layout_tags[2].ti_Data = (ULONG)app->space_left;
    row_layout_tags[3].ti_Data = (ULONG)app->space_center;
    row_layout_tags[4].ti_Data = (ULONG)app->space_right;

    app->row_layout = NewObjectA(LAYOUT_GetClass(), NULL, row_layout_tags);
    if (app->row_layout == NULL)
        goto out;

    root_layout_tags[2].ti_Data = (ULONG)app->space_top;
    root_layout_tags[3].ti_Data = (ULONG)app->row_layout;
    root_layout_tags[4].ti_Data = (ULONG)app->space_bottom;

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
        app->row_layout = NULL;
        app->space_top = NULL;
        app->space_left = NULL;
        app->space_center = NULL;
        app->space_right = NULL;
        app->space_bottom = NULL;
    }
    else
    {
        if (app->root_layout != NULL)
        {
            DisposeObject(app->root_layout);
            app->root_layout = NULL;
            app->row_layout = NULL;
            app->space_top = NULL;
            app->space_left = NULL;
            app->space_center = NULL;
            app->space_right = NULL;
            app->space_bottom = NULL;
        }
        else
        {
            if (app->row_layout != NULL)
            {
                DisposeObject(app->row_layout);
                app->row_layout = NULL;
                app->space_left = NULL;
                app->space_center = NULL;
                app->space_right = NULL;
            }

            if (app->space_bottom != NULL)
            {
                DisposeObject(app->space_bottom);
                app->space_bottom = NULL;
            }

            if (app->space_top != NULL)
            {
                DisposeObject(app->space_top);
                app->space_top = NULL;
            }

            if (app->space_right != NULL)
            {
                DisposeObject(app->space_right);
                app->space_right = NULL;
            }

            if (app->space_center != NULL)
            {
                DisposeObject(app->space_center);
                app->space_center = NULL;
            }

            if (app->space_left != NULL)
            {
                DisposeObject(app->space_left);
                app->space_left = NULL;
            }
        }
    }

    if (app->SpaceLib != NULL)
    {
        CloseLibrary(app->SpaceLib);
        app->SpaceLib = NULL;
        SpaceBase = NULL;
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
