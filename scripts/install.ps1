$coolqRoot = "D:\酷Q Air"

$appId = $args[0]
$libName = "com.company.MagicConch"
$appOutDir = "C:\Users\15278\runoob\my-awesome-app\build\Debug\Debug"

$coolqAppDir = "$coolqRoot\app"
$coolqDevDir = "$coolqRoot\dev"
$dllName = "$libName.dll"
$dllPath = "$appOutDir\$dllName"
$jsonName = "$libName.json"
$jsonPath = "$appOutDir\$jsonName"

Write-Host "正在拷贝插件到 酷Q 应用文件夹……"

New-Item -Path $coolqAppDir -ItemType Directory -ErrorAction SilentlyContinue
Copy-Item -Force $dllPath "$coolqAppDir\$dllName"
Copy-Item -Force $jsonPath "$coolqAppDir\$jsonName"

Remove-Item "$coolqDevDir\$libName" -Recurse

Write-Host "拷贝完成" -ForegroundColor Green