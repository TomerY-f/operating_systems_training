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

#If statement
$number = Read-Host "Please enter a number"
if ($number -gt 5){
    Write-Host "too high"    
}
elseif($number -lt 5){
    Write-Host "too low"
}
else {
    Write-Host "High five!"
}

#Piping (pipe util):
    #Get all processes in current time and print them into a csv.
Get-Process | Export-Csv -Path C:\Users\xbont\source\repos\operating_systems_training\proc.csv
    #Use pipelines to run methods on objects:
Get-Process | Sort-Object WS -Descending | Select-Object -First 10
    #Finding Objects by there properties values:
Get-Service | Where-Object {$_.Status -EQ "Running"}

#Invoke-Expression - Running code thurogh string (Alias iex)
$b = "dir"
iex $b