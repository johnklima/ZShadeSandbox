rem Google: batch file to build visual studio solution
rem http://markkemper1.blogspot.com/2010/10/create-build-file-for-visual-studio.html
rem Project Root:/
rem 	/Build - result of the build will be placed here
rem 	/Source - all source code (& libraries) for the project
rem 	/Tools - collection of tools used for the build process
rem 	/Build.bat - simple 'double click to build' ms dos batch file
rem 	/Build.proj - defines steps for the build process

rem Do not remove
"%windir%\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe" /nologo Build.proj  %*