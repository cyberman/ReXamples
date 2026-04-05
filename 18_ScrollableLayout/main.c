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
#include <gadgets/button.h>
#include <gadgets/scroller.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/button.h>
#include <proto/scroller.h>

/* vbcc proto headers used here expect global library bases. */
struct IntuitionBase *IntuitionBase = NULL;
struct Library *WindowBase = NULL;
struct Library *LayoutBase = NULL;
struct Library *ButtonBase = NULL;
struct Library *ScrollerBase = NULL;

/* Local prototype needed by this build setup. */
ULONG DoMethodA(Object *obj, Msg msg);

struct App
{
    struct Library *IntuitionLib;
    struct Library *WindowLib;
    struct Library *LayoutLib;
    struct Library *ButtonLib;
    struct Library *ScrollerLib;

    Object *line1_obj;
    Object *line2_obj;
    Object *line3_obj;
    Object *line4_obj;
    Object *line5_obj;
    Object *content_layout;
    Object *scroll_obj;
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

    struct TagItem line1_tags[] =
    {
        { GA_ReadOnly, TRUE },
        { GA_Text,     (ULONG)"Scrollable content line 1" },
        { TAG_DONE,    0 }
    };

    struct TagItem line2_tags[] =
    {
        { GA_ReadOnly, TRUE },
        { GA_Text,     (ULONG)"Scrollable content line 2" },
        { TAG_DONE,    0 }
    };

    struct TagItem line3_tags[] =
    {
        { GA_ReadOnly, TRUE },
        { GA_Text,     (ULONG)"Scrollable content line 3" },
        { TAG_DONE,    0 }
    };

    struct TagItem line4_tags[] =
    {
        { GA_ReadOnly, TRUE },
        { GA_Text,     (ULONG)"Scrollable content line 4" },
        { TAG_DONE,    0 }
    };

    struct TagItem line5_tags[] =
    {
        { GA_ReadOnly, TRUE },
        { GA_Text,     (ULONG)"Scrollable content line 5" },
        { TAG_DONE,    0 }
    };

    struct TagItem content_layout_tags[] =
    {
        { LAYOUT_Orientation, LAYOUT_ORIENT_VERT },
        { LAYOUT_SpaceOuter,  TRUE },
        { LAYOUT_AddChild,    0 },   /* line 1 */
        { LAYOUT_AddChild,    0 },   /* line 2 */
        { LAYOUT_AddChild,    0 },   /* line 3 */
        { LAYOUT_AddChild,    0 },   /* line 4 */
        { LAYOUT_AddChild,    0 },   /* line 5 */
        { TAG_DONE,           0 }
    };

    struct TagItem scroller_tags[] =
    {
        { GA_RelVerify,     TRUE },
        { SCROLLER_Top,     0 },
        { SCROLLER_Total,   100 },
        { SCROLLER_Visible, 20 },
        { TAG_DONE,         0 }
    };

    struct TagItem row_layout_tags[] =
    {
        { LAYOUT_Orientation, LAYOUT_ORIENT_HORIZ },
        { LAYOUT_SpaceOuter,  TRUE },
        { LAYOUT_AddChild,    0 },   /* content */
        { LAYOUT_AddChild,    0 },   /* scroller */
        { TAG_DONE,           0 }
    };

    struct TagItem root_layout_tags[] =
    {
        { LAYOUT_Orientation, LAYOUT_ORIENT_VERT },
        { LAYOUT_SpaceOuter,  TRUE },
        { LAYOUT_AddChild,    0 },   /* row */
        { TAG_DONE,           0 }
    };

    struct TagItem window_tags[] =
    {
        { WA_Title,        (ULONG)"ReXamples - 18_ScrollableLayout" },
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

    app->ScrollerLib = OpenLibrary("scroller.gadget", 47);
    if (app->ScrollerLib == NULL)
        goto out;
    ScrollerBase = app->ScrollerLib;

    app->line1_obj = NewObjectA(BUTTON_GetClass(), NULL, line1_tags);
    if (app->line1_obj == NULL)
        goto out;

    app->line2_obj = NewObjectA(BUTTON_GetClass(), NULL, line2_tags);
    if (app->line2_obj == NULL)
        goto out;

