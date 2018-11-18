******
README
******

contains some important info for the setting up the project.

*****************
PROJECT STRUCTURE
*****************
General:
--------
'Arduino' - contains source files for the Arduino
'Evaluation' - contains source files and log files for the evaluation server
'Misc' - contains miscellaneous helper scripts
Pi/Trained Models/Pi - contains trained ML models
Pi/Data - contains sensor data we collected doing particular dance moves

Noteworthy File Paths:
----------------------
Working code run on the Arduino Mega:
Arduino/SensorsAndSerial/SensorsAndSerial.ino
*all hardware must be properly setup before running

Working code run on the Raspberry Pi 3 during evaluation:
Pi/main_eval.py
*all hardware must be properly setup before running
*files for ML models must be on same directory as this file

Python script run on the Raspberry Pi 3 during data collection for the training phase:
Pi/Data/collect.py
*all hardware must be properly setup before running
*expects path of csv file e.g. 'chicken.csv' - without brackets - as an argument.

Machine Learning Training scripts for SVM, RandomForest and MLP models:
Pi/Training/Avas_ML

Machine Learning Training scripts for RNN model:
Pi/Training/RNN

Data Visualisation Scripts:
Pi/visualisation_v2.py - provide path to directory containing csv files as argument
Pi/visualisation.py - provide path to csv file as argument 

TCP/Wifi Comms Script to connect the Pi to the Evaluation Server:
wificomms.py - imported as a module in main_eval.py

*************************
SETTING UP RASPBERRY PI 3
*************************

RPi serial comms software setup:
`sudo raspi-config` in pi terminal
-> enable uart
-> disable serial login
-> enable uart hardware

Raspberry Pi Raspbian Stretch OS does not come with Python 3 installed.
Python3 is required for the ML scripts to work.

Install python 3.6 
-> follow instructions at https://gist.github.com/dschep/24aa61672a2092246eaca2824400d37f
-> sudo apt-get install libatlas-base-dev)
-> to run python3 scripts, use the command `python3 <name>.py`

Remember to install all dependencies onto Pi using:
sudo pip3 install <module> -i "https://www.piwheels.org/simple"




