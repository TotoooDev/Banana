@echo off
set /p "project_name=Enter your project name: "
rename "GameProject" %project_name%
powershell -Command "(gc premake5.lua) -replace 'GameProject', '%project_name%' | Out-File -encoding ASCII premake5.lua"
GenerateProjects.bat
pause