import os
import subprocess

os.system("g++ A.cpp -o A")


def run_cpp(input_file):
    run_command = ["./A"]
    with open(input_file, "r") as infile:
        result = subprocess.run(
            run_command, stdin=infile, capture_output=True, text=True
        )

    if "Assertion" in result.stderr:
        print(result.stderr)
        return False
    return True


input_file = "in"

while True:
    os.system("python3 gen.py > in")
    if run_cpp(input_file):
        print("Pass")
    else:
        print("Assertion false")
        break
