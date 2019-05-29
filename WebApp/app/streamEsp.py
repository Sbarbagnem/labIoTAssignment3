import sys
from datetime import datetime
from time import sleep
import json
import random

while 1:

    on = sys.argv[1]

    x = {
        "vibrazione": random.randint(0,1),
        "tilt": random.randint(0,1),
        "distanza": random.randint(0,100),
        "chiamata": random.randint(0,1),
        "temperatura": random.randint(-20,40),
        "cuore": random.randint(0,160),
    }

    if(on):
        y = json.dumps(x)

        # the result is a JSON string:
        print(y) 

        sys.stdout.flush()
    sleep(2)