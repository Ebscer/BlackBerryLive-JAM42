@echo off
for /f "delims=" %%x in ('dir /od /a-d /b *.bar') do set recent=%%x
@echo on
call blackberry-deploy -installApp -launchApp -device 192.168.1.103 -package %recent% -password playbook
