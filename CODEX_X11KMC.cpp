#include "CODEX_X11KMC.h"

/*
bool grabKeys(const std::vector<std::string>& list, Window& win, Display* display) {
    for(const auto& key: list) {
        KeySym keySym = XStringToKeysym(key.c_str());
        if (keySym == NoSymbol) { 
            std::cout << "Warning : NoSymbol for : " << key << std::endl;
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
bool unGrabKeys(const std::vector<std::string>& list, Window& win, Display* display) {
    for(const auto& key: list) {
        KeySym keySym = XStringToKeysym(key.c_str());
        if (keySym == NoSymbol) {
            std::cout << "Warning : NoSymbol for : " << key << std::endl;
            return false;
        }    
        KeyCode keyCode = XKeysymToKeycode(display, keySym);
        XUngrabKey(display,keyCode, AnyModifier, win);
    }
    return true;
}
*/

bool grabKeys(const std::unordered_map<std::string,std::string>& remap, Window& win, Display* display) {
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
bool unGrabKeys(const std::unordered_map<std::string,std::string>& remap, Window& win, Display* display) {
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
bool getKeypress(KeySym& key,Display* display) {
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
void mouseMove(int x, int y, Display* display) {
    XWarpPointer(display, None, DefaultRootWindow(display), 0, 0, 0, 0, x, y);
    XFlush(display);  // Apply the changes immediately
    //std::cout << "Mouse moved to: " << x << ", " << y << std::endl;
}
bool getMouseXY(int& x, int& y) {
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
bool isKeyDown(KeySym key, Display *display) {
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
Window getActiveWindow(Display *display) {
    Window rootWindow, focusedWindow;
    int revert;    
    // Get the window that currently has the focus
    if (XGetInputFocus(display, &focusedWindow, &revert) == 0) {
        std::cerr << "Unable to get input focus." << std::endl;
        return 0;
    }
    return focusedWindow; // Return the window that has focus
}
void mouseLButtonDown() {
    system("xdotool mousedown 1");  // Button 1 is left button
}
void mouseLButtonUp() {
    system("xdotool mouseup 1");  // Button 1 is left button
}
void mouseRButtonDown() {
    system("xdotool mousedown 3");  // Button 3 is right button
}
void mouseRButtonUp() {
    system("xdotool mouseup 3");  // Button 3 is right button
}
void mouseScrollDown() {
    system("xdotool click 5");  // 5 is the scroll down button
}
void mouseScrollUp() {
    system("xdotool click 4");  // 4 is the scroll up button
}
KeySym SK(std::string key) {
    return XStringToKeysym(key.c_str());
}
void mouseMButton() {
    system("xdotool click 2"); 
}

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