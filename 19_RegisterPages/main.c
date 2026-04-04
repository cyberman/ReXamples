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
#include <proto/clicktab.h>
#include <proto/page.h>
#include <proto/button.h>

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;
    struct Library *ClickTabBase;
    struct Library *PageBase;
    struct Library *ButtonBase;

    Object *win_obj;

    struct Window *win;

    ULONG win_sigmask;
    int running;
};

static CONST_STRPTR tab_labels[] =
{
    "Page 1",
    "Page 2",
    "Page 3",
    NULL
};

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
}

/*****************************************************************************
 *
 *  UI_Create
 *
 *  Purpose:
 *      Demonstrate a page-based layout with clearly separated sections.
 *
 *  Notes:
 *      The lesson is about page structure, not about building a full
 *      preferences application.
 *
 *****************************************************************************/

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

    app->ClickTabBase = OpenLibrary("clicktab.gadget", 47);
    if (app->ClickTabBase == NULL)
        goto out;

    app->PageBase = OpenLibrary("page.gadget", 47);
    if (app->PageBase == NULL)
        goto out;

    app->ButtonBase = OpenLibrary("button.gadget", 47);
    if (app->ButtonBase == NULL)
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 19_RegisterPages",
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

                    StartMember,
                        ClickTabObject,
                            CLICKTAB_Labels, (ULONG)tab_labels,
                        End,
                    EndMember,

                    StartMember,
                        PageObject,
                            PAGE_Add,
                                ButtonObject,
                                    GA_ReadOnly, TRUE,
                                    GA_Text, (ULONG)"This is page 1.",
                                End,
                            PAGE_Add,
                                ButtonObject,
                                    GA_ReadOnly, TRUE,
                                    GA_Text, (ULONG)"This is page 2.",
                                End,
                            PAGE_Add,
                                ButtonObject,
                                    GA_ReadOnly, TRUE,
                                    GA_Text, (ULONG)"This is page 3.",
                                End,
                        End,
                    EndMember,
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

    app->win_sigmask = RA_HandleInput(app->win_obj, NULL);
    app->running = 1;

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
    }

    if (app->ButtonBase != NULL)
    {
        CloseLibrary(app->ButtonBase);
        app->ButtonBase = NULL;
    }

    if (app->PageBase != NULL)
    {
        CloseLibrary(app->PageBase);
        app->PageBase = NULL;
    }

    if (app->ClickTabBase != NULL)
    {
        CloseLibrary(app->ClickTabBase);
        app->ClickTabBase = NULL;
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

