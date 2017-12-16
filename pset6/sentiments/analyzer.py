import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        #create an empty list and open file to put the data into the list.
        self.positives = []
        file = open(positives, "r")
        for line in file:
            #Skip the ";", space and new line
            if line.startswith(";") != True and bool(line.strip()) == True:
                self.positives.append(line.strip())
        file.close()
        
        self.negatives = []
        file = open(negatives, "r")
        for line in file:
            if line.startswith(";") != True and bool(line.strip()) == True:
                self.negatives.append(line.strip())
        file.close()
  

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        point = 0
        self.text = text.lower()
        if self.text == "":
            return 0
            
        #use nltk library's method to split the string.
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(self.text)
        
        #iterate word in tokens string list, and check whether is pos or neg.
        for word in tokens:
            if word in self.positives:
                point += 1
            elif word in self.negatives:
                point -= 1
            else:
                point +=0
                
        return point