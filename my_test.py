#!/usr/bin/env python3

import argparse, subprocess, os, time, sys
tag_counts = {
    5	:	5,
    6	:	3,
    7	:	7,
    8	:	8,
    9	:	8,
    10	:	7,
    11	:	6,
    12	:	6,
    13	:	9,
    14	:	9,
    15	:	7,
    16	:	9,
    17	:	7,
    18	:	9,
    19	:	9,
    20	:	9,
    21	:	9,
    22	:	7,
    23	:	9,
    24	:	10,
    25	:	9,
    26	:	10,
    27	:	10,
    28	:	10,
    29	:	8,
    30	:	8,
    100	:	10,
    200	:	10,
    300	:	10,
    400	:	10,
    500	:	10,
    600	:	10,
    700	:	10,
    800	:	10,
    900	:	10,
    1000:	10
}

byte_count = 4467

env = {**os.environ}
base = sys.argv[1:]
base = " ".join(base) + " "
print(base)  # everything after the script name
for chunk, tags in tag_counts.items():
    print(f"Trying chunk Size: {chunk}")
    time.sleep(.5)
    try:
        cmd = base + str(chunk)
        output = subprocess.run(cmd, shell=True, check=True, capture_output=True, text=True, timeout=2)

    except subprocess.TimeoutExpired as e:
        print(f"TIMEOUT failed with chunk size{chunk}\n")
    else:
        expected = f"Number of <h1> tags: {tags}\nNumber of bytes: 4467\n"
        if (output.stdout!=expected):
            print(f"Failed test: {chunk}\n")
            print("OUTPUT\n")
            print(output.stdout)
            print("EXPECTED\n")
            print(expected)
            print("Ran command")
            print(base)
print("Done\n")
