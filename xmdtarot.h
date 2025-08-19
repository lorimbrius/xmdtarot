/*-
 * SPDX-License-Identifier: CC0-1.0
 */

#ifndef _XMDTAROT_H
#define _XMDTAROT_H

#include <Xm/Xm.h>

/*
 * A card from the Discordian Tarot Deck consists of the name and the meaning.
 */
struct Card
{
    char name[31];    /* card name    */
    char meaning[95]; /* card meaning */
};

/* callbacks */
void draw_callback(Widget _widget, XtPointer _client_data, XtPointer _call_data);
void file_callback(Widget _widtet, XtPointer _client_data, XtPointer _call_data);

static void DrawCards(int *_draw[], int _draw_size, int _deck_size);
void About();
void RenderDraw(int *_draw[], int _draw_size);
void NewSpread(void);

/* macros */
#define DRAW_SIZE            5 /* number of cards to draw at a time */
#define X11_ARGS_MAX        20 /* maximum number of arguments to pass to XtAppInitialize */
#define INITIAL_WIDTH      640 /* initial width of the window in pixels */
#define INITIAL_HEIGHT     480 /* initial height of the window in pixels */
#define ABOUT_TOP_OFFSET    10 /* about dialog top offset in pixels */
#define ABOUT_LEFT_OFFSET   10 /* about dialog left offset in pixels */
#define ABOUT_RIGHT_OFFSET  10 /* about dialog right offset in pixels */
#define ABOUT_BOTTOM_OFFSET 10 /* about dialog bottom offset in pixels */
#define ABOUT_BUTTON_LEFT   45 /* left position of the OK button on about box in pixels */
#define ABOUT_BUTTON_RIGHT  80 /* right position of the OK button on about box in pixels */
#define NEW_SPREAD_OPTION    0
#define ABOUT_OPTION         1
#define EXIT_OPTION          2

#define XmDtAddDrawCallback(drawing_area, callback_event) \
        XtAddCallback(drawing_area, callback_event, (XtCallbackProc) draw_callback, (XtPointer) NULL); 

Widget XmDtCreateDrawingArea(Widget _main_window, ArgList _args, int _n);
Widget XmDtCreateMenubar(Widget _main_window, ArgList _args, int _n);

#endif /* !_XMDTAROT_H */