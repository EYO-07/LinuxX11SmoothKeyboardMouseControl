#pragma once
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <X11/XKBlib.h>  // Include XKB for XkbKeycodeToKeysym
#include <X11/keysym.h>
#include <cstdlib>  // For system() function
#include <unordered_map>
#include <stdexcept>
#include <functional>

namespace CodexTransmutation {
    std::vector<std::string> split(const std::string& input, char delimiter);
}
namespace CodexIncantation {
    // -- command line arguments 
    int foreachCommandLineArgument( // 0 - normal, 1 - break loop, 2 - objective find
        int argc, 
        char* argv[],
        std::function<int(
            std::string, // argument
            int, // argument index 
            std::string key, // key=value
            std::string value, // key=value
            std::vector<std::string>
        )> fnc // inner function 
    );
    // -- windows 
    Window getActiveWindow(Display *display);
    Window getToplevelWindow(Display *display, Window window);
    bool windowsShareToplevel(Display *display, Window win1, Window win2);
    // -- input 
    bool grabKeys(const std::vector<std::string>& list, Window& win, Display* display);
    bool unGrabKeys(const std::vector<std::string>& list, Window& win, Display* display);
    bool grabKeys(const std::unordered_map<std::string,std::string>& map, Window& win, Display* display);
    bool unGrabKeys(const std::unordered_map<std::string,std::string>& map, Window& win, Display* display);
    bool globalGrab(std::unordered_map<std::string, std::string>& keyRemap, Display* display);
    bool globalUngrab(std::unordered_map<std::string, std::string>& keyRemap, Display* display);
    bool getKeypress(KeySym& key,Display* display);
    void mouseMove(int x, int y, Display* display);
    bool getMouseXY(int& x, int& y);
    bool isKeyDown(KeySym key, Display *display);
    void mouseLButtonDown();
    void mouseLButtonUp();
    void mouseRButtonDown();
    void mouseRButtonUp();
    void mouseMButton();
    void mouseScrollDown();
    void mouseScrollUp();
    KeySym SK(std::string);
}

// END CODEX_X11KMC.h