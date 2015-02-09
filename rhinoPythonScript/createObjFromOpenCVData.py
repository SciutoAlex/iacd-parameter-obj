#!/usr/bin/env python

import rhinoscriptsyntax as rs
import random
import math
import json


# load the json file that inclues all the points
filePath = "/Users/alex/Documents/of/apps/myApps/opencvexport/bin/data/opencvtest.json"
f = open(filePath, 'rw');
jsonData = json.loads(f.read());


# Get the bounding points for the object, so we can center it later.
minx = 99999
maxx = -999999
miny = 99999
maxy = -999999
for obj in jsonData["data"]:
    for i in obj:
        if(i[0] > maxx):
            maxx = i[0]
        if(i[0] < minx):
            minx = i[0]
        if(i[1] > maxy):
            maxy = i[1]
        if(i[1] < miny):
            miny = i[1]
        

# Set up the spacing variables
offset = 10
length = offset*len(jsonData["data"])
initialOffset = -.5 * length

width = maxx - minx
print maxx
print minx
widthOffset = -.5 * width


        
# iterate through the frames and create points from blob points. This implementation
# assumes one blob for one frame. I belive Rhino could handle multiple blobs, but I didn't explore this.

planeList = [];
for frame in jsonData["data"]:
    
    blobList = [];
    
    for i in frame:
        pt = (initialOffset, i[0] - widthOffset - 240, 320-i[1] - miny)
        blobList.append(pt)
    
    #close the curve. There is no closeCurve command. You just have to end the curve
    #at the same spot where you began.
    pt = (initialOffset, frame[0][0] - widthOffset - 240, 320-frame[0][1] - miny)
    blobList.append(pt)
        
    planeList.append(blobList)
    
    initialOffset+=offset
        

# Draw the curves and store their IDs in a list.
curvelist = []
for plane in planeList:
    crv = rs.AddCurve(plane)
    curvelist.append(crv)

#These are the methods for creating the lofted shape and removing the unnecessary curves,
#I found I needed to do some editing of the initial and end frames, so I mostly did these commands manually

#loft = rs.AddLoftSrf(curvelist, 0, 2, 1);
#rs.CapPlanarHoles(loft)
#
#rs.DeleteObjects(curvelist)
