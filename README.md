# SwimDragon
Fixes Render Dragon for all versions (only works on windows 10/11 with RTX GPUs)
# Requirements
Windows 10 or 11, an RTX GPU, and Admin on your PC
# How it works
Modifies the NVIDIA driver profile for Minecraft to have the hidden setting "Ray Tracing - (DXR)" to be set to false.
<br>
This forces bedrock to use dx12 without ray tracing, fixing all the performance issues resulting in making the game smoother and have higher FPS.
<br>
This even fixes pink glitch somehow.
# Downloads
[SwimDragon.exe](https://github.com/Swedeachu/SwimDragon/releases/download/Public-Release-1.0/SwimDragon.exe)
<br>
[C++ Installer if needed](https://aka.ms/vs/17/release/vc_redist.x64.exe)
# Credits
NVIDIA, NvAPI, Swim Services, and a bunch of Hive Partners and Devs that helped me test.
<br>
Also thanks to Ambient for fixing the proj files so compiling out the box now works.
# Issues
Only works for PC players that have RTX cards, sorry mobiles and budget gamers. This might false flag anti viruses since this is driver level software, just ignore it. If you still have issues with input delay then disable low latency mode in your global program settings in nvidia control pannel 3D settings. Debloat your PC as best you can.
