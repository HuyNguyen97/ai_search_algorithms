#!/usr/bin/python

def checker(p,q,board,n):
    # column up
    for a in range(p-1,-1,-1):
        if board[a][q]==2:
            break
        if board[a][q] == 1:
            print("Queens Clash")
            print("a")
            return 1

    # column down
    for b in range(p+1,n):
        if board[b][q] == 2:
            break
        if board[b][q] == 1:
            print("b")
            print("Queens Clash")
            return 1
    
    # row right
    for c in range(q+1,n):
        if board[p][c]:
            break
        if board[p][c] == 1:
            print("c")
            print("Queens Clash")
            return 1
    
    # row left
    for d in range(q-1,-1,-1):
        if board[p][d]==2:
            break
        if board[p][d] == 1:
            print("d")
            print("Queens Clash")
            return 1
    
    # positive diagnol towards right top 
    for e,f in zip( range(p-1,-1,-1), range(q+1,n) ):
        if board[e][f]==2:
            break
        if board[e][f]==1:
            print("e")
            print("Queens Clash")
            return 1
    
    # positive diagnol towards left bottom
    for z1,z2 in zip( range(p+1,n),range(q-1,-1,-1) ):
        if board[z1][z2]==2:
            break
        if board[z1][z2]==1:
            print("f")
            print("Queens Clash")
            return 1

    # negative diagnol towards right bottom
    for g,h in zip( range(p+1,n),range(q+1,n) ):
        if board[g][h]==2:
            break
        if board[g][h]==1:
            print("g")
            print("Queens Clash")
            return 1

    # negative diagnol towards left top
    for t,u in zip( range(p-1,-1,-1),range(q-1,-1,-1) ):
        if board[t][u]==2:
            break
        if board[t][u]==1:
            print("h")
            print("Queens Clash")
            return 1

def main():
    board_frogs=0
    with open("input.txt") as fp:
        for i, line in enumerate(fp):
            if i == 1:
                global n
                n=int(line)
            if i == 2:
                global frogs
                frogs=int(line)
    print (n,frogs)
    board = [[0 for x in range(n)] for y in range(n)]

    with open("output.txt") as fin:
        for i, line in enumerate(fin):
            if i == 0:
                if str(line) == "FAIL\n":
                    print("Its a fail anyway!")
                    return
            elif i>=1:
                for j in range(n):
                    if line[j]=='1':
                        board_frogs += 1
                    board[i-1][j] = int(line[j])
    
    if board_frogs != frogs:
        print("Number of frogs is wrong!")
        return

    for p in range(0,n):
        for q in range(0,n):
            if board[p][q]==1:
                if(checker(p,q,board,n)):
                    return
    print("SUCCESSSSS!")


if __name__ == "__main__":
    main()




