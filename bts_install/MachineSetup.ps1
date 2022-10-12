# begin of script
echo "BTS Automation Script ver 0.1"
# install windows update module
echo "Running Windows Update..."
Get-Command -Module WindowsUpdateProvider
Install-Module PSWindowsUpdate -Force
# get any pending updates + install and reboot if needed
Install-WindowsUpdate -NotCategory "Drivers" -NotTitle "Zune" -MicrosoftUpdate -AcceptAll -AutoReboot | Out-File "./WinUpdate.log" -Force
echo "Windows Update Complete"
# download and install required software
echo "Installing Required Software Packages..."
# package manager
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
# vs community & C++ workloads
choco install visualstudio2022community -y
choco install visualstudio2022-workload-nativedesktop -y
choco install visualstudio2022-workload-nativegame -y
# cmake
choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System' -y
# command line git
choco install git.install --params "/GitAndUnixToolsOnPath /WindowsTerminal /NoAutoCrlf" -y
# refresh PATH vars
refreshenv
echo "Software Installation Complete"
# checkout BTS Demo repo (overwrite any files)
echo "Cloning BTS Repo..."
Push-Location $PSScriptRoot
git clone https://github.com/GX2Overlord/BTSDemo.git
Pop-Location
echo "Cloning Complete"
# build solution with cmake
echo "Building IDE Solution with CMake..."
Push-Location $PSScriptRoot/BTSDemo
cmake -G "Visual Studio 17 2022" -A x64 -S ./ -B ./build
Pop-Location 
echo "Solution IDE Created, building source..."
Push-Location $PSScriptRoot/BTSDemo
cmake --build ./build --config Debug
Pop-Location
# open solution
echo "Opening Solution in Editor..."
Push-Location $PSScriptRoot/BTSDemo/build
Invoke-Item WindowsPong.sln
Start-Sleep -Seconds 15
Invoke-Item ../winpong.cpp
Start-Sleep -Seconds 5
Pop-Location
# run demo executable
echo "Launching Demo Executable..."
Push-Location $PSScriptRoot/BTSDemo/build
Start-Process -FilePath "Debug/WindowsPong.exe" 
Pop-Location
echo "BTS Automation Script complete"
# Comment back in for debugging:
#Read-Host -Prompt "Press Enter to exit"

