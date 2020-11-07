import os
import sys

MESSAGE = "The reversed sentence is:"

#Reverse the given string 
def reverse(data):
    newdata = ""
    for word in reversed(data.split(" ")):
        newdata += word+" "
    return MESSAGE + newdata
 

#Read inout
datain = input()

# file descriptors for using pipes
#pipe1 is for writing in parent and reading in child
#pipe2 is for writing in child and reading in parent
pipe1read, pipe1write = os.pipe() 
pipe2read, pipe2write = os.pipe()

#Create Child process
processid = os.fork()


if processid > 0: 
    #Parent Process
    
    #pipe2write is for writing in child so we close it in parent
    os.close(pipe2write)

    #pipe1read is for reading in child so we close it in parent
    os.close(pipe1read)

    # returns an open file object connected to the file descriptor pipe2read for reading
    reader = os.fdopen(pipe2read)

    # returns an open file object connected to the file descriptor pipe1write for writing
    writer = os.fdopen(pipe1write, 'w')

    # passing the given input from user to child process
    writer.write(datain)
    # we don't need writer anymore so we close it.
    writer.close()

    # reading the child process's written data
    modifieddata = reader.read()
    # we don't need reader anymore so we close it
    reader.close()

    print(modifieddata)

    sys.exit(0)
else:
    #Child Process

    os.close(pipe1write)
    os.close(pipe2read)

    writer = os.fdopen(pipe2write, 'w')
    reader = os.fdopen(pipe1read)
    
    # reading the parent's written data
    childin = reader.read()
    reader.close()

    # reversing the given data
    childin = reverse(childin)

    # writing the modified(reversed) data to parent
    writer.write(childin)
    writer.close()

    sys.exit(0)
