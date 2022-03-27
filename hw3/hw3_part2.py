from genericpath import exists
import string


def main():
    dateAvailable(1/1/1999, 2, "C:/Users/Administrator/Desktop/cus.txt", "C:/Users/Administrator/Desktop/halls.txt")


def dateAvailable(date, hall, cus_path, halls_path):
    if(hall > 3 or hall < 1):
        print("illegal input")
    dates = open(cus_path, "r")
    halls = open(halls_path, "r")

    for line in dates:
        if(string(line) == (date + "\n")):
            for m_line in halls:
                if(string(m_line) == (hall + "\n")):
                    print("taken")
    print("ok")
    dates.close()
    halls.close()        


from genericpath import exists
import string


def main():
    dateAvailable(1/1/1999, 2, "C:/Users/Administrator/Desktop/cus.txt", "C:/Users/Administrator/Desktop/halls.txt")


def dateAvailable(date, hall, cus_path, halls_path):
    if(hall > 3 or hall < 1):
        print("illegal input")
    dates = open(cus_path, "r")
    halls = open(halls_path, "r")

    for line in dates:
        if(string(line) == (date + "\n")):
            for m_line in halls:
                if(string(m_line) == (hall + "\n")):
                    print("taken")
    print("ok")
    dates.close()
    halls.close()        


