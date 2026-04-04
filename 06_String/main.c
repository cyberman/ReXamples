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
#include <proto/string.h>

#define GID_NAME  1
#define NAME_LEN  64

struct App
{
    struct Library *WindowBase;
    struct Library *LayoutBase;
    struct Library *StringBase;

    Object *win_obj;
    Object *gad_name;

    struct Window *win;

    ULONG win_sigmask;
    int running;

    char name_buffer[NAME_LEN];
};

static void App_Clear(struct App *app)
{
    memset(app, 0, sizeof(*app));
    strcpy(app->name_buffer, "Amiga");
}

static void UI_UpdateTitle(struct App *app)
{
    char title[96];

    strcpy(title, "ReXamples - 06_String [");
    strncat(title, app->name_buffer, NAME_LEN - 1);
    strncat(title, "]", 1);

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

    app->StringBase = OpenLibrary("string.gadget", 47);
    if (app->StringBase == NULL)
        goto out;

    app->gad_name =
        StringObject,
            GA_ID, GID_NAME,
            GA_RelVerify, TRUE,
            GA_TabCycle, TRUE,
            STRINGA_TextVal, (ULONG)app->name_buffer,
            STRINGA_MaxChars, NAME_LEN - 1,
        End;

    if (app->gad_name == NULL)
        goto out;

    app->win_obj =
        WindowObject,
            WA_Title,       (ULONG)"ReXamples - 06_String [Amiga]",
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
                    StartMember, app->gad_name, EndMember,
                End,
        End;

    if (app->win_obj == NULL)
        goto out;

    ok = 1;

out:
    if (!ok && app->gad_name != NULL)
    {
        DisposeObject(app->gad_name);
        app->gad_name = NULL;
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

    UI_UpdateTitle(app);

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
        app->gad_name = NULL;
    }
    else if (app->gad_name != NULL)
    {
        DisposeObject(app->gad_name);
        app->gad_name = NULL;
    }

    if (app->StringBase != NULL)
    {
        CloseLibrary(app->StringBase);
        app->StringBase = NULL;
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
    ULONG text_ptr;

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
                    case GID_NAME:
                        GetAttr(STRINGA_TextVal, app->gad_name, &text_ptr);
                        if (text_ptr != 0)
                        {
                            strncpy(app->name_buffer, (char *)text_ptr, NAME_LEN - 1);
                            app->name_buffer[NAME_LEN - 1] = '\0';
                            UI_UpdateTitle(app);
                        }
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

