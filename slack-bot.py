import slacker
from slacker import Slacker

import pyrebase
from datetime import datetime, timedelta

#Sending Slack bot a message
def sendNotification(slackBotKey, clntKey) :
    slack = Slacker(slackBotKey)
    message = "Fuzzer from \"" + clntKey + "\"" + "has disconnected\n\n"
    slack.chat.post_message('#fuzz-push', message) #Here put your Slack channel name
    print("Sent notification!\n")
    

fuzzerClientIdentifier = []

datetimeFormat = "%Y-%m-%d %H:%M:%S"
clientLastUpdateTime = ""
currentTime = ""
clnt_id = -1


# Slack Bot's key - Personal info
slackBotKey = ''

# Authenticate user to firebase - Personal info
config = {
    "apiKey": "",
    "authDomain": "",
    "databaseURL": "",
    "projectId": "",
    "storageBucket": "",
    "messagingSenderId": "",
    "appId": "",
    "measurementId": ""
}


# Connect to firebase
firebase = pyrebase.initialize_app(config)
db = firebase.database()


print("Checking clients' last update time ...\n")
checkedStatus = {"status" : False}

# check firebase infinitely
while 1: 
    fuzzerClientIdentifier = []

    fuzzers = db.child("fuzzing").get()
    for fuzz in fuzzers.each():
        fuzzerClientIdentifier.append(fuzz.key())


    for clntKey in fuzzerClientIdentifier:
        
        clntStatus = db.child("fuzzing/" + clntKey + "/status").get()
        if(clntStatus.val() == False) :
            continue
        
        fuzzClientTime = db.child("fuzzing/" + clntKey + "/last_update").get()

        currentTime = datetime.now()
        currentTime = currentTime.strftime(datetimeFormat)
        clientLastUpdateTime = fuzzClientTime.val()
        
        #get time difference in seconds
        diff = datetime.strptime(currentTime, datetimeFormat) - datetime.strptime(clientLastUpdateTime, datetimeFormat)

        if(diff.seconds > 20) : 
            sendNotification(slackBotKey, clntKey)
            db.child("fuzzing").child(clntKey).update(checkedStatus)
