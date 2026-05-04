
// 1. Fixed Flow Goto Statements : To avoid spaghetti!
// 2. GOLEM : Logical Mapping and Manual Modelling of the Program.
// 3. CODEX : List of Spells (Framework).

// ==============================================================================================
// runomancy : language basics 

/// Basic Types
/* Inventory [ std::string ] { C++20 }
1. s.length(); / s.size(); // Returns number of characters
2. s.empty(); // Returns true if string is empty
3. s.append(str); / s += str; // Appends string to the end
4. s.push_back(c); // Adds character c to the end
5. s.starts_with(pref); // C++20: Returns true if string begins with pref
6. s.ends_with(suff); // C++20: Returns true if string ends with suff
7. s.contains(sub); // C++23 (often used with C++20): Checks for substring
8. s.substr(pos, len); // Returns a sub-string starting at pos
9. s.find(sub); // Returns index of first occurrence or std::string::npos
10. s.replace(pos, len, str); // Replaces portion of string
11. s.insert(pos, str); // Inserts string at specified position
12. s.erase(pos, len); // Removes characters from string
13. s.compare(str); // Returns 0 if strings are equal
14. s.front(); / s.back(); // Accesses first / last character
15. s.c_str(); // Returns pointer to null-terminated C-string
16. std::stoi(s); / std::to_string(val); // Numeric conversions
*/

/// Collections 
/* Inventory [ Vector ] { C++, std::vector } 
1. std::vector<T> v; // Declares an empty vector of type T
2. v.push_back(val); // Adds element to the end
3. v.emplace_back(args); // Constructs element in-place at the end
4. v.pop_back(); // Removes the last element
5. v.size(); // Returns number of elements
6. v.empty(); // Returns true if size is 0
7. v[i]; // Access element at index i (no bounds check)
8. v.at(i); // Access element at index i (with bounds check)
9. v.front(); / v.back(); // Access first / last element
10. v.clear(); // Removes all elements
11. v.reserve(n); // Allocates capacity for at least n elements
12. v.resize(n); // Changes size to n elements
13. v.begin(); / v.end(); // Returns iterator to start / past-the-end
14. v.insert(pos, val); // Inserts val before iterator pos
15. v.erase(pos); // Removes element at iterator pos
*/
/* Inventory [ <unordered_map> ] { C++ }
1. std::unordered_map<K, V> m; // Declares hash map with key type K and value type V
2. m[key] = val; // Inserts or updates element (creates default if key missing)
3. m.at(key); // Accesses value with bounds checking (throws if missing)
4. m.insert({key, val}); // Inserts pair; does nothing if key exists
5. m.emplace(args); // Constructs pair in-place for efficiency
6. m.erase(key); // Removes element by key
7. m.find(key); // Returns iterator to element, or m.end() if not found
8. m.count(key); // Returns 1 if key exists, 0 otherwise
9. m.size(); // Returns number of elements
10. m.empty(); // Returns true if map is empty
11. m.clear(); // Removes all elements
12. m.reserve(n); // Sets bucket count to accommodate n elements
13. m.bucket_count(); // Returns current number of hash buckets
14. m.load_factor(); // Returns average elements per bucket (size/bucket_count)
15. for(auto& [k, v] : m); // Structured binding loop to iterate pairs
*/

/// Filesystem 
/* Inventory [ <filesystem> ] { C++ }
1. fs::path p(str); // Creates a path object
2. fs::exists(p); // Checks if path exists
3. fs::is_directory(p); // Checks if path is a directory
4. fs::is_regular_file(p); // Checks if path is a file
5. fs::file_size(p); // Returns size of file in bytes
6. fs::create_directory(p); // Creates a new directory
7. fs::copy(src, dst); // Copies files or directories
8. fs::remove(p); // Deletes a file or empty directory
9. fs::remove_all(p); // Deletes directory and all contents
10. fs::rename(old, new); // Renames or moves a file/dir
11. fs::current_path(); // Gets or sets working directory
12. p.extension(); // Returns the file extension (e.g., ".txt")
13. p.filename(); // Returns the generic name component
14. p.parent_path(); // Returns the path to the parent directory
15. fs::directory_iterator(p); // Iterator for directory contents
16. fs::recursive_directory_iterator(p); // Iterator for contents + subfolders
*/
/* Inventory [ std::filesystem::path ] { C++ }
1. p.string(); // Returns path as a std::string
2. p.parent_path(); // Returns path of the parent directory
3. p.filename(); // Returns the filename component (name + ext)
4. p.stem(); // Returns filename without the extension
5. p.extension(); // Returns the extension (including the dot)
6. p.is_absolute(); / p.is_relative(); // Checks path type
7. p.root_name(); / p.root_directory(); // Returns root components (e.g., "C:" or "/")
8. p.replace_filename(new_f); // Replaces the last component of the path
9. p.replace_extension(new_e); // Replaces or removes (if empty) the extension
10. p.clear(); // Empties the path object
11. p1 / p2; // Operator/ joins paths with the platform's preferred separator
12. p1 /= p2; // Appends p2 to p1 using a directory separator
13. p.lexically_normal(); // Returns path with dots and redundant separators removed
14. p.lexically_relative(base); // Returns path relative to a base path
15. p.begin(); / p.end(); // Iterates through each component of the path
*/

