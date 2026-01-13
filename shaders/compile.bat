set FXC="C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x64\fxc.exe"

if not exist package\Shaders\Loose\RainDrops mkdir package\Shaders\Loose\RainDrops

%FXC% /T vs_3_0 /E Main /DVS /Fo "package\shaders\loose\RainDrops\ISRAINBLUR.vso" "shaders\RainBlur.hlsl"
%FXC% /T ps_3_0 /E Main /DPS /Fo "package\shaders\loose\RainDrops\ISRAINBLUR.pso" "shaders\RainBlur.hlsl"
%FXC% /T vs_3_0 /E Main /DVS /Fo "package\shaders\loose\RainDrops\ISRAINDROPSBLUR.vso" "shaders\RainDrops.hlsl"
%FXC% /T ps_3_0 /E Main /DPS /DBLUR /Fo "package\shaders\loose\RainDrops\ISRAINDROPSBLUR.pso" "shaders\RainDrops.hlsl"
%FXC% /T vs_3_0 /E Main /DVS /Fo "package\shaders\loose\RainDrops\ISRAINDROPS.vso" "shaders\RainDrops.hlsl"
%FXC% /T ps_3_0 /E Main /DPS /Fo "package\shaders\loose\RainDrops\ISRAINDROPS.pso" "shaders\RainDrops.hlsl"

