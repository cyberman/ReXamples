#include <string.h>
#include <stdio.h>

#include <exec/types.h>
#include <exec/libraries.h>
#include <exec/lists.h>
#include <utility/tagitem.h>
#include <dos/dosextens.h>

#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/intuitionbase.h>

#include <classes/window.h>
#include <gadgets/layout.h>
#include <gadgets/button.h>
#include <gadgets/clicktab.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/button.h>
#include <proto/clicktab.h>

/* vbcc proto headers used here expect global library bases. */
struct IntuitionBase *IntuitionBase = NULL;
struct Library *WindowBase = NULL;
struct Library *LayoutBase = NULL;
struct Library *ButtonBase = NULL;
struct Library *ClickTabBase = NULL;

/* Local prototype needed by this build setup. */
ULONG DoMethodA(Object *obj, Msg msg);

#define GID_TABS  1

struct App
{
    struct Library *IntuitionLib;
    struct Library *WindowLib;
    struct Library *LayoutLib;
    struct Library *ButtonLib;
    struct Library *ClickTabLib;

    Object *tabs_obj;
    Object *content_obj;
    Object *root_layout;
    Object *win_obj;

    struct Window *win;
    struct List tab_labels;

    ULONG win_sigmask;
    int running;
    ULONG current_page;
};

static void TabList_Init(struct List *list)
{
    list->lh_Head = (struct Node *)&list->lh_Tail;
    list->lh_Tail = NULL;
    list->lh_TailPred = (struct Node *)&list->lh_Head;
    list->lh_Type = 0;
}

static void TabList_Destroy(struct App *app);

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
    TabList_Init(&app->tab_labels);
    app->current_page = 0;
}

static CONST_STRPTR App_PageLabel(ULONG page)
{
    switch (page)
    {
        case 0: return "Page 1";
        case 1: return "Page 2";
        case 2: return "Page 3";
        default: break;
    }

    return "Unknown";
}

static CONST_STRPTR App_PageText(ULONG page)
{
    switch (page)
    {
        case 0: return "This is page 1.";
        case 1: return "This is page 2.";
        case 2: return "This is page 3.";
        default: break;
    }

    return "Unknown page.";
}

static void UI_UpdateTitle(struct App *app)
{
    char title[96];

    sprintf(title, "ReXamples - 19_RegisterPages [%s]",
        App_PageLabel(app->current_page));

    if (app->win != NULL)
        SetWindowTitles(app->win, (UBYTE *)title, (UBYTE *)~0);
}

static void UI_UpdateContent(struct App *app)
{
    SetAttrs(app->content_obj,
        GA_Text, (ULONG)App_PageText(app->current_page),
        TAG_DONE);

    UI_UpdateTitle(app);
}

static int TabList_Create(struct App *app)
{
    struct Node *node;

    struct TagItem tab1_tags[] =
    {
        { TNA_Text,   (ULONG)"Page 1" },
        { TNA_Number, 0 },
        { TAG_DONE,   0 }
    };

    struct TagItem tab2_tags[] =
    {
        { TNA_Text,   (ULONG)"Page 2" },
        { TNA_Number, 1 },
        { TAG_DONE,   0 }
    };

    struct TagItem tab3_tags[] =
    {
        { TNA_Text,   (ULONG)"Page 3" },
        { TNA_Number, 2 },
        { TAG_DONE,   0 }
    };

    node = AllocClickTabNodeA(tab1_tags);
    if (node == NULL)
        return 0;
    AddTail(&app->tab_labels, node);

    node = AllocClickTabNodeA(tab2_tags);
    if (node == NULL)
    {
        TabList_Destroy(app);
        return 0;
    }
    AddTail(&app->tab_labels, node);

    node = AllocClickTabNodeA(tab3_tags);
    if (node == NULL)
    {
        TabList_Destroy(app);
        return 0;
    }
    AddTail(&app->tab_labels, node);

    return 1;
}

static void TabList_Destroy(struct App *app)
{
    struct Node *node;
    struct Node *next;

    node = app->tab_labels.lh_Head;

    while (node->ln_Succ != NULL)
    {
        next = node->ln_Succ;
        FreeClickTabNode(node);
        node = next;
    }

    TabList_Init(&app->tab_labels);
}

static int UI_Create(struct App *app)
{
    int ok;

    struct TagItem tabs_tags[] =
    {
        { GA_ID,            GID_TABS },
        { GA_RelVerify,     TRUE },
        { CLICKTAB_Labels,  (ULONG)&app->tab_labels },
        { CLICKTAB_Current, 0 },
        { TAG_DONE,         0 }
    };

    struct TagItem content_tags[] =
    {
        { GA_ReadOnly, TRUE },
        { GA_Text,     (ULONG)"This is page 1." },
        { TAG_DONE,    0 }
    };

    struct TagItem root_layout_tags[] =
    {
        { LAYOUT_Orientation, LAYOUT_ORIENT_VERT },
        { LAYOUT_SpaceOuter,  TRUE },
        { LAYOUT_AddChild,    0 },   /* tabs */
        { LAYOUT_AddChild,    0 },   /* content */
        { TAG_DONE,           0 }
    };

    struct TagItem window_tags[] =
    {
        { WA_Title,        (ULONG)"ReXamples - 19_RegisterPages [Page 1]" },
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

    app->ClickTabLib = OpenLibrary("clicktab.gadget", 47);
    if (app->ClickTabLib == NULL)
        goto out;
    ClickTabBase = app->ClickTabLib;

    if (!TabList_Create(app))
        goto out;

    app->tabs_obj = NewObjectA(CLICKTAB_GetClass(), NULL, tabs_tags);
    if (app->tabs_obj == NULL)
        goto out;

    app->content_obj = NewObjectA(BUTTON_GetClass(), NULL, content_tags);
    if (app->content_obj == NULL)
        goto out;

    root_layout_tags[2].ti_Data = (ULONG)app->tabs_obj;
    root_layout_tags[3].ti_Data = (ULONG)app->content_obj;

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

    UI_UpdateContent(app);

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
        app->tabs_obj = NULL;
        app->content_obj = NULL;
    }
    else
    {
        if (app->root_layout != NULL)
        {
            DisposeObject(app->root_layout);
            app->root_layout = NULL;
            app->tabs_obj = NULL;
            app->content_obj = NULL;
        }
        else
        {
            if (app->content_obj != NULL)
            {
                DisposeObject(app->content_obj);
                app->content_obj = NULL;
            }

            if (app->tabs_obj != NULL)
            {
                DisposeObject(app->tabs_obj);
                app->tabs_obj = NULL;
            }
        }
    }

    TabList_Destroy(app);

    if (app->ClickTabLib != NULL)
    {
        CloseLibrary(app->ClickTabLib);
        app->ClickTabLib = NULL;
        ClickTabBase = NULL;
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

static void App_DispatchTabs(struct App *app)
{
    ULONG current;

    GetAttr(CLICKTAB_Current, app->tabs_obj, &current);
    app->current_page = current;

    UI_UpdateContent(app);
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
                            case GID_TABS:
                                App_DispatchTabs(app);
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