/* Inventory [ std::function ] { C++ } 
 * SYNTAX // SHORTDESCRIPTION 
 * #include <functional> // Header required to use std::function
 * // -- Declaration & Basics --
 * std::function<return_type(arg_types)> name; // Declares a polymorphic function wrapper
 * std::function<void(int)> func = nullptr; // Initialize as empty/null
 * if (func) { func(10); } // Safe check: operator bool() returns true if callable is stored
 * // -- Assignments --
 * func = myRegularFunction; // Wrap a standard function pointer
 * func = [](int x) {  }; // Wrap a lambda expression (closure)
 * func = std::bind(&Class::Method, &obj, _1); // Wrap a class member function using bind
 * func = MyFunctor(); // Wrap a class that implements operator()
 * // -- Advanced Operations --
 * func.target<T>(); // Access the underlying function pointer or object of type T
 * func.target_type(); // Returns typeid information of the stored callable
 * func = {}; // Clears the function wrapper (assigns empty state)
 * std::invoke(func, args...); // Invoke the function wrapper using standard utility
 * */

/* Inventory [ Pointers ] { C++ } 
1. Never new inside a function and store it as global // hard to track, called twice issue
2. 
*/

// ===========================================================================================================
// X11 

/** [ Keyboard Mouse Control X11 ]

Keywords
1. GrabKey ~ Keysupress 
2. xdotool ~ mouse control utility 
3. libx11-dev ~ sudo apt install libx11-dev 
.
* How to dinamically activate key supress ? 

Possible Inventories:
1. Inventory [ X11/Xlib.h ] { C++, Linux }
2. Inventory [ X11/Xutil.h ] { C++, Linux }
3. Inventory [ XGrabKey ] { C++, Linux }
4. Inventory [ XEvent ] { C++, Linux }
5. Inventory [ XWarpPointer ] { C++, Linux }

*/

/* Inventory [ X11/Xlib.h ] { C++, Linux }
1. Display* XOpenDisplay(const char* display_name) // Open connection to X server
2. void XCloseDisplay(Display* display) // Close connection
3. Window XDefaultRootWindow(Display* display) // Get root window ID
4. int XDefaultScreen(Display* display) // Get default screen number
*/

/* Inventory [ X11/Xutil.h ] { C++, Linux }
1. Status XStringListToTextProperty(char** list, int count, XTextProperty* prop) // Convert string list to property
2. void XFree(XTextProperty* prop) // Free text property memory
3. int XIconSizeWindow(Display* display, Window w, XSizeHints* hints) // Set icon size hints
*/

/* Inventory [ XGrabKey ] { C++, Linux }
1. int XGrabKey(Display* display, unsigned int keycode, unsigned int modifiers, Window grab_window, Bool owner_events, int pointer_mode, int keyboard_mode) // Capture key press
2. int XUngrabKey(Display* display, unsigned int keycode, unsigned int modifiers, Window grab_window) // Release key capture
*/

/* Inventory [ XEvent ] { C++, Linux }
1. int type // Event type (KeyRelease, ButtonPress, etc.)
2. union { XKeyEvent xkey, XButtonEvent xbutton, XMotionEvent xmotion, XConfigureEvent xconfigure; } // Event data union
3. void XNextEvent(Display* display, XEvent* event) // Block and wait for next event
4. Bool XPeekEvent(Display* display, XEvent* event) // Peek at next event without removing
*/

/* Inventory [ XWarpPointer ] { C++, Linux }
1. int XWarpPointer(Display* display, Window src_window, Window dest_window, int src_x, int src_y, unsigned int src_width, unsigned int src_height, int dest_x, int dest_y) // Move cursor to absolute coords
*/

