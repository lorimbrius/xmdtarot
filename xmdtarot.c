/*-
 * SPDX-License-Identifier: CC0-1.0
 */

#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/DrawingA.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <stdlib.h>

#include "xmdtarot.h"
#include "deck_v1.h"

Widget toplevel;
GC     gc; /* graphics context */

int
main(int argc, char *argv[])
{
    XtAppContext app_context;
    XmString     title_string;
    Widget       main_window, menubar, drawing_area;
    Display      *display;
    Arg          args[X11_ARGS_MAX];
    int          n;

    XtSetLanguageProc(NULL, NULL, NULL);
    toplevel     = XtVaOpenApplication(&app_context, "Games", NULL, 0, &argc, argv,
                                       NULL, sessionShellWidgetClass, NULL);
    title_string = XmStringCreateLocalized("X Discordian Tarot");
    n            = 0;

    XtSetArg(args[n], XmNtitle,  title_string);   n++;
    XtSetArg(args[n], XmNwidth,  INITIAL_WIDTH);  n++;
    XtSetArg(args[n], XmNheight, INITIAL_HEIGHT); n++;

    n           = 0;
    main_window = XmCreateMainWindow(toplevel, "main window", args, n);

    XmStringFree(title_string);

    n            = 0;
    drawing_area = XmDtCreateDrawingArea(main_window, args, n);

    n       = 0;
    menubar = XmDtCreateMenubar(main_window, args, n);

    XtManageChild(main_window);
    XtRealizeWidget(toplevel);

    display = XtDisplay(drawing_area);
    gc      = XCreateGC(display, XtWindow(drawing_area), 0, (XGCValues *) NULL);

    NewSpread();

    XmMainWindowSetAreas(main_window, menubar, (Widget) NULL, (Widget) NULL,
                         (Widget) NULL, drawing_area);
    XtVaSetValues(main_window, XmNmessageWindow, drawing_area, NULL);
    XtAppMainLoop(app_context);
    return 0;
}

/*
 * Creates a DrawingArea and wires up three callbacks:
 * XmNexpose, XmNinput, and XmNresize.
 * Also calls XtManageChild.
 */
Widget
XmDtCreateDrawingArea(Widget main_window, ArgList args, int n)
{
    Widget drawing_area;

    drawing_area = XmCreateDrawingArea(main_window, NULL, args, n);

    XmDtAddDrawCallback(drawing_area, XmNexposeCallback);
    XmDtAddDrawCallback(drawing_area, XmNinputCallback);
    XmDtAddDrawCallback(drawing_area, XmNresizeCallback);

    XtManageChild(drawing_area);

    return drawing_area;
}

/*
 * Creates and populates a menu bar with a File menu.
 */
Widget
XmDtCreateMenubar(Widget main_window, ArgList args, int n)
{
    XmString file_string, new_spread_string, about_string, exit_string,
             exit_acc_string;
    Widget   menubar, file_menu;

    file_string = XmStringCreateLocalized("File");
    menubar     = XmVaCreateSimpleMenuBar(main_window, "menubar",
                                          XmVaCASCADEBUTTON, file_string, 'F',
                                          NULL);

    XmStringFree(file_string);

    new_spread_string = XmStringCreateLocalized("New Spread");
    about_string       = XmStringCreateLocalized("About...");
    exit_string        = XmStringCreateLocalized("Quit");
    exit_acc_string    = XmStringCreateLocalized("Ctrl-Q");
    file_menu          = XmVaCreateSimplePulldownMenu(menubar, "file_menu", 0, file_callback,
                                                      XmVaPUSHBUTTON, new_spread_string,  'N', NULL,          NULL,
                                                      XmVaPUSHBUTTON, about_string,       'A', NULL,          NULL,
                                                      XmVaSEPARATOR,
                                                      XmVaPUSHBUTTON, exit_string,        'Q', "Ctrl<Key>q",  exit_acc_string,
                                                      NULL);

    XmStringFree(new_spread_string);
    XmStringFree(about_string);
    XmStringFree(exit_string);
    XmStringFree(exit_acc_string);

    XtManageChild(menubar);

    return menubar;
}

/*
 * Draws a new spread (resets the screen)
 */
void
NewSpread()
{
    //TODO
}

/*
 * Renders an about box.
 */
void
About()
{
    char about_text[] = "Code copyright (C) 2025 by Lord Imbrius the Despondent\n"
                        "(darthferrett@gmail.com)\n"
                        "under Creative Commons 0 license\n"
                        "NO rights reserved, but ALL rites reversed.\n"
                        "\n"
                        "Deck and art Kopyleft (K) 2002 Max Flax Beeblewax\n"
                        "and (Boing!) Cnoocy Mosque O'Wicz (marc@suberic.net)";
    
    XmString about_string, label_string, ok_string;
    Widget   about_dialog, about_label;
    Arg      args[X11_ARGS_MAX];
    int      n;

    n            = 0;
    about_string = XmStringCreateLocalized("About");

    XtSetArg(args[n], XmNtitle, about_string);                             n++;
    XtSetArg(args[n], XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL); n++;

    about_dialog = XmCreateFormDialog(toplevel, "about_dialog", args, n);
    label_string = XmStringCreateLocalized(about_text);
    about_label  = XtVaCreateManagedWidget("about_label", xmLabelWidgetClass, about_dialog,
                                           XmNtopAttachment,   XmATTACH_FORM,
                                           XmNtopOffset,       ABOUT_TOP_OFFSET,
                                           XmNleftAttachment,  XmATTACH_FORM,
                                           XmNleftOffset,      ABOUT_LEFT_OFFSET,
                                           XmNrightAttachment, XmATTACH_FORM,
                                           XmNrightOffset,     ABOUT_RIGHT_OFFSET,
                                           XmNlabelString,     label_string,
                                           NULL);

    XmStringFree(about_string);
    XmStringFree(label_string);
    
    ok_string = XmStringCreateLocalized("OK");

    XtVaCreateManagedWidget("ok_button", xmPushButtonWidgetClass, about_dialog,
                            XmNlabelString,      ok_string,
                            XmNtopAttachment,    XmATTACH_WIDGET,
                            XmNtopWidget,        about_label,
                            XmNtopOffset,        ABOUT_TOP_OFFSET,
                            XmNbottomAttachment, XmATTACH_FORM,
                            XmNbottomOffset,     ABOUT_BOTTOM_OFFSET,
                            XmNleftAttachment,   XmATTACH_POSITION,
                            XmNleftPosition,     ABOUT_BUTTON_LEFT,
                            XmNrightPosition,    ABOUT_BUTTON_RIGHT,
                            NULL);

    XmStringFree(ok_string);

    XtManageChild(about_dialog);
}