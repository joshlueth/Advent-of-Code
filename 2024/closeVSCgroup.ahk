HotIfWinActive "ahk_class ConsoleWindowClass" ; Checks if Command Prompt is open and active
{
  Send "^k" ; Simulates Ctrl+K
  Sleep 250 ; Wait for a quarter of a second
  Send "^w" ; Simulates Ctrl+W
}