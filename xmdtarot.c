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
#include <time.h>
#include <math.h>

#include "xmdtarot.h"

#if DECKV == 1
#   include "deck_v1.h"
#elif DECKV == 2
#   include "deck_v2.h"
#else /* DECKV != 1 && DECKV != 2 */
#   error "No valid deck selected, please define DECKV as deck_v1 or deck"
#endif /* DECKV */

Widget toplevel, drawing_area;
GC     gc;                    /* graphics context */
int    *draw;                 /* current draw */
int    show_meanings = FALSE; /* show meanings flag */
int    initialized   = FALSE; /* has the graphics context been initialized*/

const char SPREAD_LABELS[DRAW_SIZE][11] = {
    "True", "Seek", "False", "Avoid", "Meaningless"
};

/*
 * Discordian Tarot program
 * Prints out a reading from the Discordian tarot deck as created by
 * Max Flax Beeblewax and (boing!) Cnoocy Mosque O'Witz (marc@suberic.net).
 * 
 * Usage:
 * xmdtarot
 * Prints your (or someone else's) super-helpful Discordian Tarot reading.
 * 
 * For further enlightenment, consult your pineal gland.
 */
int
main(int argc, char *argv[])
{
    XtAppContext app_context;
    XmString     title_string;
    Widget       main_window, menubar;
    Dimension    width, height;
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
    main_window = XmCreateMainWindow(toplevel, "main window", args, n);

    XmStringFree(title_string);

    n = 0;
    XtSetArg(args[n], XmNwidth,  INITIAL_WIDTH);  n++;
    XtSetArg(args[n], XmNheight, INITIAL_HEIGHT); n++;
    drawing_area = XmDtCreateDrawingArea(main_window, args, n);

    n       = 0;
    menubar = XmDtCreateMenubar(main_window, args, n);

    XtManageChild(main_window);
    XtRealizeWidget(toplevel);

    display = XtDisplay(drawing_area);
    gc      = XCreateGC(display, XtWindow(drawing_area), 0, (XGCValues *) NULL);

    XtVaGetValues(drawing_area, XmNwidth, &width, XmNheight, &height, NULL);
    NewSpread(width, height);
    initialized = TRUE;

    XmMainWindowSetAreas(main_window, menubar, (Widget) NULL, (Widget) NULL,
                        (Widget) NULL, drawing_area);
    XtVaSetValues(main_window, XmNmessageWindow, drawing_area, NULL);
    XtAppMainLoop(app_context);

    return 0;
}

/*
 * Creates a DrawingArea and wires up two callbacks:
 * XmNexpose and XmNresize.
 * Also calls XtManageChild.
 */
