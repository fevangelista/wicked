import subprocess
from glob import glob

def main():
    for test_file in glob("*/test_*.py"):
        print(f"Running {test_file}")
        subprocess.run(["python", test_file])

if __name__ == "__main__":
    main()