/*-
 * SPDX-License-Identifier: CC0-1.0
 */

#include <Xm/Xm.h>
#include <stdlib.h>

#include "xmdtarot.h"

extern GC     gc;
extern Widget drawing_area;
extern int    initialized, *draw, show_meanings;

void
file_callback(Widget widget, XtPointer client_data, XtPointer call_data)
{
    extern void                  NewSpread(int _width, int _height), About(),
                                 RenderDraw(int *_draw, int _draw_size,
                                           int _width, int _height);
    int                          item_no = (int) client_data;
    Dimension                    width, height;
    XmToggleButtonCallbackStruct *cbs;

    switch (item_no)
    {
        case NEW_SPREAD_OPTION:
            XtVaGetValues(drawing_area, XmNwidth, &width, XmNheight, &height, NULL);
            NewSpread(width, height);
            break;
        case MEANINGS_OPTION:
            cbs           = (XmToggleButtonCallbackStruct*) call_data;
            show_meanings = cbs->set;
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

void
draw_callback(Widget widget, XtPointer client_data, XtPointer call_data)
{
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