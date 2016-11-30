import sys

with open("serialOutput.txt") as f:
    content = f.readlines();
lastNote = ''
for i in content:
    '''
    j = ''
    for letter in i:
        if letter == "\n":
            pass
        elif letter == ",":
            pass
        elif int(letter) > 1:
            j += "1"
        elif int(letter) == 1:
            j += "1"
        elif int(letter) == 0:
            j += "0"

        
    print j
    
    if i == "0000\n":
        print "0x00" + ","
    elif i == "1000\n":
        print "0x01"+ ","
    elif i == "0100\n":
        print "0x02"+ ","
    elif i == "0010\n":
        print "0x02"+ ","
    elif i == "0001\n":
        print "0x03"+ ","
    elif i == "1100\n":
        print "0x04"+ ","
    elif i == "1010\n":
        print "0x04"+ ","
    elif i == "1001\n":
        print "0x06"+ ","
    elif i == "0110\n":
        print "0x05"+ ","
    elif i == "0101\n":
        print "0x06"+ ","
    elif i == "0011\n":
        print "0x05"+ ","

    '''
    if i == lastNote:
        lastNote = i;
        print "0x00" +','
    else:
        lastNote = i;
        if i == "0\n":
            print "0x00" +','
        elif i == "1\n":
            print "0x01"+','
        elif i == "2\n":
            print "0x02" +','
        elif i == "3\n":
            print "0x03" +','
        elif i == "4\n":
            print "0x04" +','
        elif i == "5\n":
            print "0x05" +','
        elif i == "6\n":
            print "0x06" +','
    
