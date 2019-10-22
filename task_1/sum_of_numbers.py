import sys

def parse_data(str):
    result = ""
    i = 0
    while (i < len(str)):
        if(str[i] == '-' and i + 1 < len(str) and str[i + 1] >= '0' and str[i + 1] <= '9)'):
            result += str[i]
            i += 1

        if (str[i] >= '0' and str[i] <= '9)'):
            while(str[i] >= '0' and str[i] <= '9)'):
                result += str[i]
                i += 1
            result += ' '
            if(str[i] == '-'):
                continue

        i += 1

    return result[:-1]

def get_data():
    str = ""

    for line in sys.stdin:
        str += line + '\n'
    return str

def test():
    str = get_data()
    numbers = parse_data(str).split(' ')
    result = 0

    for i in numbers:
        result += int(i)

    print(result)


test()
