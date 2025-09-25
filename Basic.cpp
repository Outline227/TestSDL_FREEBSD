#include <X11/Xlib.h>
#include <Imlib2.h>
#include <unistd.h>
#include <cstdlib>
#include <string>

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) return 1;

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    int width = DisplayWidth(display, screen);
    int height = DisplayHeight(display, screen);

    // Initialize Imlib2
    imlib_context_set_display(display);
    imlib_context_set_visual(DefaultVisual(display, screen));
    imlib_context_set_colormap(DefaultColormap(display, screen));

    // Download the image from URL to temp file
    std::string url = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQoFSUMNxlQOimCDDV4r4nzttEJ3xyASnj4Xw&s";
    std::string temp_file = "/tmp/background.jpg";
    std::string cmd = "curl -L -o " + temp_file + " " + url;
    system(cmd.c_str());

    // Load the image
    Imlib_Image image = imlib_load_image(temp_file.c_str());
    if (!image) return 1;
    imlib_context_set_image(image);

    // Create pixmap and draw image stretched
    Pixmap pixmap = XCreatePixmap(display, root, width, height, DefaultDepth(display, screen));
    imlib_context_set_drawable(pixmap);
    imlib_render_image_on_drawable_at_size(0, 0, width, height);
    imlib_free_image();

    // Create a window on top of root
    Window win = XCreateSimpleWindow(display, root, 0, 0, width, height, 0, 0, 0);
    XMapWindow(display, win);

    // Copy pixmap to window
    GC gc = XCreateGC(display, win, 0, nullptr);
    XCopyArea(display, pixmap, win, gc, 0, 0, width, height, 0, 0);
    XFreeGC(display, gc);

    // Keep window open until keypress
    XSelectInput(display, win, KeyPressMask);
    XEvent e;
    XNextEvent(display, &e);

    XFreePixmap(display, pixmap);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
    return 0;
}
