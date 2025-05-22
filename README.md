# SAP
Simple audio player written in C++/CLI using Windows Forms for GUI

# Dependencies
- Miniaudio [Link to repo](https://github.com/mackron/miniaudio)
- TagLib [Link to repo](https://github.com/taglib/taglib)

Note: You dont need to build and link these dependencies, see below.

# Building (Windows)
1. First make sure you have the "Desktop developement with C++" package selected in VS's installer options
2. **If you dont want to manually compile and link TagLib**, also make sure to check "vcpkg project manager" component in VS's installer
3. After download and installing, clone and open this project with Visual Studio
4. If this is your first time using vcpkg, you may need to run "vcpkg integrate install" in the developer terminal (go to View -> Terminal if you dont see it)
5. Run "vcpkg install" to install and link TagLib
6. Now at this moment you should be all set, try to run the app with the "Local Windows Debugger" button
7. Enjoy :)

# Usage
Open the app and go to File > Quick Import and select a folder that contains your music files
Wait for SAP to scan your folder, this might take a moment
Once done, double click a song and click play. Enjoy.

# Features
- Metadata viewer
- LRC sync (If your file has embeded LRC text it should be visible on the "Lyrics" tab, only works for MPEG audio (mp3) and FLACs)
- Uhhhh... idk, a volume slider?

## Motivation
I simply wanted to know more about C++/CLI and what best way to do it than to do the only thing I love, making music players for no reason :p

# License
This project is licensed under the GPLv3 license
