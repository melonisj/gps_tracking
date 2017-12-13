# Author: Jacob Melonis
# Date: 12/7/17
# This file takes GPS Data from a GP20U7 module. The GPS module is connected to
# an arduino Uno via soft serial, and then that data was output to the Arduino
# serial port. That arduino is connected to a computer running this file, which
# sorts the data and will create a map of the GPS data if enough satellites are
# visible.
#
# In addition, it saves all previous data into a CSV file which can be loaded
# and displayed as a heatmap with user permission.

import serial
import gmplot
import fileinput
import sys
import os
import fcntl
import csv

#Parameters to change:
serial_port = '/dev/cu.usbmodem1411' #serial port to connect to
folder_path = '/Users/Jacob/Sites/test_files/ECEN3360/' #Path of folder this document is saved in:
html_file = 'gps_tracking_map.html' #generated web file showing map
refresh_time = '60' #Refresh Time of Website (seconds)
csv_file = 'gps_data.csv' #name of past csv data

heat_data = raw_input("Do you want to show past data from gps_data.csv?(y/n)\n")
trail_data = raw_input("Do you want to show a trail of active data?(y/n)\n")

s = serial.Serial(port=serial_port, baudrate=9600)
satellites = 0
lat = 0
lng = 0
current_time = 0
latitudes = []
longitudes = []
variable = 3

with open(csv_file, 'rU') as infile:
    reader = csv.reader(infile)
    heat_lats = [i[0] for i in reader]
with open(csv_file, 'rU') as infile:
    reader = csv.reader(infile)
    heat_lngs = [i[1] for i in reader]
for x in range(1,len(heat_lats)):
    heat_lats[x-1] = float(heat_lats[x])
    heat_lngs[x-1] = float(heat_lngs[x])
heat_lats = heat_lats[0:len(heat_lats)-1]
heat_lngs = heat_lngs[0:len(heat_lngs)-1]


fl = fcntl.fcntl(sys.stdin.fileno(), fcntl.F_GETFL)
fcntl.fcntl(sys.stdin.fileno(), fcntl.F_SETFL, fl | os.O_NONBLOCK)
while True:
    try:
        stdin = sys.stdin.read()
        if "\n" in stdin or "\r" in stdin:
            break
    except IOError:
        pass
    data = s.readline()
    # if data.count('$GPGGA') > 0:
    #     current_time = data[7:16]
    # if data.count('$GPGSV') > 0:
    #     satellites = float(data[7:data.find(',',7)])
    if data.count('$GPGLL') > 0:
        if data.count('A') > 0:
            indexN = data.find('N')
            lat = float(data[7:data.find(',',7)])
            lng = float(data[indexN+2:data.find(',',indexN+2)])
            lat_degrees = round(lat/100,0)
            lng_degrees = round(lng/100,0)
            lat_minutes = (lat%100)/60
            lng_minutes = (lng%100)/60
            lat = lat_degrees + lat_minutes
            lng = lng_degrees + lng_minutes


            if data.count('W') > 0:
                lng = lng*-1
    # hour = str(int(str(current_time)[0:2])-7)
    # minute = str(current_time)[2:4]
    # second = str(current_time)[4:6]
    # millisecond = str(current_time)[7:9]

    # print "\n"
    # print "Satellites: %s" % satellites
    # print "Time: %s:%s:%s.%s" % (hour,minute,second,millisecond)
    # print "Latitudes: %s" % lat
    # print "Longitude: %s" % lng
    if variable == 1:
        print "Acquiring Signal..."
        # print "Satellites: %s" % satellites
    else:
        if lat != 0 and lng!=0 and data.count('$GPGLL') > 0:
            gmap = gmplot.GoogleMapPlotter(lat, lng, 14)

            latitudes.append(lat)
            longitudes.append(lng)
            marker_lats = [lat]
            marker_lngs = [lng]
            #make the graphs
            if len(latitudes) >= 2 and trail_data.count('y') > 0:
                gmap.plot(latitudes, longitudes, 'cornflowerblue', edge_width=8)
            gmap.scatter(marker_lats, marker_lngs, '#A52A2A', marker=True) #use #google-maps-marker for more fun
            if heat_data.count('y') > 0:
                gmap.heatmap(heat_lats, heat_lngs)
            gmap.draw(html_file)

#Modifying the HTML File

            headtag = "<head>"
            divtag = "<div id=\"map_canvas\" style=\"width: 100%; height: 100%;\"></div>"
            bodytag = "<body"
            titletag = "<title>"
            newtitle = "<title>GPS Tracking</title>"
            refresh = "<meta http-equiv='refresh' content='"+refresh_time+"'/>"
            style = "<style type='text/css'>\nbody{\nbackground-color: #5c5c5c;\n}\n#myDiv{\nbackground-color: #5c5c5c;\nwidth: 80%;\nmargin-left: 10%;\nmargin-right: 10%;\nmargin-top:1%;\nmargin-bottom:1%;\nheight: 8%;\ntop:-200px;\ncolor:#ffffff;\n}\n</style>\n"
            div = "<div id ='myDiv' class='myDiv'><h1>GPS Tracking</h1></div>"
            replacediv = "<div id='map_canvas' style='width: 100%; height: 90%;'></div>"

            for line in fileinput.input(folder_path+html_file, inplace=1):
                if divtag in line:
                    print replacediv
                else:
                    if titletag in line:
                        print newtitle
                    else:
                        print line
                        if headtag in line:
                            print refresh # insert new data
                            print style
                        if bodytag in line:
                            print div # insert new data

            print "Map Generated"
#After user quits
print "Generating CSV"
heat_lats[:0] = ["Latitudes"]
heat_lngs[:0] = ["Longitudes"]
heat_lats[1:1] = latitudes
heat_lngs[1:1] = longitudes
l = [heat_lats, heat_lngs]
mylist = zip(*l)

with open(csv_file, 'wb') as myfile:
    wr = csv.writer(myfile, quoting=csv.QUOTE_ALL)
    for x in range(0,len(heat_lats)):
        wr.writerow(mylist[x])
