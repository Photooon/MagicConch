$libName = $args[0]
$outDir = $args[1]

$dllName = "$libName.dll"
$dllPath = "$outDir\$dllName"
$jsonName = "$libName.json"
$jsonPath = "$outDir\$jsonName"

Write-Host "正在拷贝插件到 酷Q 应用文件夹……"

Copy-Item -Force $dllPath "C:\path\to\coolq\app\$dllName" # 注意修改 酷Q 目录
Copy-Item -Force $jsonPath "C:\path\to\coolq\app\$jsonName"

Write-Host "拷贝完成" -ForegroundColor Green