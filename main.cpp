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
std::unordered_map<std::string, std::string> keyRemap;
auto sleep_duration = std::chrono::milliseconds(20); // (e.g., 16ms to run at ~60 FPS)
auto confirm_duration = std::chrono::milliseconds(200); // (e.g., 16ms to run at ~60 FPS)
static std::string USAGE_TEXT = R"(
Usage: <application> [options]

Options:
    --help                                                          Show usage.
    --speed=<value>                                                 Control the default pointer speed.
    --wasd=<up>,<left>,<down>,<right>                               Remap Movement Keys.
    --LRUD=<left-button>,<right-button>,<scroll-up>,<scroll-down>   Remap Mouse Buttons.
    --toggles=<speed>,<exit>,<script>                               Remap for Exit and Toggles.
    --ZX=<decrease-base-speed>,<increase-base-speed>                Remap for Increase and Decrease speed.
)";
// -- forward declaration || functions 
void toggleSpeed();
void toggleScript(Display* display);
void cleanUp(Display* display);
std::string vec2string(std::vector<std::string> vec, int start, int len);
int x11ErrorHandler(Display* d, XErrorEvent* e);
bool newWindow(Window& win, Display*& display);
void coutDisplay();
std::string TF(bool value); // print True of False
void increaseBaseSpeed();
void decreaseBaseSpeed();
void initKeyRemap();
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
    initKeyRemap();
    // -- commandline arguments 
    if ( CodexIncantation::foreachCommandLineArgument(argc,argv,[](
        std::string argument, 
        int index,
        std::string key, // key=value
        std::string value, // key=value
        std::vector<std::string> values // comma separated 
    )->int {
        // -- single arguments 
        if ( argument.compare("--help")==0 || argument.compare("-h")==0 ) {
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
            keyRemap["w"] = values[0];
            keyRemap["a"] = values[1];
            keyRemap["s"] = values[2];
            keyRemap["d"] = values[3];
            return 0;
        }
        if ( key.compare("--LRUD")==0 && values.size()==4 ) {
            keyRemap["e"] = values[0];
            keyRemap["q"] = values[1];
            keyRemap["r"] = values[2];
            keyRemap["f"] = values[3];
            return 0;
        }
        if ( key.compare("--toggles")==0 && values.size()==3 ) {
            keyRemap["c"] = values[0];
            keyRemap["F9"] = values[1];
            keyRemap["F12"] = values[2];
            return 0;
        }
        if ( key.compare("--ZX")==0 && values.size()==2 ) {
            keyRemap["z"] = values[0];
            keyRemap["x"] = values[1];            
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
    update_grab_state: // Section Context : b_control_active, win!=oldWin, valid win
        if ( window2ungrabstate.count(win)==0 ) { // initialization
            if (!unGrabKeys(keyRemap, win, display)) goto fail;
            window2ungrabstate[win] = true;
        }
        if ( oldWin!=0 && window2ungrabstate.count(oldWin)>0 ) { // ungrab keys for old win 
            if (!unGrabKeys(keyRemap, oldWin, display)) goto fail;
            window2ungrabstate[oldWin] = true;
        }
        if ( window2ungrabstate[win] ) { // grab keys for current window if ungrabbed
            if( !grabKeys(keyRemap, win, display)) goto fail; 
            window2ungrabstate[win] = false;
        }
    input_processing: // Section Context : b_control_active, valid keys, valid win 
        // -- pointer speed 
        if ( isKeyDown(SK(keyRemap.at("z")), display) ) decreaseBaseSpeed();
        if ( isKeyDown(SK(keyRemap.at("x")), display) ) increaseBaseSpeed();
        // -- mouse buttons 
        if (isKeyDown(SK(keyRemap.at("e")), display)) { // Left Button
            if (!b_lbutton_down) {
                mouseLButtonDown();
                b_lbutton_down = true;
                goto end_main_iteration_no_sleep;
            }
        } else {
            if (b_lbutton_down) {
                mouseLButtonUp();
                b_lbutton_down = false;
            }
        }
        if (isKeyDown(SK(keyRemap.at("q")), display)) { // Right Button 
            if (!b_rbutton_down) {
                mouseRButtonDown();
                b_rbutton_down = true;
                goto end_main_iteration_no_sleep;
            }
        } else {
            if (b_rbutton_down) {
                mouseRButtonUp();
                b_rbutton_down = false;
            }
        }
        if ( isKeyDown(SK(keyRemap.at("r")), display) ) mouseScrollUp();
        if ( isKeyDown(SK(keyRemap.at("f")), display) ) mouseScrollDown();
        if ( isKeyDown(SK(keyRemap.at("c")), display) ) toggleSpeed();
        // -- mouse movement 
        dx = 0; dy = 0;
        getMouseXY(x,y);
        if ( isKeyDown(SK(keyRemap.at("w")),display) ) dy = -moveStep;
        if ( isKeyDown(SK(keyRemap.at("a")),display) ) dx = -moveStep; 
        if ( isKeyDown(SK(keyRemap.at("s")),display) ) dy = +moveStep;     
        if ( isKeyDown(SK(keyRemap.at("d")),display) ) dx = +moveStep;
        if ( dx || dy ) mouseMove(x+dx,y+dy, display);
    end_main_iteration: // Section Context : valid keys 
        // -- always active keys || exit | toggle script 
        if ( isKeyDown(SK(keyRemap.at("F9")),display) ) goto end;
        if ( isKeyDown(SK(keyRemap.at("F12")),display) ) toggleScript(display);
        oldWin = win;
        std::this_thread::sleep_for(sleep_duration);
        continue;
    end_main_iteration_no_sleep:
        if ( isKeyDown(SK(keyRemap.at("F9")),display) ) goto end;
        if ( isKeyDown(SK(keyRemap.at("F12")),display) ) toggleScript(display);
        oldWin = win;
        continue;
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
            unGrabKeys(keyRemap, win, display);
            window2ungrabstate[win] = true;
        }
        if ( window2ungrabstate[win] ) { // grab keys for current window if ungrabbed
            grabKeys(keyRemap, win, display); 
            window2ungrabstate[win] = false;
        }
    } else {
        mouseRButtonUp();
        mouseLButtonUp();    
        for(const auto& [w, b] : window2ungrabstate) {
            bool b_ungrab = b;
            if (b) continue; 
            Window win = w;
            unGrabKeys(keyRemap,win, display);
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
        unGrabKeys(keyRemap,win, display);
        window2ungrabstate[win] = true;
    }
    XCloseDisplay(display);
}

/*
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
*/

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
    std::cout << "1. " << keyRemap["F9"] << " - Exit" << std::endl;
    std::cout << "2. " << keyRemap["F12"] << " - Toggle Key Capture" << std::endl;
    std::cout << "3. " << keyRemap["w"] << ", " << keyRemap["a"] << ", " << keyRemap["s"] << ", " << keyRemap["d"] << " - Movement" << std::endl;
    std::cout << "4. " << keyRemap["e"] << ", " << keyRemap["q"] << " - Left/Right Buttons" << std::endl;
    std::cout << "5. " << keyRemap["r"] << ", " << keyRemap["f"] << " - Scrolling" << std::endl;
    std::cout << "6. " << keyRemap["c"] << " - Toggle Speed" << std::endl;
    std::cout << "7. " << keyRemap["z"] << ", " << keyRemap["x"] << " - Decrease/Increase Base Speed" << std::endl;
    std::cout << std::endl;
}
void increaseBaseSpeed() {
    defaultMoveStep++;
    moveStep = defaultMoveStep;
    std::this_thread::sleep_for(confirm_duration);
    coutDisplay();
}
void decreaseBaseSpeed() {
    if (defaultMoveStep<=1) return;
    defaultMoveStep--;
    moveStep = defaultMoveStep;
    std::this_thread::sleep_for(confirm_duration);
    coutDisplay();
}
void initKeyRemap() {
    // movement 
    keyRemap["w"] = "w";
    keyRemap["a"] = "a";
    keyRemap["s"] = "s";
    keyRemap["d"] = "d";
    // mouse buttons 
    keyRemap["e"] = "e";
    keyRemap["q"] = "q";
    // mouse scrolling 
    keyRemap["r"] = "r";
    keyRemap["f"] = "f";
    // mouse speed 
    keyRemap["z"] = "z"; // decrease mouse base speed 
    keyRemap["x"] = "x"; // increase mouse base speed 
    keyRemap["c"] = "c"; // toggle speed
    // special buttons 
    keyRemap["F9"] = "F9"; // quit 
    keyRemap["F12"] = "F12"; // script toggle 
}























/// END GOLEM X11KMC