/// Inventory [ KeySym Codes ] { C++, Linux } 
/* -- Alphanumeric Keys -- 
XK_A,         // 'A'
XK_B,         // 'B'
XK_C,         // 'C'
XK_D,         // 'D'
XK_E,         // 'E'
XK_F,         // 'F'
XK_G,         // 'G'
XK_H,         // 'H'
XK_I,         // 'I'
XK_J,         // 'J'
XK_K,         // 'K'
XK_L,         // 'L'
XK_M,         // 'M'
XK_N,         // 'N'
XK_O,         // 'O'
XK_P,         // 'P'
XK_Q,         // 'Q'
XK_R,         // 'R'
XK_S,         // 'S'
XK_T,         // 'T'
XK_U,         // 'U'
XK_V,         // 'V'
XK_W,         // 'W'
XK_X,         // 'X'
XK_Y,         // 'Y'
XK_Z,         // 'Z'
*/
/* -- Digits (Numbers) -- 
XK_0,         // '0'
XK_1,         // '1'
XK_2,         // '2'
XK_3,         // '3'
XK_4,         // '4'
XK_5,         // '5'
XK_6,         // '6'
XK_7,         // '7'
XK_8,         // '8'
XK_9,         // '9'
*/
/* -- Punctuation Keys -- 
XK_comma,     // ','
XK_period,    // '.'
XK_semicolon, // ';'
XK_exclam,    // '!'
XK_quotedbl,  // '"'
XK_apostrophe, // '\''
XK_colon,     // ':'
XK_question,  // '?'
XK_less,      // '<'
XK_greater,   // '>'
*/
/* -- Modifier Keys -- 
XK_Shift_L,   // Left Shift
XK_Shift_R,   // Right Shift
XK_Control_L, // Left Control
XK_Control_R, // Right Control
XK_Alt_L,     // Left Alt (Meta)
XK_Alt_R,     // Right Alt (Meta)
XK_Caps_Lock, // Caps Lock
XK_Num_Lock,  // Num Lock
XK_Scroll_Lock,// Scroll Lock
XK_Super_L,   // Left Super (Windows key)
XK_Super_R,   // Right Super (Windows key)
*/
/* -- Function Keys -- 
XK_F1,        // Function key F1
XK_F2,        // Function key F2
XK_F3,        // Function key F3
XK_F4,        // Function key F4
XK_F5,        // Function key F5
XK_F6,        // Function key F6
XK_F7,        // Function key F7
XK_F8,        // Function key F8
XK_F9,        // Function key F9
XK_F10,       // Function key F10
XK_F11,       // Function key F11
XK_F12,       // Function key F12
*/
/* -- Special Keys -- 
XK_Escape,    // 'Escape'
XK_Tab,       // 'Tab'
XK_Return,    // 'Enter'
XK_BackSpace, // 'Backspace'
XK_Delete,    // 'Delete'
XK_Insert,    // 'Insert'
XK_Home,      // 'Home'
XK_End,       // 'End'
XK_Page_Up,   // 'Page Up'
XK_Page_Down, // 'Page Down'
*/
/* -- Arrow Keys -- 
XK_Up,        // 'Up Arrow'
XK_Down,      // 'Down Arrow'
XK_Left,      // 'Left Arrow'
XK_Right,     // 'Right Arrow'
*/
/* -- Numeric Keypad -- 
XK_KP_0,      // '0' on Num Pad
XK_KP_1,      // '1' on Num Pad
XK_KP_2,      // '2' on Num Pad
XK_KP_3,      // '3' on Num Pad
XK_KP_4,      // '4' on Num Pad
XK_KP_5,      // '5' on Num Pad
XK_KP_6,      // '6' on Num Pad
XK_KP_7,      // '7' on Num Pad
XK_KP_8,      // '8' on Num Pad
XK_KP_9,      // '9' on Num Pad
XK_KP_Add,    // Num Pad '+'
XK_KP_Subtract, // Num Pad '-'
XK_KP_Multiply, // Num Pad '*'
XK_KP_Divide, // Num Pad '/'
XK_KP_Enter,  // Num Pad 'Enter'
XK_KP_Decimal, // Num Pad '.'
*/
/* -- Media and Miscellaneous Keys -- 
XK_Mute,      // Mute key
XK_Volume_Up, // Volume Up key
XK_Volume_Down, // Volume Down key
XK_Play,      // Play key
XK_Pause,     // Pause key
XK_Stop,      // Stop key
XK_Next,      // Next track key
XK_Prev,      // Previous track key
XK_Media_Play, // Media play/pause key
XK_Media_Stop, // Media stop key
XK_Media_Pause, // Media pause key
*/
/* -- Symbol Keys -- 
XK_at,        // '@'
XK_dollar,    // '$'
XK_percent,   // '%'
XK_ampersand, // '&'
XK_asterisk,  // '*'
XK_leftparen, // '('
XK_rightparen,// ')'
XK_plus,      // '+'
XK_minus,     // '-'
XK_equals,    // '='
XK_tilde,     // '~'
XK_backslash, // '\\'
XK_bar,       // '|'
XK_circumflex, // '^'
*/
/* -- ISO/IEC Keys (International Layouts) -- 
XK_Section,   // Section (§) key
XK_Umlaut,    // Umlaut/Diacritic key
XK_acute,     // Acute Accent key
XK_grave,     // Grave Accent key
XK_acute,     // Acute Accent (´)
XK_circumflex, // Circumflex (^) (ˆ)
XK_tilde,     // Tilde (~) key
XK_diaeresis, // Umlaut (¨) key
XK_acute,     // Acute accent key (´)
XK_diaeresis, // Diaeresis or Umlaut key (¨)
XK_dollar,    // Dollar symbol ($) key
XK_yen,       // Yen symbol (¥)
XK_cent,      // Cent symbol (¢)
XK_EuroSign,  // Euro symbol (€)
XK_Pound,     // Pound symbol (£)
*/
/* -- Miscellaneous Symbols and Extended Keys -- 
XK_F13,       // Function key F13
XK_F14,       // Function key F14
XK_F15,       // Function key F15
XK_F16,       // Function key F16
XK_F17,       // Function key F17
XK_F18,       // Function key F18
XK_F19,       // Function key F19
XK_F20,       // Function key F20
XK_F21,       // Function key F21
XK_F22,       // Function key F22
XK_F23,       // Function key F23
XK_F24,       // Function key F24
*/






















