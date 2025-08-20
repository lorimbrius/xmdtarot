/*-
 * SPDX-License-Identifier: CC0-1.0
 */

#include <Xm/Xm.h>
#include <stdlib.h>

#include "xmdtarot.h"

extern GC     gc;
extern Widget drawing_area;

void
file_callback(Widget widget, XtPointer client_data, XtPointer call_data)
{
    extern void NewSpread(int _width, int _height), About();
    Dimension   width, height;
    int         item_no = (int) client_data;

    switch (item_no)
    {
        case NEW_SPREAD_OPTION:
            XtVaGetValues(drawing_area, XmNwidth, &width, XmNheight, &height, NULL);
            NewSpread(width, height);
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

    XtVaGetValues(widget, XmNwidth, &width, XmNheight, &height, NULL);

    /*
     * If we resize the window, keep the current draw but rerender.
     * If this is the first time we're drawing the window, we need to draw the
     * spread.
     */

    if (ptr->reason == XmCR_RESIZE)
    {
        if (gc != NULL)
            XClearWindow(XtDisplay(widget), XtWindow(widget));

        RenderDraw(NULL, DRAW_SIZE, width, height); /* should be cached */
    }
    else if (ptr->reason == XmCR_EXPOSE && ptr->event->xexpose.count == 0)
        NewSpread(width, height);
}