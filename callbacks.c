/*-
 * SPDX-License-Identifier: CC0-1.0
 */

#include <Xm/Xm.h>
#include <stdlib.h>

#include "xmdtarot.h"

extern int *draw;

/*
 * An option on the File menu was selected.
 */
void
file_callback(Widget widget, XtPointer client_data, XtPointer call_data)
{
    extern void                  NewSpread(int _width, int _height), About(),
                                 RenderDraw(int *_draw, int _draw_size,
                                            int _width, int _height);
    extern Widget                drawing_area; 
    extern int                   show_meanings;
    int                          item_no = (int) client_data;
    Dimension                    width, height;
    XmToggleButtonCallbackStruct *ptr;

    switch (item_no)
    {
        case NEW_SPREAD_OPTION:
            XtVaGetValues(drawing_area, XmNwidth, &width, XmNheight, &height, NULL);
            NewSpread(width, height);
            break;
        case MEANINGS_OPTION:
            ptr           = (XmToggleButtonCallbackStruct*) call_data;
            show_meanings = ptr->set; /* assignment to a global */
            XtVaGetValues(drawing_area, XmNwidth, &width, XmNheight, &height, NULL);
            XClearWindow(XtDisplay(drawing_area), XtWindow(drawing_area));
            RenderDraw(draw, DRAW_SIZE, width, height);
            break;
        case ABOUT_OPTION:
            About();
            break;
        case EXIT_OPTION:
            exit(0);
        /* default is no-op */
    }
}

/*
 * A drawing area event occurred.
 */
void
draw_callback(Widget widget, XtPointer client_data, XtPointer call_data)
{
    extern int                  initialized;
    extern GC                   gc;
    XmDrawingAreaCallbackStruct *ptr = (XmDrawingAreaCallbackStruct*) call_data;
    Dimension                   width, height;
    int                         i;
    static int                  exposed = FALSE;

    if (!initialized) return;

    XtVaGetValues(widget, XmNwidth, &width, XmNheight, &height, NULL);

    if (ptr->reason == XmCR_RESIZE && gc != NULL)
    {
        XClearWindow(XtDisplay(widget), XtWindow(widget));
        RenderDraw(draw, DRAW_SIZE, width, height);
    }
    else if (ptr->reason == XmCR_EXPOSE && ptr->event->xexpose.count == 0 && !exposed)
    {
        NewSpread(width, height);
        exposed = TRUE;
    }
     
}