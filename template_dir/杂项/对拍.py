import os

os.system("python3 gen.py > test.in")
os.system("g++ A.cpp -o A")
os.system("g++ bf.cpp -o bf")

while True:
    os.system("./A < test.in > test.out")
    os.system("./bf < test.in > bf.out")
    if os.system("diff test.out bf.out"):
        print("WA")
        break
    else:
        print("AC")
        os.system("python3 gen.py > test.in")
