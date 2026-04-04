#include <string.h>

#include <exec/types.h>
#include <exec/libraries.h>
#include <intuition/intuition.h>
#include <intuition/icclass.h>

#include <gadgets/listbrowser.h>
#include <intuition/classes.h>
#include <libraries/gadtools.h>

#include <reaction/reaction.h>
#include <reaction/reaction_macros.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/listbrowser.h>
#include <proto/utility.h>

#define GID_LIST   1

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;
    struct Library *ListBrowserBase;

    Object *win_obj;
    Object *gad_list;

    struct Window *win;

    struct List labels;

    ULONG win_sigmask;
    int running;
};

/*****************************************************************************
 *
 *  App_Clear
 *
 *****************************************************************************/

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
    NewList((struct List *)&app->labels);
}

/*****************************************************************************
 *
 *  List_CreateEntries
 *
 *  Purpose:
 *      Create a very small owned list model for the lesson.
 *
 *  Notes:
 *      This lesson keeps the data model intentionally tiny. The goal is to
 *      make ownership and selection visible, not to teach list frameworks.
 *
 *****************************************************************************/

static int List_CreateEntries(struct App *app)
{
    struct Node *node;

    node = AllocListBrowserNode(1,
        LBNA_Column, 0,
        LBNCA_CopyText, TRUE,
        LBNCA_Text, "One",
        TAG_DONE);
    if (node == NULL)
        return 0;
    AddTail((struct List *)&app->labels, node);

    node = AllocListBrowserNode(1,
        LBNA_Column, 0,
        LBNCA_CopyText, TRUE,
        LBNCA_Text, "Two",
        TAG_DONE);
    if (node == NULL)
        return 0;
    AddTail((struct List *)&app->labels, node);

    node = AllocListBrowserNode(1,
        LBNA_Column, 0,
        LBNCA_CopyText, TRUE,
        LBNCA_Text, "Three",
        TAG_DONE);
    if (node == NULL)
        return 0;
    AddTail((struct List *)&app->labels, node);

    return 1;
}

/*****************************************************************************
 *
 *  List_DestroyEntries
 *
 *  Purpose:
 *      Release all list browser nodes owned by the lesson.
 *
 *****************************************************************************/

static void List_DestroyEntries(struct App *app)
{
    struct Node *node;
    struct Node *next;

    node = app->labels.lh_Head;
    while (node->ln_Succ != NULL)
    {
        next = node->ln_Succ;
        FreeListBrowserNode(node);
        node = next;
    }

    NewList((struct List *)&app->labels);
}

static void UI_UpdateTitle(struct App *app, ULONG selected)
{
    STRPTR title;

    title = (STRPTR)"ReXamples - 08_ListBrowser";

    switch (selected)
    {
        case 0: title = (STRPTR)"ReXamples - 08_ListBrowser [One]"; break;
        case 1: title = (STRPTR)"ReXamples - 08_ListBrowser [Two]"; break;
        case 2: title = (STRPTR)"ReXamples - 08_ListBrowser [Three]"; break;
        default: break;
    }

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

    app->ListBrowserBase = OpenLibrary("listbrowser.gadget", 47);
    if (app->ListBrowserBase == NULL)
        goto out;

    if (!List_CreateEntries(app))
        goto out;

    app->gad_list =
        ListBrowserObject,
            GA_ID, GID_LIST,
            GA_RelVerify, TRUE,
            LISTBROWSER_Labels, (ULONG)&app->labels,
        End;

    if (app->gad_list == NULL)
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 08_ListBrowser",
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
                    StartMember, app->gad_list, EndMember,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    if (!ok && app->gad_list != NULL)
    {
        DisposeObject(app->gad_list);
        app->gad_list = NULL;
    }

    if (!ok)
        List_DestroyEntries(app);

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
        app->gad_list = NULL;
    }
    else if (app->gad_list != NULL)
    {
        DisposeObject(app->gad_list);
        app->gad_list = NULL;
    }

    List_DestroyEntries(app);

    if (app->ListBrowserBase != NULL)
    {
        CloseLibrary(app->ListBrowserBase);
        app->ListBrowserBase = NULL;
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
    ULONG selected;

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
                    case GID_LIST:
                        GetAttr(LISTBROWSER_Selected, app->gad_list, &selected);
                        UI_UpdateTitle(app, selected);
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

