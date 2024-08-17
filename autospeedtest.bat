for /f "delims=" %%A in (credentials.txt) do SET discord_webhook=%%A
::In bytes
SET max_file_size=10000000
::In MegaBits
SET warning_speed=500

::To prevent NULL errors, these need to be initialized (IDK how to check for null)
SET file_size=0
SET /A download_bandwidth=0
SET /A upload_bandwidth=0

::Checking to make sure we don't go above max_file size for results & failures
for /f "delims=" %%A in ('dir /-C ^| gawk -F" " "$5 ~ /SpeedTestResults.json/ {print $4}"') do (
    SET file_size=%%A
)
if %file_size% GEQ %max_file_size% rm SpeedTestResults.json
for /f "delims=" %%A in ('dir /-C ^| gawk -F" " "$5 ~ /SpeedTestFailures.json/ {print $4}"') do (
    SET file_size=%%A
)
if %file_size% GEQ %max_file_size% rm SpeedTestResults.json

::Getting our data, turn it into Mb/s
speedtest.exe -f json > SpeedTestResultsPartial.json
for /f "delims=" %%A in ('grep -o "download....bandwidth..[0-9]*" SpeedTestResultsPartial.json ^| gawk -F":" "{print $NF}"') do (
    SET /A download_bandwidth=%%A/125000
)
for /f "delims=" %%A in ('grep -o "upload....bandwidth..[0-9]*" SpeedTestResultsPartial.json ^| gawk -F":" "{print $NF}"') do (
    SET /A upload_bandwidth=%%A/125000
)

::Using results
SET or=F
if %download_bandwidth% LEQ 0 (SET or=T)
if %upload_bandwidth% LEQ 0 (SET or=T)
echo %or%
if %or%==T (
    echo %date%-%time% >> SpeedTestFailures.json
) else (
    cat SpeedTestResultsPartial.json >> SpeedTestResults.json
)

SET and=T
SET "message_text=@everyone There's an issue with the broadband! Check it out!\n\nDownload Bandwidth: %download_bandwidth%\nUpload Bandwidth: %upload_bandwidth%"
SET "attachment=SpeedTestResultsPartial.json"
if %download_bandwidth% GTR %warning_speed% SET and=F
if %download_bandwidth% LEQ 0 SET and=F
if %and%==T (
    curl -k -F "payload_json={\"content\": \"%message_text%\"}" -F "file1=@%attachment%" %discord_webhook%
) else (
        SET and=T
        if %upload_bandwidth% GTR %warning_speed% SET and=F
        if %upload_bandwidth% LEQ 0 SET and=F
        if %and%==T (
            curl -k -F "payload_json={\"content\": \"%message_text%\"}" -F "file1=@%attachment%" %discord_webhook%
        )
)

rm SpeedTestResultsPartial.json