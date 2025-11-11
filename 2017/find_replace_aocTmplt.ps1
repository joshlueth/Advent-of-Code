param (
  [Parameter(Mandatory=$true)][string]$file_mod,
  [Parameter(Mandatory=$true)][string]$replace_txt,
  [Parameter(Mandatory=$true)][string]$with_txt
)

(Get-Content $file_mod) | ForEach-Object { $_ -replace $replace_txt, $with_txt } | Set-Content $file_mod