Widget
XmDtCreateDrawingArea(Widget main_window, ArgList args, int n)
{
    Widget drawing_area;

    drawing_area = XmCreateDrawingArea(main_window, NULL, args, n);

    XmDtAddDrawCallback(drawing_area, XmNexposeCallback);
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
             exit_acc_string, show_meanings_string, show_meanings_acc_string,
             new_spread_acc_string;
    Widget   menubar, file_menu;

    file_string = XmStringCreateLocalized("File");
    menubar     = XmVaCreateSimpleMenuBar(main_window, "menubar",
                                          XmVaCASCADEBUTTON, file_string, 'F',
                                          NULL);

    XmStringFree(file_string);

    new_spread_string        = XmStringCreateLocalized("New Spread");
    new_spread_acc_string    = XmStringCreateLocalized("Ctrl-N");
    show_meanings_string     = XmStringCreateLocalized("Show Meanings");
    show_meanings_acc_string = XmStringCreateLocalized("Ctrl-M");
    about_string             = XmStringCreateLocalized("About...");
    exit_string              = XmStringCreateLocalized("Quit");
    exit_acc_string          = XmStringCreateLocalized("Ctrl-Q");
    file_menu                = XmVaCreateSimplePulldownMenu(menubar,
        "file_menu", 0, file_callback,
        XmVaPUSHBUTTON,  new_spread_string,    'N', "Ctrl<Key>n",  new_spread_acc_string,
        XmVaCHECKBUTTON, show_meanings_string, 'M', "Ctrl<Key>m",  show_meanings_acc_string,
        XmVaPUSHBUTTON,  about_string,         'A', NULL,          NULL,
        XmVaSEPARATOR,
        XmVaPUSHBUTTON,  exit_string,          'Q', "Ctrl<Key>q",  exit_acc_string,
        NULL);

    n = 0;
    XtSetArg(args[n], XmNtearOffModel, XmTEAR_OFF_ENABLED); n++;
    XtSetValues(file_menu, args, n);

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
NewSpread(int width, int height)
{
    XClearWindow(XtDisplay(drawing_area), XtWindow(drawing_area));
    draw = DrawCards(DRAW_SIZE, DECK_SIZE);
    RenderDraw(draw, DRAW_SIZE, width, height);
}

/*
 * Draws cards
 */
int*
DrawCards(int draw_size, int deck_size)
{
    static int draw[DRAW_SIZE];
    int        idx, i, j;

    srandom(time(NULL));

    i = 0;
    while (i < draw_size)
    {
redraw:
        idx = random() % deck_size;

        /* redraw duplicates */
        for (j = 0; j < i; j++)
            if (draw[j] == idx) goto redraw;

        draw[i++] = idx;
    }

    return draw;
}

/*
 * Determine minimum spacing between cards based on window width
 */
int
MinCardSpacing(int width, int height)
{
    int min_dim = width < height ? width : height;

    /* Leave space for card width so cards don't go off the edge */
    int max_radius = min_dim / 2 - CARD_WIDTH / 2;

    /* Use a fraction of the available space for the radius */
    int spacing = max_radius * 0.8; /* 80% of max possible radius */

    if (spacing < 0) spacing = 0;

    return spacing;
}

/*
 * Returns the center of the next drawing point for the spread.
 */
struct OrderedPair
GetNextDrawPoint(int spread_index, int width, int height)
{
    const double NUM_SIDES    = DRAW_SIZE;
    const double ANGLE_STEP   = 2 * M_PI / NUM_SIDES;
    const double RADIUS       = MinCardSpacing(width, height);

    struct OrderedPair next_draw_point;
    double             angle;

    angle             = (double) spread_index * ANGLE_STEP - M_PI / 2;
    next_draw_point.x =               width  / 2 + (int) (RADIUS * cos(angle)) - CARD_WIDTH  / 2;
    next_draw_point.y = TOP_PADDING + height / 2 + (int) (RADIUS * sin(angle)) - CARD_HEIGHT / 2;

    return next_draw_point;
}

Pixmap
GetFacePixmap(Display *display, Window window, const unsigned char *bits,
              int width, int height)
{
    static Pixmap face_pixmap;
    Pixmap        buffer;
    unsigned int  depth;

    XtVaGetValues(drawing_area, XmNdepth, &depth, NULL);

    face_pixmap = XCreatePixmap(display, (Drawable) window, width, height, depth);
    buffer      = XCreateBitmapFromData(display, (Drawable) window, bits, width, height);

    XSetForeground(display, gc, BlackPixel(display, DefaultScreen(display)));
    XSetBackground(display, gc, WhitePixel(display, DefaultScreen(display)));
    
    XCopyPlane(display, buffer, face_pixmap, gc,
               0, 0, width, height, 0, 0, 0x01);

    return face_pixmap;
}

/*
 * Renders the drawn cards to the screen
 *
 * The spread is in the form of a pentagon, the positions are True (7:30),
 * False (4:30), Meaningless (12:00), Seek (10:00), Avoid (2:00).
 */
void
RenderDraw(int *draw, int draw_size, int width, int height)
{
    int                i, spread_pos_name_len, card_meaning_len;
    struct OrderedPair next_draw_point;
    unsigned char      *card_face;
    char               *spread_pos_name, *card_meaning;
    Pixmap             face_pixmap;
    Display            *display = XtDisplay(drawing_area);
    Window             window   = XtWindow(drawing_area);
    Font               bold_font, normal_font;

    for (i = 0; i < draw_size; i++)
    {
        next_draw_point = GetNextDrawPoint(i, width, height);
        card_face       = FACES[draw[i]];
        face_pixmap     = GetFacePixmap(display, window, card_face,
                                        CARD_WIDTH, CARD_HEIGHT);
        XDrawRectangle(display, (Drawable) window, gc,
                       next_draw_point.x, next_draw_point.y,
                       CARD_WIDTH, CARD_HEIGHT);
        XCopyArea(display, face_pixmap, window, gc, 0, 0,
                  CARD_WIDTH, CARD_HEIGHT,
                  next_draw_point.x, next_draw_point.y);

        /* Draw spread position named above card */
        spread_pos_name = SPREAD_LABELS[i];
        spread_pos_name_len = strlen(spread_pos_name);

        normal_font = XLoadFont(display, "-*-*-*-*-*-*-*-*-*-*-*-*-iso8859-1"); 
        bold_font   = XLoadFont(display, "-*-*-bold-*-*-*-*-140-*-*-*-*-iso8859-1");
        XSetFont(display, gc, bold_font);
        XDrawString(display, window, gc,
                       next_draw_point.x + CARD_WIDTH / 2 - spread_pos_name_len * 4,
                       next_draw_point.y - 5,
                       spread_pos_name, spread_pos_name_len);
        XUnloadFont(display, bold_font);
        XSetFont(display, gc, normal_font);

        if (show_meanings)
        {
            /* Draw card meaning below card name */
            card_meaning     = DECK[draw[i]].meaning;
            card_meaning_len = strlen(card_meaning);
            XDrawString(display, window, gc,
                        next_draw_point.x + CARD_WIDTH / 2 - card_meaning_len * 3.5,
                        next_draw_point.y + CARD_HEIGHT + 15,
                        DECK[draw[i]].meaning, card_meaning_len);
        }

        XUnloadFont(display, normal_font);
    }

    XFlush(display);
}

/*
 * Renders an about box.
 */
void
About()
{
    const char ABOUT_TEXT[] = "Code copyright (C) 2025 by Lord Imbrius the Despondent\n"
                        "(darthferrett@gmail.com)\n"
                        "under Creative Commons 0 license\n"
                        "NO rights reserved, but ALL rites reversed.\n"
                        "\n";

    XmString about_string, label_string, ok_string;
    Widget   about_dialog, about_label;
    Arg      args[X11_ARGS_MAX];
    int      n, about_text_len, deck_about_text_len;
    char     *about_text_buffer;

    n            = 0;
    about_string = XmStringCreateLocalized("About");

    XtSetArg(args[n], XmNtitle, about_string);                             n++;
    XtSetArg(args[n], XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL); n++;

    about_text_len      = strlen(ABOUT_TEXT);
    deck_about_text_len = strlen(DECK_ABOUT_STRING);
    about_text_buffer   = calloc(about_text_len + deck_about_text_len + 1, sizeof(char));

    strncpy(about_text_buffer, ABOUT_TEXT, about_text_len);
    strncat(about_text_buffer, DECK_ABOUT_STRING, deck_about_text_len);

    about_dialog = XmCreateFormDialog(toplevel, "about_dialog", args, n);
    label_string = XmStringCreateLocalized(about_text_buffer);
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