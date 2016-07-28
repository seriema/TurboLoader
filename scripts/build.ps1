# [PowerShell script for Windows]

# Builds the project and executable.

echo "** Building project **"

cmake .
msbuild .\turboloader.sln /m
