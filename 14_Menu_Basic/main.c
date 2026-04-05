#include <string.h>

#include <exec/types.h>
#include <exec/libraries.h>
#include <utility/tagitem.h>
#include <dos/dosextens.h>
#include <libraries/gadtools.h>
#include <proto/gadtools.h>

#include <intuition/intuition.h>
#include <intuition/classusr.h>
#include <intuition/intuitionbase.h>

#include <classes/window.h>
#include <gadgets/layout.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>

/* vbcc proto headers used here expect global library bases. */
struct IntuitionBase *IntuitionBase = NULL;
struct Library *WindowBase = NULL;
struct Library *LayoutBase = NULL;
struct Library *GadToolsBase = NULL;

/* Local prototype needed by this build setup. */
ULONG DoMethodA(Object *obj, Msg msg);

#define MID_ABOUT  1
#define MID_QUIT   2

static struct NewMenu nm_basic[] =
{
    { NM_TITLE, (STRPTR)"Project",  NULL,         0, 0, NULL },
    { NM_ITEM,  (STRPTR)"About...", (STRPTR)"A",  0, 0, (APTR)MID_ABOUT },
    { NM_ITEM,  (STRPTR)"Quit",     (STRPTR)"Q",  0, 0, (APTR)MID_QUIT  },
    { NM_END,   NULL,               NULL,         0, 0, NULL }
};

struct App
{
    struct Library *IntuitionLib;
    struct Library *WindowLib;
    struct Library *LayoutLib;
    struct Library *GadToolsLib;

    Object *root_layout;
    Object *win_obj;

    struct Window *win;
    struct Menu *menu;

    ULONG win_sigmask;
    int running;
};

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
}

static void UI_UpdateTitle(struct App *app, STRPTR title)
{
    if (app->win != NULL)
        SetWindowTitles(app->win, (UBYTE *)title, (UBYTE *)~0);
}

static int Menu_Create(struct App *app)
{
    struct TagItem create_tags[] =
    {
        { TAG_DONE, 0 }
    };

    struct TagItem layout_tags[] =
    {
        { TAG_DONE, 0 }
    };

    app->menu = CreateMenusA(nm_basic, create_tags);
    if (app->menu == NULL)
        return 0;

    if (!LayoutMenusA(app->menu, NULL, layout_tags))
    {
        FreeMenus(app->menu);
        app->menu = NULL;
        return 0;
    }

    return 1;
}

static void Menu_Destroy(struct App *app)
{
    if (app->menu != NULL)
    {
        FreeMenus(app->menu);
        app->menu = NULL;
    }
}

static int UI_Create(struct App *app)
{
    int ok;

    struct TagItem root_layout_tags[] =
    {
        { LAYOUT_Orientation, LAYOUT_ORIENT_VERT },
        { LAYOUT_SpaceOuter,  TRUE },
        { TAG_DONE,           0 }
    };

    struct TagItem window_tags[] =
    {
        { WA_Title,        (ULONG)"ReXamples - 14_Menu_Basic" },
        { WA_Activate,     TRUE },
        { WA_DepthGadget,  TRUE },
        { WA_DragBar,      TRUE },
        { WA_CloseGadget,  TRUE },
        { WA_SizeGadget,   TRUE },
        { WA_IDCMP,        IDCMP_CLOSEWINDOW | IDCMP_MENUPICK },
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

    app->GadToolsLib = OpenLibrary("gadtools.library", 39);
    if (app->GadToolsLib == NULL)
        goto out;
    GadToolsBase = app->GadToolsLib;

    if (!Menu_Create(app))
        goto out;

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

    if (app->menu != NULL)
        SetMenuStrip(app->win, app->menu);

    app->win_sigmask = 1UL << app->win->UserPort->mp_SigBit;
    app->running = 1;

    return 1;
}

static void UI_Close(struct App *app)
{
    ULONG close_msg[2];

    if (app->win != NULL)
    {
        ClearMenuStrip(app->win);

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
    }
    else if (app->root_layout != NULL)
    {
        DisposeObject(app->root_layout);
        app->root_layout = NULL;
    }

    Menu_Destroy(app);

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

    if (app->GadToolsLib != NULL)
    {
        CloseLibrary(app->GadToolsLib);
        app->GadToolsLib = NULL;
        GadToolsBase = NULL;
    }
}

static void App_HandleMenuPick(struct App *app, UWORD code)
{
    struct MenuItem *item;
    ULONG userdata;

    while (code != MENUNULL)
    {
        item = ItemAddress(app->menu, code);
        if (item != NULL)
        {
            userdata = (ULONG)GTMENUITEM_USERDATA(item);

            switch (userdata)
            {
                case MID_ABOUT:
                    UI_UpdateTitle(app, (STRPTR)"ReXamples - 14_Menu_Basic [About]");
                    break;

                case MID_QUIT:
                    app->running = 0;
                    break;

                default:
                    break;
            }
        }

        code = item->NextSelect;
    }
}

static void App_Run(struct App *app)
{
    ULONG signals;
    struct IntuiMessage *msg;
    ULONG class_id;
    UWORD code;
    ULONG waitmask;

    waitmask = app->win_sigmask | SIGBREAKF_CTRL_C;

    while (app->running)
    {
        signals = Wait(waitmask);

        if (signals & SIGBREAKF_CTRL_C)
            app->running = 0;

        if (signals & app->win_sigmask)
        {
            while ((msg = (struct IntuiMessage *)GetMsg(app->win->UserPort)) != NULL)
            {
                class_id = msg->Class;
                code = msg->Code;

                ReplyMsg((struct Message *)msg);

                switch (class_id)
                {
                    case IDCMP_CLOSEWINDOW:
                        app->running = 0;
                        break;

                    case IDCMP_MENUPICK:
                        App_HandleMenuPick(app, code);
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
