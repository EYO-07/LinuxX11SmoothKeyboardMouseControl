/// BEGIN GOLEM X11KMC

// -- preprocessor directives 
#include "CODEX_X11KMC.h"
#include <thread> // For sleep functionality
#include <chrono> // For duration
// -- variables 
bool b_control_active = true;
int defaultMoveStep = 5;
int moveStep = 5; 
std::unordered_map<Window, bool> window2ungrabstate;
std::vector<std::string> keyListF = {"w","a","s","d","e","q","r","f","c","F9","F12"};
std::vector<std::string> keyList = {"w","a","s","d","e","q","r","f","c"};
auto sleep_duration = std::chrono::milliseconds(8); // (e.g., 16ms to run at ~60 FPS)
auto confirm_duration = std::chrono::milliseconds(100); // (e.g., 16ms to run at ~60 FPS)
static std::string USAGE_TEXT = R"(
Usage: <application> [options]

Options:
    --help                                                          Show usage.
    --speed=<value>                                                 Control the default pointer speed.
    --wasd=<up>,<left>,<down>,<right>                               Remap Movement Keys.
    --LRUD=<left-button>,<right-button>,<scroll-up>,<scroll-down>   Remap Mouse Buttons.
    --toggles=<speed>,<exit>,<script>                               Remap for Exit and Toggles.
)";
// -- forward declaration || functions 
void toggleSpeed();
void toggleScript(Display* display);
void cleanUp(Display* display);
std::string vec2string(std::vector<std::string> vec, int start, int len);
int x11ErrorHandler(Display* d, XErrorEvent* e);
bool newWindow(Window& win, Display*& display);
void coutDisplay();
std::string TF(bool value);
// -- entry point 
int main(int argc, char* argv[]) {
    // -- components 
    Display* display = XOpenDisplay(NULL);
    Window win;
    Window dummy;
    Window oldWin=0;
    int x = 500, y = 500; 
    int dx = 0; int dy = 0;
    int return_val = 0;
    bool b_lbutton_down = false;
    bool b_rbutton_down = false;
    // -- commandline arguments 
    if ( CodexIncantation::foreachCommandLineArgument(argc,argv,[](
        std::string argument, 
        int index,
        std::string key, // key=value
        std::string value, // key=value
        std::vector<std::string> values // comma separated 
    )->int {
        // -- single arguments 
        if ( argument.compare("--help")==0 ) {
            std::cout << USAGE_TEXT << std::endl;
            return 2;
        }
        // -- composed arguments 
        if ( key.empty() || value.empty() ) return 0;
        if ( key.compare("--speed")==0 ) {
            if ( value.empty() ) return 0;
            if (values.size()>0) return 0;
            int speed_int;    
            try {
                speed_int = std::stoi(value);
            } catch (const std::exception& e) {
                return 0;
            }
            if (speed_int>1) { 
                defaultMoveStep = speed_int;
                moveStep = speed_int;
            }
            return 0;
        }
        // -- composed values 
        if ( values.size()<=1 ) return 0;
        if ( key.compare("--wasd")==0 && values.size()==4 ) {
            int i=0;
            for (const auto& keystr: values) {
                keyList[i] = keystr;
                keyListF[i] = keystr;
                i++;
            }
            return 0;
        }
        if ( key.compare("--LRUD")==0 && values.size()==4 ) {
            int i=4;
            for (const auto& keystr: values) {
                keyList[i] = keystr;
                keyListF[i] = keystr;
                i++;
            }
            return 0;
        }
        if ( key.compare("--toggles")==0 && values.size()==3 ) {
            int i=8;
            keyList[8] = values[0];
            for (const auto& keystr: values) {
                keyListF[i] = keystr;
                i++;
            }
            return 0;
        }
        // -- end
        return 0; // continue 
    })==2 ) goto end;
    // -- logic 
    coutDisplay();
    newWindow(dummy,display);
    if (!dummy) goto fail;
    if (!display) goto fail;
    while (true) {
        while (XPending(display)) {
            XEvent ev;
            XNextEvent(display, &ev);
            if (ev.type == DestroyNotify) {
                window2ungrabstate.erase(ev.xdestroywindow.window);
            }
        } 
        if (!b_control_active) goto end_main_iteration;
        win = getActiveWindow(display);
        if (!win) goto end_main_iteration;
        if (oldWin!=0 && win == oldWin) goto input_processing;
    update_grab_state: // Section Context : b_control_active, win!=oldWin
        if ( window2ungrabstate.count(win)==0 ) { // initialization
            if (!unGrabKeys(keyList, win, display)) goto fail;
            window2ungrabstate[win] = true;
        }
        if ( oldWin!=0 && window2ungrabstate.count(oldWin)>0 ) { // ungrab keys for old win 
            if (!unGrabKeys(keyList, oldWin, display)) goto fail;
            window2ungrabstate[oldWin] = true;
        }
        if ( window2ungrabstate[win] ) { // grab keys for current window if ungrabbed
            if( !grabKeys(keyList, win, display)) goto fail; 
            window2ungrabstate[win] = false;
        }
    input_processing: // Section Context : b_control_active, valid keys 
        // -- mouse buttons 
        if (isKeyDown(SK(keyListF[4]), display)) { // Left Button
            if (!b_lbutton_down) {
                mouseLButtonDown();
                b_lbutton_down = true;
            }
        } else {
            if (b_lbutton_down) {
                mouseLButtonUp();
                b_lbutton_down = false;
            }
        }
        if (isKeyDown(SK(keyListF[5]), display)) { // Right Button 
            if (!b_rbutton_down) {
                mouseRButtonDown();
                b_rbutton_down = true;
            }
        } else {
            if (b_rbutton_down) {
                mouseRButtonUp();
                b_rbutton_down = false;
            }
        }
        if ( isKeyDown(SK(keyListF[6]), display) ) mouseScrollUp();
        if ( isKeyDown(SK(keyListF[7]), display) ) mouseScrollDown();
        if ( isKeyDown(SK(keyListF[8]), display) ) toggleSpeed();
        // -- mouse movement 
        dx = 0; dy = 0;
        getMouseXY(x,y);
        if ( isKeyDown(SK(keyListF[0]),display) ) dy = -moveStep;
        if ( isKeyDown(SK(keyListF[1]),display) ) dx = -moveStep; 
        if ( isKeyDown(SK(keyListF[2]),display) ) dy = +moveStep;     
        if ( isKeyDown(SK(keyListF[3]),display) ) dx = +moveStep;
        if ( dx || dy ) mouseMove(x+dx,y+dy, display);
    end_main_iteration: // Section Context : valid keys 
        // -- always active keys || exit | toggle script 
        if ( isKeyDown(SK(keyListF[9]),display) ) goto end;
        if ( isKeyDown(SK(keyListF[10]),display)  ) toggleScript(display);
        oldWin = win;
        std::this_thread::sleep_for(sleep_duration);
    }
    goto end;
fail:
    return_val = 1;
end:    
    cleanUp(display);
    return return_val;
}
// -- implementations 
void toggleSpeed() {
    if ( moveStep == defaultMoveStep ) {
        moveStep = 3*defaultMoveStep;
    } else {
        moveStep = defaultMoveStep;
    }
    std::this_thread::sleep_for(confirm_duration);
    coutDisplay();
}
void toggleScript(Display* display) {
    // -- toggle 
    b_control_active = !b_control_active;
    // -- update 
    if (b_control_active) {
        Window win = getActiveWindow(display);
        if ( window2ungrabstate.count(win)==0 ) { // initialization
            unGrabKeys(keyList, win, display);
            window2ungrabstate[win] = true;
        }
        if ( window2ungrabstate[win] ) { // grab keys for current window if ungrabbed
            grabKeys(keyList, win, display); 
            window2ungrabstate[win] = false;
        }
    } else {
        mouseRButtonUp();
        mouseLButtonUp();    
        for(const auto& [w, b] : window2ungrabstate) {
            bool b_ungrab = b;
            if (b) continue; 
            Window win = w;
            unGrabKeys(keyListF,win, display);
            window2ungrabstate[win] = true;
        }
    }
end_toggle:
    std::this_thread::sleep_for(confirm_duration);
    coutDisplay();
}
void cleanUp(Display* display) {
    mouseRButtonUp();
    mouseLButtonUp();    
    for(const auto& [w, b] : window2ungrabstate) {
        bool b_ungrab = b;
        if (b) continue; 
        Window win = w;
        unGrabKeys(keyListF,win, display);
        window2ungrabstate[win] = true;
    }
    XCloseDisplay(display);
}
std::string vec2string(std::vector<std::string> vec, int start, int len) {
    std::string result = "";
    int index = 0;
    int count = 0;
    for(const auto& item:vec){
        if (count>=len) break;
        if (index<start) { 
            index++;
            continue;
        }
        result.append(item+",");
        index++;
        count++;
    }
    return result;
}
int x11ErrorHandler(Display* d, XErrorEvent* e) {
    // ignore BadWindow safely
    if (e->error_code == BadWindow) return 0;
    return 0;
}
bool newWindow(Window& win, Display*& display) {
    Window root;
    if (!display) goto fail;
    root = DefaultRootWindow(display);
    XSetErrorHandler(x11ErrorHandler);
    win = XCreateSimpleWindow(display, root, 0, 0, 1, 1, 0, 0, 0);
    XSelectInput(display, win, KeyPressMask);
    XSelectInput(display, root, StructureNotifyMask);
    //XMapWindow(display, win);
    XFlush(display);
    return true;
fail:
    std::cerr << "Error opening X display or creating window!" << std::endl;
    return false;
}
std::string TF(bool value) {
    if (value) return "True";
    return "False";
}
void coutDisplay() {
    std::system("clear");
    std::cout << std::endl;
    std::cout << "Smooth Keyboard Mouse Control { XOrg/X11, xdotool }" << std::endl;
    std::cout << "- speed : " << moveStep << "/" << defaultMoveStep << std::endl;
    std::cout << "- active : " << TF(b_control_active) << std::endl;
    std::cout << "1. " << keyListF[9] << " - Exit" << std::endl;
    std::cout << "2. " << keyListF[10] << " - Toggle Key Capture" << std::endl;
    std::cout << "3. " << vec2string(keyList,0,4) << " - Movement" << std::endl;
    std::cout << "4. " << vec2string(keyList,4,2) << " - Left/Right Buttons" << std::endl;
    std::cout << "5. " << vec2string(keyList,6,2) << " - Scrolling" << std::endl;
    std::cout << "6. " << keyList[8] << " - Toggle Speed" << std::endl;
    std::cout << std::endl;
}

/// END GOLEM X11KMC