    app->line3_obj = NewObjectA(BUTTON_GetClass(), NULL, line3_tags);
    if (app->line3_obj == NULL)
        goto out;

    app->line4_obj = NewObjectA(BUTTON_GetClass(), NULL, line4_tags);
    if (app->line4_obj == NULL)
        goto out;

    app->line5_obj = NewObjectA(BUTTON_GetClass(), NULL, line5_tags);
    if (app->line5_obj == NULL)
        goto out;

    content_layout_tags[2].ti_Data = (ULONG)app->line1_obj;
    content_layout_tags[3].ti_Data = (ULONG)app->line2_obj;
    content_layout_tags[4].ti_Data = (ULONG)app->line3_obj;
    content_layout_tags[5].ti_Data = (ULONG)app->line4_obj;
    content_layout_tags[6].ti_Data = (ULONG)app->line5_obj;

    app->content_layout = NewObjectA(LAYOUT_GetClass(), NULL, content_layout_tags);
    if (app->content_layout == NULL)
        goto out;

    app->scroll_obj = NewObjectA(SCROLLER_GetClass(), NULL, scroller_tags);
    if (app->scroll_obj == NULL)
        goto out;

    row_layout_tags[2].ti_Data = (ULONG)app->content_layout;
    row_layout_tags[3].ti_Data = (ULONG)app->scroll_obj;

    app->row_layout = NewObjectA(LAYOUT_GetClass(), NULL, row_layout_tags);
    if (app->row_layout == NULL)
        goto out;

    root_layout_tags[2].ti_Data = (ULONG)app->row_layout;

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
        app->scroll_obj = NULL;
        app->content_layout = NULL;
        app->line1_obj = NULL;
        app->line2_obj = NULL;
        app->line3_obj = NULL;
        app->line4_obj = NULL;
        app->line5_obj = NULL;
    }
    else
    {
        if (app->root_layout != NULL)
        {
            DisposeObject(app->root_layout);
            app->root_layout = NULL;
            app->row_layout = NULL;
            app->scroll_obj = NULL;
            app->content_layout = NULL;
            app->line1_obj = NULL;
            app->line2_obj = NULL;
            app->line3_obj = NULL;
            app->line4_obj = NULL;
            app->line5_obj = NULL;
        }
        else
        {
            if (app->row_layout != NULL)
            {
                DisposeObject(app->row_layout);
                app->row_layout = NULL;
                app->scroll_obj = NULL;
                app->content_layout = NULL;
                app->line1_obj = NULL;
                app->line2_obj = NULL;
                app->line3_obj = NULL;
                app->line4_obj = NULL;
                app->line5_obj = NULL;
            }
            else
            {
                if (app->scroll_obj != NULL)
                {
                    DisposeObject(app->scroll_obj);
                    app->scroll_obj = NULL;
                }

                if (app->content_layout != NULL)
                {
                    DisposeObject(app->content_layout);
                    app->content_layout = NULL;
                    app->line1_obj = NULL;
                    app->line2_obj = NULL;
                    app->line3_obj = NULL;
                    app->line4_obj = NULL;
                    app->line5_obj = NULL;
                }
                else
                {
                    if (app->line5_obj != NULL)
                    {
                        DisposeObject(app->line5_obj);
                        app->line5_obj = NULL;
                    }

                    if (app->line4_obj != NULL)
                    {
                        DisposeObject(app->line4_obj);
                        app->line4_obj = NULL;
                    }

                    if (app->line3_obj != NULL)
                    {
                        DisposeObject(app->line3_obj);
                        app->line3_obj = NULL;
                    }

                    if (app->line2_obj != NULL)
                    {
                        DisposeObject(app->line2_obj);
                        app->line2_obj = NULL;
                    }

                    if (app->line1_obj != NULL)
                    {
                        DisposeObject(app->line1_obj);
                        app->line1_obj = NULL;
                    }
                }
            }
        }
    }

    if (app->ScrollerLib != NULL)
    {
        CloseLibrary(app->ScrollerLib);
        app->ScrollerLib = NULL;
        ScrollerBase = NULL;
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
