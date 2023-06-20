# Hello World
$name = Read-Host "Please enter your name"
Write-Host "Hello $name"

# Work with numbers
$a = 1
$calc = 5*$a
Write-Host "$calc"

# Work with commands
$a = "127.0.0.1"
$result = Test-Connection $a
Write-Host Test-Connection $a
Write-Host "$result"

#Get content from text file and print it:
$a = Get-Content -Path "C:\Users\xbont\source\repos\operating_systems_training\11.2.4 power shell scripting\test.txt"
Write-Host "$a"