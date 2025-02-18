# Spectr VST3 Plugin

This repository contains spectrum analyzer VST3 plugin based on VST 3 Audio Plug-Ins SDK

## Building
1. Download and install [Build Tools for Visual Studio 2022](https://aka.ms/vs/17/release/vs_BuildTools.exe)
2. Download and install [VST 3 Audio Plug-Ins SDK](https://www.steinberg.net/vst3sdk)
3. Clone this repository
4. `mkdir build; cd build; cmake -G "Visual Studio 17 2022" ..; cmake --build .`

## Testing
Run **VST3PluginTestHost** from the VST3SDK add Spectr VST plugin