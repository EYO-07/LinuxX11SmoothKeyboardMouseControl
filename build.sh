# build 
# This project requires xdotool and XOrg/X11 Graphical Server.
# if you already use xorg just install xdotools with your package manager i.e. `sudo apt install xdotool`
# if you use another graphical server, I don't recommend to use this program.
clear 
# -- gcc
g++ -std=c++20 -fpermissive \
    main.cpp \
    CODEX_X11KMC.cpp \
-lX11 -o keyboardMouseControlX11
# -- 
echo "Script Finished"    