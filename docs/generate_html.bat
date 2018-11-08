@echo off

REM Uses pandoc to generate HTML from Markdown documentation

pandoc QLAYVISUAL.md -s -o QLAYVISUAL.html --metadata pagetitle="Qlay Visual tutorial"
