from flask import Flask, redirect, render_template, request, url_for

import os
import sys
import helpers
from analyzer import Analyzer


app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)
    
    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name)
    if tweets == None:
        return redirect(url_for("index"))
        
    positive, negative, neutral = 0.0, 0.0, 0.0
    
    # Get the user’s most recent 100 tweets.
    for i in range(100):
        # if less than 50 tweets, break
        if i+1 > len(tweets):
            break
        # analyze tweets
        score = analyzer.analyze(tweets[i])

        if score > 0.0:
            positive += score
        elif score < 0.0:
            negative -= score
        else:
            neutral += 1
            
    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
