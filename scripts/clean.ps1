# [PowerShell script for Windows]

# Deletes project cache files.

echo "** Cleaning project **"

rm .\CMakeCache.txt
rm -r .\Win32
rm -r .\Debug
rm -r .\a_retro_test.dir
rm -r .\a_retro_ui.dir
