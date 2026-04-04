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

#define MID_SHOWGRID   10
#define MID_SHOWINFO   11
#define MID_QUIT       12

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;

    Object *win_obj;
    struct Window *win;

    struct Menu *menu;

    ULONG win_sigmask;
    int running;
    int show_grid;
    int show_info;
};

static struct NewMenu nm_advanced[] =
{
    { NM_TITLE, (STRPTR)"Project",  NULL, 0, 0, NULL },
    { NM_ITEM,  (STRPTR)"Quit",     (STRPTR)"Q", 0, 0, (APTR)MID_QUIT },

    { NM_TITLE, (STRPTR)"View",     NULL, 0, 0, NULL },
    { NM_ITEM,  (STRPTR)"Show Grid", NULL, CHECKIT | MENUTOGGLE, 1, (APTR)MID_SHOWGRID },
    { NM_ITEM,  (STRPTR)"Show Info", NULL, CHECKIT | MENUTOGGLE, 1, (APTR)MID_SHOWINFO },

    { NM_END, NULL, NULL, 0, 0, NULL }
};

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
    app->show_grid = 0;
    app->show_info = 0;
}

static void UI_UpdateTitle(struct App *app)
{
    char title[128];

    strcpy(title, "ReXamples - 15_Menu_Advanced [");
    strcat(title, app->show_grid ? "Grid " : "");
    strcat(title, app->show_info ? "Info" : "");
    if (!app->show_grid && !app->show_info)
        strcat(title, "None");
    strcat(title, "]");

    if (app->win != NULL)
        SetWindowTitles(app->win, (UBYTE *)title, (UBYTE *)~0);
}

static int Menu_Create(struct App *app)
{
    app->menu = CreateMenus(nm_advanced, TAG_DONE);
    if (app->menu == NULL)
        return 0;

    if (!LayoutMenus(app->menu, NULL, TAG_DONE))
        return 0;

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

    ok = 0;

    app->WindowBase = OpenLibrary("window.class", 47);
    if (app->WindowBase == NULL)
        goto out;

    app->LayoutBase = OpenLibrary("layout.gadget", 47);
    if (app->LayoutBase == NULL)
        goto out;

    if (!Menu_Create(app))
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 15_Menu_Advanced [None]",
            WA_Activate,    TRUE,
            WA_DepthGadget, TRUE,
            WA_DragBar,     TRUE,
            WA_CloseGadget, TRUE,
            WA_SizeGadget,  TRUE,
            WA_IDCMP,       IDCMP_CLOSEWINDOW | IDCMP_MENUPICK,
            WINDOW_Position, WPOS_CENTERSCREEN,
            WINDOW_Layout,
                VLayoutObject,
                    LAYOUT_SpaceOuter, TRUE,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    return ok;
}

static int UI_Open(struct App *app)
{
    app->win = (struct Window *)RA_OpenWindow(app->win_obj);
    if (app->win == NULL)
        return 0;

    if (app->menu != NULL)
        SetMenuStrip(app->win, app->menu);

    app->win_sigmask = RA_HandleInput(app->win_obj, NULL);
    app->running = 1;

    UI_UpdateTitle(app);

    return 1;
}

static void UI_Close(struct App *app)
{
    if (app->win != NULL)
    {
        ClearMenuStrip(app->win);
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
    }

    Menu_Destroy(app);

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

static void App_HandleMenuPick(struct App *app, ULONG menucode)
{
    ULONG userdata;
    struct MenuItem *mi;

    while (menucode != MENUNULL)
    {
        mi = ItemAddress(app->menu, menucode);
        userdata = (ULONG)GTMENUITEM_USERDATA(mi);

        switch (userdata)
        {
            case MID_SHOWGRID:
                app->show_grid = ((mi->Flags & CHECKED) != 0);
                UI_UpdateTitle(app);
                break;

            case MID_SHOWINFO:
                app->show_info = ((mi->Flags & CHECKED) != 0);
                UI_UpdateTitle(app);
                break;

            case MID_QUIT:
                app->running = 0;
                break;

            default:
                break;
        }

        menucode = MENUNEXT(menucode);
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

            case WMHI_MENUPICK:
                App_HandleMenuPick(app, result & WMHI_MENUMASK);
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

