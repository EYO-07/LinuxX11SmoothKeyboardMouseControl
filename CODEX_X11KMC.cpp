#include "CODEX_X11KMC.h"

// Transmutation
std::vector<std::string> CodexTransmutation::split(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::string current;
    for (char ch : input) {
        if (ch == delimiter) {
            tokens.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }
    if (!current.empty()) {
        tokens.push_back(current);
    }
    return tokens;
}

// Incantation 
bool CodexIncantation::grabKeys(const std::unordered_map<std::string,std::string>& remap, Window& win, Display* display) {
    for(auto& [key, value] : remap) {
        KeySym keySym = XStringToKeysym(value.c_str());
        if (keySym == NoSymbol) { 
            std::cout << "Warning : NoSymbol for : " << value << std::endl;
            return false; 
        }
        KeyCode keyCode = XKeysymToKeycode(display, keySym);
        XGrabKey(
            display, 
            keyCode, 
            AnyModifier, 
            win, 
            True, 
            GrabModeAsync, 
            GrabModeAsync
        );
    }
    return true;
}
bool CodexIncantation::unGrabKeys(const std::unordered_map<std::string,std::string>& remap, Window& win, Display* display) {
    for(auto& [key, value] : remap) {
        KeySym keySym = XStringToKeysym(value.c_str());
        if (keySym == NoSymbol) {
            std::cout << "Warning : NoSymbol for : " << value << std::endl;
            return false;
        }    
        KeyCode keyCode = XKeysymToKeycode(display, keySym);
        XUngrabKey(display,keyCode, AnyModifier, win);
    }
    return true;
}

// Global flag to detect grab failures
static bool g_grabFailed = false;

// Error Handler to catch BadAccess synchronously
int errorHandler(Display* d, XErrorEvent* e) {
    if (e->error_code == BadAccess) {
        std::cerr << "Error: Key combination already grabbed by another app (BadAccess)." << std::endl;
        g_grabFailed = true;
    }
    return 0; // Suppress default X11 error message
}

bool CodexIncantation::globalGrab(std::unordered_map<std::string, std::string>& keyRemap, Display* display) {
    if (!display) {
        std::cerr << "Error: Cannot open display." << std::endl;
        return false;
    }
    // Install error handler
    XSetErrorHandler(errorHandler);
    Window root = DefaultRootWindow(display);
    // CRITICAL: Select input on root to receive events
    XSelectInput(display, root, KeyPressMask);
    // Define modifier masks to grab (covers NumLock, CapsLock, etc.)
    // AnyModifier is unreliable; explicit masks prevent "all-or-nothing" failure.
    std::vector<unsigned int> modifiers = {
        0, 
        LockMask, 
        Mod2Mask, 
        LockMask | Mod2Mask,
        Mod5Mask, // ISO_Level3_Shift
        LockMask | Mod5Mask,
        Mod2Mask | Mod5Mask,
        LockMask | Mod2Mask | Mod5Mask
    };
    g_grabFailed = false;
    int successCount = 0;
    for (auto& [key, value] : keyRemap) {
        KeySym keySym = XStringToKeysym(value.c_str());
        if (keySym == NoSymbol) { 
            std::cerr << "Warning: NoSymbol for key: " << value << std::endl;
            continue; 
        }
        KeyCode keyCode = XKeysymToKeycode(display, keySym);
        if (keyCode == 0) {
            std::cerr << "Warning: KeyCode 0 for keysym: " << value << std::endl;
            continue;
        }        
        // Grab explicitly for each modifier combination
        for (unsigned int mod : modifiers) {
            g_grabFailed = false; // Reset flag
            XGrabKey(
                display, 
                keyCode, 
                mod, // Specific mask
                root,               
                False,              // owner_events: False ensures events come ONLY to us
                GrabModeAsync, 
                GrabModeAsync
            );       
            if (!g_grabFailed) successCount++;
        }
    }
    XFlush(display);
    if (successCount == 0) {
        std::cerr << "Fatal: No keys were grabbed successfully." << std::endl;
        return false;
    }
    std::cout << "Success: Keys grabbed (combinations: " << successCount << "). Start event loop now." << std::endl;
    return true;
}

bool CodexIncantation::globalUngrab(std::unordered_map<std::string, std::string>& keyRemap, Display* display) {
    if (!display) return false;
    // Restore default error handler if needed, or keep custom one
    // XSetErrorHandler(NULL); 
    Window root = DefaultRootWindow(display);
    std::vector<unsigned int> modifiers = {
        0, LockMask, Mod2Mask, LockMask | Mod2Mask,
        Mod5Mask, LockMask | Mod5Mask, Mod2Mask | Mod5Mask, LockMask | Mod2Mask | Mod5Mask
    };
    for (auto& [key, value] : keyRemap) {
        KeySym keySym = XStringToKeysym(value.c_str());
        if (keySym == NoSymbol) continue;
        KeyCode keyCode = XKeysymToKeycode(display, keySym);
        if (keyCode == 0) continue;
        for (unsigned int mod : modifiers) {
            XUngrabKey(display, keyCode, mod, root);
        }
    }
    XFlush(display);
    std::cout << "Success: Keys ungrabbed." << std::endl;
    return true;
}


bool CodexIncantation::getKeypress(KeySym& key,Display* display) {
    XEvent event;
    if (XPending(display) > 0) {
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            //key = XKeycodeToKeysym(display, event.xkey.keycode, 0);
            key = XkbKeycodeToKeysym(
                display, event.xkey.keycode, 0, 
                (event.xkey.state & ShiftMask) ? 1 : 0
            );
            return true;
        }
    }
    return false;
}
void CodexIncantation::mouseMove(int x, int y, Display* display) {
    XWarpPointer(display, None, DefaultRootWindow(display), 0, 0, 0, 0, x, y);
    XFlush(display);  // Apply the changes immediately
    //std::cout << "Mouse moved to: " << x << ", " << y << std::endl;
}
bool CodexIncantation::getMouseXY(int& x, int& y) {
    // Get the display connection
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Unable to open X display!" << std::endl;
        return false;
    }

    // Get the root window
    Window root = DefaultRootWindow(display);
    
    // Get the current mouse position
    Window root_return, child_return;
    int root_x, root_y;
    unsigned int mask_return;
    
    // Query the pointer location
    if (XQueryPointer(display, root, &root_return, &child_return, &root_x, &root_y, &x, &y, &mask_return)) {
        // Successfully retrieved the mouse position
        XCloseDisplay(display);  // Don't forget to close the display after you're done
        return true;
    } else {
        std::cerr << "Failed to get mouse position!" << std::endl;
        XCloseDisplay(display);
        return false;
    }
}
bool CodexIncantation::isKeyDown(KeySym key, Display *display) {
    if (!display) return false;
    KeyCode keycode = XKeysymToKeycode(display, key);
    if (keycode == NoSymbol) return false;
    char keymap[32];
    XQueryKeymap(display, keymap);
    if (keymap[keycode / 8] & (1 << (keycode % 8))) {
        return true;
    }
    return false;
}
Window CodexIncantation::getActiveWindow(Display *display) {
    Window rootWindow, focusedWindow;
    int revert;    
    // Get the window that currently has the focus
    if (XGetInputFocus(display, &focusedWindow, &revert) == 0) {
        std::cerr << "Unable to get input focus." << std::endl;
        return 0;
    }
    return focusedWindow; // Return the window that has focus
}
void CodexIncantation::mouseLButtonDown() {
    system("sleep 0.1 && xdotool mousedown 1");  // Button 1 is left button
}
void CodexIncantation::mouseLButtonUp() {
    system("xdotool mouseup 1");  // Button 1 is left button
}
void CodexIncantation::mouseRButtonDown() {
    system("sleep 0.1 && xdotool mousedown 3");  // Button 3 is right button
}
void CodexIncantation::mouseRButtonUp() {
    system("xdotool mouseup 3");  // Button 3 is right button
}
void CodexIncantation::mouseScrollDown() {
    system("xdotool click 5");  // 5 is the scroll down button
}
void CodexIncantation::mouseScrollUp() {
    system("xdotool click 4");  // 4 is the scroll up button
}
KeySym CodexIncantation::SK(std::string key) {
    return XStringToKeysym(key.c_str());
}
void CodexIncantation::mouseMButton() {
    system("xdotool click 2"); 
}
Window CodexIncantation::getToplevelWindow(Display *display, Window window) { // unused
    Window root, parent;
    Window *children;
    unsigned int num_children;
    while (true) {
        // Query the tree to find the parent
        // Return current on error
        if (!XQueryTree(display, window, &root, &parent, &children, &num_children)) return window;
        // Free the children list allocated by XQueryTree
        if (children) XFree(children); 
        // If parent is root (or window is root), we are at the top level
        if (window == root || parent == root) return window;
        // Move up one level
        window = parent;
    }
}
bool CodexIncantation::windowsShareToplevel(Display *display, Window win1, Window win2) { // unused
    if (win1 == None || win2 == None) return false;
    if (win1 == win2) return true;
    Window top1 = CodexIncantation::getToplevelWindow(display, win1);
    Window top2 = CodexIncantation::getToplevelWindow(display, win2);
    return (top1 == top2);
}
int CodexIncantation::foreachCommandLineArgument(
    int argc, 
    char* argv[],
    std::function<int(
        std::string, // argument
        int, // argument index 
        std::string key, // key=value
        std::string value, // key=value
        std::vector<std::string>
    )> fnc // inner function 
) {
    // 0 - continue
    // 1 - break
    // 2 - object find or objective find
    int result = 0;
    std::vector<std::string> key_value;
    std::vector<std::string> values;
    if (argc<=1) return 0;
    for (int i = 1; i < argc; ++i) {
        std::string strArgument = argv[i];
        key_value.clear();
        values.clear();
        std::string key = "";
        std::string value = "";
        // -- 
        if ( strArgument.find("=") != std::string::npos ) {
            key_value = CodexTransmutation::split(strArgument,'=');
            if (key_value.size()==2) {
                key = key_value.front();
                value = key_value.back();
                if ( value.find(",") != std::string::npos ) {
                    values = CodexTransmutation::split(value,',');
                }
            }
        }
        result = fnc(strArgument,i,key,value,values);
        if (result == 0) continue;
        if (result == 1) break;
        if (result == 2) break;
    }
    return result;
}

// END CODEX_X11KMC.cpp