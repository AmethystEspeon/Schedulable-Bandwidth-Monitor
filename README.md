# What it is
This is a .batch file (and requisite other files) that you can schedule using Window's built in Task Scheduler to run automatic Bandwidth monitoring. It uses Ookla's SpeedTest CLI for testing, Git's install with Unix/Linx commands for GREP and GAWK for some filtering, and Discord for notifications! It's finished output is a txt file that includes all of the JSON results. Included is an executable that converts all the JSON outputs into a single CSV file usable with applications like Microsoft Excel and Google Sheets to make charts and graphs for easier readability.

The source code and includes for the executable are all in the repository if you want to compile it yourself!


# How it works
Ookla Speed Test has a Command Line Interface that you can download for free (included in this repo). This batch file uses that and outputs it in a JSON format. It then uses GREP and GAWK to get the download & upload speeds. If there's any connection at all, it will append that JSON to SpeedTestResults.json. If there isn't, it'll note the date and time and put it into SpeedTestFailures.json (*NOTE: The Failures file isn't actually in JSON formatting yet*). If you're connected, it then checks your speed against whatever speed you have set to be your warning (default is 500 Mb/s). If it's higher, it just lets it pass. If it's lower, it puts out a message through the discord webhook that there's an issue, including the current download & upload speeds, as well as the JSON file of the offending speed test. It does this all in the background by using a VBScript as the starting point, which can start hidden and make hidden CMDs.

When you want to see how your net is doing, you can run jsonBandwidthToCSV.exe to convert the results into a CSV file. I recommend deleting the SpeedTestResults.txt file after so that it doesn't duplicate results into it. The results in the CSV are in Megabits per second.

# What you need to do
In order for the Discord notifications to work, you need to put in your own discord webhook URL in a file named 'credentials.txt' in the same directory.

In order for it to be usable with the Windows Task Scheduler, the repository must be in a shared space, such as C:/Users/Public, or have the permissions for Authenticated Users.

If you're going to use the Windows Task Scheduler, you just schedule the file named SCHEDULE_THIS.vbs. That file will be hidden when launched and spawn a hidden CMD so it will not interrupt what you are doing. If you want to see the CMD every time that it pops, you can schedule autospeedtest.bat.


# What you can Customize
At the top of autospeedtest.bat, there are a couple of customizable parameters. max_file_size is the biggest you want to allow the SpeedTestResults.json or SpeedTestFailures.json to get before they are deleted. warning_speed is the speed in Mb/s that you want to be warned of when it receives a result under.

# What you want to be wary of
This IS a full bandwidth test. It is NOT a small amount of data. If you have a data cap, this is NOT the thing to do. There are some other ping monitors that are going to be way less bandwidth intensive that may or may not help you find your issues. If you have a low bandwidth max in the first place, this may eat it all. I made this because I just upgraded to Gigabit fiber, but am periodically getting significantly less than I paid for and I wanted to see if there was a trend.

The JSON file that it outputs also includes things like your internal & external IP, your ISP, your mac address, etc. Do **NOT** make the webhook part public. You could easily be tracked though it. You should make a private discord server just for yourself or for you and your housemates.

The executable was not written by you! Always be wary of random executables, especially on the internet! Thankfully, all the parts are in this repository! If you want, you can read the code and compile it yourself. The JSON library for C++ is found [here](https://github.com/nlohmann/json).
