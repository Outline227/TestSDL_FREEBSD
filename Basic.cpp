#include <X11/Xlib.h>
#include <unistd.h>

int main() {
    Display *d;
    Window w;
    XEvent e;
    int s;

    d = XOpenDisplay(NULL);
    if (d == NULL) return 1;

    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 400, 300, 1,
                            BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);

    while (1) {
        XNextEvent(d, &e);
        if (e.type == Expose) {
            XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 100, 50);
        }
        if (e.type == KeyPress) break;
    }

    XCloseDisplay(d);
    return 0;
}
