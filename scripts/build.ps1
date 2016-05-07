# [PowerShell script for Windows]

# Builds the project and executable.

echo "** Building project **"

cmake .
msbuild .\a_retro_ui.sln /m
