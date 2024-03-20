# This is testPython.py
print("Hello, there!I'm PySub, the Interpreter.\nHow has your experience been so far?\n")
y=int(input("I'm a robot, so be kind and talk to me in my language. \nType 1 for AMAZING, 0 for MEH:\n(Note: There ARE wrong answers here.)"))
print(y)


y=int(input("\nAnyway, so... Will you consider my application? 1 for yes, 0 for no: \n(Note: There ARE wrong answers here.)"))

index=0
while index<5:
y=int(input("Anyway, so... Will you consider my application? 1 for yes, 0 for no: "))
index=index+1

if y==1:
print("\nAww, thanks! Hope to see you soon!\nHope you enjoyed this journey!")
else:
print("\nPlease consider again..???\nHope you enjoyed this journey!")


    y = int(input(
        "I'm a robot, so be kind and talk to me in my language. \nType 1 for AMAZING, 0 for MEH:\n(Note: There ARE wrong answers here.)"))

if y == 1:
    print("Aww, thanks! I'm a robot but I have emotions too!\n")
else:
    while (y != 1):
        y=int(input("WRONG ANSWER! BOO! Please try again: "))
y=int(input("Anyway, so... Will you consider my application? 1 for yes, 0 for no: \n(Note: There ARE wrong answers here.)"))
if x == 1:
    print("Wow, making me emotional today. I like you too. Hope to see you soon!\n")
elif x == 0:
    print("Give me a chance! I work hard! All you gotta do is add 1, which is me??\n")
else:
    print("Hmm... Not the best answer but I'll take that as a hesitant yes.\nI'll keep working hard and improve, I promise!\n")