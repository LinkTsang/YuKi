#requires -Version 3
$oldLocation = Get-Location
$rootDirPath = "../"
Set-Location $rootDirPath
$sourceDirPath = "./src"

workflow Run-ClangFormat
{
  param
  (
    [String[]]
    $sourceFiles
  )
  foreach -parallel -throttlelimit 8 ($sourceFile in $sourceFiles)
  {
    Write-Output "Formatting: $sourceFile"
    clang-format -i $sourceFile -style=file
    Write-Output "Formatted: $sourceFile"
  }
}

$startTime = Get-Date
$sourceFiles = Get-ChildItem $sourceDirPath -File -Recurse -Include *.h,*.c,*.cpp,*.hpp,*.cxx,*.ccc
Run-ClangFormat -sourceFiles $sourceFiles
$endTime = Get-Date
Write-Host Time Used: ($endTime - $startTime)

Set-Location $oldLocation
