# [PowerShell script for Windows]

# Deletes project cache files.

echo "** Cleaning project **"

rm .\CMakeCache.txt
rm -r .\Win32
rm -r .\Debug
