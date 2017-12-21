#!/usr/bin/python
from random import randint
import math

def main():
    n = randint(5,13)
    queens = randint(n+1,n+6)
    nSquare = math.pow(n,2)
    trees = randint(10,nSquare)
    co_ords = []
    for i in range(1,trees):
        x = randint(0,n)
        y = randint(0,n)
        co_ords.append([x,y])

    fout = open("input.txt","w")
    fout.write("DFS")
    fout.write('\n')
    fout.write(str(n))
    fout.write('\n')
    fout.write(str(queens))
    for i in range(0,n):
        fout.write('\n')
        for j in range(0,n):
            if [i,j] in co_ords:
                fout.write(str('2').rstrip('\n'))
            else:
                fout.write(str('0').rstrip('\n'))

if __name__ == "__main__":
    main()
