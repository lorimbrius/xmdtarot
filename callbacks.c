/*-
 * SPDX-License-Identifier: CC0-1.0
 */

#include <Xm/Xm.h>
#include <stdlib.h>

#include "xmdtarot.h"

void
file_callback(Widget widget, XtPointer client_data, XtPointer call_data)
{
    extern void NewSpread(void), About();
    int         item_no = (int) client_data;

    switch (item_no)
    {
        case NEW_SPREAD_OPTION:
            NewSpread();
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
    ; //TODO
}