$tests = Get-ChildItem -Recurse -Path ./build_mingw -Filter *Test.exe -Name | Where-Object { $_ -match '.*[\\/]test[\\/].*' }

$sectionStr    = '[==========]';
$testStartStr  = '[ RUN      ]';
$testFailedStr = '[  FAILED  ]';

for ($i = 0; $i -lt $tests.count; $i++) {
  $testLocation = "./build_mingw/" + $tests[$i]
  $testOutput   = & "$testLocation"

  $lastTestLine    = -1;
  $lastSectionLine = -1;

  $tests[$i]

  for ($j = 0; $j -lt $testOutput.count; $j++) {
    if ($testOutput[$j].length -lt 12) {
      continue;
    }

    $lineType = $testOutput[$j].Substring(0, 12);

    if ($lineType.Equals($testStartStr)) {
       $lastTestLine    = $j;
       $lastSectionLine = -1;
    }
    elseif ($lineType.Equals($testFailedStr)) {
      if ($lastTestLine -ne -1) {
        for ($k = $lastTestLine; $k -le $j; $k++) {
          echo $testOutput[$k]
        }

        ""
      }
    }
    elseif ($lineType.Equals($sectionStr)) {
       $lastTestLine    = -1;
       $lastSectionLine = $j;
    }
  }

  if ($lastSectionLine -ne -1) {
    for ($j = $lastSectionLine; $j -le $testOutput.count; $j++) {
      echo $testOutput[$j]
    }

    ""
  }
}
