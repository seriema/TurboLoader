# [PowerShell script for Windows]

# Builds the project and executable.

echo "** Building project **"

cmake .
msbuild .\ALL_BUILD.vcxproj
