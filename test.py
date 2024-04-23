# python library for http requests using json (mostly)
import requests
from collections import deque

#debug = True
debug = False

creatingGraph = True
#creatingGraph = False

def bfsArts(toArt, fromArt):
    # create queue for links, starting with fromArt
    # [note]: using deque since its faster than a regular python list
    queue = deque()
    queue.appendleft(fromArt)

    # while queue not empty
    while len(queue) > 0:
        # get top [note]: pop gets from right of deque
        art = queue.pop()
        
        # take top of queue and add it's adj links
        links = queryLinks(art)
        for l in links:
            # if found the link looking for, return
            if l == toArt:
                print("found article " + l + " from " + art)
                return 
            queue.appendleft(l)

# return an array of links from an article name
def queryLinks(article, totalKeyErrors):
    session = requests.Session()

    url = "https://en.wikipedia.org/w/api.php"
    
    params = {
        "action": "query",
        "format": "json",
        "titles": article, 
        "prop": "links",
        "pllimit": "max"
    }
    
    response = session.get(url=url, params=params)
    data = response.json()
    pages = data["query"]["pages"]
    
    page_titles = []

    for key, val in pages.items():
        try:
            for link in val["links"]:
                page_titles.append(link["title"])
                if debug:
                    print(link["title"])
        except KeyError:
            totalKeyErrors += 1
            

    while "continue" in data:
        plcontinue = data["continue"]["plcontinue"]
        params["plcontinue"] = plcontinue

        response = session.get(url=url, params=params)
        data = response.json()
        pages = data["query"]["pages"]

        #print("\nPage %d" % pg_count)
        for key, val in pages.items():
            for link in val["links"]:
                page_titles.append(link["title"])
                if debug:
                    print(link["title"])
                

    if debug:
        print("Page titles for " + article + ":")
        for p in page_titles:
            print(p)

    return page_titles


# get size number of unique wikipedia links, starting from one article
def getLargeDataset(article, size, totalKeyErrors):
    print("Getting dataset")
    uniqueArticlesSet = set()
    previouslySearchedSet = set()
    currentArticleToQuery = article

    #initially find based on article
    arr = queryLinks(article, totalKeyErrors)
    for l in arr:
        uniqueArticlesSet.add(l)
    
    # add the starting article to previously searched
    previouslySearchedSet.add(article)

    # while the number of the unique articles is less than the needed size, keep looking
    while len(uniqueArticlesSet) < size:
        # update article to look for
        difference = uniqueArticlesSet - previouslySearchedSet
        # [note]: this is some sketchy stuff
        for c in difference:
            break
        currentArticleToQuery = c

        # find links from that article        
        arr = queryLinks(currentArticleToQuery, totalKeyErrors)

        # add them to the set
        for l in arr:
            uniqueArticlesSet.add(l)

        # add that article to the previous set
        previouslySearchedSet.add(currentArticleToQuery)        
    
    return uniqueArticlesSet

def outputDataset(dataset, filename):
    print("Outputting dataset")
    file = open(filename, "w", encoding="utf-8")

    # print the size of dataset
    file.write(str(len(dataset)) + '\n')

    count = 0

    for title in dataset:
        if count % 100 == 0:
            printProgress(count, len(dataset), 100)

        #print("Writing " + title)
        # get the titles it links to
        articleLinks = queryLinks(title, 0)

        # print size of that set
        file.write(title + " " + str(len(articleLinks)) + '\n')

        # print the links
        for art in articleLinks:
            file.write(art + '\n')

        count += 1

    file.close()

# prints progress bar for number / outof with a char size of sizeofbar
def printProgress(number, outof, sizeofbar):
    print("[", end ="")
    percent = int((number / outof) * sizeofbar)
    for p in range(percent):
        print("X", end ="")
    for p in range(sizeofbar - percent):
        print("-", end ="")
    print("]", end =" ")
    print(str(int((number / outof) * 100)) + " percent done")

fromArt = "Tetris"
totalKeyErrors = 0

# debug for testing file io
largeset = getLargeDataset(fromArt, 100000, totalKeyErrors)

size = len(largeset)
#print("List of unique articles (%d total):" % size)
print("Total Unique Articles: %d" % size)
print("Total KeyErrors: %d" % totalKeyErrors)

if totalKeyErrors == 0:
    print("somehow")

outputDataset(largeset, "myfile.txt")
