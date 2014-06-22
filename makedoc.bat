@echo off
doxygen docfiles\Doxyfile
rmdir dochtml
mkdir dochtml
for %%a in (docxml\*.xml) do (
	msxsl %%a docfiles\template.xsl -o dochtml\%%~na.html
)
copy docxml\index.xml dochtml\index.xml
copy docfiles\search.html dochtml\search.html
copy docfiles\format.css dochtml\format.css

