# Name: Bradley Tate
# Course: 3500
# Activity 08
# Description: stack.py with error handling 


#########################
#  Basic Stack Simulator
#########################


print("***********************************")
print("          Stack Simulator          ")
print("***********************************")
print("Please only use digits from 0 to 9 ")
print("***********************************")
print("Please enter 'pop' for popping")
print("Please enter 'push' for pushing")
print("Please enter 'print' to print")
print("Please enter 'IsEmpty' to check if the stack is empty")
print("Please enter 'IsFull' to check if the stack is full")
print("Please enter 'size' to print the current size of the stack")
print("Please enter 'end' to terminate the program")
stack = [] #make a list named 'stack'

while (True):
    try:

        val = input("...") #get input from the user
        if  (val == 'push'):
            if (len(stack) < 10):
                while True: #To check if the input is a number or not
                    a = input("Which number to push?... ")
                    if( 48 <= ord(a) <= 57): #check ascii code for digits
                        stack.append(a)  #push the number
                        break
    #if the input is number, then terminate the nested while loop,
    # else continue the loop until numeric value is obtained
                    else:
                        print("Please enter only numbers ") #error return
            else:
                print("The stack is full, please pop an element to continue")

        elif( val =='pop'):
            print(stack.pop()) #pop, ie. remove and return

        elif( val =='print'):
            print(stack) #print the list

        elif (val == 'IsEmpty'):
            if (len(stack) == 0):
                print("Stack is empty")
            else:
                print("Stack is not empty")

        elif (val == 'IsFull'):
            if (len(stack) == 10):
                print("Stack is full")
            else:
                print("Stack is not full")

        elif (val == 'size'):
            print('The current size of the stack is ' + str(len(stack)))

        elif( val =='end'):
            break  #terminate the while loop

        else:
            print("Unknown command ")

    except IndexError:
        print("Cannot pop from an empty stack, please push some elements")

    except TypeError:
        print("Please enter only a 1 digit positive number")

print("Thank you")
